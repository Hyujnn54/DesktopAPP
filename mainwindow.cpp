#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QInputDialog>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QTime>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QUrlQuery>

MainWindow::MainWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    isDarkTheme(false),
    m_dbConnected(dbConnected),
    emailSender(new EmailSender(this)),
    emailAttempts(0),
    emailSuccesses(0),
    trainingTableModel(nullptr),
    trainingProxyModel(new QSortFilterProxyModel(this)),
    notificationCount(0),
    changeHistory(),
    networkManager(new QNetworkAccessManager(this)),
    totalFormations(0),
    totalCost(0.0),
    avgCost(0.0)
{
    qDebug() << "Starting MainWindow constructor";
    ui->setupUi(this);
    applyLightTheme();

    setAttribute(Qt::WA_DeleteOnClose);

    // Client Management Initialization
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
    } else {
        qDebug() << "Failed to cast layout to QGridLayout";
    }

    ui->clientTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->clientTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Training Management Initialization
    ui->trainingDateEdit->setDate(QDate::currentDate());
    ui->trainingNotificationLabel->setText("Notifications: 0");
    ui->trainingNotificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");

    QRegularExpression formationRx("[A-Za-z0-9\\s]{1,50}");
    QRegularExpression descriptionRx("[A-Za-z0-9\\s,.!?]{1,200}");
    QRegularExpression trainerRx("[A-Za-z\\s]{1,50}");
    QRegularExpression phoneRx("\\+\\d{10,15}");

    QValidator *formationValidator = new QRegularExpressionValidator(formationRx, this);
    QValidator *descriptionValidator = new QRegularExpressionValidator(descriptionRx, this);
    QValidator *trainerValidator = new QRegularExpressionValidator(trainerRx, this);
    QValidator *phoneValidator = new QRegularExpressionValidator(phoneRx, this);

    ui->trainingNameInput->setValidator(formationValidator);
    ui->trainingDescriptionInput->setValidator(descriptionValidator);
    ui->trainingTrainerInput->setValidator(trainerValidator);
    ui->trainingPhoneNumberInput->setValidator(phoneValidator);

    ui->trainingTimeSpinBox->setRange(1, 30);
    ui->trainingPriceSpinBox->setRange(1, 1000);
    ui->trainingTableView->setSortingEnabled(true);

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::on_trainingSmsRequestFinished);

    // Disable UI elements if database is not connected
    if (!m_dbConnected) {
        // Client Management
        ui->clientAddButton->setEnabled(false);
        ui->clientDeleteButton->setEnabled(false);
        ui->clientUpdateButton->setEnabled(false);
        ui->clientSearchInput->setEnabled(false);
        ui->clientResetSearchButton->setEnabled(false);
        ui->clientRefreshStatsButton->setEnabled(false);
        ui->calendarButton->setEnabled(false);

        // Training Management
        ui->trainingAddButton->setEnabled(false);
        ui->trainingDeleteButton->setEnabled(false);
        ui->trainingUpdateButton->setEnabled(false);
        ui->trainingExportButton->setEnabled(false);
        ui->trainingRefreshStatsButton->setEnabled(false);
        ui->trainingSearchInput->setEnabled(false);
        ui->trainingResetSearchButton->setEnabled(false);

        statusBar()->showMessage("Database not connected. Some features are disabled.");
    } else {
        loadEmployees();
        refreshClientTable();
        refreshTrainingTableView();
        updateTrainingStatistics();
    }

    // Connections for Client Management
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->clientAddButton, &QPushButton::clicked, this, &MainWindow::on_clientAddButton_clicked);
    connect(ui->clientDeleteButton, &QPushButton::clicked, this, &MainWindow::on_clientDeleteButton_clicked);
    connect(ui->clientUpdateButton, &QPushButton::clicked, this, &MainWindow::on_clientUpdateButtonClicked);
    connect(ui->clientResetSearchButton, &QPushButton::clicked, this, &MainWindow::on_clientResetSearchButton_clicked);
    connect(ui->clientTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::on_clientTableViewHeaderClicked);
    connect(ui->calendarButton, &QPushButton::clicked, this, &MainWindow::sendConsultationReminders);
    connect(ui->reportsButton, &QPushButton::clicked, this, &MainWindow::updateClientStatisticsDisplay);
    connect(ui->clientRefreshStatsButton, &QPushButton::clicked, this, &MainWindow::on_clientRefreshStatsButton_clicked);
    connect(ui->clientSearchInput, &QLineEdit::textChanged, this, &MainWindow::on_clientSearchInput_textChanged);
    connect(ui->clientSearchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_clientSearchCriteriaComboBox_currentIndexChanged);
    connect(ui->clientExportPdfButton, &QPushButton::clicked, this, &MainWindow::on_clientExportPdfButton_clicked);
    connect(ui->clientOpenChartButton, &QPushButton::clicked, this, &MainWindow::on_clientOpenChartButton_clicked);

    // Connections for Training Management
    connect(ui->trainingAddButton, &QPushButton::clicked, this, &MainWindow::on_trainingAddButtonClicked);
    connect(ui->trainingDeleteButton, &QPushButton::clicked, this, &MainWindow::on_trainingDeleteButtonClicked);
    connect(ui->trainingUpdateButton, &QPushButton::clicked, this, &MainWindow::on_trainingUpdateButtonClicked);
    connect(ui->trainingExportButton, &QPushButton::clicked, this, &MainWindow::on_trainingExportButtonClicked);
    connect(ui->trainingRefreshStatsButton, &QPushButton::clicked, this, &MainWindow::on_trainingRefreshStatsButton_clicked);
    connect(ui->trainingSearchInput, &QLineEdit::textChanged, this, &MainWindow::on_trainingSearchInput_textChanged);
    connect(ui->trainingResetSearchButton, &QPushButton::clicked, this, &MainWindow::on_trainingResetSearchButton_clicked);

    // Navigation Connections
    connect(ui->clientSectionButton, &QPushButton::clicked, this, &MainWindow::on_clientSectionButton_clicked);
    connect(ui->trainingSectionButton, &QPushButton::clicked, this, &MainWindow::on_trainingSectionButton_clicked);

    setupCalendarView();
    ui->clientConsultationCalendar->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete trainingTableModel;
    delete ui;
}

