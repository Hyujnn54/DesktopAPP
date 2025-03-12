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
#include <QtWidgets/QCalendarWidget>
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
#include <QtWidgets/QTableView>
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
    QLineEdit *Consultant;
    QDateTimeEdit *searchDateButton_2;
    QLabel *label_contact;
    QLabel *label_sector;
    QLineEdit *Contact_info;
    QLabel *label_consultant;
    QPushButton *add;
    QLineEdit *Sector;
    QSpacerItem *horizontalSpacer;
    QLabel *label_name;
    QLineEdit *Name;
    QLabel *label_date;
    QLineEdit *Email;
    QLabel *label_email;
    QWidget *listClient;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *searchGroupBox;
    QHBoxLayout *searchLayout;
    QComboBox *searchCriteriaComboBox;
    QLineEdit *searchInput;
    QPushButton *resetSearchButton;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Delet;
    QPushButton *update;
    QPushButton *exportPdfButton;
    QWidget *calendarTab;
    QVBoxLayout *verticalLayout_7;
    QCalendarWidget *consultationCalendar;
    QGroupBox *calendarDetailsBox;
    QVBoxLayout *verticalLayout_8;
    QLabel *selectedDateLabel;
    QLabel *consultationCountLabel;
    QTableView *dateConsultationsView;
    QWidget *tab;
    QVBoxLayout *verticalLayoutStats;
    QGroupBox *statsTextGroup;
    QVBoxLayout *verticalLayoutText;
    QTextEdit *statsDisplay;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer2;
    QPushButton *openChartButton;
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
        pushButton->setStyleSheet(QString::fromUtf8("#label {\n"
"    color: white;\n"
"    padding: 5px;\n"
"    font-size: 15pt;\n"
"    font-weight: bold;\n"
"    text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.3);\n"
"}"));

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
        ClientForm->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        gridLayout_2 = new QGridLayout(ClientForm);
        gridLayout_2->setObjectName("gridLayout_2");
        Consultant = new QLineEdit(ClientForm);
        Consultant->setObjectName("Consultant");

        gridLayout_2->addWidget(Consultant, 5, 1, 1, 1);

        searchDateButton_2 = new QDateTimeEdit(ClientForm);
        searchDateButton_2->setObjectName("searchDateButton_2");
        searchDateButton_2->setMinimumSize(QSize(125, 0));

        gridLayout_2->addWidget(searchDateButton_2, 4, 1, 1, 1);

        label_contact = new QLabel(ClientForm);
        label_contact->setObjectName("label_contact");

        gridLayout_2->addWidget(label_contact, 2, 0, 1, 1);

        label_sector = new QLabel(ClientForm);
        label_sector->setObjectName("label_sector");

        gridLayout_2->addWidget(label_sector, 1, 0, 1, 1);

        Contact_info = new QLineEdit(ClientForm);
        Contact_info->setObjectName("Contact_info");

        gridLayout_2->addWidget(Contact_info, 2, 1, 1, 1);

        label_consultant = new QLabel(ClientForm);
        label_consultant->setObjectName("label_consultant");

        gridLayout_2->addWidget(label_consultant, 5, 0, 1, 1);

        add = new QPushButton(ClientForm);
        add->setObjectName("add");

        gridLayout_2->addWidget(add, 6, 1, 1, 1);

        Sector = new QLineEdit(ClientForm);
        Sector->setObjectName("Sector");

        gridLayout_2->addWidget(Sector, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 7, 1);

        label_name = new QLabel(ClientForm);
        label_name->setObjectName("label_name");

        gridLayout_2->addWidget(label_name, 0, 0, 1, 1);

        Name = new QLineEdit(ClientForm);
        Name->setObjectName("Name");

        gridLayout_2->addWidget(Name, 0, 1, 1, 1);

        label_date = new QLabel(ClientForm);
        label_date->setObjectName("label_date");

        gridLayout_2->addWidget(label_date, 4, 0, 1, 1);

        Email = new QLineEdit(ClientForm);
        Email->setObjectName("Email");

        gridLayout_2->addWidget(Email, 3, 1, 1, 1);

        label_email = new QLabel(ClientForm);
        label_email->setObjectName("label_email");

        gridLayout_2->addWidget(label_email, 3, 0, 1, 1);

        tabWidget->addTab(ClientForm, QString());
        listClient = new QWidget();
        listClient->setObjectName("listClient");
        verticalLayout_6 = new QVBoxLayout(listClient);
        verticalLayout_6->setObjectName("verticalLayout_6");
        searchGroupBox = new QGroupBox(listClient);
        searchGroupBox->setObjectName("searchGroupBox");
        searchLayout = new QHBoxLayout(searchGroupBox);
        searchLayout->setObjectName("searchLayout");
        searchCriteriaComboBox = new QComboBox(searchGroupBox);
        searchCriteriaComboBox->addItem(QString());
        searchCriteriaComboBox->addItem(QString());
        searchCriteriaComboBox->addItem(QString());
        searchCriteriaComboBox->addItem(QString());
        searchCriteriaComboBox->addItem(QString());
        searchCriteriaComboBox->addItem(QString());
        searchCriteriaComboBox->setObjectName("searchCriteriaComboBox");

        searchLayout->addWidget(searchCriteriaComboBox);

        searchInput = new QLineEdit(searchGroupBox);
        searchInput->setObjectName("searchInput");

        searchLayout->addWidget(searchInput);

        resetSearchButton = new QPushButton(searchGroupBox);
        resetSearchButton->setObjectName("resetSearchButton");

        searchLayout->addWidget(resetSearchButton);


        verticalLayout_6->addWidget(searchGroupBox);

        tableView = new QTableView(listClient);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_6->addWidget(tableView);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        Delet = new QPushButton(listClient);
        Delet->setObjectName("Delet");

        horizontalLayout_4->addWidget(Delet);

        update = new QPushButton(listClient);
        update->setObjectName("update");

        horizontalLayout_4->addWidget(update);

        exportPdfButton = new QPushButton(listClient);
        exportPdfButton->setObjectName("exportPdfButton");

        horizontalLayout_4->addWidget(exportPdfButton);


        verticalLayout_6->addLayout(horizontalLayout_4);

        tabWidget->addTab(listClient, QString());
        calendarTab = new QWidget();
        calendarTab->setObjectName("calendarTab");
        verticalLayout_7 = new QVBoxLayout(calendarTab);
        verticalLayout_7->setObjectName("verticalLayout_7");
        consultationCalendar = new QCalendarWidget(calendarTab);
        consultationCalendar->setObjectName("consultationCalendar");
        consultationCalendar->setMinimumSize(QSize(0, 400));
        consultationCalendar->setGridVisible(true);

        verticalLayout_7->addWidget(consultationCalendar);

        calendarDetailsBox = new QGroupBox(calendarTab);
        calendarDetailsBox->setObjectName("calendarDetailsBox");
        verticalLayout_8 = new QVBoxLayout(calendarDetailsBox);
        verticalLayout_8->setObjectName("verticalLayout_8");
        selectedDateLabel = new QLabel(calendarDetailsBox);
        selectedDateLabel->setObjectName("selectedDateLabel");
        selectedDateLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_8->addWidget(selectedDateLabel);

        consultationCountLabel = new QLabel(calendarDetailsBox);
        consultationCountLabel->setObjectName("consultationCountLabel");
        consultationCountLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_8->addWidget(consultationCountLabel);

        dateConsultationsView = new QTableView(calendarDetailsBox);
        dateConsultationsView->setObjectName("dateConsultationsView");
        dateConsultationsView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        dateConsultationsView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_8->addWidget(dateConsultationsView);


        verticalLayout_7->addWidget(calendarDetailsBox);

        tabWidget->addTab(calendarTab, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayoutStats = new QVBoxLayout(tab);
        verticalLayoutStats->setObjectName("verticalLayoutStats");
        statsTextGroup = new QGroupBox(tab);
        statsTextGroup->setObjectName("statsTextGroup");
        verticalLayoutText = new QVBoxLayout(statsTextGroup);
        verticalLayoutText->setObjectName("verticalLayoutText");
        statsDisplay = new QTextEdit(statsTextGroup);
        statsDisplay->setObjectName("statsDisplay");
        statsDisplay->setReadOnly(true);

        verticalLayoutText->addWidget(statsDisplay);


        verticalLayoutStats->addWidget(statsTextGroup);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer2);

        openChartButton = new QPushButton(tab);
        openChartButton->setObjectName("openChartButton");

        buttonLayout->addWidget(openChartButton);

        refreshStatsButton = new QPushButton(tab);
        refreshStatsButton->setObjectName("refreshStatsButton");

        buttonLayout->addWidget(refreshStatsButton);


        verticalLayoutStats->addLayout(buttonLayout);

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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Client Management System", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Client Management System", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Clients", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Reports", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Calendar", nullptr));
        themeButton->setText(QCoreApplication::translate("MainWindow", "Toggle Theme", nullptr));
        label_contact->setText(QCoreApplication::translate("MainWindow", "Contact Info:", nullptr));
        label_sector->setText(QCoreApplication::translate("MainWindow", "Sector:", nullptr));
        label_consultant->setText(QCoreApplication::translate("MainWindow", "Consultant ID:", nullptr));
        add->setText(QCoreApplication::translate("MainWindow", "Add Client", nullptr));
        label_name->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        label_date->setText(QCoreApplication::translate("MainWindow", "Consultation Date:", nullptr));
        label_email->setText(QCoreApplication::translate("MainWindow", "Email:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ClientForm), QCoreApplication::translate("MainWindow", "Add Client", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        searchCriteriaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Name", nullptr));
        searchCriteriaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Sector", nullptr));
        searchCriteriaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Email", nullptr));
        searchCriteriaComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Contact", nullptr));
        searchCriteriaComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Date  Time", nullptr));
        searchCriteriaComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Consultant", nullptr));

        searchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        resetSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        Delet->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        update->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        exportPdfButton->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(listClient), QCoreApplication::translate("MainWindow", "Manage Clients", nullptr));
        calendarDetailsBox->setTitle(QCoreApplication::translate("MainWindow", "Consultation Details", nullptr));
        selectedDateLabel->setText(QCoreApplication::translate("MainWindow", "Selected date: ", nullptr));
        consultationCountLabel->setText(QCoreApplication::translate("MainWindow", "Consultations: 0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(calendarTab), QCoreApplication::translate("MainWindow", "Consultation Calendar", nullptr));
        statsTextGroup->setTitle(QCoreApplication::translate("MainWindow", "Statistical Summary", nullptr));
        openChartButton->setText(QCoreApplication::translate("MainWindow", "View Chart", nullptr));
        refreshStatsButton->setText(QCoreApplication::translate("MainWindow", "Refresh Statistics", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
