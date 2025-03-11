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
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_6;
    QWidget *widget;
    QPushButton *pushButton_2;
    QPushButton *AddButton;
    QLineEdit *title;
    QLineEdit *organiser;
    QLineEdit *participant;
    QLabel *label_4;
    QLabel *label_7;
    QLabel *label_8;
    QDateTimeEdit *date_time;
    QComboBox *agenda;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *duration;
    QTableWidget *tableWidget;
    QFrame *frame;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *on_addButton_clicked;
    QPushButton *pushButton_7;
    QPushButton *deleteButton;
    QPushButton *updateButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1299, 706);
        MainWindow->setStyleSheet(QString::fromUtf8("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, \n"
"        stop:0 rgba(173, 216, 230, 255),\n"
"        stop:1 rgba(255, 255, 255, 255)); "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(460, -40, 511, 131));
        label_6->setBaseSize(QSize(30, 0));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(25);
        font.setBold(true);
        font.setItalic(true);
        font.setUnderline(false);
        font.setStrikeOut(false);
        font.setStyleStrategy(QFont::PreferDefault);
        font.setHintingPreference(QFont::PreferDefaultHinting);
        label_6->setFont(font);
        label_6->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        label_6->setMouseTracking(true);
        label_6->setTabletTracking(false);
        label_6->setStyleSheet(QString::fromUtf8("background-color: transparent"));
        label_6->setWordWrap(false);
        label_6->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(190, 130, 411, 491));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 200); \n"
"    border: 1px solid #A0A0A0;\n"
"    border-radius: 10px;\n"
"    padding: 10px;"));
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(250, 400, 91, 31));
        QFont font1;
        font1.setPointSize(12);
        pushButton_2->setFont(font1);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        AddButton = new QPushButton(widget);
        AddButton->setObjectName("AddButton");
        AddButton->setGeometry(QRect(80, 400, 91, 31));
        AddButton->setFont(font1);
        AddButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        AddButton->setFlat(false);
        title = new QLineEdit(widget);
        title->setObjectName("title");
        title->setGeometry(QRect(120, 50, 161, 22));
        title->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; \n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        organiser = new QLineEdit(widget);
        organiser->setObjectName("organiser");
        organiser->setGeometry(QRect(120, 170, 161, 22));
        organiser->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; \n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        participant = new QLineEdit(widget);
        participant->setObjectName("participant");
        participant->setGeometry(QRect(120, 240, 161, 22));
        participant->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; \n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(80, 260, 151, 71));
        label_4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; \n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_7 = new QLabel(widget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(110, 70, 171, 41));
        label_7->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; \n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_8 = new QLabel(widget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(170, 320, 71, 41));
        label_8->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; \n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        date_time = new QDateTimeEdit(widget);
        date_time->setObjectName("date_time");
        date_time->setGeometry(QRect(110, 110, 194, 22));
        agenda = new QComboBox(widget);
        agenda->addItem(QString());
        agenda->addItem(QString());
        agenda->addItem(QString());
        agenda->setObjectName("agenda");
        agenda->setGeometry(QRect(210, 280, 171, 22));
        label_9 = new QLabel(widget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(140, 200, 141, 41));
        label_9->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; \n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_10 = new QLabel(widget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(130, 140, 151, 31));
        label_10->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; \n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        label_11 = new QLabel(widget);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(150, 0, 81, 51));
        label_11->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #0078D7; \n"
"    border: none;\n"
"    background: transparent;\n"
"text-align: center;\n"
"padding: 5px;\n"
"}\n"
"\n"
"\n"
"QLabel {\n"
"    font-size: 14px;\n"
"    color: black;\n"
"}"));
        duration = new QLineEdit(widget);
        duration->setObjectName("duration");
        duration->setGeometry(QRect(130, 360, 161, 22));
        duration->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 1px solid #0078D7; \n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit::placeholder {\n"
"    color: gray;\n"
"}"));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
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
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(630, 130, 601, 281));
        tableWidget->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: white;\n"
"    border: 1px solid #A0A0A0;\n"
"    border-radius: 3px;\n"
"    gridline-color: #A0A0A0;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #0078D7;\n"
"    color: white;\n"
"    padding: 2px;\n"
"    border: 1px solid #A0A0A0;\n"
"}"));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 181, 661));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_3 = new QPushButton(frame);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(30, 90, 101, 41));
        pushButton_3->setFont(font1);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_3->setFlat(false);
        pushButton_4 = new QPushButton(frame);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(30, 190, 101, 41));
        pushButton_4->setFont(font1);
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_4->setFlat(false);
        pushButton_5 = new QPushButton(frame);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(30, 280, 101, 41));
        pushButton_5->setFont(font1);
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_5->setFlat(false);
        on_addButton_clicked = new QPushButton(frame);
        on_addButton_clicked->setObjectName("on_addButton_clicked");
        on_addButton_clicked->setGeometry(QRect(30, 380, 101, 41));
        on_addButton_clicked->setFont(font1);
        on_addButton_clicked->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        on_addButton_clicked->setFlat(false);
        pushButton_7 = new QPushButton(frame);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(30, 470, 101, 41));
        pushButton_7->setFont(font1);
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #0078D7; \n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #0056A3;\n"
"}"));
        pushButton_7->setFlat(false);
        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setGeometry(QRect(640, 430, 93, 29));
        QFont font2;
        font2.setStrikeOut(false);
        deleteButton->setFont(font2);
        updateButton = new QPushButton(centralwidget);
        updateButton->setObjectName("updateButton");
        updateButton->setGeometry(QRect(800, 430, 93, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1299, 26));
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
        label_6->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:25pt; font-weight:700; font-style:italic;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:26pt; color:#0000ff;\">Ajouter un r\303\251union</span></p></body></html>", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "effacer", nullptr));
        AddButton->setText(QCoreApplication::translate("MainWindow", "ajouter ", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">Agenda :</span></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">date et heure</span></p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">Durre :</span></p></body></html>", nullptr));
        agenda->setItemText(0, QCoreApplication::translate("MainWindow", "car advice", nullptr));
        agenda->setItemText(1, QCoreApplication::translate("MainWindow", "life advice", nullptr));
        agenda->setItemText(2, QCoreApplication::translate("MainWindow", "future adive", nullptr));

        label_9->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">Participants</span></p></body></html>", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">Organisateur</span></p></body></html>", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">Titre</span></p></body></html>", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Titre ", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Organisateur ", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Participants", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Agenda ", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Dur\303\251e ", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Statut ", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Employee ", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Client ", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Reservation ", nullptr));
        on_addButton_clicked->setText(QCoreApplication::translate("MainWindow", "r\303\251union", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "formation ", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "delete", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindow", "update", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