void MainWindow::on_clientSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_trainingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
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
    QString clientName = ui->clientTableView->model()->data(ui->clientTableView->model()->index(row, 0)).toString();

    if (client.removeByName(clientName)) {
        QMessageBox::information(this, "Success", "Client removed successfully!");
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

    QItemSelectionModel *selectionModel = ui->clientTableView->selectionModel();
    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, "No Selection", "Please select a client to update.");
        return;
    }

    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a client to update.");
        return;
    }

    QModelIndex index = selectedRows.at(0);
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->clientTableView->model());
    if (!model) {
        qDebug() << "Failed to get table model";
        return;
    }

    QString originalName = model->data(model->index(index.row(), 0)).toString();
    QString sector = model->data(model->index(index.row(), 1)).toString();
    QString contactInfo = model->data(model->index(index.row(), 2)).toString();
    QString email = model->data(model->index(index.row(), 3)).toString();
    QDateTime consultationDateTime = model->data(model->index(index.row(), 4)).toDateTime();

    QSqlQuery query;
    query.prepare("SELECT CONSULTANT_ID FROM AHMED.CLIENTS WHERE NAME = :name");
    query.bindValue(":name", originalName);
    if (!query.exec() || !query.next()) {
        qDebug() << "Error fetching CONSULTANT_ID:" << query.lastError().text();
        QMessageBox::warning(this, "Error", "Failed to fetch consultant information.");
        return;
    }
    QString consultantId = query.value("CONSULTANT_ID").toString();
    qDebug() << "Fetched CONSULTANT_ID for update:" << consultantId;

    UpdateClientDialog dialog(this);
    dialog.setClientData(originalName, sector, contactInfo, email, consultationDateTime, consultantId);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getName();
        QString newSector = dialog.getSector();
        QString newContactInfo = dialog.getContactInfo();
        QString newEmail = dialog.getEmail();
        QDateTime newConsultationDateTime = dialog.getConsultationDateTime();
        QString newConsultantId = dialog.getConsultant();

        if (newConsultantId.isEmpty()) {
            QMessageBox::warning(this, "Invalid Selection", "Please select a valid consultant.");
            return;
        }

        bool success = client.updateClient(originalName, newName, newSector, newContactInfo, newEmail, newConsultationDateTime, newConsultantId);
        if (success) {
            QMessageBox::information(this, "Success", "Client updated successfully.");
            refreshClientTable();
        } else {
            QMessageBox::warning(this, "Error", "Failed to update client.");
        }
    }
}

