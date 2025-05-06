// managers/client/clientmanager.cpp
#include "clientmanager.h"
#include "ui_mainwindow.h"
#include "dialog/updateclientdialog/updateclientdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTimeEdit>
#include <QGridLayout>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlError>
#include <QFileDialog>
#include <QSqlQuery>

ClientManager::ClientManager(bool dbConnected, QObject *parent)
    : QObject(parent),
    m_dbConnected(dbConnected),
    ui(nullptr),
    client(new Client()),
    chartWindow(nullptr),
    emailSender(new EmailSender(this)),
    clientTableModel(new QSqlQueryModel(this)),
    clientProxyModel(new QSortFilterProxyModel(this)),
    emailAttempts(0),
    emailSuccesses(0),
    lastCalendarUpdate(QDateTime::currentDateTime()),
    notificationManager(nullptr)
{
}

ClientManager::~ClientManager()
{
    delete client;
    delete chartWindow;
    delete emailSender;
    delete clientTableModel;
    delete clientProxyModel;
}

void ClientManager::setNotificationManager(NotificationManager *manager)
{
    notificationManager = manager;
}

void ClientManager::initialize(Ui::MainWindow *ui)
{
    qDebug() << "Entering ClientManager::initialize";
    this->ui = ui;
    if (!ui || !ui->clientTableView || !ui->clientAddTab) {
        qDebug() << "Error: ui or required widgets are null";
        return;
    }

    // Initialize client input form
    QDateTimeEdit *dateTimeEdit = new QDateTimeEdit;
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
        qDebug() << "Error: clientAddTab layout is not a QGridLayout";
    }

    // Block signals during table setup
    ui->clientTableView->blockSignals(true);
    ui->clientDateConsultationsView->blockSignals(true);

    // Initialize client table view
    clientProxyModel->setSourceModel(clientTableModel);
    ui->clientTableView->setModel(clientProxyModel);
    ui->clientTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->clientTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clientTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->clientTableView->verticalHeader()->setVisible(false);

    // Initialize client date consultations view with a separate model
    QSqlQueryModel *dateConsultationsModel = new QSqlQueryModel(this);
    ui->clientDateConsultationsView->setModel(dateConsultationsModel);
    ui->clientDateConsultationsView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->clientDateConsultationsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->clientDateConsultationsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->clientDateConsultationsView->verticalHeader()->setVisible(false);

    // Connect signals
    connect(ui->clientAddButton, &QPushButton::clicked, this, &ClientManager::on_clientAddButton_clicked);
    connect(ui->clientDeleteButton, &QPushButton::clicked, this, &ClientManager::on_clientDeleteButton_clicked);
    connect(ui->clientUpdateButton, &QPushButton::clicked, this, &ClientManager::on_clientUpdateButton_clicked);
    connect(ui->clientSearchInput, &QLineEdit::textChanged, this, &ClientManager::on_clientSearchInput_textChanged);
    connect(ui->clientSearchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientManager::on_clientSearchCriteriaComboBox_currentIndexChanged);
    connect(ui->clientResetSearchButton, &QPushButton::clicked, this, &ClientManager::on_clientResetSearchButton_clicked);
    connect(ui->clientTableView->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &ClientManager::on_clientTableViewHeader_clicked);
    connect(ui->clientConsultationCalendar, &QCalendarWidget::selectionChanged,
            this, &ClientManager::on_clientConsultationCalendar_selectionChanged);
    connect(ui->clientConsultationCalendar, &QCalendarWidget::activated,
            this, &ClientManager::on_clientConsultationCalendar_activated);
    connect(ui->clientExportPdfButton, &QPushButton::clicked, this, &ClientManager::on_clientExportPdfButton_clicked);
    connect(ui->clientExportPdfButton, &QPushButton::clicked, this, &ClientManager::sendConsultationReminders);

    // Re-enable signals
    ui->clientTableView->blockSignals(false);
    ui->clientDateConsultationsView->blockSignals(false);

    if (m_dbConnected) {
        loadEmployees();
        refreshClientTable();
        setupCalendarView();
        checkAndSendReminders();
    }

    qDebug() << "Exiting ClientManager::initialize";
}

