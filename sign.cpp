#include "sign.h"
#include "ui_sign.h"  // ✅ Changed from ui_login.h to ui_sign.h
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"

sign::sign(QWidget *parent) : QMainWindow(parent), ui(new Ui::sign)
{
    ui->setupUi(this);
    connect(ui->signBtn, &QPushButton::clicked, this, &sign::handleLogin);
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
}
