#include "updateemployeedialog.h"
#include "ui_UpdateEmployeeDialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

UpdateEmployeeDialog::UpdateEmployeeDialog(int employeeId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateEmployeeDialog),
    m_employeeId(employeeId),
    employee(new Employee())
{
    ui->setupUi(this);
    
    // Connexions des signaux aux slots
    connect(ui->selectImageButton, &QPushButton::clicked, this, &UpdateEmployeeDialog::on_selectImageButton_clicked);
    connect(ui->confirmButton, &QPushButton::clicked, this, &UpdateEmployeeDialog::on_confirmButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &UpdateEmployeeDialog::on_cancelButton_clicked);
    
    // Chargement des données de l'employé sélectionné
    loadEmployeeData();
}

UpdateEmployeeDialog::~UpdateEmployeeDialog()
{
    delete ui;
    delete employee;
}

void UpdateEmployeeDialog::loadEmployeeData()
{
    QSqlQuery query;
    query.prepare("SELECT CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, "
                  "SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", m_employeeId);
    
    if (query.exec() && query.next()) {
        ui->cinLineEdit->setText(query.value("CIN").toString());
        ui->lastNameLineEdit->setText(query.value("LAST_NAME").toString());
        ui->firstNameLineEdit->setText(query.value("FIRST_NAME").toString());
        ui->dateOfBirthEdit->setDate(query.value("DATE_BIRTH").toDate());
        ui->phoneLineEdit->setText(query.value("PHONE").toString());
        ui->emailLineEdit->setText(query.value("EMAIL").toString());
        
        // Gender
        QString gender = query.value("GENDER").toString();
        if (gender == "Homme") {
            ui->radioButton_H->setChecked(true);
        } else {
            ui->radioButton_F->setChecked(true);
        }
        
        ui->salaryLineEdit->setText(query.value("SALARY").toString());
        ui->dateOfHireEdit->setDate(query.value("DATE_HIRING").toDate());
        
        // Specialty
        QString specialty = query.value("SPECIALITY").toString();
        int index = ui->specialtyComboBox->findText(specialty);
        if (index != -1) {
            ui->specialtyComboBox->setCurrentIndex(index);
        }
        
        // Image
        m_imagePath = query.value("IMAGE").toString();
        if (!m_imagePath.isEmpty()) {
            QPixmap pixmap(m_imagePath);
            if (!pixmap.isNull()) {
                QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);
                ui->imageLabel->setPixmap(scaledPixmap);
                ui->imageLabel->setScaledContents(true);
            } else {
                ui->imageLabel->setText("(Image introuvable)");
            }
        }
        
        // Role
        QString role = query.value("ROLE").toString();
        index = ui->roleComboBox->findText(role);
        if (index != -1) {
            ui->roleComboBox->setCurrentIndex(index);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données de l'employé: " + query.lastError().text());
        this->reject();
    }
}

void UpdateEmployeeDialog::on_selectImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Sélectionner une image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);
            ui->imageLabel->setPixmap(scaledPixmap);
            ui->imageLabel->setScaledContents(true);
            m_imagePath = fileName;
        } else {
            QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger l'image."));
        }
    }
}

bool UpdateEmployeeDialog::validateInput()
{
    // CIN validation: 8 digits
    QString cin = ui->cinLineEdit->text();
    QRegularExpression cinRegex("^\\d{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Erreur de validation", "Le CIN doit contenir exactement 8 chiffres.");
        return false;
    }
    
    // Names validation
    QString lastName = ui->lastNameLineEdit->text();
    QString firstName = ui->firstNameLineEdit->text();
    QRegularExpression nameRegex("^[A-Za-z\\s]+$");
    if (lastName.isEmpty() || !nameRegex.match(lastName).hasMatch()) {
        QMessageBox::warning(this, "Erreur de validation", "Le nom de famille doit contenir uniquement des lettres.");
        return false;
    }
    if (firstName.isEmpty() || !nameRegex.match(firstName).hasMatch()) {
        QMessageBox::warning(this, "Erreur de validation", "Le prénom doit contenir uniquement des lettres.");
        return false;
    }
    
    // Phone validation
    QString phone = ui->phoneLineEdit->text();
    QRegularExpression phoneRegex("^\\d{8}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Erreur de validation", "Le numéro de téléphone doit contenir 8 chiffres.");
        return false;
    }
    
    // Email validation
    QString email = ui->emailLineEdit->text();
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur de validation", "Veuillez entrer une adresse email valide.");
        return false;
    }
    
    // Salary validation
    QString salaryText = ui->salaryLineEdit->text();
    bool ok;
    int salary = salaryText.toInt(&ok);
    if (!ok || salary <= 0) {
        QMessageBox::warning(this, "Erreur de validation", "Le salaire doit être un nombre positif.");
        return false;
    }
    
    // Date validations
    QDate birthDate = ui->dateOfBirthEdit->date();
    QDate hiringDate = ui->dateOfHireEdit->date();
    QDate currentDate = QDate::currentDate();
    
    if (birthDate > currentDate.addYears(-18)) {
        QMessageBox::warning(this, "Erreur de validation", "L'employé doit avoir au moins 18 ans.");
        return false;
    }
    
    QDate minHiringDate = birthDate.addYears(18);
    if (hiringDate < minHiringDate) {
        QMessageBox::warning(this, "Erreur de validation", "La date d'embauche ne peut pas être avant que l'employé ait 18 ans.");
        return false;
    }
    
    if (hiringDate > currentDate) {
        QMessageBox::warning(this, "Erreur de validation", "La date d'embauche ne peut pas être dans le futur.");
        return false;
    }
    
    return true;
}

void UpdateEmployeeDialog::on_confirmButton_clicked()
{
    if (!validateInput()) {
        return;
    }
    
    QString cin = ui->cinLineEdit->text();
    QString lastName = ui->lastNameLineEdit->text();
    QString firstName = ui->firstNameLineEdit->text();
    QDate dateOfBirth = ui->dateOfBirthEdit->date();
    QString phoneNumber = ui->phoneLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString gender = ui->radioButton_H->isChecked() ? "Homme" : "Femme";
    int salary = ui->salaryLineEdit->text().toInt();
    QDate dateOfHire = ui->dateOfHireEdit->date();
    QString field = ui->specialtyComboBox->currentText();
    QString role = ui->roleComboBox->currentText();
    
    bool success = employee->updateEmployee(m_employeeId, cin, lastName, firstName, dateOfBirth, 
                                         phoneNumber, email, gender, salary, dateOfHire, 
                                         field, m_imagePath, role);
                                         
    if (success) {
        QMessageBox::information(this, "Succès", "Les informations de l'employé ont été mises à jour avec succès.");
        accept();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de l'employé. Veuillez réessayer.");
    }
}

void UpdateEmployeeDialog::on_cancelButton_clicked()
{
    reject();
}