void ClientManager::refresh()
{
    if (m_dbConnected) {
        refreshClientTable();
        updateCalendarConsultations();
    }
}

void ClientManager::showStatistics()
{
    if (!m_dbConnected) {
        return;
    }
    if (!chartWindow) {
        chartWindow = new ChartWindow(qobject_cast<QWidget*>(parent()));
    }
    chartWindow->show();
}

void ClientManager::on_clientAddButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot add client: Database is not connected.");
        return;
    }
    if (!validateClientInputs()) {
        return;
    }
    QString name = ui->clientNameInput->text().trimmed();
    QString sector = ui->clientSectorInput->text().trimmed();
    QString contact = ui->clientContactInfoInput->text().trimmed();
    QString email = ui->clientEmailInput->text().trimmed();
    QDateTimeEdit *dateTimeEdit = ui->clientAddTab->findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime dateTime = dateTimeEdit ? dateTimeEdit->dateTime() : QDateTime::currentDateTime();
    
    // Get the consultant ID directly from the combo box's item data
    int currentIndex = ui->clientConsultantComboBox->currentIndex();
    QString consultant = ui->clientConsultantComboBox->itemData(currentIndex).toString();
    
    if (consultant.isEmpty() || consultant == "-1") {
        QMessageBox::warning(nullptr, "Input Error", "Please select a valid consultant.");
        return;
    }
    
    auto result = client->ajouter(name, sector, contact, email, dateTime, consultant);
    if (result.first) {
        QMessageBox::information(nullptr, "Success", "Client added successfully!");
        refreshClientTable();
        clientProxyModel->setFilterRegularExpression(""); // Reset filter
        clientProxyModel->sort(-1); // Reset sorting
        ui->clientTableView->viewport()->update(); // Force repaint
        int newRow = clientTableModel->rowCount() - 1; // Last row
        if (notificationManager) {
            notificationManager->addNotification("Added Client", "Client Section", QString("Client: %1").arg(name), newRow);
        }
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
        QMessageBox::critical(nullptr, "Error", "Failed to add client: " + result.second);
    }
}

void ClientManager::on_clientDeleteButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot delete client: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->clientTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(nullptr, "Selection Error", "Please select a client to delete.");
        return;
    }
    int row = selectedRows[0].row();
    QString clientName = clientProxyModel->data(clientProxyModel->index(row, 0)).toString();
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr, "Confirm Delete",
        QString("Are you sure you want to delete client '%1'?").arg(clientName),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (client->removeByName(clientName)) {
            QMessageBox::information(nullptr, "Success", "Client deleted successfully!");
            if (notificationManager) {
                notificationManager->addNotification("Deleted Client", "Client Section", QString("Client: %1").arg(clientName), row);
            }
            refreshClientTable();
            clientProxyModel->setFilterRegularExpression(""); // Reset filter
            clientProxyModel->sort(-1); // Reset sorting
            ui->clientTableView->viewport()->update(); // Force repaint
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(nullptr, "Error", "Failed to delete client.");
        }
    }
}

void ClientManager::on_clientUpdateButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot update client: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->clientTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(nullptr, "Selection Error", "Please select a client to update.");
        return;
    }
    int row = selectedRows[0].row();
    QString name = clientProxyModel->data(clientProxyModel->index(row, 0)).toString();
    QString sector = clientProxyModel->data(clientProxyModel->index(row, 1)).toString();
    QString contact = clientProxyModel->data(clientProxyModel->index(row, 2)).toString();
    QString email = clientProxyModel->data(clientProxyModel->index(row, 3)).toString();
    QDateTime consultationDate = clientProxyModel->data(clientProxyModel->index(row, 4)).toDateTime();
    QString consultantName = clientProxyModel->data(clientProxyModel->index(row, 5)).toString();
    QString consultantId = employeeMap.key(consultantName, "-1");

    UpdateClientDialog dialog(nullptr);
    dialog.setClientData(name, sector, contact, email, consultationDate, consultantId);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getName();
        QString newSector = dialog.getSector();
        QString newContact = dialog.getContactInfo();
        QString newEmail = dialog.getEmail();
        QDateTime newDateTime = dialog.getConsultationDateTime();
        QString newConsultant = dialog.getConsultant();

        if (updateClient(name, newName, newSector, newContact, newEmail, newDateTime, newConsultant)) {
            QMessageBox::information(nullptr, "Success", "Client updated successfully!");
            if (notificationManager) {
                notificationManager->addNotification("Updated Client", "Client Section", QString("Client: %1 updated to %2").arg(name, newName), row);
            }
            refreshClientTable();
            clientProxyModel->setFilterRegularExpression(""); // Reset filter
            clientProxyModel->sort(-1); // Reset sorting
            ui->clientTableView->viewport()->update(); // Force repaint
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(nullptr, "Error", "Failed to update client.");
        }
    }
}

