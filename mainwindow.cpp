#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "employee.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <Qstring>
#include <QDate>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QTableView>
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) ,employee()

{
    ui->setupUi(this);
    ui->hiddenId->setVisible(false);
    QObject::connect(ui->addButton, SIGNAL(clicked()), this, SLOT(on_addButtonclicked()));
    QObject::connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(on_deleteBtnClicked()));
    QObject::connect(ui->listBtn, SIGNAL(clicked()), this, SLOT(on_listBtnClicked()));
    QObject::connect(ui->modifyBtn, SIGNAL(clicked()), this, SLOT(on_modifyBtnClicked()));
    QObject::connect(ui->saveUpdateBtn, SIGNAL(clicked()), this, SLOT(on_saveUpdateBtnClicked()));
    QRegularExpression cinRegex("^[0-9]{8}$");
    QRegularExpressionValidator* cinValidator = new QRegularExpressionValidator(cinRegex, this);
    ui->lineEdit_CIN->setValidator(cinValidator);
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");
    QValidator *nameValidator = new QRegularExpressionValidator(nameRegex, this);
    ui->lineEdit_Nom->setValidator(nameValidator);
    ui->lineEdit_Prenom->setValidator(nameValidator);
    QRegularExpression phoneRegex("^[0-9]{8}$");
    QRegularExpressionValidator* phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    ui->lineEdit_phone->setValidator(phoneValidator);
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    QRegularExpressionValidator* emailValidator = new QRegularExpressionValidator(emailRegex, this);
    ui->lineEdit_email->setValidator(emailValidator);
    QIntValidator *salaryValidator = new QIntValidator(0, 1000000, this);
    ui->lineEdit_salaire->setValidator(salaryValidator);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_addButtonclicked(){
    QString cin = ui->lineEdit_CIN->text();
    QString lastName = ui->lineEdit_Nom->text();
    QString firstName = ui->lineEdit_Prenom->text();
    QDate dateOfBirth = ui->dateEdit_birth->date();
    QString phoneNumber = ui->lineEdit_phone->text();
    QString email = ui->lineEdit_email->text();
    QString gender;
    if (ui->radioButton_H->isChecked()) {
        gender = "Homme";
    } else if (ui->radioButton_F->isChecked()) {
        gender = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre.");
        return;
    }
    int salary = ui->lineEdit_salaire->text().toInt();
    QDate dateOfHire = ui->dateEdit_hiring->date();
    QString field = ui->comboBox->currentText();
    Employee e(cin, lastName, firstName, dateOfBirth, phoneNumber, email, gender, salary, dateOfHire, field);

    if (e.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé.");
    }
}
void MainWindow::on_listBtnClicked(){
    QSqlQuery query;
    query.prepare("SELECT ID, CIN, LAST_NAME, FIRST_NAME,Date_Birth, PHONE, EMAIL, GENDER, SALARY,DATE_HIRING, SPECIALITY FROM EMPLOYEE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors du chargement des employés: " + query.lastError().text());
        return;
    }

    ui->tableWidget->clear(); // Clear old data
    ui->tableWidget->setRowCount(0); // Reset row count
    ui->tableWidget->setColumnCount(11); // 4 columns (ID, CIN, Last Name, First Name)
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "CIN" << "Last_Name" << "First_Name" << "Date_Birth" << "Phone" <<"Email"
                                                             << "Gender"<<"Salary"<<"Date_of_hiring"<<"speacialty");

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString()));
        ui->tableWidget->setItem(row, 8, new QTableWidgetItem(query.value(8).toString()));
        ui->tableWidget->setItem(row, 9, new QTableWidgetItem(query.value(9).toString()));
        ui->tableWidget->setItem(row, 10, new QTableWidgetItem(query.value(10).toString()));

        row++;
    }

    ui->tableWidget->resizeColumnsToContents();
}
void MainWindow::on_modifyBtnClicked(){
    bool ok;
    int id = QInputDialog::getInt(this, "Update Employee", "Enter Employee ID:", 0, 0, 1000000, 1, &ok);

    if (!ok) return; // If user cancels, stop execution

    // Fetch existing data
    QSqlQuery query;
    query.prepare("SELECT CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        // Populate input fields
        ui->lineEdit_CIN->setText(query.value(0).toString());
        ui->lineEdit_Nom->setText(query.value(1).toString());
        ui->lineEdit_Prenom->setText(query.value(2).toString());
        ui->dateEdit_birth->setDate(query.value(3).toDate());
        ui->lineEdit_phone->setText(query.value(4).toString());
        ui->lineEdit_email->setText(query.value(5).toString());
        QString gender = query.value(6).toString();
        if (gender == "Homme") {
            ui->radioButton_H->setChecked(true);
        } else if (gender == "Femme") {
            ui->radioButton_F->setChecked(true);
        }
        ui->lineEdit_salaire->setText(query.value(7).toString());
        ui->dateEdit_hiring->setDate(query.value(8).toDate());
        ui->comboBox->setCurrentText(query.value(9).toString());

        // Save ID for later update
        ui->hiddenId->setText(QString::number(id));  // Use a hidden field to store ID
    } else {
        QMessageBox::warning(this, "Error", "Employee not found!");
    }
}
void MainWindow::on_saveUpdateBtnClicked(){
int id = ui->hiddenId->text().toInt();
if (id == 0) {
    QMessageBox::warning(this, "Error", "No employee selected for update!");
    return;
}

// Determine gender selection
QString gender;
if (ui->radioButton_H->isChecked()) {
    gender = "Homme";
} else if (ui->radioButton_F->isChecked()) {
    gender = "Femme";
} else {
    QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre.");
    return;
}

// Create Employee object and call update function
Employee emp;
bool success = emp.updateEmployee(
    id,
    ui->lineEdit_CIN->text(),
    ui->lineEdit_Nom->text(),
    ui->lineEdit_Prenom->text(),
    ui->dateEdit_birth->date(),
    ui->lineEdit_phone->text(),
         ui->lineEdit_email->text(),
    gender,
    ui->lineEdit_salaire->text().toInt(),
    ui->dateEdit_hiring->date(),
    ui->comboBox->currentText()
    );

if (success) {
    QMessageBox::information(this, "Success", "Employee updated successfully!");
} else {
    QMessageBox::critical(this, "Error", "Update failed!");
}
}
void MainWindow::on_deleteBtnClicked(){
    bool ok;
    int id = QInputDialog::getInt(this, "Delete Employee", "Enter Employee ID:", 0, 0, 1000000, 1, &ok);

    if (!ok) return; // If user cancels, stop execution

    Employee emp;
    bool success = emp.deleteEmployee(id);

    if (success) {
        QMessageBox::information(this, "Success", "Employee deleted successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Delete failed!");
    }
}