void MainWindow::on_clientSearchInput_textChanged()
{
    performClientSearch();
}

void MainWindow::on_clientSearchCriteriaComboBox_currentIndexChanged()
{
    performClientSearch();
}

void MainWindow::on_clientResetSearchButton_clicked()
{
    ui->clientSearchInput->clear();
    refreshClientTable();
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
    ui->clientTableView->setModel(sortedModel);

    QString column;
    switch (logicalIndex) {
    case 0: column = "Name"; break;
    case 1: column = "Sector"; break;
    case 2: column = "Contact"; break;
    case 3: column = "Email"; break;
    case 4: column = "Date"; break;
    case 5: column = "Consultant"; break;
    default: column = "Unknown"; break;
    }

    QString order = (currentOrder == Qt::AscendingOrder) ? "ascending" : "descending";
    ui->statusBar->showMessage(QString("Sorted by %1 (%2)").arg(column, order));
}

void MainWindow::on_clientConsultationCalendar_selectionChanged()
{
    QDate selectedDate = ui->clientConsultationCalendar->selectedDate();
    updateSelectedDateInfo(selectedDate);
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

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty())
        return;

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
    QString headerText = QString("%1\nGenerated on: %2").arg(title, date);
    painter.drawText(QRectF(margin, margin, pageWidth, 50 * scaleFactor), Qt::AlignCenter, headerText);

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
        painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                         Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
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
            painter.setFont(bodyFont);
            painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                             Qt::AlignRight, QString("Page %1").arg(pageNumber++));

            pdfWriter.newPage();
            yPos = margin;

            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(200, 220, 255)));
            accumulatedWidth = margin;
            painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                                 Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
                accumulatedWidth += actualWidths[i];
            }
            yPos += rowHeight;
        }

        accumulatedWidth = margin;
        painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
        for (int col = 0; col < colCount; ++col) {
            QString text = model->data(model->index(row, col)).toString();
            QRectF textRect(accumulatedWidth + 2 * scaleFactor, yPos, actualWidths[col] - 4 * scaleFactor, rowHeight);
            painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
            accumulatedWidth += actualWidths[col];
        }
        yPos += rowHeight;
    }

    painter.setFont(bodyFont);
    painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                     Qt::AlignRight, QString("Page %1").arg(pageNumber));

    yPos += 20 * scaleFactor;
    if (yPos + 60 * scaleFactor < pageHeight - margin) {
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QDateTime start = QDateTime::currentDateTime().addDays(-30);
        QDateTime end = QDateTime::currentDateTime();
        int totalConsultations = client.getTotalConsultations(start, end);
        int uniqueClients = client.getUniqueClients(start, end);
        QString summary = QString("Summary (Last 30 Days):\n"
                                  "Total Consultations: %1\n"
                                  "Unique Clients: %2")
                              .arg(totalConsultations)
                              .arg(uniqueClients);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
    } else {
        pdfWriter.newPage();
        yPos = margin;
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QDateTime start = QDateTime::currentDateTime().addDays(-30);
        QDateTime end = QDateTime::currentDateTime();
        int totalConsultations = client.getTotalConsultations(start, end);
        int uniqueClients = client.getUniqueClients(start, end);
        QString summary = QString("Summary (Last 30 Days):\n"
                                  "Total Consultations: %1\n"
                                  "Unique Clients: %2")
                              .arg(totalConsultations)
                              .arg(uniqueClients);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
        painter.setFont(bodyFont);
        painter.setPen(Qt::black);
        painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                         Qt::AlignRight, QString("Page %1").arg(pageNumber + 1));
    }

    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to " + fileName);
}

void MainWindow::on_clientOpenChartButton_clicked()
{
    ChartWindow *chartWindow = new ChartWindow(this);
    chartWindow->show();
}

