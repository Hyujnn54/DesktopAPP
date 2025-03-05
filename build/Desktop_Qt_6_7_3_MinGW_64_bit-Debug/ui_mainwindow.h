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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QLabel *label_10;
    QPushButton *modifyBtn;
    QPushButton *deleteBtn;
    QLineEdit *hiddenId;
    QPushButton *saveUpdateBtn;
    QPushButton *listBtn;
    QWidget *widget_3;
    QLabel *label_9;
    QRadioButton *radioButton_F;
    QLabel *label_8;
    QLabel *label_7;
    QDateEdit *dateEdit_hiring;
    QRadioButton *radioButton_H;
    QLineEdit *lineEdit_salaire;
    QLabel *label_11;
    QComboBox *comboBox;
    QPushButton *addButton;
    QWidget *widget_2;
    QLineEdit *lineEdit_email;
    QDateEdit *dateEdit_birth;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_phone;
    QLineEdit *lineEdit_Prenom;
    QLineEdit *lineEdit_Nom;
    QLineEdit *lineEdit_CIN;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1533, 1130);
        MainWindow->setStyleSheet(QString::fromUtf8("/* Global Styles */\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(163, 230, 255, 255), stop:1 rgba(255, 255, 255, 255));\n"
"QWidget {\n"
"    background-color:#qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1,\n"
" stop:0 rgba(163, 230, 255, 255), \n"
"stop:1 rgba(255, 255, 255, 255)); \n"
"    color: white;\n"
"    font-size: 16px;\n"
"}\n"
"\n"
"/* Labels */\n"
"QLabel {\n"
"    color: white;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* Input Fields */\n"
"QLineEdit, QDateEdit, QComboBox {\n"
"    background-color: transparent;\n"
"    color: black;\n"
"    border: 1px solid #5DADE2;\n"
"    padding: 6px;\n"
"    border-radius: 5px;\n"
"    font-size: 16px;\n"
"}\n"
"\n"
"/* Buttons */\n"
"\n"
"\n"
"/* Icons for Specific Fields */\n"
"QLineEdit#cinField { background-image: url('icons/id.png'); background-position: left center; background-repeat: no-repeat; padding-left: 30px; }\n"
"QLineEdit#emailField { background-image: url('icons/email.pn"
                        "g'); background-position: left center; background-repeat: no-repeat; padding-left: 30px; }\n"
"QLineEdit#phoneField { background-image: url('icons/phone.png'); background-position: left center; background-repeat: no-repeat; padding-left: 30px; }\n"
"\n"
"/* Dropdown Buttons */\n"
"QPushButton#addEmployeeButton, QPushButton#listEmployeeButton {\n"
"    background-color: #5DADE2;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font-size: 18px;\n"
"}\n"
"\n"
"QPushButton#addEmployeeButton::after, QPushButton#listEmployeeButton::after {\n"
"    content: '\\25BC'; /* Downward Arrow for Dropdown */\n"
"    color: white;\n"
"    font-size: 18px;\n"
"    padding-left: 8px;\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 201, 531));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(40, 80, 121, 41));
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_4 = new QPushButton(frame);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(40, 170, 121, 41));
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_5 = new QPushButton(frame);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(40, 260, 121, 41));
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_6 = new QPushButton(frame);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(40, 350, 121, 41));
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_7 = new QPushButton(frame);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(40, 440, 121, 41));
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(580, 20, 381, 31));
        label_10->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"	color: rgb(170, 170, 255);\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}\n"
""));
        modifyBtn = new QPushButton(centralwidget);
        modifyBtn->setObjectName("modifyBtn");
        modifyBtn->setGeometry(QRect(420, 480, 181, 41));
        modifyBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        deleteBtn = new QPushButton(centralwidget);
        deleteBtn->setObjectName("deleteBtn");
        deleteBtn->setGeometry(QRect(830, 480, 161, 41));
        deleteBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        hiddenId = new QLineEdit(centralwidget);
        hiddenId->setObjectName("hiddenId");
        hiddenId->setEnabled(true);
        hiddenId->setGeometry(QRect(1180, 490, 121, 31));
        hiddenId->setClearButtonEnabled(false);
        saveUpdateBtn = new QPushButton(centralwidget);
        saveUpdateBtn->setObjectName("saveUpdateBtn");
        saveUpdateBtn->setGeometry(QRect(630, 480, 181, 41));
        saveUpdateBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        listBtn = new QPushButton(centralwidget);
        listBtn->setObjectName("listBtn");
        listBtn->setGeometry(QRect(230, 480, 161, 41));
        listBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        widget_3 = new QWidget(centralwidget);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(750, 60, 471, 381));
        widget_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:1px solid;\n"
"border-radius:10px;\n"
"padding:10px"));
        label_9 = new QLabel(widget_3);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(30, 160, 131, 31));
        label_9->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}\n"
