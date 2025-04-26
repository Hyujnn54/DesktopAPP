#include "ClientManager.h"
#include "ui_mainwindow.h"
#include "UpdateClientDialog.h"
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
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

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
    lastCalendarUpdate(QDateTime::currentDateTime())
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

void ClientManager::initialize(Ui::MainWindow *ui)
{
    this->ui = ui;

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
    }

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

    // Set initial notification button text
    ui->trainingNotificationLabel->setText("0 modifications");

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
    connect(ui->reportsButton, &QPushButton::clicked, this, &ClientManager::sendConsultationReminders);
    connect(ui->trainingNotificationLabel, &QPushButton::clicked, this, &ClientManager::on_trainingNotificationLabel_clicked);

    if (m_dbConnected) {
        loadEmployees();
        refreshClientTable();
        setupCalendarView();
        checkAndSendReminders();
    }
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
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot add client: Database is not connected.");
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
    QString consultantName = ui->clientConsultantComboBox->currentText();
    QString consultant = employeeMap.value(consultantName, "-1");

    if (consultant == "-1") {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Please select a valid consultant.");
        return;
    }
    auto result = client->ajouter(name, sector, contact, email, dateTime, consultant);
    if (result.first) {
        QMessageBox::information(qobject_cast<QWidget*>(parent()), "Success", "Client added successfully!");
        refreshClientTable();
        clientProxyModel->setFilterRegularExpression(""); // Reset filter
        clientProxyModel->sort(-1); // Reset sorting
        ui->clientTableView->viewport()->update(); // Force repaint
        int newRow = clientTableModel->rowCount() - 1; // Last row
        logNotification("Added Client", "Client Section", QString("Client: %1").arg(name), newRow);
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
        QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Failed to add client: " + result.second);
    }
}

void ClientManager::on_clientDeleteButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot delete client: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->clientTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Selection Error", "Please select a client to delete.");
        return;
    }
    int row = selectedRows[0].row();
    QString clientName = clientProxyModel->data(clientProxyModel->index(row, 0)).toString();
    QMessageBox::StandardButton reply = QMessageBox::question(
        qobject_cast<QWidget*>(parent()), "Confirm Delete",
        QString("Are you sure you want to delete client '%1'?").arg(clientName),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (client->removeByName(clientName)) {
            QMessageBox::information(qobject_cast<QWidget*>(parent()), "Success", "Client deleted successfully!");
            logNotification("Deleted Client", "Client Section", QString("Client: %1").arg(clientName), row);
            refreshClientTable();
            clientProxyModel->setFilterRegularExpression(""); // Reset filter
            clientProxyModel->sort(-1); // Reset sorting
            ui->clientTableView->viewport()->update(); // Force repaint
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Failed to delete client.");
        }
    }
}

void ClientManager::on_clientUpdateButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot update client: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->clientTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Selection Error", "Please select a client to update.");
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

    UpdateClientDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.setClientData(name, sector, contact, email, consultationDate, consultantId);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getName();
        QString newSector = dialog.getSector();
        QString newContact = dialog.getContactInfo();
        QString newEmail = dialog.getEmail();
        QDateTime newDateTime = dialog.getConsultationDateTime();
        QString newConsultant = dialog.getConsultant();

        if (updateClient(name, newName, newSector, newContact, newEmail, newDateTime, newConsultant)) {
            QMessageBox::information(qobject_cast<QWidget*>(parent()), "Success", "Client updated successfully!");
            logNotification("Updated Client", "Client Section", QString("Client: %1 updated to %2").arg(name, newName), row);
            refreshClientTable();
            clientProxyModel->setFilterRegularExpression(""); // Reset filter
            clientProxyModel->sort(-1); // Reset sorting
            ui->clientTableView->viewport()->update(); // Force repaint
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Failed to update client.");
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
    if (!m_dbConnected) {
        return;
    }
    Qt::SortOrder order = ui->clientTableView->horizontalHeader()->sortIndicatorOrder();
    clientProxyModel->sort(logicalIndex, order);
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
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot export to PDF: Database is not connected.");
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

void ClientManager::on_trainingNotificationLabel_clicked()
{
    if (notifications.isEmpty()) {
        QMessageBox::information(qobject_cast<QWidget*>(parent()), "Notifications", "No modifications recorded.");
        return;
    }

    QDialog *notificationDialog = new QDialog(qobject_cast<QWidget*>(parent()));
    notificationDialog->setWindowTitle("Modification History");
    notificationDialog->setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(notificationDialog);
    QTextEdit *textEdit = new QTextEdit(notificationDialog);
    textEdit->setReadOnly(true);
    QPushButton *closeButton = new QPushButton("Close", notificationDialog);

    layout->addWidget(textEdit);
    layout->addWidget(closeButton);

    QString notificationText;
    for (const Notification &notification : notifications) {
        notificationText += QString("[%1] %2\nLocation: %3\nLine: %4\nDetails: %5\n\n")
        .arg(notification.timestamp.toString("yyyy-MM-dd HH:mm:ss"),
             notification.action,
             notification.location,
             QString::number(notification.lineNumber),
             notification.details);
    }
    textEdit->setText(notificationText);

    connect(closeButton, &QPushButton::clicked, notificationDialog, &QDialog::accept);

    notificationDialog->exec();
    delete notificationDialog;
}

void ClientManager::logNotification(const QString &action, const QString &location, const QString &details, int lineNumber)
{
    Notification notification;
    notification.action = action;
    notification.timestamp = QDateTime::currentDateTime();
    notification.location = location;
    notification.details = details;
    notification.lineNumber = lineNumber;
    notifications.append(notification);

    ui->trainingNotificationLabel->setText(QString("%1 modifications").arg(notifications.size()));
}

bool ClientManager::validateClientInputs()
{
    QString name = ui->clientNameInput->text().trimmed();
    QString email = ui->clientEmailInput->text().trimmed();
    QDateTimeEdit *dateTimeEdit = ui->clientAddTab->findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime dateTime = dateTimeEdit ? dateTimeEdit->dateTime() : QDateTime::currentDateTime();

    if (name.isEmpty() || ui->clientSectorInput->text().trimmed().isEmpty() ||
        ui->clientContactInfoInput->text().trimmed().isEmpty() || email.isEmpty()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Please fill in all fields.");
        return false;
    }
    if (!isValidName(name)) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Invalid name format.");
        return false;
    }
    if (!isValidEmail(email)) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Invalid email format.");
        return false;
    }
    if (!isValidDateTime(dateTime)) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Consultation date must be in the future.");
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
    highlightFormat.setBackground(Qt::yellow);
    highlightFormat.setForeground(Qt::black);

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
            QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Query Error",
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
    if (!m_dbConnected) {
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
    QSqlQuery query("SELECT ID, (FIRST_NAME || ' ' || LAST_NAME) AS FULL_NAME FROM AHMED.EMPLOYEE");
    ui->clientConsultantComboBox->clear();
    employeeMap.clear();
    while (query.next()) {
        QString id = query.value("ID").toString();
        QString name = query.value("FULL_NAME").toString();
        employeeMap.insert(name, id);
        ui->clientConsultantComboBox->addItem(name);
    }
}

