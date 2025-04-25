#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "updatetrainingdialog.h"
#include "updateclientdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTimeEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QTextCharFormat>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QNetworkReply>
#include <QToolTip>

MainWindow::MainWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbConnected(dbConnected),
    chartWindow(nullptr),
    emailSender(new EmailSender(this)),
    networkManager(new QNetworkAccessManager(this)),
    clientTableModel(new QSqlQueryModel(this)),
    clientProxyModel(new QSortFilterProxyModel(this)),
    trainingTableModel(new QSqlQueryModel(this)),
    trainingProxyModel(new QSortFilterProxyModel(this)),
    emailAttempts(0),
    emailSuccesses(0),
    consultationTableModel(nullptr) // Initialize to nullptr
{
    ui->setupUi(this);
    applyLightTheme();
    setAttribute(Qt::WA_DeleteOnClose);

    ui->trainingNotificationLabel->setProperty("clickable", true);
    ui->trainingNotificationLabel->setStyleSheet("QLabel { font-weight: bold; color: #3A5DAE; } "
                                                 "QLabel[clickable=true]:hover { text-decoration: underline; }");
    connect(ui->trainingNotificationLabel, &QLabel::linkActivated, this, &::MainWindow::handleTrainingNotificationLabelClick);
    ui->trainingNotificationLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    // Initialize client input form
    QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(this);
    dateTimeEdit->setObjectName("consultation_datetime");
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->clientAddTab->layout());
    if (gridLayout) {
        int row = 0, column = 0;
        for (int i = 0; i < gridLayout->count(); ++i) {
            int r, c, rs, cs;
            gridLayout->getItemPosition(i, &r, &c, &rs, &cs);
            if (gridLayout->itemAtPosition(r, c) &&
                gridLayout->itemAtPosition(r, c)->widget() == ui->clientConsultationDateEdit) {
                row = r;
                column = c;
                break;
            }
        }
        gridLayout->removeWidget(ui->clientConsultationDateEdit);
        ui->clientConsultationDateEdit->deleteLater();
        gridLayout->addWidget(dateTimeEdit, row, column);
    }

    // Initialize client table view
    ui->clientTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->clientTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    if (m_dbConnected) {
        clientTableModel->setQuery("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                                   "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                                   "FROM AHMED.CLIENTS c LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID");
        clientProxyModel->setSourceModel(clientTableModel);
        ui->clientTableView->setModel(clientProxyModel);
        ui->clientTableView->resizeColumnsToContents();
    }

    // Initialize consultation table model
    if (m_dbConnected) {
        consultationTableModel = new QSqlTableModel(this);
        consultationTableModel->setTable("AHMED.CLIENTS");
        ui->clientDateConsultationsView->setModel(consultationTableModel);
        consultationTableModel->select(); // Initial load
        ui->clientDateConsultationsView->resizeColumnsToContents();
    }

    // Initialize training input form
    ui->trainingDateEdit->setDate(QDate::currentDate());
    ui->trainingNotificationLabel->setText("<a href='notifications'>Notifications: 0</a>");
    ui->trainingNotificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");
    ui->trainingTableView->setSortingEnabled(true);

    QRegularExpression formationRx("[A-Za-z0-9\\s]{1,50}");
    QRegularExpression descriptionRx("[A-Za-z0-9\\s,.!?]{1,200}");
    QRegularExpression trainerRx("[A-Za-z\\s]{1,50}");
    QRegularExpression phoneRx("\\+\\d{10,15}");
    ui->trainingNameInput->setValidator(new QRegularExpressionValidator(formationRx, this));
    ui->trainingDescriptionInput->setValidator(new QRegularExpressionValidator(descriptionRx, this));
    ui->trainingTrainerInput->setValidator(new QRegularExpressionValidator(trainerRx, this));
    ui->trainingPhoneNumberInput->setValidator(new QRegularExpressionValidator(phoneRx, this));
    ui->trainingTimeSpinBox->setRange(1, 30);
    ui->trainingPriceSpinBox->setRange(1, 1000);

    // Disable UI if database is not connected
    if (!m_dbConnected) {
        ui->clientAddButton->setEnabled(false);
        ui->clientDeleteButton->setEnabled(false);
        ui->clientUpdateButton->setEnabled(false);
        ui->clientSearchInput->setEnabled(false);
        ui->clientResetSearchButton->setEnabled(false);
        ui->statisticsButton->setEnabled(false);
        ui->trainingAddButton->setEnabled(false);
        ui->trainingDeleteButton->setEnabled(false);
        ui->trainingUpdateButton->setEnabled(false);
        ui->trainingExportButton->setEnabled(false);
        ui->trainingSearchInput->setEnabled(false);
        ui->trainingResetSearchButton->setEnabled(false);
        statusBar()->showMessage("Database not connected. Some features are disabled.");
    } else {
        loadEmployees();
        refreshClientTable();
        refreshTrainingTableView();
    }

    connect(ui->clientUpdateButton, &QPushButton::clicked, this, &MainWindow::on_clientUpdateButtonClicked);
    // Connect signals (removed manual connections for client section buttons)
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->clientTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::on_clientTableViewHeaderClicked);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::on_statisticsButton_clicked);
    connect(ui->reportsButton, &QPushButton::clicked, this, &MainWindow::sendConsultationReminders);
    connect(ui->clientSearchInput, &QLineEdit::textChanged, this, &MainWindow::on_clientSearchInput_textChanged);
    connect(ui->clientSearchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_clientSearchCriteriaComboBox_currentIndexChanged);
    connect(ui->trainingAddButton, &QPushButton::clicked, this, &MainWindow::on_trainingAddButtonClicked);
    connect(ui->trainingDeleteButton, &QPushButton::clicked, this, &MainWindow::on_trainingDeleteButtonClicked);
    connect(ui->trainingUpdateButton, &QPushButton::clicked, this, &MainWindow::on_trainingUpdateButtonClicked);
    connect(ui->trainingExportButton, &QPushButton::clicked, this, &MainWindow::on_trainingExportButtonClicked);
    connect(ui->trainingSearchInput, &QLineEdit::textChanged, this, &MainWindow::on_trainingSearchInput_textChanged);
    connect(ui->trainingResetSearchButton, &QPushButton::clicked, this, &MainWindow::on_trainingResetSearchButton_clicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::on_trainingSmsRequestFinished);
    connect(ui->clientSectionButton, &QPushButton::clicked, this, &MainWindow::on_clientSectionButton_clicked);
    connect(ui->trainingSectionButton, &QPushButton::clicked, this, &MainWindow::on_trainingSectionButton_clicked);

    setupCalendarView();
}

