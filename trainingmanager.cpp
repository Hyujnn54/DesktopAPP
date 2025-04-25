#include "TrainingManager.h"
#include "ui_mainwindow.h"
#include "UpdateTrainingDialog.h"
#include <QMessageBox>
#include <QSettings>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QDateTime>
#include <QFileDialog>
#include <QSqlRecord>

TrainingManager::TrainingManager(bool dbConnected, QObject *parent)
    : QObject(parent),
    m_dbConnected(dbConnected),
    ui(nullptr),
    formation(new formations()),
    networkManager(new QNetworkAccessManager(this)),
    trainingTableModel(new QSqlQueryModel(this)),
    trainingProxyModel(new QSortFilterProxyModel(this)),
    lastTrainingTableRefresh(QDateTime::currentDateTime())
{
}

TrainingManager::~TrainingManager()
{
    delete formation;
    delete networkManager;
    delete trainingTableModel;
    delete trainingProxyModel;
}

void TrainingManager::initialize(Ui::MainWindow *ui)
{
    this->ui = ui;

    // Initialize training input form
    ui->trainingDateEdit->setDate(QDate::currentDate());
    ui->trainingNotificationLabel->setText("<a href='notifications'>Notifications: 0</a>");
    ui->trainingNotificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");
    ui->trainingTableView->setSortingEnabled(true);
    ui->trainingTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->trainingTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->trainingTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->trainingTableView->verticalHeader()->setVisible(false);

    // Set input validators
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

    // Connect signals
    connect(ui->trainingAddButton, &QPushButton::clicked, this, &TrainingManager::on_trainingAddButton_clicked);
    connect(ui->trainingDeleteButton, &QPushButton::clicked, this, &TrainingManager::on_trainingDeleteButton_clicked);
    connect(ui->trainingUpdateButton, &QPushButton::clicked, this, &TrainingManager::on_trainingUpdateButton_clicked);
    connect(ui->trainingSearchInput, &QLineEdit::textChanged, this, &TrainingManager::on_trainingSearchInput_textChanged);
    connect(ui->trainingResetSearchButton, &QPushButton::clicked, this, &TrainingManager::on_trainingResetSearchButton_clicked);
    connect(ui->trainingExportButton, &QPushButton::clicked, this, &TrainingManager::on_trainingExportButton_clicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &TrainingManager::on_trainingSmsRequest_finished);
    connect(ui->trainingNotificationLabel, &QLabel::linkActivated, this, &TrainingManager::handleTrainingNotificationLabel_clicked);

    if (m_dbConnected) {
        refreshTrainingTableView();
    }
}

void TrainingManager::refresh()
{
    if (m_dbConnected) {
        refreshTrainingTableView();
        updateNotificationLabel();
    }
}

void TrainingManager::on_trainingAddButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot add formation: Database is not connected.");
        return;
    }
    if (!validateTrainingInputs()) {
        return;
    }
    QString formationName = ui->trainingNameInput->text().trimmed();
    QString description = ui->trainingDescriptionInput->text().trimmed();
    QString trainer = ui->trainingTrainerInput->text().trimmed();
    QDate date = ui->trainingDateEdit->date();
    int time = ui->trainingTimeSpinBox->value();
    double prix = ui->trainingPriceSpinBox->value();
    QString phoneNumber = ui->trainingPhoneNumberInput->text().trimmed();

    formation->setFormation(formationName);
    formation->setDescription(description);
    formation->setTrainer(trainer);
    formation->setDatef(date);
    formation->setTime(time);
    formation->setPrix(prix);
    if (formation->ajoutforma()) {
        QMessageBox::information(qobject_cast<QWidget*>(parent()), "Success", "Formation added successfully!");
        sendSmsNotification(phoneNumber, formationName, date);
        refreshTrainingTableView();
        addNotification("Added", QString("Training: %1").arg(formationName));
        ui->trainingNameInput->clear();
        ui->trainingDescriptionInput->clear();
        ui->trainingTrainerInput->clear();
        ui->trainingDateEdit->setDate(QDate::currentDate());
        ui->trainingTimeSpinBox->setValue(1);
        ui->trainingPriceSpinBox->setValue(1);
        ui->trainingPhoneNumberInput->clear();
    } else {
        QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Failed to add formation.");
    }
}