void ClientManager::refreshClientTable()
{
    clientTableModel->setQuery(
        "SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, (e.FIRST_NAME || ' ' || e.LAST_NAME) AS CONSULTANT_NAME "
        "FROM AHMED.CLIENTS c "
        "JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID"
        );

    if (clientTableModel->lastError().isValid()) {
        qDebug() << "Client query failed:" << clientTableModel->lastError().text();
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Query Error",
                             "Failed to load clients: " + clientTableModel->lastError().text());
        return;
    }

    ui->clientTableView->setModel(clientProxyModel);
    ui->clientTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    clientTableModel->setHeaderData(0, Qt::Horizontal, "Name");
    clientTableModel->setHeaderData(1, Qt::Horizontal, "Sector");
    clientTableModel->setHeaderData(2, Qt::Horizontal, "Contact Info");
    clientTableModel->setHeaderData(3, Qt::Horizontal, "Email");
    clientTableModel->setHeaderData(4, Qt::Horizontal, "Consultation Date");
    clientTableModel->setHeaderData(5, Qt::Horizontal, "Consultant");
}

bool ClientManager::updateClient(const QString &originalName, const QString &newName, const QString sector,
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
    QString fileName = QFileDialog::getSaveFileName(qobject_cast<QWidget*>(parent()), "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&pdfWriter);
    QFont font("Arial", 10);
    painter.setFont(font);

    QStringList headers = {"Name", "Sector", "Contact", "Email", "Consultation Date", "Consultant"};
    int y = 50;
    int tableWidth = pdfWriter.width() - 40;
    QVector<qreal> columnWidths = {0.2, 0.15, 0.15, 0.15, 0.2, 0.15};

    // Draw title
    painter.drawText(20, 30, "Client List");

    // Draw headers
    int x = 20;
    for (int i = 0; i < headers.size(); ++i) {
        int columnWidth = tableWidth * columnWidths[i];
        painter.drawText(x, y, columnWidth, 20, Qt::AlignLeft, headers[i]);
        x += columnWidth;
    }
    y += 20;
    painter.drawLine(20, y, tableWidth + 20, y);
    y += 10;

    // Draw rows using the proxy model to reflect filtering and sorting
    int rowCount = clientProxyModel->rowCount();
    if (rowCount == 0) {
        painter.drawText(20, y, "No clients to display.");
        return;
    }

    for (int row = 0; row < rowCount; ++row) {
        x = 20;
        for (int col = 0; col < headers.size(); ++col) {
            int columnWidth = tableWidth * columnWidths[col];
            QString text = clientProxyModel->data(clientProxyModel->index(row, col)).toString();
            painter.drawText(x, y, columnWidth, 20, Qt::AlignLeft, text);
            x += columnWidth;
        }
        y += 20;
        if (y > pdfWriter.height() - 40) {
            pdfWriter.newPage();
            y = 50;
            x = 20;
            for (int i = 0; i < headers.size(); ++i) {
                int columnWidth = tableWidth * columnWidths[i];
                painter.drawText(x, y, columnWidth, 20, Qt::AlignLeft, headers[i]);
                x += columnWidth;
            }
            y += 20;
            painter.drawLine(20, y, tableWidth + 20, y);
            y += 10;
        }
    }
}