MainWindow::~MainWindow()
{
    delete chartWindow;
    delete clientTableModel;
    delete clientProxyModel;
    delete trainingTableModel;
    delete trainingProxyModel;
    delete consultationTableModel; // Added
    delete emailSender;
    delete networkManager;
    delete ui;
}

void MainWindow::on_statisticsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    if (!chartWindow) {
        chartWindow = new ChartWindow(this);
    }
    chartWindow->show();
}

void MainWindow::on_clientSectionButton_clicked()
{
    int clientPageIndex = -1;
    for (int i = 0; i < ui->mainStackedWidget->count(); ++i) {
        if (ui->mainStackedWidget->widget(i)->findChild<QTabWidget*>("clientTabWidget")) {
            clientPageIndex = i;
            break;
        }
    }
    if (clientPageIndex != -1) {
        ui->mainStackedWidget->setCurrentIndex(clientPageIndex);
        refreshClientTable();
    } else {
        QMessageBox::critical(this, "UI Error", "Failed to switch to Client Section.");
    }
}

void MainWindow::on_trainingSectionButton_clicked()
{
    int trainingPageIndex = -1;
    for (int i = 0; i < ui->mainStackedWidget->count(); ++i) {
        if (ui->mainStackedWidget->widget(i)->findChild<QWidget*>("trainingTabWidget")) {
            trainingPageIndex = i;
            break;
        }
    }
    if (trainingPageIndex != -1) {
        ui->mainStackedWidget->setCurrentIndex(trainingPageIndex);
        refreshTrainingTableView();
    } else {
        QMessageBox::critical(this, "UI Error", "Failed to switch to Training Section.");
    }
}

void MainWindow::on_clientAddButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot add client: Database is not connected.");
        return;
    }
    if (!validateClientInputs()) {
        return;
    }
    QString name = ui->clientNameInput->text().trimmed();
    QString sector = ui->clientSectorInput->text().trimmed();
    QString contact = ui->clientContactInfoInput->text().trimmed();
    QString email = ui->clientEmailInput->text().trimmed();
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime dateTime = dateTimeEdit ? dateTimeEdit->dateTime() : QDateTime::currentDateTime();
    QString consultantName = ui->clientConsultantComboBox->currentText();
    QString consultant = employeeMap.value(consultantName, "-1");

    if (consultant == "-1") {
        QMessageBox::warning(this, "Input Error", "Please select a valid consultant.");
        return;
    }
    auto result = client.ajouter(name, sector, contact, email, dateTime, consultant);
    if (result.first) {
        QMessageBox::information(this, "Success", "Client added successfully!");
        addNotification("Added", QString("Client: %1").arg(name)); // Add notification
        refreshClientTable();
        ui->clientNameInput->clear();
        ui->clientSectorInput->clear();
        ui->clientContactInfoInput->clear();
        ui->clientEmailInput->clear();
        if (dateTimeEdit) {
            dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        }
        ui->clientConsultantComboBox->setCurrentIndex(0);
        updateCalendarConsultations();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add client: " + result.second);
    }
}

void MainWindow::on_clientDeleteButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot delete client: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->clientTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a client to remove.");
        return;
    }
    int row = selectedRows.first().row();
    QString clientName = clientProxyModel->data(clientProxyModel->index(row, 0)).toString();
    if (client.removeByName(clientName)) {
        QMessageBox::information(this, "Success", "Client removed successfully!");
        addNotification("Deleted", QString("Client: %1").arg(clientName)); // Add notification
        refreshClientTable();
        updateCalendarConsultations();
    } else {
        QMessageBox::critical(this, "Error", "Failed to remove the client.");
    }
}