void ClientManager::on_clientSearchInput_textChanged(const QString &/*text*/)
{
    if (!m_dbConnected) {
        return;
    }
    performClientSearch();
}

void ClientManager::on_clientSearchCriteriaComboBox_currentIndexChanged(int /*index*/)
{
    if (!m_dbConnected) {
        return;
    }
    performClientSearch();
}

void ClientManager::on_clientResetSearchButton_clicked()
{
    if (!m_dbConnected) {
        return;
    }
    ui->clientSearchInput->clear();
    clientProxyModel->setFilterRegularExpression("");
    refreshClientTable();
    clientProxyModel->sort(-1); // Reset sorting
    ui->clientTableView->viewport()->update(); // Force repaint
}

void ClientManager::on_clientTableViewHeader_clicked(int logicalIndex)
{
    qDebug() << "Entering on_clientTableViewHeader_clicked, logicalIndex:" << logicalIndex;
    if (!m_dbConnected || !clientProxyModel) {
        qDebug() << "Error: Database not connected or proxy model is null";
        return;
    }

    // Get current sort order or toggle if same column is clicked
    Qt::SortOrder order = Qt::AscendingOrder;
    if (ui->clientTableView->horizontalHeader()->sortIndicatorSection() == logicalIndex) {
        order = (ui->clientTableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        ? Qt::DescendingOrder : Qt::AscendingOrder;
    }

    // Update the sort indicator
    ui->clientTableView->horizontalHeader()->setSortIndicator(logicalIndex, order);

    // Use the Client::sortByColumn method for enhanced sorting
    QSqlQueryModel* sortedModel = client->sortByColumn(logicalIndex, order);
    
    // Fix for deprecation warning - use the setQuery method with a QString instead
    QString queryString = sortedModel->query().lastQuery();
    clientTableModel->setQuery(queryString);
    
    delete sortedModel;

    // Make sure the proxy model reflects the changes
    clientProxyModel->setSourceModel(clientTableModel);
    ui->clientTableView->setModel(clientProxyModel);

    // Apply headers and formatting
    setClientTableHeaders();

    qDebug() << "Exiting on_clientTableViewHeader_clicked";
}

void ClientManager::on_clientConsultationCalendar_selectionChanged()
{
    if (!m_dbConnected) {
        return;
    }
    QDate selectedDate = ui->clientConsultationCalendar->selectedDate();
    updateSelectedDateInfo(selectedDate);
}

void ClientManager::on_clientConsultationCalendar_activated(const QDate &date)
{
    if (!m_dbConnected) {
        return;
    }
    updateSelectedDateInfo(date);
}

void ClientManager::on_clientExportPdfButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }
    exportClientsToPdf();
}

void ClientManager::sendConsultationReminders()
{
    if (!m_dbConnected) {
        return;
    }
    checkAndSendReminders();
}

bool ClientManager::validateClientInputs()
{
    QString name = ui->clientNameInput->text().trimmed();
    QString email = ui->clientEmailInput->text().trimmed();
    QDateTimeEdit *dateTimeEdit = ui->clientAddTab->findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime dateTime = dateTimeEdit ? dateTimeEdit->dateTime() : QDateTime::currentDateTime();

    if (name.isEmpty() || ui->clientSectorInput->text().trimmed().isEmpty() ||
        ui->clientContactInfoInput->text().trimmed().isEmpty() || email.isEmpty()) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in all fields.");
        return false;
    }
    if (!isValidName(name)) {
        QMessageBox::warning(nullptr, "Input Error", "Invalid name format.");
        return false;
    }
    if (!isValidEmail(email)) {
        QMessageBox::warning(nullptr, "Input Error", "Invalid email format.");
        return false;
    }
    if (!isValidDateTime(dateTime)) {
        QMessageBox::warning(nullptr, "Input Error", "Consultation date must be in the future.");
        return false;
    }
    return true;
}

