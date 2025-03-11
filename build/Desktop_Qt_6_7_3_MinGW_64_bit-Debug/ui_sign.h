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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sign
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *logoLabel;
    QSpacerItem *horizontalSpacer_5;
    QFrame *loginFrame;
    QVBoxLayout *verticalLayout_2;
    QLabel *loginLabel;
    QSpacerItem *verticalSpacer_3;
    QLabel *emailLabel;
    QLineEdit *lineEdit_us;
    QSpacerItem *verticalSpacer_4;
    QLabel *cinLabel;
    QLineEdit *lineEdit_pwd;
    QCheckBox *rememberMeCheckBox;
    QSpacerItem *verticalSpacer_2;
    QPushButton *signBtn;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *noteLabel;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_5;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *sign)
    {
        if (sign->objectName().isEmpty())
            sign->setObjectName("sign");
        sign->resize(800, 600);
        sign->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow {\n"
"      background-color: #f5f5f5;\n"
"    }\n"
"    QLabel {\n"
"      color: #333333;\n"
"      font-family: Arial;\n"
"      font-weight: bold;\n"
"    }\n"
"    QLineEdit {\n"
"      padding: 10px;\n"
"      border: 1px solid #cccccc;\n"
"      border-radius: 4px;\n"
"      background-color: white;\n"
"      font-size: 11pt;\n"
"      color: #000000; /* Changed to black text */\n"
"    }\n"
"    QLineEdit:focus {\n"
"      border: 1px solid #2c7fb8;\n"
"    }\n"
"    QPushButton {\n"
"      background-color: #2c7fb8;\n"
"      color: white;\n"
"      font-weight: bold;\n"
"      border: none;\n"
"      border-radius: 4px;\n"
"      padding: 8px 16px;\n"
"      font-size: 11pt;\n"
"    }\n"
"    QPushButton:hover {\n"
"      background-color: #1c6ea4;\n"
"    }\n"
"    QPushButton:pressed {\n"
"      background-color: #185c8c;\n"
"    }\n"
"    QCheckBox {\n"
"      color: #333333;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(sign);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        logoLabel = new QLabel(centralwidget);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(320, 80));
        logoLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(logoLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_3);

        loginFrame = new QFrame(centralwidget);
        loginFrame->setObjectName("loginFrame");
        loginFrame->setMinimumSize(QSize(400, 300));
        loginFrame->setMaximumSize(QSize(16777215, 16777215));
        loginFrame->setStyleSheet(QString::fromUtf8("\n"
"        QFrame#loginFrame {\n"
"          background-color: white;\n"
"          border-radius: 8px;\n"
"          border: 1px solid #e0e0e0;\n"
"        }\n"
"       "));
        loginFrame->setFrameShape(QFrame::StyledPanel);
        loginFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(loginFrame);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(40, 30, 40, 30);
        loginLabel = new QLabel(loginFrame);
        loginLabel->setObjectName("loginLabel");
        loginLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(loginLabel);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_3);

        emailLabel = new QLabel(loginFrame);
        emailLabel->setObjectName("emailLabel");

        verticalLayout_2->addWidget(emailLabel);

        lineEdit_us = new QLineEdit(loginFrame);
        lineEdit_us->setObjectName("lineEdit_us");
        lineEdit_us->setMinimumSize(QSize(0, 40));

        verticalLayout_2->addWidget(lineEdit_us);

        verticalSpacer_4 = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_4);

        cinLabel = new QLabel(loginFrame);
        cinLabel->setObjectName("cinLabel");

        verticalLayout_2->addWidget(cinLabel);

        lineEdit_pwd = new QLineEdit(loginFrame);
        lineEdit_pwd->setObjectName("lineEdit_pwd");
        lineEdit_pwd->setMinimumSize(QSize(0, 40));
        lineEdit_pwd->setEchoMode(QLineEdit::Password);

        verticalLayout_2->addWidget(lineEdit_pwd);

        rememberMeCheckBox = new QCheckBox(loginFrame);
        rememberMeCheckBox->setObjectName("rememberMeCheckBox");

        verticalLayout_2->addWidget(rememberMeCheckBox);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        signBtn = new QPushButton(loginFrame);
        signBtn->setObjectName("signBtn");
        signBtn->setMinimumSize(QSize(0, 45));

        verticalLayout_2->addWidget(signBtn);


        verticalLayout->addWidget(loginFrame);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        noteLabel = new QLabel(centralwidget);
        noteLabel->setObjectName("noteLabel");

        horizontalLayout_2->addWidget(noteLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        sign->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(sign);
        statusbar->setObjectName("statusbar");
        sign->setStatusBar(statusbar);

        retranslateUi(sign);

        QMetaObject::connectSlotsByName(sign);
    } // setupUi

    void retranslateUi(QMainWindow *sign)
    {
        sign->setWindowTitle(QCoreApplication::translate("sign", "Consulting Company - Login", nullptr));
        logoLabel->setText(QCoreApplication::translate("sign", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; color:#2c7fb8;\">CONSULTING COMPANY</span></p></body></html>", nullptr));
        loginLabel->setText(QCoreApplication::translate("sign", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt;\">Employee Login</span></p></body></html>", nullptr));
        emailLabel->setText(QCoreApplication::translate("sign", "Email", nullptr));
        lineEdit_us->setPlaceholderText(QCoreApplication::translate("sign", "Enter your email", nullptr));
        cinLabel->setText(QCoreApplication::translate("sign", "CIN", nullptr));
        lineEdit_pwd->setPlaceholderText(QCoreApplication::translate("sign", "Enter your CIN", nullptr));
        rememberMeCheckBox->setText(QCoreApplication::translate("sign", "Remember me", nullptr));
        signBtn->setText(QCoreApplication::translate("sign", "Sign In", nullptr));
        noteLabel->setText(QCoreApplication::translate("sign", "<html><head/><body><p>Contact HR for account assistance</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class sign: public Ui_sign {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGN_H
