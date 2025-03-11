#include "sign.h"
#include "ui_sign.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"
#include <QSettings>

sign::sign(QWidget *parent) : QMainWindow(parent), ui(new Ui::sign)
{
    ui->setupUi(this);
    connect(ui->signBtn, &QPushButton::clicked, this, &sign::handleLogin);
    loadLoginCredentials(); // Load credentials when the window opens
}

sign::~sign()
{
    delete ui;
}

void sign::handleLogin()
{
    QString email = ui->lineEdit_us->text();
    QString cin = ui->lineEdit_pwd->text();

    if (email.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both email and CIN.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYEE WHERE EMAIL = :email AND CIN = :cin");
    query.bindValue(":email", email);
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        QMessageBox::information(this, "Login Successful", "Welcome!");

        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
        this->close();

    } else {
        QMessageBox::critical(this, "Login Failed", "Invalid email or CIN.");
    }

    if (ui->rememberMeCheckBox->isChecked()) {
        saveLoginCredentials(email, cin, true); // Save both email and CIN
    } else {
        saveLoginCredentials("", "", false); // Clear both
    }
}

void sign::saveLoginCredentials(const QString &email, const QString &cin, bool remember)
{
    QSettings settings("YourCompany", "ConsultingApp");
    if (remember) {
        settings.setValue("email", email);
        settings.setValue("cin", cin); // Save CIN
    } else {
        settings.remove("email");
        settings.remove("cin"); // Remove CIN
    }
}

void sign::loadLoginCredentials()
{
    QSettings settings("YourCompany", "ConsultingApp");
    QString savedEmail = settings.value("email").toString();
    QString savedCin = settings.value("cin").toString();

    if (!savedEmail.isEmpty() && !savedCin.isEmpty()) {
        ui->lineEdit_us->setText(savedEmail);
        ui->lineEdit_pwd->setText(savedCin);
        ui->rememberMeCheckBox->setChecked(true);
    }
}