bool ClientManager::isValidName(const QString &name)
{
    QRegularExpression rx("[A-Za-z\\s]{1,50}");
    return rx.match(name).hasMatch();
}

bool ClientManager::isValidEmail(const QString &email)
{
    QRegularExpression rx("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
    return rx.match(email).hasMatch();
}

bool ClientManager::isValidDateTime(const QDateTime &dateTime)
{
    return dateTime.isValid() && dateTime > QDateTime::currentDateTime();
}

void ClientManager::setupCalendarView()
{
    ui->clientConsultationCalendar->setGridVisible(true);
    ui->clientConsultationCalendar->installEventFilter(this);
    highlightDatesWithConsultations();
}

void ClientManager::highlightDatesWithConsultations()
{
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QBrush(QColor("#00BCD4"))); // Distinct cyan
    highlightFormat.setForeground(Qt::white);
    highlightFormat.setFontWeight(QFont::Bold);
    highlightFormat.setFontUnderline(true);
    highlightFormat.setUnderlineColor(QColor("#FF1744")); // Red underline
    highlightFormat.setTextOutline(QPen(QColor("#FF1744"), 2)); // Outline for extra visibility

    QSqlQuery query;
    query.prepare("SELECT DISTINCT TRUNC(CONSULTATION_DATE) AS consult_date FROM AHMED.CLIENTS");
    if (query.exec()) {
        while (query.next()) {
            QDate date = query.value("consult_date").toDate();
            ui->clientConsultationCalendar->setDateTextFormat(date, highlightFormat);
        }
    } else {
        qDebug() << "Failed to highlight consultation dates:" << query.lastError().text();
    }

    QMap<QDate, int> consultationCounts = client->getConsultationCountsByDate();
    for (auto it = consultationCounts.begin(); it != consultationCounts.end(); ++it) {
        if (it.value() > 0) {
            ui->clientConsultationCalendar->setDateTextFormat(it.key(), highlightFormat);
        }
    }
}

void ClientManager::updateSelectedDateInfo(const QDate &date)
{
    ui->clientSelectedDateLabel->setText(QString("Selected date: %1").arg(date.toString("yyyy-MM-dd")));

    QSqlQueryModel *dateConsultationsModel = qobject_cast<QSqlQueryModel*>(ui->clientDateConsultationsView->model());
    if (dateConsultationsModel) {
        dateConsultationsModel->setQuery(QString(
                                             "SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.CONSULTATION_DATE, (e.FIRST_NAME || ' ' || e.LAST_NAME) AS CONSULTANT_NAME "
                                             "FROM AHMED.CLIENTS c "
                                             "JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                                             "WHERE TRUNC(c.CONSULTATION_DATE) = TO_DATE('%1', 'YYYY-MM-DD')"
                                             ).arg(date.toString("yyyy-MM-dd")));

        if (dateConsultationsModel->lastError().isValid()) {
            qDebug() << "Consultation query failed:" << dateConsultationsModel->lastError().text();
            QMessageBox::warning(nullptr, "Query Error",
                                 "Failed to load consultations: " + dateConsultationsModel->lastError().text());
            return;
        }

        dateConsultationsModel->setHeaderData(0, Qt::Horizontal, "Client Name");
        dateConsultationsModel->setHeaderData(1, Qt::Horizontal, "Sector");
        dateConsultationsModel->setHeaderData(2, Qt::Horizontal, "Contact Info");
        dateConsultationsModel->setHeaderData(3, Qt::Horizontal, "Consultation Date");
        dateConsultationsModel->setHeaderData(4, Qt::Horizontal, "Consultant");

        int count = dateConsultationsModel->rowCount();
        ui->clientConsultationCountLabel->setText(QString("Consultations: %1").arg(count));
    }
}

