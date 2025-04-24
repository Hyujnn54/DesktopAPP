#include "updateclientdialog.h"
#include "ui_updateclientdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

UpdateClientDialog::UpdateClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateClientDialog)
{
    ui->setupUi(this);
    setWindowTitle("Update Client");
    connect(ui->confirmButton, &QPushButton::clicked, this, &UpdateClientDialog::on_confirmButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &UpdateClientDialog::on_cancelButton_clicked);

    // Add a placeholder item as the default selection
    ui->consultantComboBox->addItem("Select a consultant", QVariant("")); // Empty string as the data for placeholder

    // Populate the consultantComboBox with consultant names from the EMPLOYEE table
    QSqlQuery query;
    query.prepare("SELECT ID, FIRST_NAME || ' ' || LAST_NAME AS FULL_NAME FROM AHMED.EMPLOYEE ORDER BY FULL_NAME");
    if (query.exec()) {
        while (query.next()) {
            QString consultantId = query.value("ID").toString();
            QString consultantName = query.value("FULL_NAME").toString();
            ui->consultantComboBox->addItem(consultantName, consultantId);
        }
    } else {
        qDebug() << "Error fetching consultants:" << query.lastError().text();
        QMessageBox::warning(this, "Database Error", "Failed to load consultants: " + query.lastError().text());
    }

    // If no consultants were loaded, disable the confirm button
    if (ui->consultantComboBox->count() == 1) { // Only the placeholder item
        QMessageBox::warning(this, "No Consultants", "No consultants available. Please add a consultant first.");
        ui->confirmButton->setEnabled(false);
    }
}

UpdateClientDialog::~UpdateClientDialog()
{
    delete ui;
}

void UpdateClientDialog::setClientData(const QString &name, const QString &sector, const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime, const QString &consultant) {
    ui->nameLineEdit->setText(name);
    ui->sectorLineEdit->setText(sector);
    ui->contactLineEdit->setText(contactInfo);
    ui->emailLineEdit->setText(email);
    ui->consultationDateTime->setDateTime(consultationDateTime);

    qDebug() << "Setting consultant ID:" << consultant;

    // Find and select the consultant in the combo box based on the consultant ID
    int index = ui->consultantComboBox->findData(consultant);
    if (index != -1) {
        ui->consultantComboBox->setCurrentIndex(index);
    } else {
        qDebug() << "Consultant ID" << consultant << "not found in combo box";
        ui->consultantComboBox->setCurrentIndex(0); // Default to placeholder
    }
}

QString UpdateClientDialog::getName() const {
    return ui->nameLineEdit->text();
}

QString UpdateClientDialog::getSector() const {
    return ui->sectorLineEdit->text();
}

QString UpdateClientDialog::getContactInfo() const {
    return ui->contactLineEdit->text();
}

QString UpdateClientDialog::getEmail() const {
    return ui->emailLineEdit->text();
}

QDateTime UpdateClientDialog::getConsultationDateTime() const {
    return ui->consultationDateTime->dateTime();
}

QString UpdateClientDialog::getConsultant() const {
    return ui->consultantComboBox->currentData().toString();
}

void UpdateClientDialog::on_confirmButton_clicked()
{
    // Validate consultant selection
    if (ui->consultantComboBox->currentIndex() == 0) { // Placeholder selected
        QMessageBox::warning(this, "Invalid Selection", "Please select a valid consultant.");
        return;
    }
    accept();
}

void UpdateClientDialog::on_cancelButton_clicked()
{
    reject();
}