void MainWindow::on_clientRefreshStatsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot refresh statistics: Database is not connected.");
        return;
    }

    static QElapsedTimer lastRefresh;
    if (!lastRefresh.isValid() || lastRefresh.elapsed() > 1000) {
        lastRefresh.start();
        updateClientStatisticsDisplay();
    } else {
        qDebug() << "Throttling refresh, too frequent";
    }
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

    QRegularExpression phoneRx("\\+\\d{10,15}");
    if (!phoneRx.match(phoneNumber).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Phone number must start with '+' followed by 10-15 digits.");
        return;
    }

    // Set the member variables of the formation object before calling ajoutforma()
    formation.setFormation(formationName);
    formation.setDescription(description);
    formation.setTrainer(trainer);
    formation.setDatef(date);
    formation.setTime(time);
    formation.setPrix(prix);

    bool success = formation.ajoutforma();
    if (success) {
        QMessageBox::information(this, "Success", "Formation added successfully!");
        sendSmsNotification(phoneNumber, formationName, date);
        refreshTrainingTableView();
        updateTrainingStatistics();
        addChangeToHistory("Added", formationName);

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
    int idForm = trainingTableModel->data(trainingTableModel->index(index.row(), 0)).toInt(); // IDFORM is in column 0

    if (formations::deleteFormation(idForm)) {
        QMessageBox::information(this, "Success", "Formation deleted successfully!");
        refreshTrainingTableView();
        updateTrainingStatistics();
        addChangeToHistory("Deleted", trainingTableModel->data(trainingTableModel->index(index.row(), 1)).toString());
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

    QString idForm = trainingTableModel->data(trainingTableModel->index(index.row(), 0)).toString();
    QString formationName = trainingTableModel->data(trainingTableModel->index(index.row(), 1)).toString();
    QString description = trainingTableModel->data(trainingTableModel->index(index.row(), 2)).toString();
    QString trainer = trainingTableModel->data(trainingTableModel->index(index.row(), 3)).toString();
    QDate date = QDate::fromString(trainingTableModel->data(trainingTableModel->index(index.row(), 4)).toString(), "yyyy-MM-dd");

    UpdateTrainingDialog dialog(idForm.toInt(), formation, this);

    if (dialog.exec() == QDialog::Accepted) {
        QString newFormationName = dialog.getFormation();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();

        bool success = formations::updateFormation(idForm.toInt(), newFormationName, newDescription, newTrainer, newDate, newTime, newPrix);
        if (success) {
            QMessageBox::information(this, "Success", "Formation updated successfully!");
            refreshTrainingTableView();
            updateTrainingStatistics();
            addChangeToHistory("Updated", newFormationName);
        } else {
            QMessageBox::critical(this, "Error", "Failed to update formation.");
        }
    }
}

void MainWindow::on_trainingSearchInput_textChanged(const QString &text)
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot search: Database is not connected.");
        return;
    }
    trainingProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    trainingProxyModel->setFilterKeyColumn(-1);
    trainingProxyModel->setFilterWildcard(text);
}

void MainWindow::on_trainingResetSearchButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot reset search: Database is not connected.");
        return;
    }
    ui->trainingSearchInput->clear();
    trainingProxyModel->setFilterWildcard("");
}