void ClientManager::performClientSearch()
{
    QString searchText = ui->clientSearchInput->text().trimmed();
    QString criteria = ui->clientSearchCriteriaComboBox->currentText();

    QString column;
    if (criteria == "Name") {
        column = "NAME";
    } else if (criteria == "Sector") {
        column = "SECTOR";
    } else if (criteria == "Email") {
        column = "EMAIL";
    } else if (criteria == "Contact") {
        column = "CONTACT_INFO";
    } else if (criteria == "Date Time") {
        column = "CONSULTATION_DATE";
    } else if (criteria == "Consultant") {
        column = "CONSULTANT_NAME";
    } else {
        column = "NAME";
    }

    clientProxyModel->setFilterKeyColumn(clientTableModel->record().indexOf(column));
    clientProxyModel->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(searchText),
                                                                    QRegularExpression::CaseInsensitiveOption));
}

void ClientManager::checkAndSendReminders()
{
    qDebug() << "Entering checkAndSendReminders";
    if (!m_dbConnected) {
        qDebug() << "Error: Database not connected";
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT NAME, EMAIL, CONSULTATION_DATE "
                  "FROM AHMED.CLIENTS "
                  "WHERE CONSULTATION_DATE BETWEEN SYSDATE AND SYSDATE + 1");
    if (!query.exec()) {
        qDebug() << "Reminder query failed:" << query.lastError().text();
        return;
    }
    while (query.next()) {
        QString name = query.value("NAME").toString();
        QString email = query.value("EMAIL").toString();
        QDateTime consultationDate = query.value("CONSULTATION_DATE").toDateTime();
        qDebug() << "Sending reminder to" << email << "for consultation on" << consultationDate;
        QString subject = "Consultation Reminder";
        QString body = QString("Dear %1,\n\nThis is a reminder for your consultation scheduled on %2.\n\nBest regards,\nTraining Management")
                           .arg(name, consultationDate.toString("yyyy-MM-dd HH:mm"));
        emailSender->sendEmail(email, subject, body);
        emailAttempts++;
        if (emailSender->isLastSendSuccessful()) {
            emailSuccesses++;
        } else {
            qDebug() << "Failed to send email to" << email;
        }
    }
    qDebug() << "Exiting checkAndSendReminders, attempts:" << emailAttempts << ", successes:" << emailSuccesses;
}

bool ClientManager::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->clientConsultationCalendar && event->type() == QEvent::Paint) {
        if (QDateTime::currentDateTime() >= lastCalendarUpdate.addMSecs(60000)) {
            highlightDatesWithConsultations();
            lastCalendarUpdate = QDateTime::currentDateTime();
        }
    }
    return QObject::eventFilter(watched, event);
}

void ClientManager::loadEmployees()
{
    qDebug() << "Entering loadEmployees";
    try {
        // Clear existing employee data
        employeeMap.clear();
        
        // Clear and repopulate the consultant combo box
        if (ui && ui->clientConsultantComboBox) {
            ui->clientConsultantComboBox->clear();
        }
        
        QSqlQuery query;
        
        // Prepare the query first to avoid function sequence errors
        if (!query.prepare("SELECT ID, FIRST_NAME, LAST_NAME FROM AHMED.EMPLOYEE")) {
            qDebug() << "Error preparing employee query:" << query.lastError().text();
            throw std::runtime_error("Failed to prepare employee query: " + query.lastError().text().toStdString());
        }
        
        // Execute the prepared query
        if (!query.exec()) {
            qDebug() << "QODBCResult::exec: Unable to execute statement:" << query.lastError().text();
            throw std::runtime_error("Error in loadEmployees: " + query.lastError().text().toStdString());
        }
        
        // Process results
        while (query.next()) {
            int id = query.value(0).toInt();
            QString firstName = query.value(1).toString();
            QString lastName = query.value(2).toString();
            QString fullName = firstName + " " + lastName;
            
            // Store in map for later lookup
            employeeMap[QString::number(id)] = fullName;
            
            // Add to the combo box if it exists
            if (ui && ui->clientConsultantComboBox) {
                ui->clientConsultantComboBox->addItem(fullName, QString::number(id));
            }
        }
        
        qDebug() << "Loaded" << employeeMap.size() << "employees successfully";
        if (ui && ui->clientConsultantComboBox) {
            qDebug() << "Combo box now has" << ui->clientConsultantComboBox->count() << "items";
        }
    } catch (const std::exception& e) {
        qDebug() << "Error in loadEmployees:" << e.what();
    }
    qDebug() << "Exiting loadEmployees";
}