void TrainingManager::on_trainingDeleteButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot delete formation: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->trainingTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Selection Error", "Please select a formation to delete.");
        return;
    }
    int idForm = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 0)).toInt();
    QString formationName = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 1)).toString();
    QMessageBox::StandardButton reply = QMessageBox::question(
        qobject_cast<QWidget*>(parent()), "Confirm Delete",
        QString("Are you sure you want to delete formation '%1'?").arg(formationName),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (formation->deleteFormation(idForm)) {
            QMessageBox::information(qobject_cast<QWidget*>(parent()), "Success", "Formation deleted successfully!");
            refreshTrainingTableView();
            addNotification("Deleted", QString("Training: %1").arg(formationName));
        } else {
            QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Failed to delete formation.");
        }
    }
}

void TrainingManager::on_trainingUpdateButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot update formation: Database is not connected.");
        return;
    }
    QModelIndexList selectedRows = ui->trainingTableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Selection Error", "Please select a formation to update.");
        return;
    }
    int idForm = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 0)).toInt();
    QString formationName = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 1)).toString();
    QString description = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 2)).toString();
    QString trainer = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 3)).toString();
    QDate date = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 4)).toDate();
    int time = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 5)).toInt();
    double prix = trainingProxyModel->data(trainingProxyModel->index(selectedRows[0].row(), 6)).toDouble();

    formations trainingData;
    trainingData.setFormation(formationName);
    trainingData.setDescription(description);
    trainingData.setTrainer(trainer);
    trainingData.setDatef(date);
    trainingData.setTime(time);
    trainingData.setPrix(prix);

    UpdateTrainingDialog dialog(qobject_cast<QWidget*>(parent()));
    dialog.setTrainingData(idForm, trainingData);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getFormation();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();
        int trainingId = dialog.getTrainingId();

        if (formation->updateFormation(trainingId, newName, newDescription, newTrainer, newDate, newTime, newPrix)) {
            QMessageBox::information(qobject_cast<QWidget*>(parent()), "Success", "Formation updated successfully!");
            refreshTrainingTableView();
            addNotification("Updated", QString("Training: %1").arg(newName));
        } else {
            QMessageBox::critical(qobject_cast<QWidget*>(parent()), "Error", "Failed to update formation.");
        }
    }
}

void TrainingManager::on_trainingSearchInput_textChanged(const QString &text)
{
    if (!m_dbConnected) {
        return;
    }
    trainingProxyModel->setFilterKeyColumn(trainingTableModel->record().indexOf("FORMATION"));
    trainingProxyModel->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(text),
                                                                      QRegularExpression::CaseInsensitiveOption));
}

void TrainingManager::on_trainingResetSearchButton_clicked()
{
    if (!m_dbConnected) {
        return;
    }
    ui->trainingSearchInput->clear();
    trainingProxyModel->setFilterRegularExpression("");
    refreshTrainingTableView();
}

void TrainingManager::on_trainingExportButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }
    exportTrainingsToPdf();
}

void TrainingManager::on_trainingSmsRequest_finished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        if (obj.value("status").toString() == "success") {
            addNotification("SMS Sent", "SMS notification sent successfully.");
        } else {
            addNotification("SMS Failed", "Failed to send SMS notification.");
        }
    } else {
        addNotification("SMS Error", QString("SMS request failed: %1").arg(reply->errorString()));
    }
    updateNotificationLabel();
    reply->deleteLater();
}

void TrainingManager::handleTrainingNotificationLabel_clicked()
{
    QStringList notificationDetails;
    for (const auto &notification : notifications) {
        notificationDetails << QString("[%1] %2: %3")
        .arg(notification.timestamp.toString("yyyy-MM-dd HH:mm:ss"),
             notification.action, notification.details);
    }
    QMessageBox::information(qobject_cast<QWidget*>(parent()), "Notifications",
                             notificationDetails.isEmpty() ? "No notifications." : notificationDetails.join("\n"));
}