""));
        radioButton_F = new QRadioButton(widget_3);
        radioButton_F->setObjectName("radioButton_F");
        radioButton_F->setGeometry(QRect(300, 30, 111, 41));
        radioButton_F->setStyleSheet(QString::fromUtf8("/* General Styles */\n"
"* {\n"
"    font-family: Arial, sans-serif;\n"
"}\n"
"\n"
"/* Styling for the main background */\n"
"QWidget {\n"
"    background-color: #2C3E50; /* Dark gray-blue */\n"
"    color: white;\n"
"}\n"
"\n"
"/* Styling for labels */\n"
"QLabel {\n"
"    color: white;\n"
"    font-size: 15px;\n"
"}\n"
"\n"
"/* Styling for input fields */\n"
"QLineEdit, QComboBox, QDateEdit {\n"
"    background-color: #34495E; /* Slightly lighter blue */\n"
"    color: white;\n"
"    border: 1px solid #5DADE2; /* Blue border */\n"
"    padding: 6px;\n"
"    border-radius: 5px;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus {\n"
"    border: 1px solid #3498DB; /* Brighter blue when focused */\n"
"}\n"
"\n"
"/* Styling for Date Picker (QDateEdit) */\n"
"QDateEdit::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: right;\n"
"    width: 20px;\n"
"    border-left: 1px solid #5DADE2;\n"
"}\n"
"\n"
"QDateEdit::down-arrow {\n"
"    image: url(:/icons/calendar.png); /* Rep"
                        "lace with actual icon path */\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"/* Styling for Dropdown (Gender Selection) */\n"
"QComboBox::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: right;\n"
"    width: 20px;\n"
"    border-left: 1px solid #5DADE2;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: url(:/icons/dropdown.png); /* Replace with actual icon */\n"
"    width: 14px;\n"
"    height: 14px;\n"
"}\n"
"\n"
"/* Styling for Buttons */\n"
"QPushButton {\n"
"    background-color: #5DADE2; /* Soft blue */\n"
"    color: white;\n"
"    border-radius: 8px;\n"
"    padding: 8px 14px;\n"
"    font-size: 15px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #3498DB; /* Brighter blue on hover */\n"
"}\n"
"\n"
"/* Special Styling for Add & List Buttons */\n"
"QPushButton#btnAdd {\n"
"    background-color: #2ECC71; /* Green for adding */\n"
"}\n"
"\n"
"QPushButton#btnAdd:hover {\n"
"    background-color: #27AE"
                        "60; /* Darker green on hover */\n"
"}\n"
"\n"
"QPushButton#btnList {\n"
"    background-color: #E67E22; /* Orange for listing */\n"
"}\n"
"\n"
"QPushButton#btnList:hover {\n"
"    background-color: #D35400; /* Darker orange on hover */\n"
"}\n"
"\n"
"/* Employee List Styling */\n"
"QTableView, QListWidget {\n"
"    background-color: #34495E;\n"
"    border: 1px solid #5DADE2;\n"
"    color: white;\n"
"    selection-background-color: #5DADE2;\n"
"    alternate-background-color: #2D3E50;\n"
"}\n"
"\n"
"QTableView::item:hover, QListWidget::item:hover {\n"
"    background-color: #5DADE2;\n"
"    color: black;\n"
"}\n"
"\n"
"/* Table Header */\n"
"QHeaderView::section {\n"
"    background-color: #5DADE2;\n"
"    color: white;\n"
"    padding: 6px;\n"
"    border: 1px solid #2C3E50;\n"
"}\n"
""));
        label_8 = new QLabel(widget_3);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 100, 101, 31));
        label_8->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}\n"
""));
        label_7 = new QLabel(widget_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(30, 30, 91, 31));
        label_7->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}\n"
""));
        dateEdit_hiring = new QDateEdit(widget_3);
        dateEdit_hiring->setObjectName("dateEdit_hiring");
        dateEdit_hiring->setGeometry(QRect(200, 160, 211, 29));
        dateEdit_hiring->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"QTimeEdit::up-button, QTimeEdit::down-button {\n"
"      background-color: #0078D7; /* Blue arrows */\n"
"    border-radius: 2px;\n"
"    width: 16px; /* Adjust button size */\n"
"}\n"
"QTimeEdit::up-button:hover, QTimeEdit::down-button:hover {\n"
"    background-color: #005bb5;\n"
"}"));
        radioButton_H = new QRadioButton(widget_3);
        radioButton_H->setObjectName("radioButton_H");
        radioButton_H->setGeometry(QRect(200, 30, 111, 41));
        radioButton_H->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        lineEdit_salaire = new QLineEdit(widget_3);
        lineEdit_salaire->setObjectName("lineEdit_salaire");
        lineEdit_salaire->setGeometry(QRect(200, 100, 211, 31));
        lineEdit_salaire->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black; /* Ensure text is visible */\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; /* Light blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Placeholder text color */\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        label_11 = new QLabel(widget_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(40, 240, 131, 31));
        label_11->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}\n"