void ClientManager::refreshClientTable()
{
    qDebug() << "Entering refreshClientTable";
    clientTableModel->setQuery(
        "SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
        "(e.FIRST_NAME || ' ' || e.LAST_NAME) AS CONSULTANT_NAME "
        "FROM AHMED.CLIENTS c "
        "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID"
        );

    if (clientTableModel->lastError().isValid()) {
        qDebug() << "Client query failed:" << clientTableModel->lastError().text();
        // Avoid QMessageBox during initialization
        return;
    }

    qDebug() << "Client count:" << clientTableModel->rowCount();

    ui->clientTableView->setModel(clientProxyModel);
    ui->clientTableView->setSortingEnabled(true);
    ui->clientTableView->horizontalHeader()->setSortIndicatorShown(true);
    ui->clientTableView->horizontalHeader()->setSectionsClickable(true);
    QObject::connect(ui->clientTableView->horizontalHeader(), &QHeaderView::sectionClicked, [this](int logicalIndex) {
        this->on_clientTableViewHeader_clicked(logicalIndex);
    });
    // Hide ID column if present (should be column 0 if exists)
    if (ui->clientTableView->model()->columnCount() > 0 && ui->clientTableView->model()->headerData(0, Qt::Horizontal).toString().toLower().contains("id")) {
        ui->clientTableView->hideColumn(0);
    }
    // Apply improved table display for readability
    if (auto mainWindow = qobject_cast<QMainWindow*>(ui->clientTableView->window())) {
        if (mainWindow->metaObject()->indexOfMethod("improveTableDisplay(QTableView*)") != -1) {
            QMetaObject::invokeMethod(mainWindow, "improveTableDisplay", Q_ARG(QTableView*, ui->clientTableView));
        }
    }
    setClientTableHeaders();

    qDebug() << "Exiting refreshClientTable";
}

void ClientManager::setClientTableHeaders()
{
    // Only set headers for visible columns (skip ID)
    clientTableModel->setHeaderData(0, Qt::Horizontal, "Name");
    clientTableModel->setHeaderData(1, Qt::Horizontal, "Sector");
    clientTableModel->setHeaderData(2, Qt::Horizontal, "Contact Info");
    clientTableModel->setHeaderData(3, Qt::Horizontal, "Email");
    clientTableModel->setHeaderData(4, Qt::Horizontal, "Consultation Date");
    clientTableModel->setHeaderData(5, Qt::Horizontal, "Consultant");
    // Hide ID column if present
    if (clientTableModel->headerData(0, Qt::Horizontal).toString().toLower().contains("id")) {
        if (ui->clientTableView) ui->clientTableView->hideColumn(0);
    }
}

bool ClientManager::updateClient(const QString &originalName, const QString &newName, const QString &sector,
                                 const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime,
                                 const QString &consultant)
{
    if (!isValidName(newName) || !isValidEmail(email) || !isValidDateTime(consultationDateTime)) {
        return false;
    }
    QSqlQuery query;
    query.prepare("UPDATE AHMED.CLIENTS "
                  "SET NAME = :newName, SECTOR = :sector, CONTACT_INFO = :contactInfo, "
                  "EMAIL = :email, CONSULTATION_DATE = TO_TIMESTAMP(:consultationDate, 'YYYY-MM-DD HH24:MI:SS'), "
                  "CONSULTANT_ID = :consultant "
                  "WHERE NAME = :originalName");
    query.bindValue(":newName", newName);
    query.bindValue(":sector", sector);
    query.bindValue(":contactInfo", contactInfo);
    query.bindValue(":email", email);
    query.bindValue(":consultationDate", consultationDateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":consultant", consultant);
    query.bindValue(":originalName", originalName);
    return query.exec();
}

