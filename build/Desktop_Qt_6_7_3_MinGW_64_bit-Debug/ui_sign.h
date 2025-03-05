/********************************************************************************
** Form generated from reading UI file 'sign.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGN_H
#define UI_SIGN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sign
{
public:
    QWidget *centralwidget;
    QLineEdit *lineEdit_pwd;
    QLineEdit *lineEdit_us;
    QPushButton *signBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *sign)
    {
        if (sign->objectName().isEmpty())
            sign->setObjectName("sign");
        sign->resize(800, 600);
        centralwidget = new QWidget(sign);
        centralwidget->setObjectName("centralwidget");
        lineEdit_pwd = new QLineEdit(centralwidget);
        lineEdit_pwd->setObjectName("lineEdit_pwd");
        lineEdit_pwd->setGeometry(QRect(240, 290, 321, 51));
        lineEdit_us = new QLineEdit(centralwidget);
        lineEdit_us->setObjectName("lineEdit_us");
        lineEdit_us->setGeometry(QRect(240, 150, 321, 51));
        signBtn = new QPushButton(centralwidget);
        signBtn->setObjectName("signBtn");
        signBtn->setGeometry(QRect(360, 420, 111, 41));
        sign->setCentralWidget(centralwidget);
        menubar = new QMenuBar(sign);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        sign->setMenuBar(menubar);
        statusbar = new QStatusBar(sign);
        statusbar->setObjectName("statusbar");
        sign->setStatusBar(statusbar);

        retranslateUi(sign);

        QMetaObject::connectSlotsByName(sign);
    } // setupUi

    void retranslateUi(QMainWindow *sign)
    {
        sign->setWindowTitle(QCoreApplication::translate("sign", "MainWindow", nullptr));
        signBtn->setText(QCoreApplication::translate("sign", "Sign in", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sign: public Ui_sign {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGN_H