""));
        comboBox = new QComboBox(widget_3);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(200, 240, 211, 28));
        comboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    background-color: #0078D7; /* Blue arrow */\n"
"    border-radius: 2px;\n"
"    width: 16px; /* Adjust button size */\n"
"}\n"
"\n"
"QComboBox::drop-down:hover {\n"
"    background-color: #005bb5;\n"
"}"));
        addButton = new QPushButton(widget_3);
        addButton->setObjectName("addButton");
        addButton->setGeometry(QRect(320, 300, 101, 31));
        addButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"    background-color: #0078D7; /* Windows blue */\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 8px;\n"
"    font-weight: bold;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        label_9->raise();
        label_8->raise();
        label_7->raise();
        dateEdit_hiring->raise();
        radioButton_H->raise();
        radioButton_F->raise();
        lineEdit_salaire->raise();
        label_11->raise();
        comboBox->raise();
        addButton->raise();
        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(210, 60, 491, 381));
        widget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"border:1px solid;\n"
"border-radius:10px;\n"
"padding:10px"));
        lineEdit_email = new QLineEdit(widget_2);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setGeometry(QRect(230, 320, 211, 31));
        lineEdit_email->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black; /* Ensure text is visible */\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; /* Light blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Placeholder text color */\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        dateEdit_birth = new QDateEdit(widget_2);
        dateEdit_birth->setObjectName("dateEdit_birth");
        dateEdit_birth->setGeometry(QRect(230, 200, 211, 31));
        dateEdit_birth->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"QTimeEdit::up-button, QTimeEdit::down-button {\n"
"      background-color: #0078D7; /* Blue arrows */\n"
"    border-radius: 2px;\n"
"    width: 16px; /* Adjust button size */\n"
"}\n"
"QTimeEdit::up-button:hover, QTimeEdit::down-button:hover {\n"
"    background-color: #005bb5;\n"
"}"));
        label = new QLabel(widget_2);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 20, 91, 31));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}\n"
""));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 80, 101, 31));
        label_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_3 = new QLabel(widget_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 140, 91, 31));
        label_3->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_4 = new QLabel(widget_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 190, 151, 51));
        label_4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_5 = new QLabel(widget_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(40, 250, 111, 41));
        label_5->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_6 = new QLabel(widget_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(40, 320, 91, 41));
        label_6->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; /* Blue color */\n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"/* Apply styles to all QLabel widgets */\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        lineEdit_phone = new QLineEdit(widget_2);
        lineEdit_phone->setObjectName("lineEdit_phone");
        lineEdit_phone->setGeometry(QRect(230, 260, 211, 31));
        lineEdit_phone->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black; /* Ensure text is visible */\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; /* Light blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Placeholder text color */\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        lineEdit_Prenom = new QLineEdit(widget_2);
        lineEdit_Prenom->setObjectName("lineEdit_Prenom");
        lineEdit_Prenom->setGeometry(QRect(230, 140, 211, 31));
        lineEdit_Prenom->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black; /* Ensure text is visible */\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; /* Light blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Placeholder text color */\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        lineEdit_Nom = new QLineEdit(widget_2);
        lineEdit_Nom->setObjectName("lineEdit_Nom");
        lineEdit_Nom->setGeometry(QRect(230, 80, 211, 31));
        lineEdit_Nom->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black; /* Ensure text is visible */\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; /* Light blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Placeholder text color */\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        lineEdit_CIN = new QLineEdit(widget_2);
        lineEdit_CIN->setObjectName("lineEdit_CIN");
        lineEdit_CIN->setGeometry(QRect(230, 20, 211, 31));
        lineEdit_CIN->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black; /* Ensure text is visible */\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; /* Light blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"/* Placeholder text color */\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 10)
            tableWidget->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(0, 550, 1301, 361));
        tableWidget->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1533, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Employ\303\251s", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Clients", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "D\303\251partements", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Formation", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Mat\303\251riels", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Gestion d'employ\303\251es", nullptr));
        modifyBtn->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        saveUpdateBtn->setText(QCoreApplication::translate("MainWindow", "Save Update", nullptr));
        listBtn->setText(QCoreApplication::translate("MainWindow", "Liste d'empoly\303\251es", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Date of hiring", nullptr));
        radioButton_F->setText(QCoreApplication::translate("MainWindow", "Femme", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Salary", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Gender", nullptr));
        radioButton_H->setText(QCoreApplication::translate("MainWindow", "Homme", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Specialty", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Informatique", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Finance", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "communication", nullptr));

        addButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        dateEdit_birth->setDisplayFormat(QCoreApplication::translate("MainWindow", "d/M/yyyy", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Last_name", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "First_name", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Date of birth", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Phone number", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "E-mail", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Last_Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "First_Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Date of birth", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Gender", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Salary", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Date of hiring", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Specialty", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