void ClientManager::updateCalendarConsultations()
{
    highlightDatesWithConsultations();
    updateSelectedDateInfo(ui->clientConsultationCalendar->selectedDate());
}

void ClientManager::exportClientsToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Export PDF", "", "PDF Files (*.pdf)");
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
    QString headerText = QString("%1\nGenerated on: %2").arg(title, date);
    painter.drawText(QRectF(margin, margin, pageWidth, 50 * scaleFactor), Qt::AlignCenter, headerText);

    qreal yPos = margin + 60 * scaleFactor;

    QStringList headers = {"Name", "Sector", "Contact", "Email", "Date & Time", "Consultant"};
    qreal columnWidths[] = {1.5, 1.5, 1.2, 2.8, 1.5, 1.6};
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

    int rowCount = clientProxyModel->rowCount();
    int colCount = headers.size();
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
            QString text = clientProxyModel->data(clientProxyModel->index(row, col)).toString();
            if (col == 4 && !text.isEmpty()) {
                QDateTime dateTime = clientProxyModel->data(clientProxyModel->index(row, col)).toDateTime();
                if (dateTime.isValid()) {
                    text = dateTime.toString("yyyy-MM-dd HH:mm");
                }
            }
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
        int totalConsultations = clientProxyModel->rowCount(); // Placeholder, replace with actual method if available
        int uniqueClients = clientProxyModel->rowCount(); // Placeholder, replace with actual method if available
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
        int totalConsultations = clientProxyModel->rowCount(); // Placeholder, replace with actual method if available
        int uniqueClients = clientProxyModel->rowCount(); // Placeholder, replace with actual method if available
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
    QMessageBox::information(nullptr, "Success", "PDF exported successfully to " + fileName);
    if (notificationManager) {
        notificationManager->addNotification("PDF Exported", "Client Section", "Client list exported to " + fileName, -1);
    }
}

QMap<QString, int> ClientManager::getStatisticsByCategory(const QString &category)
{
    QMap<QString, int> statistics;
    
    if (!m_dbConnected) {
        return statistics;
    }
    
    QSqlQuery query;
    
    if (category == "Sector") {
        query.prepare("SELECT SECTOR, COUNT(*) as count FROM AHMED.CLIENTS GROUP BY SECTOR ORDER BY count DESC");
    } 
    else if (category == "Consultant") {
        query.prepare("SELECT (e.FIRST_NAME || ' ' || e.LAST_NAME) AS consultant, COUNT(*) as count "
                     "FROM AHMED.CLIENTS c "
                     "JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                     "GROUP BY e.FIRST_NAME, e.LAST_NAME "
                     "ORDER BY count DESC");
    }
    else if (category == "Month") {
        query.prepare("SELECT TO_CHAR(CONSULTATION_DATE, 'MONTH') as month, COUNT(*) as count "
                     "FROM AHMED.CLIENTS "
                     "GROUP BY TO_CHAR(CONSULTATION_DATE, 'MONTH') "
                     "ORDER BY count DESC");
    }
    else if (category == "Date") {
        // Default case - by day
        query.prepare("SELECT TO_CHAR(CONSULTATION_DATE, 'YYYY-MM-DD') as day, COUNT(*) as count "
                     "FROM AHMED.CLIENTS "
                     "GROUP BY TO_CHAR(CONSULTATION_DATE, 'YYYY-MM-DD') "
                     "ORDER BY day ASC");
    }
    else {
        // Fallback if category doesn't match
        query.prepare("SELECT TO_CHAR(CONSULTATION_DATE, 'YYYY-MM-DD') as day, COUNT(*) as count "
                     "FROM AHMED.CLIENTS "
                     "GROUP BY TO_CHAR(CONSULTATION_DATE, 'YYYY-MM-DD') "
                     "ORDER BY day ASC");
    }
    
    if (query.exec()) {
        while (query.next()) {
            QString key = query.value(0).toString().trimmed();
            int value = query.value(1).toInt();
            statistics[key] = value;
        }
    } else {
        qDebug() << "Error getting statistics by category:" << query.lastError().text();
    }
    
    return statistics;
}