void MainWindow::on_trainingExportButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) return;

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
        painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                         Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
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
            painter.setFont(bodyFont);
            painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                             Qt::AlignRight, QString("Page %1").arg(pageNumber++));
            pdfWriter.newPage();
            yPos = margin;
            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(200, 220, 255)));
            accumulatedWidth = margin;
            painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                                 Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
                accumulatedWidth += actualWidths[i];
            }
            yPos += rowHeight;
        }

        accumulatedWidth = margin;
        painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
        for (int col = 0; col < colCount; ++col) {
            QString text = trainingTableModel->data(trainingTableModel->index(row, col)).toString();
            QRectF textRect(accumulatedWidth + 2 * scaleFactor, yPos, actualWidths[col] - 4 * scaleFactor, rowHeight);
            painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
            accumulatedWidth += actualWidths[col];
        }
        yPos += rowHeight;
    }

    painter.setFont(bodyFont);
    painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                     Qt::AlignRight, QString("Page %1").arg(pageNumber));

    yPos += 20 * scaleFactor;
    if (yPos + 60 * scaleFactor < pageHeight - margin) {
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QString summary = QString("Summary:\nTotal Formations: %1\nTotal Cost: $%2\nAverage Cost: $%3")
                              .arg(totalFormations)
                              .arg(totalCost, 0, 'f', 2)
                              .arg(avgCost, 0, 'f', 2);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
    } else {
        pdfWriter.newPage();
        yPos = margin;
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QString summary = QString("Summary:\nTotal Formations: %1\nTotal Cost: $%2\nAverage Cost: $%3")
                              .arg(totalFormations)
                              .arg(totalCost, 0, 'f', 2)
                              .arg(avgCost, 0, 'f', 2);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
        painter.setFont(bodyFont);
        painter.setPen(Qt::black);
        painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                         Qt::AlignRight, QString("Page %1").arg(pageNumber + 1));
    }

    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to " + fileName);
}

void MainWindow::on_trainingRefreshStatsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot refresh statistics: Database is not connected.");
        return;
    }
    updateTrainingStatistics();
}

void MainWindow::on_trainingSmsRequestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        QString status = obj["status"].toString();
        if (status == "queued" || status == "sent") {
            QMessageBox::information(this, "SMS Success", "SMS notification sent successfully!");
            notificationCount++;
            ui->trainingNotificationLabel->setText(QString("Notifications: %1").arg(notificationCount));
        } else {
            QMessageBox::warning(this, "SMS Error", "Failed to send SMS: " + obj["message"].toString());
        }
    } else {
        QMessageBox::warning(this, "SMS Error", "Failed to send SMS: " + reply->errorString());
    }
    reply->deleteLater();
}

void MainWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::toggleTheme()
{
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
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #FFFFFF, stop:1 #A1B8E6);
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
            border-radius: 2px;
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
        QToolTip {
            color: #333333;
            background-color: #E6ECF5;
            border: 1px solid #3A5DAE;
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
        QFrame#header {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
        }
        QFrame#sideMenu {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            background-color: #E6ECF5;
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #D3DCE6;
            border-radius: 5px;
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::applyDarkTheme()
{
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #F28C6F, stop:1 #5C5C5C);
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
            box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
        }
        QPushButton:hover {
            background-color: #F5A38A;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.4);
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
            border-radius: 2px;
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
        QToolTip {
            color: #F0F0F0;
            background-color: #F28C6F;
            border: 1px solid #D96C53;
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
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);
            qproperty-alignment: AlignCenter;
        }
        #trainingNotificationLabel {
            font-size: 10pt;
            font-weight: bold;
            color: #F28C6F;
        }
        QFrame#header {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
        }
        QFrame#sideMenu {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
            background-color: #7A7A7A;
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #4A4A4A;
            border-radius: 5px;
            box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.3);
        }
    )";
    qApp->setStyleSheet(styleSheet);
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
    if (name.isEmpty()) return false;
    for (const QChar &ch : name) {
        if (ch.isDigit()) return false;
    }
    return true;
}

bool MainWindow::isValidDate(const QDate &date)
{
    return date >= QDate::currentDate();
}

bool MainWindow::isValidDateTime(const QDateTime &dateTime)
{
    return dateTime >= QDateTime::currentDateTime();
}

bool MainWindow::isValidConsultant(const QString &consultant)
{
    if (consultant.isEmpty()) return false;
    bool ok;
    consultant.toInt(&ok);
    return ok;
}

void MainWindow::setupCalendarView()
{
    connect(ui->clientConsultationCalendar, &QCalendarWidget::selectionChanged,
            this, &MainWindow::on_clientConsultationCalendar_selectionChanged);
    connect(ui->clientConsultationCalendar, &QCalendarWidget::activated,
            this, &MainWindow::on_clientConsultationCalendar_activated);

    ui->clientConsultationCalendar->installEventFilter(this);
    ui->clientConsultationCalendar->setFirstDayOfWeek(Qt::Monday);
    updateCalendarConsultations();
    updateSelectedDateInfo(ui->clientConsultationCalendar->selectedDate());
}

