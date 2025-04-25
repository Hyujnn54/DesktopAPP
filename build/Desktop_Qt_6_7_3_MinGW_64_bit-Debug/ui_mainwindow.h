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
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDoubleSpinBox>
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
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
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
    QLabel *headerLabel;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QFrame *sideMenu;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_3;
    QLabel *trainingNotificationLabel;
    QPushButton *clientSectionButton;
    QPushButton *trainingSectionButton;
    QPushButton *reportsButton;
    QPushButton *settingsButton;
    QPushButton *statisticsButton;
    QPushButton *themeButton;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_5;
    QStackedWidget *mainStackedWidget;
    QWidget *clientPage;
    QVBoxLayout *clientPageLayout;
    QTabWidget *clientTabWidget;
    QWidget *clientAddTab;
    QGridLayout *clientAddGridLayout;
    QComboBox *clientConsultantComboBox;
    QDateTimeEdit *clientConsultationDateEdit;
    QLabel *clientContactLabel;
    QLabel *clientSectorLabel;
    QLineEdit *clientContactInfoInput;
    QLabel *clientConsultantLabel;
    QPushButton *clientAddButton;
    QLineEdit *clientSectorInput;
    QSpacerItem *clientAddHorizontalSpacer;
    QLabel *clientNameLabel;
    QLineEdit *clientNameInput;
    QLabel *clientConsultationDateLabel;
    QLineEdit *clientEmailInput;
    QLabel *clientEmailLabel;
    QWidget *clientManageTab;
    QVBoxLayout *clientManageLayout;
    QGroupBox *clientSearchGroupBox;
    QHBoxLayout *clientSearchLayout;
    QComboBox *clientSearchCriteriaComboBox;
    QLineEdit *clientSearchInput;
    QPushButton *clientResetSearchButton;
    QTableView *clientTableView;
    QHBoxLayout *clientManageButtonLayout;
    QSpacerItem *clientManageHorizontalSpacer;
    QPushButton *clientDeleteButton;
    QPushButton *clientUpdateButton;
    QPushButton *clientExportPdfButton;
    QWidget *clientCalendarTab;
    QVBoxLayout *clientCalendarLayout;
    QCalendarWidget *clientConsultationCalendar;
    QGroupBox *clientCalendarDetailsBox;
    QVBoxLayout *clientCalendarDetailsLayout;
    QLabel *clientSelectedDateLabel;
    QLabel *clientConsultationCountLabel;
    QTableView *clientDateConsultationsView;
    QWidget *trainingPage;
    QVBoxLayout *trainingPageLayout;
    QTabWidget *trainingTabWidget;
    QWidget *trainingAddTab;
    QGridLayout *trainingAddGridLayout;
    QLineEdit *trainingDescriptionInput;
    QLabel *trainingPriceLabel;
    QLabel *trainingDescriptionLabel;
    QDateEdit *trainingDateEdit;
    QLabel *trainingTimeLabel;
    QLineEdit *trainingPhoneNumberInput;
    QLineEdit *trainingTrainerInput;
    QDoubleSpinBox *trainingPriceSpinBox;
    QLabel *trainingTrainerLabel;
    QLabel *trainingDateLabel;
    QSpinBox *trainingTimeSpinBox;
    QLineEdit *trainingNameInput;
    QSpacerItem *trainingAddHorizontalSpacer;
    QLabel *trainingNameLabel;
    QPushButton *trainingAddButton;
    QLabel *trainingPhoneLabel;
    QWidget *trainingManageTab;
    QVBoxLayout *trainingManageLayout;
    QGroupBox *trainingSearchGroupBox;
    QHBoxLayout *trainingSearchLayout;
    QComboBox *trainingSearchCriteriaComboBox;
    QLineEdit *trainingSearchInput;
    QPushButton *trainingResetSearchButton;
    QTableView *trainingTableView;
    QHBoxLayout *trainingManageButtonLayout;
    QSpacerItem *trainingManageHorizontalSpacer;
    QPushButton *trainingDeleteButton;
    QPushButton *trainingUpdateButton;
    QPushButton *trainingExportButton;
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        header = new QFrame(centralwidget);
        header->setObjectName("header");
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
        frame_3->setMinimumSize(QSize(114, 0));
        frame_3->setMaximumSize(QSize(167, 16777215));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        menuButton = new QPushButton(frame_3);
        menuButton->setObjectName("menuButton");
        menuButton->setMinimumSize(QSize(0, 30));
        menuButton->setMaximumSize(QSize(16777215, 30));
        menuButton->setIconSize(QSize(24, 24));

        horizontalLayout_3->addWidget(menuButton);


        horizontalLayout_2->addWidget(frame_3);

        frame_5 = new QFrame(header);
        frame_5->setObjectName("frame_5");
        frame_5->setMinimumSize(QSize(0, 0));
        frame_5->setFrameShape(QFrame::Shape::StyledPanel);
        frame_5->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_5);
        verticalLayout_4->setObjectName("verticalLayout_4");
        headerLabel = new QLabel(frame_5);
        headerLabel->setObjectName("headerLabel");
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        headerLabel->setFont(font);
        headerLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(headerLabel);


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
        sideMenu->setMaximumSize(QSize(201, 16777215));
        sideMenu->setFrameShape(QFrame::Shape::StyledPanel);
        sideMenu->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(sideMenu);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        frame_6 = new QFrame(sideMenu);
        frame_6->setObjectName("frame_6");
        frame_6->setMinimumSize(QSize(147, 600));
        frame_6->setMaximumSize(QSize(185, 16777215));
        frame_6->setFrameShape(QFrame::Shape::StyledPanel);
        frame_6->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_6);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        trainingNotificationLabel = new QLabel(frame_6);
        trainingNotificationLabel->setObjectName("trainingNotificationLabel");
        trainingNotificationLabel->setMaximumSize(QSize(1000, 60));
        trainingNotificationLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #3A5DAE;"));
        trainingNotificationLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(trainingNotificationLabel);

        clientSectionButton = new QPushButton(frame_6);
        clientSectionButton->setObjectName("clientSectionButton");

        verticalLayout_3->addWidget(clientSectionButton);

        trainingSectionButton = new QPushButton(frame_6);
        trainingSectionButton->setObjectName("trainingSectionButton");

        verticalLayout_3->addWidget(trainingSectionButton);

        reportsButton = new QPushButton(frame_6);
        reportsButton->setObjectName("reportsButton");

        verticalLayout_3->addWidget(reportsButton);

        settingsButton = new QPushButton(frame_6);
        settingsButton->setObjectName("settingsButton");

        verticalLayout_3->addWidget(settingsButton);

        statisticsButton = new QPushButton(frame_6);
        statisticsButton->setObjectName("statisticsButton");

        verticalLayout_3->addWidget(statisticsButton);

        themeButton = new QPushButton(frame_6);
        themeButton->setObjectName("themeButton");

        verticalLayout_3->addWidget(themeButton);


        verticalLayout_2->addWidget(frame_6, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);


        horizontalLayout->addWidget(sideMenu);

        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName("frame_4");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy1);
        frame_4->setMaximumSize(QSize(1200, 16777215));
        frame_4->setFrameShape(QFrame::Shape::StyledPanel);
        frame_4->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        mainStackedWidget = new QStackedWidget(frame_4);
        mainStackedWidget->setObjectName("mainStackedWidget");
        clientPage = new QWidget();
        clientPage->setObjectName("clientPage");
        clientPageLayout = new QVBoxLayout(clientPage);
        clientPageLayout->setObjectName("clientPageLayout");
        clientTabWidget = new QTabWidget(clientPage);
        clientTabWidget->setObjectName("clientTabWidget");
        clientAddTab = new QWidget();
        clientAddTab->setObjectName("clientAddTab");
        clientAddTab->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        clientAddGridLayout = new QGridLayout(clientAddTab);
        clientAddGridLayout->setObjectName("clientAddGridLayout");
        clientConsultantComboBox = new QComboBox(clientAddTab);
        clientConsultantComboBox->setObjectName("clientConsultantComboBox");

        clientAddGridLayout->addWidget(clientConsultantComboBox, 5, 1, 1, 1);

        clientConsultationDateEdit = new QDateTimeEdit(clientAddTab);
        clientConsultationDateEdit->setObjectName("clientConsultationDateEdit");
        clientConsultationDateEdit->setMinimumSize(QSize(125, 0));

        clientAddGridLayout->addWidget(clientConsultationDateEdit, 4, 1, 1, 1);

        clientContactLabel = new QLabel(clientAddTab);
        clientContactLabel->setObjectName("clientContactLabel");

        clientAddGridLayout->addWidget(clientContactLabel, 2, 0, 1, 1);

        clientSectorLabel = new QLabel(clientAddTab);
        clientSectorLabel->setObjectName("clientSectorLabel");

        clientAddGridLayout->addWidget(clientSectorLabel, 1, 0, 1, 1);

        clientContactInfoInput = new QLineEdit(clientAddTab);
        clientContactInfoInput->setObjectName("clientContactInfoInput");

        clientAddGridLayout->addWidget(clientContactInfoInput, 2, 1, 1, 1);

        clientConsultantLabel = new QLabel(clientAddTab);
        clientConsultantLabel->setObjectName("clientConsultantLabel");

        clientAddGridLayout->addWidget(clientConsultantLabel, 5, 0, 1, 1);

        clientAddButton = new QPushButton(clientAddTab);
        clientAddButton->setObjectName("clientAddButton");

        clientAddGridLayout->addWidget(clientAddButton, 6, 1, 1, 1);

        clientSectorInput = new QLineEdit(clientAddTab);
        clientSectorInput->setObjectName("clientSectorInput");

        clientAddGridLayout->addWidget(clientSectorInput, 1, 1, 1, 1);

        clientAddHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        clientAddGridLayout->addItem(clientAddHorizontalSpacer, 0, 2, 7, 1);

        clientNameLabel = new QLabel(clientAddTab);
        clientNameLabel->setObjectName("clientNameLabel");

        clientAddGridLayout->addWidget(clientNameLabel, 0, 0, 1, 1);

        clientNameInput = new QLineEdit(clientAddTab);
        clientNameInput->setObjectName("clientNameInput");

        clientAddGridLayout->addWidget(clientNameInput, 0, 1, 1, 1);

        clientConsultationDateLabel = new QLabel(clientAddTab);
        clientConsultationDateLabel->setObjectName("clientConsultationDateLabel");

        clientAddGridLayout->addWidget(clientConsultationDateLabel, 4, 0, 1, 1);

        clientEmailInput = new QLineEdit(clientAddTab);
        clientEmailInput->setObjectName("clientEmailInput");

        clientAddGridLayout->addWidget(clientEmailInput, 3, 1, 1, 1);

        clientEmailLabel = new QLabel(clientAddTab);
        clientEmailLabel->setObjectName("clientEmailLabel");

        clientAddGridLayout->addWidget(clientEmailLabel, 3, 0, 1, 1);

        clientTabWidget->addTab(clientAddTab, QString());
        clientManageTab = new QWidget();
        clientManageTab->setObjectName("clientManageTab");
        clientManageLayout = new QVBoxLayout(clientManageTab);
        clientManageLayout->setObjectName("clientManageLayout");
        clientSearchGroupBox = new QGroupBox(clientManageTab);
        clientSearchGroupBox->setObjectName("clientSearchGroupBox");
        clientSearchLayout = new QHBoxLayout(clientSearchGroupBox);
        clientSearchLayout->setObjectName("clientSearchLayout");
        clientSearchCriteriaComboBox = new QComboBox(clientSearchGroupBox);
        clientSearchCriteriaComboBox->addItem(QString());
        clientSearchCriteriaComboBox->addItem(QString());
        clientSearchCriteriaComboBox->addItem(QString());
        clientSearchCriteriaComboBox->addItem(QString());
        clientSearchCriteriaComboBox->addItem(QString());
        clientSearchCriteriaComboBox->addItem(QString());
        clientSearchCriteriaComboBox->setObjectName("clientSearchCriteriaComboBox");

        clientSearchLayout->addWidget(clientSearchCriteriaComboBox);

        clientSearchInput = new QLineEdit(clientSearchGroupBox);
        clientSearchInput->setObjectName("clientSearchInput");

        clientSearchLayout->addWidget(clientSearchInput);

        clientResetSearchButton = new QPushButton(clientSearchGroupBox);
        clientResetSearchButton->setObjectName("clientResetSearchButton");

        clientSearchLayout->addWidget(clientResetSearchButton);


        clientManageLayout->addWidget(clientSearchGroupBox);

        clientTableView = new QTableView(clientManageTab);
        clientTableView->setObjectName("clientTableView");
        clientTableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        clientTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        clientManageLayout->addWidget(clientTableView);

        clientManageButtonLayout = new QHBoxLayout();
        clientManageButtonLayout->setObjectName("clientManageButtonLayout");
        clientManageHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        clientManageButtonLayout->addItem(clientManageHorizontalSpacer);

        clientDeleteButton = new QPushButton(clientManageTab);
        clientDeleteButton->setObjectName("clientDeleteButton");

        clientManageButtonLayout->addWidget(clientDeleteButton);

        clientUpdateButton = new QPushButton(clientManageTab);
        clientUpdateButton->setObjectName("clientUpdateButton");

        clientManageButtonLayout->addWidget(clientUpdateButton);

        clientExportPdfButton = new QPushButton(clientManageTab);
        clientExportPdfButton->setObjectName("clientExportPdfButton");

        clientManageButtonLayout->addWidget(clientExportPdfButton);


        clientManageLayout->addLayout(clientManageButtonLayout);

        clientTabWidget->addTab(clientManageTab, QString());
        clientCalendarTab = new QWidget();
        clientCalendarTab->setObjectName("clientCalendarTab");
        clientCalendarLayout = new QVBoxLayout(clientCalendarTab);
        clientCalendarLayout->setObjectName("clientCalendarLayout");
        clientConsultationCalendar = new QCalendarWidget(clientCalendarTab);
        clientConsultationCalendar->setObjectName("clientConsultationCalendar");
        clientConsultationCalendar->setMinimumSize(QSize(0, 400));
        clientConsultationCalendar->setGridVisible(true);

        clientCalendarLayout->addWidget(clientConsultationCalendar);

        clientCalendarDetailsBox = new QGroupBox(clientCalendarTab);
        clientCalendarDetailsBox->setObjectName("clientCalendarDetailsBox");
        clientCalendarDetailsLayout = new QVBoxLayout(clientCalendarDetailsBox);
        clientCalendarDetailsLayout->setObjectName("clientCalendarDetailsLayout");
        clientSelectedDateLabel = new QLabel(clientCalendarDetailsBox);
        clientSelectedDateLabel->setObjectName("clientSelectedDateLabel");
        clientSelectedDateLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        clientCalendarDetailsLayout->addWidget(clientSelectedDateLabel);

        clientConsultationCountLabel = new QLabel(clientCalendarDetailsBox);
        clientConsultationCountLabel->setObjectName("clientConsultationCountLabel");
        clientConsultationCountLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        clientCalendarDetailsLayout->addWidget(clientConsultationCountLabel);

        clientDateConsultationsView = new QTableView(clientCalendarDetailsBox);
        clientDateConsultationsView->setObjectName("clientDateConsultationsView");
        clientDateConsultationsView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        clientDateConsultationsView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        clientCalendarDetailsLayout->addWidget(clientDateConsultationsView);


        clientCalendarLayout->addWidget(clientCalendarDetailsBox);

        clientTabWidget->addTab(clientCalendarTab, QString());

        clientPageLayout->addWidget(clientTabWidget);

        mainStackedWidget->addWidget(clientPage);
        trainingPage = new QWidget();
        trainingPage->setObjectName("trainingPage");
        trainingPageLayout = new QVBoxLayout(trainingPage);
        trainingPageLayout->setObjectName("trainingPageLayout");
        trainingTabWidget = new QTabWidget(trainingPage);
        trainingTabWidget->setObjectName("trainingTabWidget");
        trainingAddTab = new QWidget();
        trainingAddTab->setObjectName("trainingAddTab");
        trainingAddTab->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        trainingAddGridLayout = new QGridLayout(trainingAddTab);
        trainingAddGridLayout->setObjectName("trainingAddGridLayout");
        trainingDescriptionInput = new QLineEdit(trainingAddTab);
        trainingDescriptionInput->setObjectName("trainingDescriptionInput");

        trainingAddGridLayout->addWidget(trainingDescriptionInput, 1, 2, 1, 1);

        trainingPriceLabel = new QLabel(trainingAddTab);
        trainingPriceLabel->setObjectName("trainingPriceLabel");

        trainingAddGridLayout->addWidget(trainingPriceLabel, 5, 0, 1, 1);

        trainingDescriptionLabel = new QLabel(trainingAddTab);
        trainingDescriptionLabel->setObjectName("trainingDescriptionLabel");

        trainingAddGridLayout->addWidget(trainingDescriptionLabel, 1, 0, 1, 1);

        trainingDateEdit = new QDateEdit(trainingAddTab);
        trainingDateEdit->setObjectName("trainingDateEdit");

        trainingAddGridLayout->addWidget(trainingDateEdit, 4, 2, 1, 1);

        trainingTimeLabel = new QLabel(trainingAddTab);
        trainingTimeLabel->setObjectName("trainingTimeLabel");

        trainingAddGridLayout->addWidget(trainingTimeLabel, 3, 0, 1, 1);

        trainingPhoneNumberInput = new QLineEdit(trainingAddTab);
        trainingPhoneNumberInput->setObjectName("trainingPhoneNumberInput");

        trainingAddGridLayout->addWidget(trainingPhoneNumberInput, 6, 2, 1, 1);

        trainingTrainerInput = new QLineEdit(trainingAddTab);
        trainingTrainerInput->setObjectName("trainingTrainerInput");

        trainingAddGridLayout->addWidget(trainingTrainerInput, 2, 2, 1, 1);

        trainingPriceSpinBox = new QDoubleSpinBox(trainingAddTab);
        trainingPriceSpinBox->setObjectName("trainingPriceSpinBox");

        trainingAddGridLayout->addWidget(trainingPriceSpinBox, 5, 2, 1, 1);

        trainingTrainerLabel = new QLabel(trainingAddTab);
        trainingTrainerLabel->setObjectName("trainingTrainerLabel");

        trainingAddGridLayout->addWidget(trainingTrainerLabel, 2, 0, 1, 1);

        trainingDateLabel = new QLabel(trainingAddTab);
        trainingDateLabel->setObjectName("trainingDateLabel");

        trainingAddGridLayout->addWidget(trainingDateLabel, 4, 0, 1, 1);

        trainingTimeSpinBox = new QSpinBox(trainingAddTab);
        trainingTimeSpinBox->setObjectName("trainingTimeSpinBox");

        trainingAddGridLayout->addWidget(trainingTimeSpinBox, 3, 2, 1, 1);

        trainingNameInput = new QLineEdit(trainingAddTab);
        trainingNameInput->setObjectName("trainingNameInput");

        trainingAddGridLayout->addWidget(trainingNameInput, 0, 2, 1, 1);

        trainingAddHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        trainingAddGridLayout->addItem(trainingAddHorizontalSpacer, 0, 5, 7, 1);

        trainingNameLabel = new QLabel(trainingAddTab);
        trainingNameLabel->setObjectName("trainingNameLabel");

        trainingAddGridLayout->addWidget(trainingNameLabel, 0, 0, 1, 1);

        trainingAddButton = new QPushButton(trainingAddTab);
        trainingAddButton->setObjectName("trainingAddButton");

        trainingAddGridLayout->addWidget(trainingAddButton, 6, 3, 1, 1);

        trainingPhoneLabel = new QLabel(trainingAddTab);
        trainingPhoneLabel->setObjectName("trainingPhoneLabel");

        trainingAddGridLayout->addWidget(trainingPhoneLabel, 6, 0, 1, 1);

        trainingTabWidget->addTab(trainingAddTab, QString());
        trainingManageTab = new QWidget();
        trainingManageTab->setObjectName("trainingManageTab");
        trainingManageLayout = new QVBoxLayout(trainingManageTab);
        trainingManageLayout->setObjectName("trainingManageLayout");
        trainingSearchGroupBox = new QGroupBox(trainingManageTab);
        trainingSearchGroupBox->setObjectName("trainingSearchGroupBox");
        trainingSearchLayout = new QHBoxLayout(trainingSearchGroupBox);
        trainingSearchLayout->setObjectName("trainingSearchLayout");
        trainingSearchCriteriaComboBox = new QComboBox(trainingSearchGroupBox);
        trainingSearchCriteriaComboBox->addItem(QString());
        trainingSearchCriteriaComboBox->addItem(QString());
        trainingSearchCriteriaComboBox->addItem(QString());
        trainingSearchCriteriaComboBox->addItem(QString());
        trainingSearchCriteriaComboBox->setObjectName("trainingSearchCriteriaComboBox");

        trainingSearchLayout->addWidget(trainingSearchCriteriaComboBox);

        trainingSearchInput = new QLineEdit(trainingSearchGroupBox);
        trainingSearchInput->setObjectName("trainingSearchInput");

        trainingSearchLayout->addWidget(trainingSearchInput);

        trainingResetSearchButton = new QPushButton(trainingSearchGroupBox);
        trainingResetSearchButton->setObjectName("trainingResetSearchButton");

        trainingSearchLayout->addWidget(trainingResetSearchButton);


        trainingManageLayout->addWidget(trainingSearchGroupBox);

        trainingTableView = new QTableView(trainingManageTab);
        trainingTableView->setObjectName("trainingTableView");
        trainingTableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        trainingTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        trainingManageLayout->addWidget(trainingTableView);

        trainingManageButtonLayout = new QHBoxLayout();
        trainingManageButtonLayout->setObjectName("trainingManageButtonLayout");
        trainingManageHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        trainingManageButtonLayout->addItem(trainingManageHorizontalSpacer);

        trainingDeleteButton = new QPushButton(trainingManageTab);
        trainingDeleteButton->setObjectName("trainingDeleteButton");

        trainingManageButtonLayout->addWidget(trainingDeleteButton);

        trainingUpdateButton = new QPushButton(trainingManageTab);
        trainingUpdateButton->setObjectName("trainingUpdateButton");

        trainingManageButtonLayout->addWidget(trainingUpdateButton);

        trainingExportButton = new QPushButton(trainingManageTab);
        trainingExportButton->setObjectName("trainingExportButton");

        trainingManageButtonLayout->addWidget(trainingExportButton);


        trainingManageLayout->addLayout(trainingManageButtonLayout);

        trainingTabWidget->addTab(trainingManageTab, QString());

        trainingPageLayout->addWidget(trainingTabWidget);

        mainStackedWidget->addWidget(trainingPage);

        verticalLayout_5->addWidget(mainStackedWidget);


        horizontalLayout->addWidget(frame_4);


        verticalLayout->addWidget(frame_2);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        mainStackedWidget->setCurrentIndex(1);
        clientTabWidget->setCurrentIndex(1);
        trainingTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Client and Training Management System", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        headerLabel->setText(QCoreApplication::translate("MainWindow", "Client and Training Management System", nullptr));
        trainingNotificationLabel->setText(QCoreApplication::translate("MainWindow", "Notifications: 0", nullptr));
        clientSectionButton->setText(QCoreApplication::translate("MainWindow", "Clients", nullptr));
        trainingSectionButton->setText(QCoreApplication::translate("MainWindow", "Training", nullptr));
        reportsButton->setText(QCoreApplication::translate("MainWindow", "Reports", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        statisticsButton->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        themeButton->setText(QCoreApplication::translate("MainWindow", "Toggle Theme", nullptr));
        clientConsultantComboBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "Select Consultant...", nullptr));
        clientContactLabel->setText(QCoreApplication::translate("MainWindow", "Contact Info:", nullptr));
        clientSectorLabel->setText(QCoreApplication::translate("MainWindow", "Sector:", nullptr));
        clientConsultantLabel->setText(QCoreApplication::translate("MainWindow", "Consultant:", nullptr));
        clientAddButton->setText(QCoreApplication::translate("MainWindow", "Add Client", nullptr));
        clientNameLabel->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        clientConsultationDateLabel->setText(QCoreApplication::translate("MainWindow", "Consultation Date:", nullptr));
        clientEmailLabel->setText(QCoreApplication::translate("MainWindow", "Email:", nullptr));
        clientTabWidget->setTabText(clientTabWidget->indexOf(clientAddTab), QCoreApplication::translate("MainWindow", "Add Client", nullptr));
        clientSearchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        clientSearchCriteriaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Name", nullptr));
        clientSearchCriteriaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Sector", nullptr));
        clientSearchCriteriaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Email", nullptr));
        clientSearchCriteriaComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Contact", nullptr));
        clientSearchCriteriaComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Date Time", nullptr));
        clientSearchCriteriaComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Consultant", nullptr));

        clientSearchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        clientResetSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        clientDeleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        clientUpdateButton->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        clientExportPdfButton->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        clientTabWidget->setTabText(clientTabWidget->indexOf(clientManageTab), QCoreApplication::translate("MainWindow", "Manage Clients", nullptr));
        clientCalendarDetailsBox->setTitle(QCoreApplication::translate("MainWindow", "Consultation Details", nullptr));
        clientSelectedDateLabel->setText(QCoreApplication::translate("MainWindow", "Selected date: ", nullptr));
        clientConsultationCountLabel->setText(QCoreApplication::translate("MainWindow", "Consultations: 0", nullptr));
        clientTabWidget->setTabText(clientTabWidget->indexOf(clientCalendarTab), QCoreApplication::translate("MainWindow", "Consultation Calendar", nullptr));
        trainingPriceLabel->setText(QCoreApplication::translate("MainWindow", "Price", nullptr));
        trainingDescriptionLabel->setText(QCoreApplication::translate("MainWindow", "Description", nullptr));
        trainingTimeLabel->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        trainingTrainerLabel->setText(QCoreApplication::translate("MainWindow", "Trainer", nullptr));
        trainingDateLabel->setText(QCoreApplication::translate("MainWindow", "Date:", nullptr));
        trainingNameLabel->setText(QCoreApplication::translate("MainWindow", "Training", nullptr));
        trainingAddButton->setText(QCoreApplication::translate("MainWindow", "Add Training", nullptr));
        trainingPhoneLabel->setText(QCoreApplication::translate("MainWindow", "Phone", nullptr));
        trainingTabWidget->setTabText(trainingTabWidget->indexOf(trainingAddTab), QCoreApplication::translate("MainWindow", "Add Training", nullptr));
        trainingSearchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        trainingSearchCriteriaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Formation", nullptr));
        trainingSearchCriteriaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Trainer", nullptr));
        trainingSearchCriteriaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Price", nullptr));
        trainingSearchCriteriaComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Time", nullptr));

        trainingSearchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        trainingResetSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        trainingDeleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        trainingUpdateButton->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        trainingExportButton->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        trainingTabWidget->setTabText(trainingTabWidget->indexOf(trainingManageTab), QCoreApplication::translate("MainWindow", "Manage Training", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
