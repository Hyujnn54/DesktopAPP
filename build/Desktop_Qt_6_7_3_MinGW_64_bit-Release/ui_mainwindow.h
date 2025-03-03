/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *ClientForm;
    QPushButton *add;
    QLineEdit *Sector;
    QLineEdit *Name;
    QLineEdit *Contact_info;
    QDateEdit *consultation_date;
    QLineEdit *Consultant;
    QWidget *listClient;
    QTableView *tableView;
    QPushButton *Delet;
    QPushButton *Delet_2;
    QWidget *UpdateClient;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1387, 758);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(230, 90, 821, 531));
        ClientForm = new QWidget();
        ClientForm->setObjectName("ClientForm");
        ClientForm->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        add = new QPushButton(ClientForm);
        add->setObjectName("add");
        add->setGeometry(QRect(60, 370, 111, 24));
        Sector = new QLineEdit(ClientForm);
        Sector->setObjectName("Sector");
        Sector->setGeometry(QRect(60, 80, 113, 24));
        Name = new QLineEdit(ClientForm);
        Name->setObjectName("Name");
        Name->setGeometry(QRect(60, 20, 113, 24));
        Contact_info = new QLineEdit(ClientForm);
        Contact_info->setObjectName("Contact_info");
        Contact_info->setGeometry(QRect(60, 150, 113, 24));
        consultation_date = new QDateEdit(ClientForm);
        consultation_date->setObjectName("consultation_date");
        consultation_date->setGeometry(QRect(60, 220, 110, 25));
        Consultant = new QLineEdit(ClientForm);
        Consultant->setObjectName("Consultant");
        Consultant->setGeometry(QRect(60, 290, 113, 24));
        tabWidget->addTab(ClientForm, QString());
        listClient = new QWidget();
        listClient->setObjectName("listClient");
        tableView = new QTableView(listClient);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(120, 150, 551, 192));
        Delet = new QPushButton(listClient);
        Delet->setObjectName("Delet");
        Delet->setGeometry(QRect(250, 410, 80, 24));
        Delet_2 = new QPushButton(listClient);
        Delet_2->setObjectName("Delet_2");
        Delet_2->setGeometry(QRect(420, 410, 80, 24));
        tabWidget->addTab(listClient, QString());
        UpdateClient = new QWidget();
        UpdateClient->setObjectName("UpdateClient");
        tabWidget->addTab(UpdateClient, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1387, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        add->setText(QCoreApplication::translate("MainWindow", "add", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ClientForm), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        Delet->setText(QCoreApplication::translate("MainWindow", "delete", nullptr));
        Delet_2->setText(QCoreApplication::translate("MainWindow", "update", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(listClient), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(UpdateClient), QCoreApplication::translate("MainWindow", "Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