void MainWindow::highlightDatesWithConsultations()
{
    ui->clientConsultationCalendar->setDateTextFormat(QDate(), QTextCharFormat());

    QTextCharFormat consultationFormat;
    consultationFormat.setBackground(QColor(100, 150, 255, 100));

    QMapIterator<QDate, int> i(consultationCountMap);
    while (i.hasNext()) {
        i.next();
        ui->clientConsultationCalendar->setDateTextFormat(i.key(), consultationFormat);
    }
}

void MainWindow::updateSelectedDateInfo(const QDate &date)
{
    ui->clientSelectedDateLabel->setText(QString("Selected date: %1").arg(date.toString("yyyy-MM-dd")));
    int count = consultationCountMap.value(date, 0);
    ui->clientConsultationCountLabel->setText(QString("Consultations: %1").arg(count));
    if (m_dbConnected) {
        QSqlQueryModel *model = client.getConsultationsForDate(date);
        ui->clientDateConsultationsView->setModel(model);
        ui->clientDateConsultationsView->resizeColumnsToContents();
    }
}

void MainWindow::performClientSearch()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot perform search: Database is not connected.");
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
        ui->clientTableView->setModel(model);
    }
}

void MainWindow::checkAndSendReminders()
{
    qDebug() << "Starting checkAndSendReminders()";

    QDateTime now = QDateTime::currentDateTime();
    QDateTime reminderEnd = now.addSecs(24 * 60 * 60);
    qDebug() << "Reminder range:" << now.toString() << "to" << reminderEnd.toString();

    QSqlQueryModel *model = client.getUpcomingConsultations(now, reminderEnd);
    if (!model) {
        qDebug() << "Model is null in checkAndSendReminders()";
        return;
    }
    int rowCount = model->rowCount();
    qDebug() << "Upcoming consultations count:" << rowCount;

    for (int row = 0; row < rowCount; ++row) {
        QString name = model->data(model->index(row, 0)).toString();
        QString email = model->data(model->index(row, 3)).toString();
        QDateTime consultationDate = model->data(model->index(row, 4)).toDateTime();
        qDebug() << "Processing reminder for:" << name << "Email:" << email << "Date:" << consultationDate.toString();

        if (!email.isEmpty()) {
            QString subject = "Consultation Reminder";
            QString body = QString("Dear %1,\n\nThis is a reminder for your consultation scheduled on %2.\n\nBest regards,\nClient Management System")
                               .arg(name)
                               .arg(consultationDate.toString("yyyy-MM-dd HH:mm"));

            emailAttempts++;
            if (emailSender->sendEmail(email, subject, body)) {
                emailSuccesses++;
                qDebug() << "Reminder sent to:" << email;
                ui->statusBar->showMessage("Reminder sent to " + email);
            } else {
                qDebug() << "Failed to send reminder to:" << email;
                ui->statusBar->showMessage("Failed to send reminder to " + email);
            }
        }
    }

    if (rowCount == 0) {
        ui->statusBar->showMessage("No upcoming consultations found.");
    }

    qDebug() << "checkAndSendReminders() completed";
}