void MainWindow::on_clientUpdateButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot update client: Database is not connected.");
        return;
    }
    QModelIndexList selectedIndexes = ui->clientTableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a client to update.");
        return;
    }
    QModelIndex index = clientProxyModel->mapToSource(selectedIndexes.first());
    QString name = clientTableModel->data(clientTableModel->index(index.row(), 0)).toString();
    QString sector = clientTableModel->data(clientTableModel->index(index.row(), 1)).toString();
    QString contactInfo = clientTableModel->data(clientTableModel->index(index.row(), 2)).toString();
    QString email = clientTableModel->data(clientTableModel->index(index.row(), 3)).toString();
    QDateTime consultationDateTime = clientTableModel->data(clientTableModel->index(index.row(), 4)).toDateTime();
    QString consultantName = clientTableModel->data(clientTableModel->index(index.row(), 5)).toString();
    QString consultantId = employeeMap.key(consultantName, "-1");

    UpdateClientDialog dialog(this);
    dialog.setClientData(name, sector, contactInfo, email, consultationDateTime, consultantId);
    if (dialog.exec() == QDialog::Accepted) {
        QString originalName = name;
        QString newName = dialog.getName();
        QString newSector = dialog.getSector();
        QString newContactInfo = dialog.getContactInfo();
        QString newEmail = dialog.getEmail();
        QDateTime newConsultationDateTime = dialog.getConsultationDateTime();
        QString newConsultantId = dialog.getConsultant();
        if (updateClient(originalName, newName, newSector, newContactInfo, newEmail, newConsultationDateTime, newConsultantId)) {
            QMessageBox::information(this, "Success", "Client updated successfully!");
            addNotification("Updated", QString("Client: %1").arg(newName)); // Add notification
            refreshClientTable();
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update client.");
        }
    }
}

void MainWindow::on_clientSearchInput_textChanged()
{
    if (!m_dbConnected) {
        return;
    }
    QString searchText = ui->clientSearchInput->text().trimmed();
    int column = ui->clientSearchCriteriaComboBox->currentIndex();
    clientProxyModel->setFilterRegularExpression(QRegularExpression(searchText, QRegularExpression::CaseInsensitiveOption));
    clientProxyModel->setFilterKeyColumn(column);
}

void MainWindow::on_clientSearchCriteriaComboBox_currentIndexChanged()
{
    on_clientSearchInput_textChanged();
}

void MainWindow::on_clientResetSearchButton_clicked()
{
    ui->clientSearchInput->clear();
    clientProxyModel->setFilterRegularExpression("");
    ui->statusBar->showMessage("Showing all clients");
}

void MainWindow::on_clientTableViewHeaderClicked(int logicalIndex)
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot sort table: Database is not connected.");
        return;
    }
    static Qt::SortOrder currentOrder = Qt::AscendingOrder;
    currentOrder = (currentOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    QSqlQueryModel* sortedModel = client.sortByColumn(logicalIndex, currentOrder);
    clientProxyModel->setSourceModel(sortedModel);
    ui->clientTableView->setModel(clientProxyModel);
    QString column = QStringList{"Name", "Sector", "Contact", "Email", "Date", "Consultant"}[logicalIndex];
    QString order = (currentOrder == Qt::AscendingOrder) ? "ascending" : "descending";
    ui->statusBar->showMessage(QString("Sorted by %1 (%2)").arg(column, order));
}

void MainWindow::on_clientConsultationCalendar_selectionChanged()
{
    QDate selectedDate = ui->clientConsultationCalendar->selectedDate();
    ui->clientSelectedDateLabel->setText("Selected date: " + selectedDate.toString("yyyy-MM-dd"));

    if (!m_dbConnected) {
        ui->clientConsultationCountLabel->setText("Consultations: Error");
        return;
    }

    // Ensure database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        db.close();
        if (!db.open()) {
            qDebug() << "Database error:" << db.lastError().text();
            ui->clientConsultationCountLabel->setText("Consultations: Error");
            return;
        }
    }

    // Start a transaction
    if (!db.transaction()) {
        qDebug() << "Failed to start transaction:" << db.lastError().text();
        ui->clientConsultationCountLabel->setText("Consultations: Error");
        return;
    }

    // Query to count consultations on the selected date
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM AHMED.CLIENTS WHERE TRUNC(CONSULTATION_DATE) = TO_DATE(:selectedDate, 'YYYY-MM-DD')");
    query.bindValue(":selectedDate", selectedDate.toString("yyyy-MM-dd"));
    if (!query.exec()) {
        qDebug() << "Consultation Count Query Error:" << query.lastError().text();
        db.rollback();
        ui->clientConsultationCountLabel->setText("Consultations: Error");
        return;
    }

    int count = 0;
    if (query.next()) {
        count = query.value(0).toInt();
    }
    ui->clientConsultationCountLabel->setText(QString("Consultations: %1").arg(count));

    // Update the table with consultations for the selected date
    if (consultationTableModel) {
        consultationTableModel->setFilter(QString("TRUNC(CONSULTATION_DATE) = TO_DATE('%1', 'YYYY-MM-DD')")
                                              .arg(selectedDate.toString("yyyy-MM-dd")));
        consultationTableModel->select();
        ui->clientDateConsultationsView->resizeColumnsToContents();
    }

    // Commit the transaction
    if (!db.commit()) {
        qDebug() << "Failed to commit transaction:" << db.lastError().text();
        db.rollback();
        ui->clientConsultationCountLabel->setText("Consultations: Error");
        return;
    }
}

void MainWindow::on_clientConsultationCalendar_activated(const QDate &date)
{
    QDateTime currentDateTime = QDateTime(date, QTime::currentTime());
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    if (dateTimeEdit) {
        dateTimeEdit->setDateTime(currentDateTime);
    }
    ui->clientTabWidget->setCurrentIndex(0);
}

