/********************************************************************************
** Form generated from reading UI file 'formationwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMATIONWINDOW_H
#define UI_FORMATIONWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_formationwindow
{
public:
    QWidget *centralwidget;
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
    QLineEdit *des;
    QLabel *label_consultant;
    QLabel *label_sector;
    QDateEdit *date;
    QLabel *label_email;
    QLineEdit *phoneNumberInput;
    QLineEdit *tr;
    QDoubleSpinBox *prixb;
    QLabel *label_contact;
    QLabel *label_date;
    QSpinBox *timeb;
    QLabel *notificationLabel;
    QLineEdit *format;
    QSpacerItem *horizontalSpacer;
    QLabel *label_name;
    QPushButton *add;
    QLabel *label_2;
    QWidget *listClient;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *searchGroupBox;
    QHBoxLayout *searchLayout;
    QComboBox *searchCriteriaComboBox;
    QLineEdit *searchInput;
    QPushButton *resetSearchButton;
    QTableView *tabtr;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *deletef;
    QPushButton *updateButton;
    QPushButton *exportButton;
    QWidget *widget;
    QChartView *statsChartView;
    QPushButton *refreshStatsButton;
    QWidget *waiting_room;
    QPlainTextEdit *setText;
    QPushButton *wrr;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *formationwindow)
    {
        if (formationwindow->objectName().isEmpty())
            formationwindow->setObjectName("formationwindow");
        formationwindow->resize(1113, 685);
        formationwindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(formationwindow);
        centralwidget->setObjectName("centralwidget");
        header = new QFrame(centralwidget);
        header->setObjectName("header");
        header->setGeometry(QRect(0, 0, 1071, 75));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
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
        frame_3->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
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
        frame_5->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
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

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(0, 80, 1071, 551));
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
        sideMenu->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"border-color: rgb(0, 0, 0);"));
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
        frame_6->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"border-color: rgb(0, 0, 0);"));
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
        frame_4->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"border-color: rgb(0, 0, 0);\n"
""));
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        tabWidget = new QTabWidget(frame_4);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"border-color: rgb(0, 0, 0);"));
        ClientForm = new QWidget();
        ClientForm->setObjectName("ClientForm");
        ClientForm->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        gridLayout_2 = new QGridLayout(ClientForm);
        gridLayout_2->setObjectName("gridLayout_2");
        des = new QLineEdit(ClientForm);
        des->setObjectName("des");

        gridLayout_2->addWidget(des, 1, 2, 1, 1);

        label_consultant = new QLabel(ClientForm);
        label_consultant->setObjectName("label_consultant");

        gridLayout_2->addWidget(label_consultant, 5, 0, 1, 1);

        label_sector = new QLabel(ClientForm);
        label_sector->setObjectName("label_sector");

        gridLayout_2->addWidget(label_sector, 1, 0, 1, 1);

        date = new QDateEdit(ClientForm);
        date->setObjectName("date");

        gridLayout_2->addWidget(date, 4, 2, 1, 1);

        label_email = new QLabel(ClientForm);
        label_email->setObjectName("label_email");

        gridLayout_2->addWidget(label_email, 3, 0, 1, 1);

        phoneNumberInput = new QLineEdit(ClientForm);
        phoneNumberInput->setObjectName("phoneNumberInput");

        gridLayout_2->addWidget(phoneNumberInput, 6, 2, 1, 1);

        tr = new QLineEdit(ClientForm);
        tr->setObjectName("tr");

        gridLayout_2->addWidget(tr, 2, 2, 1, 1);

        prixb = new QDoubleSpinBox(ClientForm);
        prixb->setObjectName("prixb");

        gridLayout_2->addWidget(prixb, 5, 2, 1, 1);

        label_contact = new QLabel(ClientForm);
        label_contact->setObjectName("label_contact");

        gridLayout_2->addWidget(label_contact, 2, 0, 1, 1);

        label_date = new QLabel(ClientForm);
        label_date->setObjectName("label_date");

        gridLayout_2->addWidget(label_date, 4, 0, 1, 1);

        timeb = new QSpinBox(ClientForm);
        timeb->setObjectName("timeb");

        gridLayout_2->addWidget(timeb, 3, 2, 1, 1);

        notificationLabel = new QLabel(ClientForm);
        notificationLabel->setObjectName("notificationLabel");
        notificationLabel->setPixmap(QPixmap(QString::fromUtf8("../../../../Mega Pc/Downloads/Ra.png")));
        notificationLabel->setScaledContents(false);
        notificationLabel->setWordWrap(false);

        gridLayout_2->addWidget(notificationLabel, 1, 6, 1, 1);

        format = new QLineEdit(ClientForm);
        format->setObjectName("format");

        gridLayout_2->addWidget(format, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 5, 7, 1);

        label_name = new QLabel(ClientForm);
        label_name->setObjectName("label_name");

        gridLayout_2->addWidget(label_name, 0, 0, 1, 1);

        add = new QPushButton(ClientForm);
        add->setObjectName("add");

        gridLayout_2->addWidget(add, 6, 3, 1, 1);

        label_2 = new QLabel(ClientForm);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 6, 0, 1, 1);

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
        searchCriteriaComboBox->setObjectName("searchCriteriaComboBox");

        searchLayout->addWidget(searchCriteriaComboBox);

        searchInput = new QLineEdit(searchGroupBox);
        searchInput->setObjectName("searchInput");

        searchLayout->addWidget(searchInput);

        resetSearchButton = new QPushButton(searchGroupBox);
        resetSearchButton->setObjectName("resetSearchButton");

        searchLayout->addWidget(resetSearchButton);


        verticalLayout_6->addWidget(searchGroupBox);

        tabtr = new QTableView(listClient);
        tabtr->setObjectName("tabtr");
        tabtr->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tabtr->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_6->addWidget(tabtr);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        deletef = new QPushButton(listClient);
        deletef->setObjectName("deletef");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imgs/delete.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        deletef->setIcon(icon);

        horizontalLayout_4->addWidget(deletef);

        updateButton = new QPushButton(listClient);
        updateButton->setObjectName("updateButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/imgs/Downloads/6372226-removebg-preview.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        updateButton->setIcon(icon1);
        updateButton->setIconSize(QSize(13, 12));

        horizontalLayout_4->addWidget(updateButton);

        exportButton = new QPushButton(listClient);
        exportButton->setObjectName("exportButton");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/imgs/file.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        exportButton->setIcon(icon2);

        horizontalLayout_4->addWidget(exportButton);


        verticalLayout_6->addLayout(horizontalLayout_4);

        tabWidget->addTab(listClient, QString());
        widget = new QWidget();
        widget->setObjectName("widget");
        statsChartView = new QChartView(widget);
        statsChartView->setObjectName("statsChartView");
        statsChartView->setGeometry(QRect(10, 10, 871, 461));
        refreshStatsButton = new QPushButton(widget);
        refreshStatsButton->setObjectName("refreshStatsButton");
        refreshStatsButton->setGeometry(QRect(10, 480, 80, 18));
        tabWidget->addTab(widget, QString());
        waiting_room = new QWidget();
        waiting_room->setObjectName("waiting_room");
        setText = new QPlainTextEdit(waiting_room);
        setText->setObjectName("setText");
        setText->setGeometry(QRect(170, 180, 104, 70));
        wrr = new QPushButton(waiting_room);
        wrr->setObjectName("wrr");
        wrr->setGeometry(QRect(230, 290, 80, 18));
        tabWidget->addTab(waiting_room, QString());

        verticalLayout_5->addWidget(tabWidget);


        horizontalLayout->addWidget(frame_4);

        formationwindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(formationwindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1113, 21));
        formationwindow->setMenuBar(menubar);
        statusbar = new QStatusBar(formationwindow);
        statusbar->setObjectName("statusbar");
        formationwindow->setStatusBar(statusbar);

        retranslateUi(formationwindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(formationwindow);
    } // setupUi

    void retranslateUi(QMainWindow *formationwindow)
    {
        formationwindow->setWindowTitle(QCoreApplication::translate("formationwindow", "formationwindow", nullptr));
        menuButton->setText(QCoreApplication::translate("formationwindow", "Menu", nullptr));
        label->setText(QCoreApplication::translate("formationwindow", "TrainingManagement System", nullptr));
        pushButton->setText(QCoreApplication::translate("formationwindow", "Dashboard", nullptr));
        pushButton_2->setText(QCoreApplication::translate("formationwindow", "Clients", nullptr));
        pushButton_3->setText(QCoreApplication::translate("formationwindow", "Reports", nullptr));
        pushButton_4->setText(QCoreApplication::translate("formationwindow", "Settings", nullptr));
        pushButton_5->setText(QCoreApplication::translate("formationwindow", "Calendar", nullptr));
        themeButton->setText(QCoreApplication::translate("formationwindow", "Toggle Theme", nullptr));
#if QT_CONFIG(whatsthis)
        tabWidget->setWhatsThis(QCoreApplication::translate("formationwindow", "<html><head/><body><p>ddd</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        label_consultant->setText(QCoreApplication::translate("formationwindow", "prix", nullptr));
        label_sector->setText(QCoreApplication::translate("formationwindow", "description", nullptr));
        label_email->setText(QCoreApplication::translate("formationwindow", "time", nullptr));
        label_contact->setText(QCoreApplication::translate("formationwindow", "trainer", nullptr));
        label_date->setText(QCoreApplication::translate("formationwindow", " Date:", nullptr));
        notificationLabel->setText(QString());
        label_name->setText(QCoreApplication::translate("formationwindow", "Training", nullptr));
        add->setText(QCoreApplication::translate("formationwindow", "Add formation", nullptr));
        label_2->setText(QCoreApplication::translate("formationwindow", "phone", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(ClientForm), QCoreApplication::translate("formationwindow", "Add Training", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("formationwindow", "Search", nullptr));
        searchCriteriaComboBox->setItemText(0, QCoreApplication::translate("formationwindow", "formation", nullptr));
        searchCriteriaComboBox->setItemText(1, QCoreApplication::translate("formationwindow", "trainer", nullptr));
        searchCriteriaComboBox->setItemText(2, QCoreApplication::translate("formationwindow", "prix", nullptr));
        searchCriteriaComboBox->setItemText(3, QCoreApplication::translate("formationwindow", "time", nullptr));
        searchCriteriaComboBox->setItemText(4, QString());

        searchInput->setPlaceholderText(QCoreApplication::translate("formationwindow", "Type to search...", nullptr));
        resetSearchButton->setText(QCoreApplication::translate("formationwindow", "Reset Search", nullptr));
        deletef->setText(QString());
        updateButton->setText(QCoreApplication::translate("formationwindow", "update", nullptr));
        exportButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(listClient), QCoreApplication::translate("formationwindow", "Manage Training", nullptr));
        refreshStatsButton->setText(QCoreApplication::translate("formationwindow", "Statistics", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget), QCoreApplication::translate("formationwindow", "Statistics", nullptr));
        setText->setPlainText(QString());
        wrr->setText(QCoreApplication::translate("formationwindow", "-", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(waiting_room), QCoreApplication::translate("formationwindow", "Waiting room", nullptr));
    } // retranslateUi

};

namespace Ui {
    class formationwindow: public Ui_formationwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMATIONWINDOW_H