bool MainWindow::calendarHoverEventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->clientConsultationCalendar && event->type() == QEvent::HoverMove) {
        QHoverEvent* hoverEvent = static_cast<QHoverEvent*>(event);
        QPoint pos = hoverEvent->position().toPoint();

        QTableView* tableView = ui->clientConsultationCalendar->findChild<QTableView*>();
        QDate hoverDate;

        if (tableView) {
            QModelIndex index = tableView->indexAt(pos);
            if (index.isValid()) {
                int row = index.row();
                int col = index.column();

                QDate firstDayOfMonth = ui->clientConsultationCalendar->selectedDate().addDays(-ui->clientConsultationCalendar->selectedDate().day() + 1);
                int offset = firstDayOfMonth.dayOfWeek() - 1;

                int day = (row * 7) + col - offset + 1;
                if (day > 0 && day <= firstDayOfMonth.daysInMonth()) {
                    hoverDate = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), day);
                } else {
                    hoverDate = ui->clientConsultationCalendar->selectedDate();
                }
            }
        } else {
            hoverDate = ui->clientConsultationCalendar->selectedDate();
        }

        if (hoverDate.isValid()) {
            int count = consultationCountMap.value(hoverDate, 0);
            if (count > 0) {
                QString tooltipText = QString("%1: %2 consultation(s)")
                .arg(hoverDate.toString("yyyy-MM-dd"))
                    .arg(count);
                QToolTip::showText(ui->clientConsultationCalendar->mapToGlobal(hoverEvent->position().toPoint()), tooltipText, ui->clientConsultationCalendar);
            } else {
                QToolTip::hideText();
            }
        }
    }
    return false;
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (calendarHoverEventFilter(watched, event)) {
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::updateClientStatisticsDisplay()
{
    QDate startDate = QDate::currentDate().addDays(-30);
    QDate endDate = QDate::currentDate();
    QDateTime start = QDateTime(startDate, QTime(0, 0, 0));
    QDateTime end = QDateTime(endDate, QTime(23, 59, 59));

    qDebug() << "Updating statistics for range:" << start.toString() << "to" << end.toString();

    int total = client.getTotalConsultations(start, end);
    int upcoming = client.getUpcomingConsultationsCount(start, end.addDays(1));
    int uniqueClients = client.getUniqueClients(start, end);
    double successRate = (emailAttempts > 0) ? (double)emailSuccesses / emailAttempts * 100 : 0;

    QMap<QDate, int> consultationsPerDay = client.getConsultationsPerDay(start, end);
    qDebug() << "Consultations per day size:" << consultationsPerDay.size();

    QString dayStats;
    for (QMap<QDate, int>::const_iterator it = consultationsPerDay.constBegin(); it != consultationsPerDay.constEnd(); ++it) {
        dayStats += QString("%1: %2 consultations<br>").arg(it.key().toString("yyyy-MM-dd"), QString::number(it.value()));
    }

    QString statsText = QString(
                            "<h3>Statistics (as of %1):</h3>"
                            "<p><b>Total Consultations:</b> %2</p>"
                            "<p><b>Upcoming Consultations (Next 24h):</b> %3</p>"
                            "<p><b>Unique Clients:</b> %4</p>"
                            "<p><b>Email Reminder Success Rate:</b> %5%</p>"
                            "<p><b>Consultations Per Day:</b><br>%6</p>"
                            ).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                            .arg(total)
                            .arg(upcoming)
                            .arg(uniqueClients)
                            .arg(successRate, 0, 'f', 2)
                            .arg(dayStats);

    if (ui->clientStatsDisplay) {
        ui->clientStatsDisplay->setHtml(statsText);
    } else {
        qDebug() << "statsDisplay is nullptr";
        QMessageBox::warning(this, "Error", "Statistics display text edit is not initialized.");
    }
}

void MainWindow::loadEmployees()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot load employees: Database is not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID, FIRST_NAME, LAST_NAME FROM AHMED.EMPLOYEE");
    if (!query.exec()) {
        qDebug() << "Error loading employees:" << query.lastError().text();
        return;
    }

    ui->clientConsultantComboBox->clear();
    employeeMap.clear();
    ui->clientConsultantComboBox->addItem("Select Consultant...");

    while (query.next()) {
        QString id = query.value("ID").toString();
        QString firstName = query.value("FIRST_NAME").toString();
        QString lastName = query.value("LAST_NAME").toString();
        QString fullName = QString("%1 %2").arg(firstName, lastName).trimmed();
        employeeMap[fullName] = id;
        ui->clientConsultantComboBox->addItem(fullName);
        qDebug() << "Added employee:" << fullName << "with ID:" << id;
    }

    if (employeeMap.isEmpty()) {
        ui->clientConsultantComboBox->addItem("No employees found");
        ui->clientConsultantComboBox->setEnabled(false);
        qDebug() << "No employees found in the database.";
    } else {
        ui->clientConsultantComboBox->setEnabled(true);
    }
}

void MainWindow::refreshClientTable()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot refresh table: Database is not connected.");
        return;
    }

    QSqlQueryModel *model = client.afficher();
    ui->clientTableView->setModel(model);
    ui->clientTableView->resizeColumnsToContents();
    for (int i = 0; i < model->columnCount(); ++i) {
        ui->clientTableView->showColumn(i);
    }
}