void MainWindow::on_clientExportPdfButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot export PDF: Database is not connected.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));
    pdfWriter.setResolution(300);
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    const qreal scaleFactor = pdfWriter.logicalDpiX() / 72.0;
    const qreal mmToPoints = 2.83465;
    const qreal margin = 20 * mmToPoints * scaleFactor;
    const qreal pageWidth = pdfWriter.width() - 2 * margin;
    const qreal pageHeight = pdfWriter.height() - 2 * margin;

    QFont titleFont("Arial", 14, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont bodyFont("Arial", 9);
    QFont summaryFont("Arial", 11, QFont::Bold);

    painter.setFont(titleFont);
    painter.setPen(Qt::darkBlue);
    QString title = "Client Management System Report";
    QString date = QDate::currentDate().toString("MMMM d, yyyy");
    painter.drawText(QRectF(margin, margin, pageWidth, 50 * scaleFactor), Qt::AlignCenter, title + "\nGenerated on: " + date);

    qreal yPos = margin + 60 * scaleFactor;
    QStringList headers = {"Name", "Sector", "Contact", "Email", "Date & Time", "Consultant"};
    qreal columnWidths[] = {1.5, 1.2, 1.2, 2.0, 1.5, 0.6};
    qreal totalWidthUnits = 0;
    for (qreal width : columnWidths) totalWidthUnits += width;
    qreal tableWidth = pageWidth;
    qreal rowHeight = 30 * scaleFactor;

    QVector<qreal> actualWidths(headers.size());
    qreal accumulatedWidth = margin;
    for (int i = 0; i < headers.size(); ++i) {
        actualWidths[i] = (columnWidths[i] / totalWidthUnits) * tableWidth;
    }

    painter.setFont(headerFont);
    painter.setBrush(QBrush(QColor(200, 220, 255)));
    painter.setPen(Qt::black);
    painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight), Qt::AlignCenter, headers[i]);
        accumulatedWidth += actualWidths[i];
    }
    yPos += rowHeight;

    QSqlQueryModel *model = client.afficher();
    int rowCount = model->rowCount();
    int colCount = model->columnCount();
    int pageNumber = 1;

    painter.setFont(bodyFont);
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < rowCount; ++row) {
        if (yPos + rowHeight > pageHeight - margin) {
            painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                             Qt::AlignRight, QString("Page %1").arg(pageNumber++));
            pdfWriter.newPage();
            yPos = margin;
            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(200, 220, 255)));
            accumulatedWidth = margin;
            painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight), Qt::AlignCenter, headers[i]);
                accumulatedWidth += actualWidths[i];
            }
            yPos += rowHeight;
        }
        accumulatedWidth = margin;
        painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
        for (int col = 0; col < colCount; ++col) {
            QString text = model->data(model->index(row, col)).toString();
            painter.drawText(QRectF(accumulatedWidth + 2 * scaleFactor, yPos, actualWidths[col] - 4 * scaleFactor, rowHeight),
                             Qt::AlignCenter, text);
            accumulatedWidth += actualWidths[col];
        }
        yPos += rowHeight;
    }

    painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                     Qt::AlignRight, QString("Page %1").arg(pageNumber));
    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to " + fileName);
}

void MainWindow::on_trainingAddButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot add formation: Database is not connected.");
        return;
    }
    QString formationName = ui->trainingNameInput->text().trimmed();
    QString description = ui->trainingDescriptionInput->text().trimmed();
    QString trainer = ui->trainingTrainerInput->text().trimmed();
    QDate date = ui->trainingDateEdit->date();
    int time = ui->trainingTimeSpinBox->value();
    double prix = ui->trainingPriceSpinBox->value();
    QString phoneNumber = ui->trainingPhoneNumberInput->text().trimmed();

    if (formationName.isEmpty() || description.isEmpty() || trainer.isEmpty() || phoneNumber.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }
    if (!date.isValid() || date < QDate::currentDate()) {
        QMessageBox::warning(this, "Input Error", "Please select a valid future date.");
        return;
    }
    if (!QRegularExpression("\\+\\d{10,15}").match(phoneNumber).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Phone number must start with '+' followed by 10-15 digits.");
        return;
    }
    formation.setFormation(formationName);
    formation.setDescription(description);
    formation.setTrainer(trainer);
    formation.setDatef(date);
    formation.setTime(time);
    formation.setPrix(prix);
    if (formation.ajoutforma()) {
        QMessageBox::information(this, "Success", "Formation added successfully!");
        sendSmsNotification(phoneNumber, formationName, date);
        refreshTrainingTableView();
        addNotification("Added", QString("Training: %1").arg(formationName)); // Use addNotification instead
        ui->trainingNameInput->clear();
        ui->trainingDescriptionInput->clear();
        ui->trainingTrainerInput->clear();
        ui->trainingDateEdit->setDate(QDate::currentDate());
        ui->trainingTimeSpinBox->setValue(1);
        ui->trainingPriceSpinBox->setValue(1);
        ui->trainingPhoneNumberInput->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add formation.");
    }
}

void MainWindow::on_trainingDeleteButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot delete formation: Database is not connected.");
        return;
    }
    QModelIndexList selectedIndexes = ui->trainingTableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a formation to delete.");
        return;
    }
    QModelIndex index = trainingProxyModel->mapToSource(selectedIndexes.first());
    int idForm = trainingTableModel->data(trainingTableModel->index(index.row(), 0)).toInt();
    QString formationName = trainingTableModel->data(trainingTableModel->index(index.row(), 1)).toString();
    if (formations::deleteFormation(idForm)) {
        QMessageBox::information(this, "Success", "Formation deleted successfully!");
        refreshTrainingTableView();
        addNotification("Deleted", QString("Training: %1").arg(formationName)); // Use addNotification instead
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete formation.");
    }
}