void TrainingManager::refreshTrainingTableView()
{
    if (QDateTime::currentDateTime() < lastTrainingTableRefresh.addMSecs(TRAINING_REFRESH_INTERVAL_MS)) {
        return;
    }
    trainingTableModel->setQuery(
        "SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX "
        "FROM AHMED.FORMATIONS"
        );
    trainingProxyModel->setSourceModel(trainingTableModel);
    ui->trainingTableView->setModel(trainingProxyModel);
    ui->trainingTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Hide IDFORM column
    ui->trainingTableView->setColumnHidden(0, true);
    // Set custom headers
    trainingTableModel->setHeaderData(1, Qt::Horizontal, "Formation Name");
    trainingTableModel->setHeaderData(2, Qt::Horizontal, "Description");
    trainingTableModel->setHeaderData(3, Qt::Horizontal, "Trainer");
    trainingTableModel->setHeaderData(4, Qt::Horizontal, "Date");
    trainingTableModel->setHeaderData(5, Qt::Horizontal, "Time (Hours)");
    trainingTableModel->setHeaderData(6, Qt::Horizontal, "Price");
    lastTrainingTableRefresh = QDateTime::currentDateTime();
}

void TrainingManager::sendSmsNotification(const QString &phoneNumber, const QString &formationName, const QDate &date)
{
    QSettings settings("TrainingManagement", "SMS");
    QString apiKey = settings.value("apiKey", "").toString();
    QString sender = settings.value("sender", "Training").toString();
    if (apiKey.isEmpty()) {
        addNotification("SMS Error", "SMS API key not configured.");
        updateNotificationLabel();
        return;
    }
    QString message = QString("Reminder: Your training '%1' is scheduled for %2.").arg(formationName, date.toString("yyyy-MM-dd"));
    QUrl url("https://api.smsprovider.com/send");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["apiKey"] = apiKey;
    json["to"] = phoneNumber;
    json["sender"] = sender;
    json["message"] = message;
    QJsonDocument doc(json);
    networkManager->post(request, doc.toJson());
}

void TrainingManager::addNotification(const QString &action, const QString &details)
{
    notifications.append({action, details, QDateTime::currentDateTime()});
    if (notifications.size() > 50) {
        notifications.removeFirst();
    }
    updateNotificationLabel();
}

void TrainingManager::updateNotificationLabel()
{
    ui->trainingNotificationLabel->setText(QString("<a href='notifications'>Notifications: %1</a>").arg(notifications.size()));
}

void TrainingManager::exportTrainingsToPdf()
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

    QStringList headers = {"Name", "Description", "Trainer", "Date", "Time", "Price"};
    int y = 50;
    int tableWidth = pdfWriter.width() - 40;
    QVector<qreal> columnWidths = {0.2, 0.25, 0.15, 0.15, 0.05, 0.1};

    // Draw title
    painter.drawText(20, 30, "Training List");

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

    // Draw rows
    for (int row = 0; row < trainingTableModel->rowCount(); ++row) {
        x = 20;
        for (int col = 0; col < headers.size(); ++col) {
            int columnWidth = tableWidth * columnWidths[col];
            // Skip IDFORM (col 0)
            QString text = trainingTableModel->data(trainingTableModel->index(row, col + 1)).toString();
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

bool TrainingManager::validateTrainingInputs()
{
    QString formationName = ui->trainingNameInput->text().trimmed();
    QString description = ui->trainingDescriptionInput->text().trimmed();
    QString trainer = ui->trainingTrainerInput->text().trimmed();
    QString phoneNumber = ui->trainingPhoneNumberInput->text().trimmed();
    QDate date = ui->trainingDateEdit->date();

    if (formationName.isEmpty() || description.isEmpty() || trainer.isEmpty() || phoneNumber.isEmpty()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Please fill in all fields.");
        return false;
    }
    if (!date.isValid() || date < QDate::currentDate()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Please select a valid future date.");
        return false;
    }
    if (!QRegularExpression("\\+\\d{10,15}").match(phoneNumber).hasMatch()) {
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Input Error", "Phone number must start with '+' followed by 10-15 digits.");
        return false;
    }
    return true;
}
