#include "updateemployeedialog.h"
#include "ui_UpdateClientDialog.h" // Ensure this matches the actual UI header file
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

UpdateEmployeeDialog::UpdateEmployeeDialog(int employeeId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateEmployeeDialog)
    , m_employeeId(employeeId)
    , employee(new Employee())
    , m_imagePath("")
{
    ui->setupUi(this);

    // Populate the dialog with current employee data
    QSqlQuery query;
    query.prepare("SELECT CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE "
                  "FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", m_employeeId);
    if (query.exec() && query.next()) {
        ui->cinLineEdit->setText(query.value(0).toString());
        ui->lastNameLineEdit->setText(query.value(1).toString());
        ui->firstNameLineEdit->setText(query.value(2).toString());
        ui->dateOfBirthEdit->setDate(query.value(3).toDate());
        ui->phoneLineEdit->setText(query.value(4).toString());
        ui->emailLineEdit->setText(query.value(5).toString());
        QString gender = query.value(6).toString();
        if (gender == "Homme") {
            ui->radioButton_H->setChecked(true);
        } else if (gender == "Femme") {
            ui->radioButton_F->setChecked(true);
        }
        ui->salaryLineEdit->setText(query.value(7).toString());
        ui->dateOfHireEdit->setDate(query.value(8).toDate());
        ui->specialtyComboBox->setCurrentText(query.value(9).toString());
        ui->roleComboBox->setCurrentText(query.value(11).toString());

        // Display the image if it exists
        m_imagePath = query.value(10).toString(); // Store the image path
        if (!m_imagePath.isEmpty() && QFile::exists(m_imagePath)) {
            QPixmap pixmap(m_imagePath);
            ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->imageLabel->setText(""); // Clear placeholder text
        }
    } else {
        QMessageBox::warning(this, "Error", "Failed to load employee data!");
        reject();
    }

    // Connect image selection button
    connect(ui->selectImageButton, &QPushButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        if (!fileName.isEmpty() && QFile::exists(fileName)) {
            m_imagePath = fileName; // Update the stored image path
            QPixmap pixmap(fileName);
            ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->imageLabel->setText("");
        }
    });
}

UpdateEmployeeDialog::~UpdateEmployeeDialog()
{
    delete ui;
    delete employee;
}

void UpdateEmployeeDialog::on_confirmButton_clicked()
{
    QString gender;
    if (ui->radioButton_H->isChecked()) {
        gender = "Homme";
    } else if (ui->radioButton_F->isChecked()) {
        gender = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre.");
        return;
    }

    bool success = employee->updateEmployee(
        m_employeeId,
        ui->cinLineEdit->text(),
        ui->lastNameLineEdit->text(),
        ui->firstNameLineEdit->text(),
        ui->dateOfBirthEdit->date(),
        ui->phoneLineEdit->text(),
        ui->emailLineEdit->text(),
        gender,
        ui->salaryLineEdit->text().toInt(),
        ui->dateOfHireEdit->date(),
        ui->specialtyComboBox->currentText(),
        m_imagePath, // Use the stored image path
        ui->roleComboBox->currentText()
        );

    if (success) {
        QMessageBox::information(this, "Success", "Employee updated successfully!");
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to update employee!");
    }
}

void UpdateEmployeeDialog::on_cancelButton_clicked()
{
    reject();
}