void MainWindow::on_trainingUpdateButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot update formation: Database is not connected.");
        return;
    }
    QModelIndexList selectedIndexes = ui->trainingTableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a formation to update.");
        return;
    }
    QModelIndex index = trainingProxyModel->mapToSource(selectedIndexes.first());
    int idForm = trainingTableModel->data(trainingTableModel->index(index.row(), 0)).toInt();
    QString formationName = trainingTableModel->data(trainingTableModel->index(index.row(), 1)).toString();
    QString description = trainingTableModel->data(trainingTableModel->index(index.row(), 2)).toString();
    QString trainer = trainingTableModel->data(trainingTableModel->index(index.row(), 3)).toString();
    QDate date = QDate::fromString(trainingTableModel->data(trainingTableModel->index(index.row(), 4)).toString(), "yyyy-MM-dd");
    int time = trainingTableModel->data(trainingTableModel->index(index.row(), 5)).toInt();
    double prix = trainingTableModel->data(trainingTableModel->index(index.row(), 6)).toDouble();

    formations form(idForm, formationName, description, trainer, date, time, prix);
    UpdateTrainingDialog dialog(this);
    dialog.setTrainingData(idForm, form);
    if (dialog.exec() == QDialog::Accepted) {
        QString newFormationName = dialog.getFormation();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();
        if (formations::updateFormation(idForm, newFormationName, newDescription, newTrainer, newDate, newTime, newPrix)) {
            QMessageBox::information(this, "Success", "Formation updated successfully!");
            refreshTrainingTableView();
            addNotification("Updated", QString("Training: %1").arg(newFormationName)); // Use addNotification instead
        } else {
            QMessageBox::critical(this, "Error", "Failed to update formation.");
        }
    }
}
void MainWindow::on_trainingSearchInput_textChanged(const QString &text)
{
    if (!m_dbConnected) {
        return;
    }
    trainingProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    trainingProxyModel->setFilterKeyColumn(-1);
    trainingProxyModel->setFilterWildcard(text);
}

void MainWindow::on_trainingResetSearchButton_clicked()
{
    ui->trainingSearchInput->clear();
    trainingProxyModel->setFilterWildcard("");
}

void MainWindow::on_trainingExportButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));
    pdfWriter.setResolution(300);
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    const qreal scaleFactor = pdfWriter.logicalDpiX() / 72.0;
    const qreal mmToPoints = 2.83465;
    const qreal margin = 20 * mmToPoints * scaleFactor;
    const qreal pageWidth = pdfWriter.width() - 2 * margin;
    const qreal pageHeight = pdfWriter.height() - 2 * margin;

    QFont titleFont("Arial", 14, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont bodyFont("Arial", 9);

    painter.setFont(titleFont);
    painter.setPen(Qt::darkBlue);
    QString title = "Formation Management Report";
    QString date = QDate::currentDate().toString("MMMM d, yyyy");
    painter.drawText(QRectF(margin, margin, pageWidth, 50 * scaleFactor), Qt::AlignCenter, title + "\nGenerated on: " + date);

    qreal yPos = margin + 60 * scaleFactor;
    QStringList headers = {"ID", "Formation", "Description", "Trainer", "Date", "Time", "Price"};
    qreal columnWidths[] = {0.5, 1.0, 1.5, 1.0, 1.0, 0.5, 0.5};
    qreal totalWidthUnits = 0;
    for (qreal width : columnWidths) totalWidthUnits += width;
    qreal tableWidth = pageWidth;
    qreal rowHeight = 30 * scaleFactor;

    QVector<qreal> actualWidths(headers.size());
    qreal accumulatedWidth = margin;
    for (int i = 0; i < headers.size(); ++i) {
        actualWidths[i] = (columnWidths[i] / totalWidthUnits) * tableWidth;
    }

    painter.setFont(headerFont);
    painter.setBrush(QBrush(QColor(200, 220, 255)));
    painter.setPen(Qt::black);
    painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight), Qt::AlignCenter, headers[i]);
        accumulatedWidth += actualWidths[i];
    }
    yPos += rowHeight;

    int rowCount = trainingTableModel->rowCount();
    int colCount = trainingTableModel->columnCount();
    int pageNumber = 1;

    painter.setFont(bodyFont);
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < rowCount; ++row) {
        if (yPos + rowHeight > pageHeight - margin) {
            painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                             Qt::AlignRight, QString("Page %1").arg(pageNumber++));
            pdfWriter.newPage();
            yPos = margin;
            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(200, 220, 255)));
            accumulatedWidth = margin;
            painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight), Qt::AlignCenter, headers[i]);
                accumulatedWidth += actualWidths[i];
            }
            yPos += rowHeight;
        }
        accumulatedWidth = margin;
        painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
        for (int col = 0; col < colCount; ++col) {
            QString text = trainingTableModel->data(trainingTableModel->index(row, col)).toString();
            painter.drawText(QRectF(accumulatedWidth + 2 * scaleFactor, yPos, actualWidths[col] - 4 * scaleFactor, rowHeight),
                             Qt::AlignCenter, text);
            accumulatedWidth += actualWidths[col];
        }
        yPos += rowHeight;
    }

    painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                     Qt::AlignRight, QString("Page %1").arg(pageNumber));
    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to " + fileName);
}