void MainWindow::updateCalendarConsultations()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot update calendar: Database is not connected.");
        return;
    }

    QMap<QDateTime, int> dtMap = client.getConsultationCountsByDateTime();
    consultationCountMap.clear();

    for (auto it = dtMap.begin(); it != dtMap.end(); ++it) {
        QDate date = it.key().date();
        consultationCountMap[date] += it.value();
    }

    highlightDatesWithConsultations();
}

void MainWindow::refreshTrainingTableView()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot load formation data: Database is not connected.");
        return;
    }

    if (trainingTableModel) {
        delete trainingTableModel;
        trainingTableModel = nullptr;
    }
    trainingTableModel = new QSqlQueryModel(this);
    QSqlQuery query;
    query.exec("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS");
    trainingTableModel->setQuery(std::move(query));
    if (trainingTableModel->lastError().isValid()) {
        qDebug() << "SQL Error:" << trainingTableModel->lastError().text();
        QMessageBox::warning(this, "Error", "Failed to load formation data: " + trainingTableModel->lastError().text());
        return;
    }
    qDebug() << "RefreshTrainingTableView - Row count:" << trainingTableModel->rowCount()
             << "Column count:" << trainingTableModel->columnCount();
    if (!trainingProxyModel) {
        qDebug() << "Error: trainingProxyModel is not initialized";
        QMessageBox::critical(this, "Error", "Table view proxy model is not initialized.");
        return;
    }
    trainingProxyModel->setSourceModel(trainingTableModel);
    ui->trainingTableView->setModel(trainingProxyModel);
    ui->trainingTableView->resizeColumnsToContents();
}

void MainWindow::updateTrainingStatistics()
{
    qDebug() << "Starting updateTrainingStatistics";

    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot update statistics: Database is not connected.");
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS total_count, SUM(PRIX) AS total_cost FROM AHMED.FORMATIONS");

    if (!query.exec()) {
        qDebug() << "Statistics query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to retrieve statistics: " + query.lastError().text());
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
        return;
    }

    if (query.next()) {
        totalFormations = query.value("total_count").toInt();
        totalCost = query.value("total_cost").toDouble();
        avgCost = (totalFormations > 0) ? totalCost / totalFormations : 0.0;
    } else {
        qDebug() << "No statistics data retrieved";
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
    }

    qDebug() << "Updated statistics - Total Formations:" << totalFormations
             << "Total Cost:" << totalCost
             << "Average Cost:" << avgCost;

    QString statsText = QString(
                            "<h3>Training Statistics (as of %1):</h3>"
                            "<p><b>Total Formations:</b> %2</p>"
                            "<p><b>Total Cost:</b> $%3</p>"
                            "<p><b>Average Cost:</b> $%4</p>"
                            ).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                            .arg(totalFormations)
                            .arg(totalCost, 0, 'f', 2)
                            .arg(avgCost, 0, 'f', 2);

    // Comment out until trainingStatsDisplay is added to the UI
    /*
    if (ui->trainingStatsDisplay) {
        ui->trainingStatsDisplay->setHtml(statsText);
    } else {
        qDebug() << "trainingStatsDisplay is nullptr";
        QMessageBox::warning(this, "Error", "Training statistics display text edit is not initialized.");
    }
    */
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

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();
    networkManager->post(request, postData);
}

void MainWindow::addChangeToHistory(const QString &action, const QString &formationName)
{
    QString change = QString("%1: %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), action, formationName);
    changeHistory.prepend(change);

    if (changeHistory.size() > 10) {
        changeHistory.removeLast();
    }

    QString historyText;
    for (const QString &entry : changeHistory) {
        historyText += entry + "<br>";
    }

    // Comment out until trainingChangeHistoryDisplay is added to the UI
    /*
    if (ui->trainingChangeHistoryDisplay) {
        ui->trainingChangeHistoryDisplay->setHtml(historyText);
    } else {
        qDebug() << "changeHistoryDisplay is nullptr";
        QMessageBox::warning(this, "Error", "Change history display text edit is not initialized.");
    }
    */
}

