/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *header;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *menuButton;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QFrame *sideMenu;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *themeButton;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *ClientForm;
    QGridLayout *gridLayout_2;
    QLabel *label_name;
    QLineEdit *title;
    QLabel *label_sector;
    QLineEdit *organiser;
    QLabel *label_contact;
    QLineEdit *participant;
    QLabel *label_2;
    QComboBox *agenda;
    QLabel *label_date;
    QDateTimeEdit *dateTimeEdit;
    QLabel *label_consultant;
    QLineEdit *duration;
    QPushButton *AddButton;
    QSpacerItem *horizontalSpacer;
    QWidget *listClient;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *searchGroupBox;
    QHBoxLayout *searchLayout;
    QComboBox *sortComboBox;
    QLineEdit *searchInput;
    QPushButton *searchButton;
    QHBoxLayout *horizontalLayout_4;
    QTableWidget *tableWidget;
    QPushButton *exportPdfButton;
    QPushButton *deleteButton;
    QPushButton *updateButton;
    QPushButton *statisticsButton;
    QPushButton *generateQRCodeButton;
    QWidget *calendarTab;
    QVBoxLayout *verticalLayout_7;
    QTextEdit *chatTextEdit;
    QHBoxLayout *chatInputLayout;
    QLineEdit *chatInputLineEdit;
    QPushButton *chatSendButton;
    QPushButton *chatClearButton;
    QWidget *tab;
    QVBoxLayout *verticalLayoutStats;
    QHBoxLayout *chartsLayout;
    QChartView *agendaChartView;
    QChartView *durationChartView;
    QHBoxLayout *controlsLayout;
    QCheckBox *toggleLegendCheckBox;
    QSpacerItem *horizontalSpacer2;
    QPushButton *refreshStatsButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1387, 758);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 600));
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        header = new QFrame(centralwidget);
        header->setObjectName("header");
        sizePolicy.setHeightForWidth(header->sizePolicy().hasHeightForWidth());
        header->setSizePolicy(sizePolicy);
        header->setMinimumSize(QSize(0, 75));
        header->setMaximumSize(QSize(16777215, 75));
        header->setFrameShape(QFrame::Shape::StyledPanel);
        header->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_2 = new QHBoxLayout(header);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_3 = new QFrame(header);
        frame_3->setObjectName("frame_3");
        frame_3->setMinimumSize(QSize(50, 0));
        frame_3->setMaximumSize(QSize(151, 16777215));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        menuButton = new QPushButton(frame_3);
        menuButton->setObjectName("menuButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(menuButton->sizePolicy().hasHeightForWidth());
        menuButton->setSizePolicy(sizePolicy1);
        menuButton->setMinimumSize(QSize(0, 30));
        menuButton->setMaximumSize(QSize(16777215, 30));
        menuButton->setIconSize(QSize(24, 24));

        horizontalLayout_3->addWidget(menuButton);


        horizontalLayout_2->addWidget(frame_3);

        frame_5 = new QFrame(header);
        frame_5->setObjectName("frame_5");
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_5);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label = new QLabel(frame_5);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(label);


        horizontalLayout_2->addWidget(frame_5);


        verticalLayout->addWidget(header);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sideMenu = new QFrame(frame_2);
        sideMenu->setObjectName("sideMenu");
        sideMenu->setMinimumSize(QSize(150, 0));
        sideMenu->setMaximumSize(QSize(150, 16777215));
        sideMenu->setFrameShape(QFrame::Shape::StyledPanel);
        sideMenu->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(sideMenu);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_6 = new QFrame(sideMenu);
        frame_6->setObjectName("frame_6");
        frame_6->setMinimumSize(QSize(125, 600));
        frame_6->setMaximumSize(QSize(150, 16777215));
        frame_6->setFrameShape(QFrame::Shape::StyledPanel);
        frame_6->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_6);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(frame_6);
        pushButton->setObjectName("pushButton");

        verticalLayout_3->addWidget(pushButton);

        pushButton_2 = new QPushButton(frame_6);
        pushButton_2->setObjectName("pushButton_2");

        verticalLayout_3->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(frame_6);
        pushButton_3->setObjectName("pushButton_3");

        verticalLayout_3->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(frame_6);
        pushButton_4->setObjectName("pushButton_4");

        verticalLayout_3->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(frame_6);
        pushButton_5->setObjectName("pushButton_5");

        verticalLayout_3->addWidget(pushButton_5);

        themeButton = new QPushButton(frame_6);
        themeButton->setObjectName("themeButton");

        verticalLayout_3->addWidget(themeButton);


        verticalLayout_2->addWidget(frame_6, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);


        horizontalLayout->addWidget(sideMenu);

        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName("frame_4");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy2);
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        tabWidget = new QTabWidget(frame_4);
        tabWidget->setObjectName("tabWidget");
        ClientForm = new QWidget();
        ClientForm->setObjectName("ClientForm");
        gridLayout_2 = new QGridLayout(ClientForm);
        gridLayout_2->setObjectName("gridLayout_2");
        label_name = new QLabel(ClientForm);
        label_name->setObjectName("label_name");

        gridLayout_2->addWidget(label_name, 0, 0, 1, 1);

        title = new QLineEdit(ClientForm);
        title->setObjectName("title");

        gridLayout_2->addWidget(title, 0, 2, 1, 1);

        label_sector = new QLabel(ClientForm);
        label_sector->setObjectName("label_sector");

        gridLayout_2->addWidget(label_sector, 1, 0, 1, 1);

        organiser = new QLineEdit(ClientForm);
        organiser->setObjectName("organiser");

        gridLayout_2->addWidget(organiser, 1, 2, 1, 1);

        label_contact = new QLabel(ClientForm);
        label_contact->setObjectName("label_contact");

        gridLayout_2->addWidget(label_contact, 2, 0, 1, 1);

        participant = new QLineEdit(ClientForm);
        participant->setObjectName("participant");

        gridLayout_2->addWidget(participant, 2, 2, 1, 1);

        label_2 = new QLabel(ClientForm);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 3, 0, 1, 1);

        agenda = new QComboBox(ClientForm);
        agenda->addItem(QString());
        agenda->addItem(QString());
        agenda->addItem(QString());
        agenda->setObjectName("agenda");

        gridLayout_2->addWidget(agenda, 3, 2, 1, 1);

        label_date = new QLabel(ClientForm);
        label_date->setObjectName("label_date");

        gridLayout_2->addWidget(label_date, 4, 0, 1, 1);

        dateTimeEdit = new QDateTimeEdit(ClientForm);
        dateTimeEdit->setObjectName("dateTimeEdit");

        gridLayout_2->addWidget(dateTimeEdit, 4, 2, 1, 1);

        label_consultant = new QLabel(ClientForm);
        label_consultant->setObjectName("label_consultant");

        gridLayout_2->addWidget(label_consultant, 5, 0, 1, 1);

        duration = new QLineEdit(ClientForm);
        duration->setObjectName("duration");

        gridLayout_2->addWidget(duration, 5, 2, 1, 1);

        AddButton = new QPushButton(ClientForm);
        AddButton->setObjectName("AddButton");

        gridLayout_2->addWidget(AddButton, 6, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 3, 7, 1);

        tabWidget->addTab(ClientForm, QString());
        listClient = new QWidget();
        listClient->setObjectName("listClient");
        verticalLayout_6 = new QVBoxLayout(listClient);
        verticalLayout_6->setObjectName("verticalLayout_6");
        searchGroupBox = new QGroupBox(listClient);
        searchGroupBox->setObjectName("searchGroupBox");
        searchLayout = new QHBoxLayout(searchGroupBox);
        searchLayout->setObjectName("searchLayout");
        sortComboBox = new QComboBox(searchGroupBox);
        sortComboBox->setObjectName("sortComboBox");

        searchLayout->addWidget(sortComboBox);

        searchInput = new QLineEdit(searchGroupBox);
        searchInput->setObjectName("searchInput");

        searchLayout->addWidget(searchInput);

        searchButton = new QPushButton(searchGroupBox);
        searchButton->setObjectName("searchButton");

        searchLayout->addWidget(searchButton);


        verticalLayout_6->addWidget(searchGroupBox);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        tableWidget = new QTableWidget(listClient);
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
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(tableWidget);

        exportPdfButton = new QPushButton(listClient);
        exportPdfButton->setObjectName("exportPdfButton");

        horizontalLayout_4->addWidget(exportPdfButton);

        deleteButton = new QPushButton(listClient);
        deleteButton->setObjectName("deleteButton");

        horizontalLayout_4->addWidget(deleteButton);

        updateButton = new QPushButton(listClient);
        updateButton->setObjectName("updateButton");

        horizontalLayout_4->addWidget(updateButton);

        statisticsButton = new QPushButton(listClient);
        statisticsButton->setObjectName("statisticsButton");

        horizontalLayout_4->addWidget(statisticsButton);

        generateQRCodeButton = new QPushButton(listClient);
        generateQRCodeButton->setObjectName("generateQRCodeButton");

        horizontalLayout_4->addWidget(generateQRCodeButton);


        verticalLayout_6->addLayout(horizontalLayout_4);

        tabWidget->addTab(listClient, QString());
        calendarTab = new QWidget();
        calendarTab->setObjectName("calendarTab");
        verticalLayout_7 = new QVBoxLayout(calendarTab);
        verticalLayout_7->setObjectName("verticalLayout_7");
        chatTextEdit = new QTextEdit(calendarTab);
        chatTextEdit->setObjectName("chatTextEdit");
        chatTextEdit->setReadOnly(true);

        verticalLayout_7->addWidget(chatTextEdit);

        chatInputLayout = new QHBoxLayout();
        chatInputLayout->setObjectName("chatInputLayout");
        chatInputLineEdit = new QLineEdit(calendarTab);
        chatInputLineEdit->setObjectName("chatInputLineEdit");

        chatInputLayout->addWidget(chatInputLineEdit);

        chatSendButton = new QPushButton(calendarTab);
        chatSendButton->setObjectName("chatSendButton");

        chatInputLayout->addWidget(chatSendButton);

        chatClearButton = new QPushButton(calendarTab);
        chatClearButton->setObjectName("chatClearButton");

        chatInputLayout->addWidget(chatClearButton);


        verticalLayout_7->addLayout(chatInputLayout);

        tabWidget->addTab(calendarTab, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayoutStats = new QVBoxLayout(tab);
        verticalLayoutStats->setObjectName("verticalLayoutStats");
        chartsLayout = new QHBoxLayout();
        chartsLayout->setObjectName("chartsLayout");
        agendaChartView = new QChartView(tab);
        agendaChartView->setObjectName("agendaChartView");
        sizePolicy3.setHeightForWidth(agendaChartView->sizePolicy().hasHeightForWidth());
        agendaChartView->setSizePolicy(sizePolicy3);

        chartsLayout->addWidget(agendaChartView);

        durationChartView = new QChartView(tab);
        durationChartView->setObjectName("durationChartView");
        sizePolicy3.setHeightForWidth(durationChartView->sizePolicy().hasHeightForWidth());
        durationChartView->setSizePolicy(sizePolicy3);

        chartsLayout->addWidget(durationChartView);


        verticalLayoutStats->addLayout(chartsLayout);

        controlsLayout = new QHBoxLayout();
        controlsLayout->setObjectName("controlsLayout");
        toggleLegendCheckBox = new QCheckBox(tab);
        toggleLegendCheckBox->setObjectName("toggleLegendCheckBox");
        toggleLegendCheckBox->setChecked(true);

        controlsLayout->addWidget(toggleLegendCheckBox);

        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlsLayout->addItem(horizontalSpacer2);

        refreshStatsButton = new QPushButton(tab);
        refreshStatsButton->setObjectName("refreshStatsButton");

        controlsLayout->addWidget(refreshStatsButton);


        verticalLayoutStats->addLayout(controlsLayout);

        tabWidget->addTab(tab, QString());

        verticalLayout_5->addWidget(tabWidget);


        horizontalLayout->addWidget(frame_4);


        verticalLayout->addWidget(frame_2);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Meeting Management System", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Meeting Management System", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Clients", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Reports", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Calendar", nullptr));
        themeButton->setText(QCoreApplication::translate("MainWindow", "Toggle Theme", nullptr));
        label_name->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        label_sector->setText(QCoreApplication::translate("MainWindow", "Organiser", nullptr));
        label_contact->setText(QCoreApplication::translate("MainWindow", "Participant", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Agenda", nullptr));
        agenda->setItemText(0, QCoreApplication::translate("MainWindow", "car advice", nullptr));
        agenda->setItemText(1, QCoreApplication::translate("MainWindow", "life advice", nullptr));
        agenda->setItemText(2, QCoreApplication::translate("MainWindow", "future advice", nullptr));

        label_date->setText(QCoreApplication::translate("MainWindow", "Date and Time:", nullptr));
        label_consultant->setText(QCoreApplication::translate("MainWindow", "Duration (min)", nullptr));
        AddButton->setText(QCoreApplication::translate("MainWindow", "Add Meeting", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ClientForm), QCoreApplication::translate("MainWindow", "Add Meeting", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        searchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        searchButton->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Organiser", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Participant", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Agenda", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Duration", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Date and Time", nullptr));
        exportPdfButton->setText(QCoreApplication::translate("MainWindow", "pdf", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        statisticsButton->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        generateQRCodeButton->setText(QCoreApplication::translate("MainWindow", "Generate QR Code", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(listClient), QCoreApplication::translate("MainWindow", "Manage Meetings", nullptr));
        chatInputLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type your message here...", nullptr));
        chatSendButton->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        chatClearButton->setText(QCoreApplication::translate("MainWindow", "Clear Chat", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(calendarTab), QCoreApplication::translate("MainWindow", "Chat Assistant", nullptr));
        toggleLegendCheckBox->setText(QCoreApplication::translate("MainWindow", "Show Legend", nullptr));
        refreshStatsButton->setText(QCoreApplication::translate("MainWindow", "Refresh Statistics", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