void MainWindow::on_trainingSmsRequestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        addNotification("SMS Sent", "Training reminder sent successfully"); // Use addNotification
    } else {
        qDebug() << "SMS Request Error:" << reply->errorString();
        QMessageBox::warning(this, "SMS Error", "Failed to send notification: " + reply->errorString());
        addNotification("SMS Failed", "Failed to send training reminder: " + reply->errorString());
    }
    reply->deleteLater();
}
void MainWindow::toggleSidebar()
{
    ui->sideMenu->setVisible(!ui->sideMenu->isVisible());
}

void MainWindow::toggleTheme()
{
    static bool isDarkTheme = false;
    isDarkTheme = !isDarkTheme;
    if (isDarkTheme) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

void MainWindow::sendConsultationReminders()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot send reminders: Database is not connected.");
        return;
    }
    checkAndSendReminders();
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #A1B8E6);
            color: #333333;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QPushButton {
            background-color: #3A5DAE;
            color: white;
            border: 1px solid #2A4682;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #4A70C2;
        }
        QPushButton:pressed {
            background-color: #2A4682;
        }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
            padding: 4px;
            color: #333333;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus {
            border: 2px solid #3A5DAE;
        }
        QTableView {
            background-color: #FFFFFF;
            border: 1px solid #D3DCE6;
            border-radius: 4px;
            selection-background-color: #A1B8E6;
            selection-color: #333333;
        }
        QHeaderView::section {
            background-color: #3A5DAE;
            color: white;
            padding: 5px;
            border: none;
        }
        QCalendarWidget {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
        }
        QCalendarWidget QToolButton {
            background-color: #3A5DAE;
            color: white;
            border-radius: 3px;
        }
        QTabWidget::pane {
            border: 1px solid #3A5DAE;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #D3DCE6;
            color: #333333;
            padding: 6px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #3A5DAE;
            color: white;
        }
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel {
            font-size: 12pt;
            font-weight: bold;
            color: #3A5DAE;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        #headerLabel {
            font-size: 18pt;
            font-weight: bold;
            color: #3A5DAE;
            qproperty-alignment: AlignCenter;
        }
        #trainingNotificationLabel {
            font-size: 10pt;
            font-weight: bold;
            color: #3A5DAE;
        }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
        }
    )");
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #F28C6F, stop:1 #5C5C5C);
            color: #F0F0F0;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QPushButton {
            background-color: #F28C6F;
            color: white;
            border: 1px solid #D96C53;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #F5A38A;
        }
        QPushButton:pressed {
            background-color: #D96C53;
        }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
            padding: 4px;
            color: #F0F0F0;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus {
            border: 2px solid #F28C6F;
        }
        QTableView {
            background-color: #6A6A6A;
            border: 1px solid #4A4A4A;
            border-radius: 4px;
            selection-background-color: #F28C6F;
            selection-color: #F0F0F0;
        }
        QHeaderView::section {
            background-color: #F28C6F;
            color: white;
            padding: 5px;
            border: none;
        }
        QCalendarWidget {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
        }
        QCalendarWidget QToolButton {
            background-color: #F28C6F;
            color: white;
            border-radius: 3px;
        }
        QTabWidget::pane {
            border: 1px solid #F28C6F;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #7A7A7A;
            color: #F0F0F0;
            padding: 6px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #F28C6F;
            color: white;
        }
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel {
            font-size: 12pt;
            font-weight: bold;
            color: #F28C6F;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        #headerLabel {
            font-size: 18pt;
            font-weight: bold;
            color: #F28C6F;
            qproperty-alignment: AlignCenter;
        }
        #trainingNotificationLabel {
            font-size: 10pt;
            font-weight: bold;
            color: #F28C6F;
        }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 {
            border: 2px solid #F28C6F;
            border-radius: 5px;
        }
    )");
}

bool MainWindow::validateClientInputs()
{
    QString name = ui->clientNameInput->text().trimmed();
    if (!isValidName(name)) {
        QMessageBox::warning(this, "Input Error", "Name cannot be empty or contain numbers.");
        ui->clientNameInput->setFocus();
        return false;
    }
    QString sector = ui->clientSectorInput->text().trimmed();
    if (sector.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Sector cannot be empty.");
        ui->clientSectorInput->setFocus();
        return false;
    }
    QString contact = ui->clientContactInfoInput->text().trimmed();
    if (contact.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Contact information cannot be empty.");
        ui->clientContactInfoInput->setFocus();
        return false;
    }
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    if (!dateTimeEdit) {
        QMessageBox::critical(this, "Error", "DateTime widget not found.");
        return false;
    }
    QDateTime dateTime = dateTimeEdit->dateTime();
    if (!isValidDateTime(dateTime)) {
        QMessageBox::warning(this, "Input Error", "Date and time must be current or in the future.");
        dateTimeEdit->setFocus();
        return false;
    }
    QString email = ui->clientEmailInput->text().trimmed();
    if (email.isEmpty() || !email.contains('@') || !email.contains('.')) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid email address.");
        ui->clientEmailInput->setFocus();
        return false;
    }
    if (ui->clientConsultantComboBox->currentIndex() == 0) {
        QMessageBox::warning(this, "Input Error", "Please select a consultant.");
        ui->clientConsultantComboBox->setFocus();
        return false;
    }
    return true;
}

