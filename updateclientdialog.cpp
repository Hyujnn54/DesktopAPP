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

    ui->consultantComboBox->addItem("Select a consultant", QVariant(""));
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

    if (ui->consultantComboBox->count() == 1) {
        QMessageBox::warning(this, "No Consultants", "No consultants available. Please add a consultant first.");
        ui->confirmButton->setEnabled(false);
    }

    ui->consultationDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
}

UpdateClientDialog::~UpdateClientDialog()
{
    delete ui;
}

void UpdateClientDialog::setClientData(const QString &name, const QString &sector, const QString &contactInfo,
                                       const QString &email, const QDateTime &consultationDateTime, const QString &consultantId)
{
    qDebug() << "Setting dialog data - Name:" << name << "Sector:" << sector << "Contact:" << contactInfo
             << "Email:" << email << "DateTime:" << consultationDateTime.toString("yyyy-MM-dd HH:mm:ss")
             << "Consultant ID:" << consultantId;
    ui->nameLineEdit->setText(name);
    ui->sectorLineEdit->setText(sector);
    ui->contactLineEdit->setText(contactInfo);
    ui->emailLineEdit->setText(email);
    ui->consultationDateTime->setDateTime(consultationDateTime);

    int index = ui->consultantComboBox->findData(consultantId);
    if (index != -1) {
        ui->consultantComboBox->setCurrentIndex(index);
    } else {
        ui->consultantComboBox->setCurrentIndex(0);
    }
}

QString UpdateClientDialog::getName() const
{
    return ui->nameLineEdit->text().trimmed();
}

QString UpdateClientDialog::getSector() const
{
    return ui->sectorLineEdit->text().trimmed();
}

QString UpdateClientDialog::getContactInfo() const
{
    return ui->contactLineEdit->text().trimmed();
}

QString UpdateClientDialog::getEmail() const
{
    return ui->emailLineEdit->text().trimmed();
}

QDateTime UpdateClientDialog::getConsultationDateTime() const
{
    return ui->consultationDateTime->dateTime();
}

QString UpdateClientDialog::getConsultant() const
{
    return ui->consultantComboBox->currentData().toString();
}

void UpdateClientDialog::on_confirmButton_clicked()
{
    if (ui->consultantComboBox->currentIndex() == 0) {
        QMessageBox::warning(this, "Invalid Selection", "Please select a valid consultant.");
        return;
    }

    if (ui->nameLineEdit->text().trimmed().isEmpty() ||
        ui->sectorLineEdit->text().trimmed().isEmpty() ||
        ui->contactLineEdit->text().trimmed().isEmpty() ||
        ui->emailLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all required fields.");
        return;
    }

    if (ui->consultationDateTime->dateTime() < QDateTime::currentDateTime()) {
        QMessageBox::warning(this, "Invalid Date", "Please select a future date and time.");
        return;
    }

    accept();
}

void UpdateClientDialog::on_cancelButton_clicked()
{
    reject();
}