bool MainWindow::isValidName(const QString &name)
{
    if (name.isEmpty()) {
        return false;
    }
    for (const QChar &ch : name) {
        if (ch.isDigit()) {
            return false;
        }
    }
    return true;
}

bool MainWindow::isValidDateTime(const QDateTime &dateTime)
{
    return dateTime >= QDateTime::currentDateTime();
}

void MainWindow::setupCalendarView()
{
    ui->clientConsultationCalendar->setFirstDayOfWeek(Qt::Monday);
    connect(ui->clientConsultationCalendar, &QCalendarWidget::selectionChanged,
            this, &MainWindow::on_clientConsultationCalendar_selectionChanged);
    connect(ui->clientConsultationCalendar, &QCalendarWidget::activated,
            this, &MainWindow::on_clientConsultationCalendar_activated);
    updateCalendarConsultations();
    updateSelectedDateInfo(ui->clientConsultationCalendar->selectedDate());
}

void MainWindow::highlightDatesWithConsultations()
{
    ui->clientConsultationCalendar->setDateTextFormat(QDate(), QTextCharFormat());
    QTextCharFormat consultationFormat;
    consultationFormat.setBackground(QColor(100, 150, 255, 100));
    QSqlQuery query("SELECT CONSULTATION_DATE FROM AHMED.CLIENTS");
    while (query.next()) {
        QDate date = query.value(0).toDateTime().date();
        ui->clientConsultationCalendar->setDateTextFormat(date, consultationFormat);
    }
}

void MainWindow::updateSelectedDateInfo(const QDate &date)
{
    ui->clientSelectedDateLabel->setText(QString("Selected date: %1").arg(date.toString("yyyy-MM-dd")));
    if (m_dbConnected) {
        QSqlQueryModel *model = client.getConsultationsForDate(date);
        ui->clientDateConsultationsView->setModel(model);
        ui->clientDateConsultationsView->resizeColumnsToContents();
    }
}

void MainWindow::performClientSearch()
{
    if (!m_dbConnected) {
        return;
    }
    QString searchType = ui->clientSearchCriteriaComboBox->currentText();
    QString searchText = ui->clientSearchInput->text().trimmed();
    QSqlQueryModel *model = nullptr;

    if (searchType == "Name") {
        model = client.searchByName(searchText);
    } else if (searchType == "Sector") {
        model = client.searchBySector(searchText);
    } else if (searchType == "Contact") {
        model = client.searchByContact(searchText);
    } else if (searchType == "Email") {
        model = client.searchByEmail(searchText);
    } else if (searchType == "Consultant") {
        bool ok;
        int consultantId = searchText.toInt(&ok);
        if (ok) {
            model = client.searchByConsultant(QString::number(consultantId));
        } else {
            QMessageBox::warning(this, "Input Error", "Please enter a valid Consultant ID.");
            return;
        }
    } else if (searchType == "Date & Time") {
        QDateTime dateTime = QDateTime::fromString(searchText, "yyyy-MM-dd HH:mm:ss");
        if (dateTime.isValid()) {
            model = client.searchByDateTime(dateTime);
        } else {
            QMessageBox::warning(this, "Input Error", "Please enter a valid date (yyyy-MM-dd HH:mm:ss).");
            return;
        }
    }
    if (model) {
        clientProxyModel->setSourceModel(model);
        ui->clientTableView->setModel(clientProxyModel);
    }
}

void MainWindow::checkAndSendReminders()
{
    QSqlQuery query;
    query.prepare("SELECT NAME, EMAIL, CONSULTATION_DATE FROM AHMED.CLIENTS "
                  "WHERE CONSULTATION_DATE >= :currentDateTime AND CONSULTATION_DATE <= :endDateTime");
    QDateTime currentDateTime = QDateTime::currentDateTime();
    query.bindValue(":currentDateTime", currentDateTime);
    query.bindValue(":endDateTime", currentDateTime.addDays(1));
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to fetch clients for reminders: " + query.lastError().text());
        return;
    }
    while (query.next()) {
        QString clientName = query.value("NAME").toString();
        QString email = query.value("EMAIL").toString();
        QDateTime consultationDateTime = query.value("CONSULTATION_DATE").toDateTime();
        QString subject = "Consultation Reminder";
        QString body = QString("Dear %1,\n\nThis is a reminder for your consultation scheduled on %2.\n\nBest regards,\nYour Team")
                           .arg(clientName, consultationDateTime.toString("yyyy-MM-dd hh:mm"));
        if (emailSender->sendEmail(email, subject, body)) {
            emailSuccesses++;
        }
        emailAttempts++;
    }
    QMessageBox::information(this, "Reminders Sent", QString("Sent %1 reminders successfully out of %2 attempts.")
                                                         .arg(emailSuccesses).arg(emailAttempts));
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->clientConsultationCalendar && event->type() == QEvent::HoverMove) {
        QHoverEvent* hoverEvent = static_cast<QHoverEvent*>(event);
        QPoint pos = hoverEvent->position().toPoint();
        QTableView* tableView = ui->clientConsultationCalendar->findChild<QTableView*>();
        if (tableView) {
            QModelIndex index = tableView->indexAt(pos);
            if (index.isValid()) {
                int row = index.row();
                int col = index.column();
                QDate firstDayOfMonth = ui->clientConsultationCalendar->selectedDate().addDays(-ui->clientConsultationCalendar->selectedDate().day() + 1);
                int offset = firstDayOfMonth.dayOfWeek() - 1;
                int day = (row * 7) + col - offset + 1;
                if (day > 0 && day <= firstDayOfMonth.daysInMonth()) {
                    QDate hoverDate = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), day);
                    QSqlQuery query;
                    query.prepare("SELECT COUNT(*) FROM AHMED.CLIENTS WHERE DATE(CONSULTATION_DATE) = :date");
                    query.bindValue(":date", hoverDate);
                    if (query.exec() && query.next()) {
                        int count = query.value(0).toInt();
                        if (count > 0) {
                            QToolTip::showText(hoverEvent->position().toPoint(),
                                               QString("%1: %2 consultation(s)").arg(hoverDate.toString("yyyy-MM-dd")).arg(count),
                                               ui->clientConsultationCalendar);
                            return true;
                        }
                    }
                }
            }
        }
        QToolTip::hideText();
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::loadEmployees()
{
    if (!m_dbConnected) {
        return;
    }
    QSqlQuery query("SELECT ID, FIRST_NAME, LAST_NAME FROM AHMED.EMPLOYEE");
    if (!query.exec()) {
        QMessageBox::warning(this, "Error", "Failed to load employees: " + query.lastError().text());
        return;
    }
    ui->clientConsultantComboBox->clear();
    employeeMap.clear();
    ui->clientConsultantComboBox->addItem("Select Consultant...");
    while (query.next()) {
        QString id = query.value("ID").toString();
        QString fullName = QString("%1 %2").arg(query.value("FIRST_NAME").toString(), query.value("LAST_NAME").toString()).trimmed();
        employeeMap[fullName] = id;
        ui->clientConsultantComboBox->addItem(fullName);
    }
    if (employeeMap.isEmpty()) {
        ui->clientConsultantComboBox->addItem("No employees found");
        ui->clientConsultantComboBox->setEnabled(false);
    }
}

void MainWindow::refreshClientTable()
{
    if (!m_dbConnected) {
        return;
    }
    clientTableModel->setQuery("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                               "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                               "FROM AHMED.CLIENTS c LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID");
    if (clientTableModel->lastError().isValid()) {
        QMessageBox::critical(this, "Error", "Failed to refresh client data: " + clientTableModel->lastError().text());
    } else {
        ui->clientTableView->resizeColumnsToContents();
    }
}

bool MainWindow::updateClient(const QString &originalName, const QString &newName, const QString &sector,
                              const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime,
                              const QString &consultant)
{
    return client.updateClient(originalName, newName, sector, contactInfo, email, consultationDateTime, consultant);
}

void MainWindow::updateCalendarConsultations()
{
    if (!m_dbConnected) {
        return;
    }
    highlightDatesWithConsultations();
}

void MainWindow::refreshTrainingTableView()
{
    if (!m_dbConnected) {
        return;
    }
    trainingTableModel->setQuery("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS");
    if (trainingTableModel->lastError().isValid()) {
        QMessageBox::warning(this, "Error", "Failed to load formation data: " + trainingTableModel->lastError().text());
        return;
    }
    trainingProxyModel->setSourceModel(trainingTableModel);
    ui->trainingTableView->setModel(trainingProxyModel);
    ui->trainingTableView->resizeColumnsToContents();
}

void MainWindow::sendSmsNotification(const QString &phoneNumber, const QString &formationName, const QDate &date)
{
    QSettings settings("MyApp", "FormationApp");
    QString apiKey = settings.value("twilioApiKey", "").toString();
    QString senderNumber = settings.value("twilioSenderNumber", "").toString();
    if (apiKey.isEmpty() || senderNumber.isEmpty()) {
        QMessageBox::warning(this, "SMS Error", "Twilio API key or sender number not configured.");
        return;
    }
    QString message = QString("Reminder: You have a formation '%1' scheduled on %2.").arg(formationName, date.toString("yyyy-MM-dd"));
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/YOUR_ACCOUNT_SID/Messages.json");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QString auth = QString("Basic %1").arg(QString(QByteArray(QString("YOUR_ACCOUNT_SID:%1").arg(apiKey).toUtf8()).toBase64()));
    request.setRawHeader("Authorization", auth.toUtf8());
    QUrlQuery params;
    params.addQueryItem("From", senderNumber);
    params.addQueryItem("To", phoneNumber);
    params.addQueryItem("Body", message);
    networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void MainWindow::addNotification(const QString &action, const QString &details)
{
    Notification notification;
    notification.action = action;
    notification.details = details;
    notification.timestamp = QDateTime::currentDateTime();
    notifications.append(notification);
    updateNotificationLabel();
}

void MainWindow::updateNotificationLabel()
{
    ui->trainingNotificationLabel->setText(QString("<a href='notifications'>Activity Notifications: %1</a>").arg(notifications.count()));
}

void MainWindow::handleTrainingNotificationLabelClick()
{
    if (notifications.isEmpty()) {
        QMessageBox::information(this, "Notifications", "No notifications available.");
        return;
    }

    QString notificationDetails;
    for (const Notification &notification : notifications) {
        notificationDetails += QString("[%1] %2: %3\n")
        .arg(notification.timestamp.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(notification.action)
            .arg(notification.details);
    }

    QMessageBox::information(this, "Notification Details", notificationDetails);
}
