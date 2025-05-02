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
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
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
    QLabel *headerLabel;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QFrame *sideMenu;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_3;
    QPushButton *trainingNotificationLabel;
    QPushButton *clientSectionButton;
    QPushButton *trainingSectionButton;
    QPushButton *meetingSectionButton;
    QPushButton *employeeSectionButton;
    QPushButton *resourceSectionButton;
    QPushButton *settingsButton;
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
    QHBoxLayout *clientCalendarLayout;
    QCalendarWidget *clientConsultationCalendar;
    QGroupBox *clientCalendarDetailsBox;
    QVBoxLayout *clientCalendarDetailsLayout;
    QLabel *clientSelectedDateLabel;
    QLabel *clientConsultationCountLabel;
    QTableView *clientDateConsultationsView;
    QWidget *clientStatisticsTab;
    QVBoxLayout *clientStatsLayout;
    QGroupBox *clientChartControlsGroupBox;
    QHBoxLayout *clientChartControlsLayout;
    QLabel *clientChartTypeLabel;
    QComboBox *clientChartTypeComboBox;
    QLabel *clientChartFilterLabel;
    QComboBox *clientChartFilterComboBox;
    QCheckBox *clientToggleLegendCheckBox;
    QPushButton *clientChartRefreshButton;
    QChartView *clientChartView;
    QLabel *clientHoverDescriptionLabel;
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
    QWidget *trainingStatisticsTab;
    QVBoxLayout *trainingStatsLayout;
    QGroupBox *trainingChartControlsGroupBox;
    QHBoxLayout *trainingChartControlsLayout;
    QLabel *trainingChartTypeLabel;
    QComboBox *trainingChartTypeComboBox;
    QLabel *trainingChartFilterLabel;
    QComboBox *trainingChartFilterComboBox;
    QCheckBox *trainingToggleLegendCheckBox;
    QPushButton *trainingChartRefreshButton;
    QChartView *trainingChartView;
    QLabel *trainingHoverDescriptionLabel;
    QWidget *meetingPage;
    QVBoxLayout *meetingPageLayout;
    QTabWidget *meetingTabWidget;
    QWidget *meetingAddTab;
    QGridLayout *meetingAddGridLayout;
    QLabel *meetingTitleLabel;
    QLineEdit *meetingTitleInput;
    QLabel *meetingOrganiserLabel;
    QLineEdit *meetingOrganiserInput;
    QLabel *meetingParticipantLabel;
    QLineEdit *meetingParticipantInput;
    QLabel *meetingAgendaLabel;
    QComboBox *meetingAgendaComboBox;
    QLabel *meetingDateLabel;
    QDateTimeEdit *meetingDateTimeEdit;
    QLabel *meetingDurationLabel;
    QLineEdit *meetingDurationInput;
    QPushButton *meetingAddButton;
    QSpacerItem *meetingAddHorizontalSpacer;
    QWidget *meetingManageTab;
    QVBoxLayout *meetingManageLayout;
    QGroupBox *meetingSearchGroupBox;
    QHBoxLayout *meetingSearchLayout;
    QComboBox *meetingSearchCriteriaComboBox;
    QLineEdit *meetingSearchInput;
    QPushButton *meetingResetSearchButton;
    QTableWidget *meetingTableWidget;
    QHBoxLayout *meetingManageButtonLayout;
    QSpacerItem *meetingManageHorizontalSpacer;
    QPushButton *meetingDeleteButton;
    QPushButton *meetingUpdateButton;
    QPushButton *meetingGenerateQRCodeButton;
    QPushButton *meetingExportPdfButton;
    QWidget *meetingChatTab;
    QVBoxLayout *meetingChatLayout;
    QTextEdit *meetingChatTextEdit;
    QHBoxLayout *meetingChatInputLayout;
    QLineEdit *meetingChatInputLineEdit;
    QPushButton *meetingChatSendButton;
    QPushButton *meetingChatClearButton;
    QWidget *meetingStatisticsTab;
    QVBoxLayout *meetingStatsLayout;
    QGroupBox *meetingChartControlsGroupBox;
    QHBoxLayout *meetingChartControlsLayout;
    QLabel *meetingChartTypeLabel;
    QComboBox *meetingChartTypeComboBox;
    QLabel *meetingChartFilterLabel;
    QComboBox *meetingChartFilterComboBox;
    QCheckBox *meetingToggleLegendCheckBox;
    QPushButton *meetingChartRefreshButton;
    QChartView *meetingChartView;
    QLabel *meetingHoverDescriptionLabel;
    QWidget *employeePage;
    QVBoxLayout *employeePageLayout;
    QTabWidget *employeeTabWidget;
    QWidget *AddEmployee;
    QGridLayout *gridLayout;
    QLineEdit *hiddenId;
    QLineEdit *lineEdit_salaire;
    QDateEdit *dateEdit_hiring;
    QLabel *label_8;
    QLabel *label_3;
    QLineEdit *lineEdit_email;
    QDateEdit *dateEdit_birth;
    QLabel *label;
    QLineEdit *lineEdit_Prenom;
    QLabel *label_9;
    QLabel *label_5;
    QLabel *label_4;
    QPushButton *addButton;
    QLabel *label_11;
    QComboBox *comboBox;
    QLabel *label_2;
    QRadioButton *radioButton_H;
    QLineEdit *lineEdit_Nom;
    QLabel *label_6;
    QLineEdit *lineEdit_CIN;
    QLabel *label_7;
    QLineEdit *lineEdit_phone;
    QRadioButton *radioButton_F;
    QLabel *label_image;
    QHBoxLayout *imageLayout;
    QLineEdit *imagePathLineEdit;
    QPushButton *selectImageButton;
    QLabel *label_role;
    QComboBox *roleComboBox;
    QWidget *listEmployee;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *searchGroupBox;
    QHBoxLayout *searchLayout;
    QComboBox *searchCriteriaComboBox;
    QLineEdit *searchInput;
    QPushButton *resetSearchButton;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *deleteBtn;
    QPushButton *modifyBtn;
    QPushButton *saveUpdateBtn;
    QPushButton *downloadBtn;
    QPushButton *generateQRCodeBtn;
    QWidget *employeeStatsTab;
    QVBoxLayout *verticalLayoutStats;
    QWidget *chartsContainer;
    QVBoxLayout *chartsLayout;
    QLabel *employeeHoverLabel;
    QLabel *employeeTotalCountLabel;
    QLabel *employeeAverageSalaryLabel;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer2;
    QPushButton *refreshStatsButton;
    QWidget *resourcePage;
    QVBoxLayout *resourcePageLayout;
    QTabWidget *resourceTabWidget;
    QWidget *resourceAddTab;
    QGridLayout *gridLayout_2;
    QLabel *label_brand;
    QLineEdit *quantityLineEdit;
    QPushButton *confirmFormButton;
    QLabel *lblImagePreview;
    QSpacerItem *resourceHorizontalSpacer;
    QLabel *label_quantity;
    QPushButton *cancelFormButton;
    QLabel *label_name;
    QComboBox *typeComboBox;
    QLineEdit *nameLineEdit;
    QLabel *label_type;
    QPushButton *btnSelectImage;
    QLineEdit *brandLineEdit;
    QLabel *label_date;
    QDateEdit *purchaseDateEdit;
    QWidget *resourceManageTab;
    QVBoxLayout *resourceManageLayout;
    QGroupBox *resourceSearchGroupBox;
    QHBoxLayout *resourceSearchLayout;
    QLineEdit *searchLineEdit;
    QPushButton *resetResourceSearchButton;
    QTableWidget *tableWidget;
    QHBoxLayout *resourceButtonLayout;
    QPushButton *exportPdfButton;
    QPushButton *downloadHistoryButton;
    QPushButton *clearHistoryButton;
    QPushButton *btnLookForResource;
    QSpacerItem *resourceHorizontalSpacer2;
    QPushButton *deleteButton;
    QPushButton *updateButton;
    QWidget *resourceStatisticsTab;
    QVBoxLayout *resourceStatsLayout;
    QGroupBox *statsTextGroup;
    QVBoxLayout *resourceVerticalLayoutStats;
    QChartView *chartView;
    QChartView *stackedBarChartView;
    QHBoxLayout *resourceStatsButtonLayout;
    QSpacerItem *horizontalSpacer21;
    QPushButton *resourceRefreshStatsButton;
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
        trainingNotificationLabel = new QPushButton(frame_6);
        trainingNotificationLabel->setObjectName("trainingNotificationLabel");
        trainingNotificationLabel->setStyleSheet(QString::fromUtf8("QPushButton { color: blue; text-decoration: underline; border: none; background: transparent; }\n"
"QPushButton:hover { color: darkblue; }"));

        verticalLayout_3->addWidget(trainingNotificationLabel);

        clientSectionButton = new QPushButton(frame_6);
        clientSectionButton->setObjectName("clientSectionButton");

        verticalLayout_3->addWidget(clientSectionButton);

        trainingSectionButton = new QPushButton(frame_6);
        trainingSectionButton->setObjectName("trainingSectionButton");

        verticalLayout_3->addWidget(trainingSectionButton);

        meetingSectionButton = new QPushButton(frame_6);
        meetingSectionButton->setObjectName("meetingSectionButton");

        verticalLayout_3->addWidget(meetingSectionButton);

        employeeSectionButton = new QPushButton(frame_6);
        employeeSectionButton->setObjectName("employeeSectionButton");

        verticalLayout_3->addWidget(employeeSectionButton);

        resourceSectionButton = new QPushButton(frame_6);
        resourceSectionButton->setObjectName("resourceSectionButton");

        verticalLayout_3->addWidget(resourceSectionButton);

        settingsButton = new QPushButton(frame_6);
        settingsButton->setObjectName("settingsButton");

        verticalLayout_3->addWidget(settingsButton);

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
        clientCalendarLayout = new QHBoxLayout(clientCalendarTab);
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
        clientStatisticsTab = new QWidget();
        clientStatisticsTab->setObjectName("clientStatisticsTab");
        clientStatsLayout = new QVBoxLayout(clientStatisticsTab);
        clientStatsLayout->setObjectName("clientStatsLayout");
        clientChartControlsGroupBox = new QGroupBox(clientStatisticsTab);
        clientChartControlsGroupBox->setObjectName("clientChartControlsGroupBox");
        clientChartControlsLayout = new QHBoxLayout(clientChartControlsGroupBox);
        clientChartControlsLayout->setObjectName("clientChartControlsLayout");
        clientChartTypeLabel = new QLabel(clientChartControlsGroupBox);
        clientChartTypeLabel->setObjectName("clientChartTypeLabel");

        clientChartControlsLayout->addWidget(clientChartTypeLabel);

        clientChartTypeComboBox = new QComboBox(clientChartControlsGroupBox);
        clientChartTypeComboBox->addItem(QString());
        clientChartTypeComboBox->addItem(QString());
        clientChartTypeComboBox->setObjectName("clientChartTypeComboBox");

        clientChartControlsLayout->addWidget(clientChartTypeComboBox);

        clientChartFilterLabel = new QLabel(clientChartControlsGroupBox);
        clientChartFilterLabel->setObjectName("clientChartFilterLabel");

        clientChartControlsLayout->addWidget(clientChartFilterLabel);

        clientChartFilterComboBox = new QComboBox(clientChartControlsGroupBox);
        clientChartFilterComboBox->addItem(QString());
        clientChartFilterComboBox->addItem(QString());
        clientChartFilterComboBox->addItem(QString());
        clientChartFilterComboBox->setObjectName("clientChartFilterComboBox");

        clientChartControlsLayout->addWidget(clientChartFilterComboBox);

        clientToggleLegendCheckBox = new QCheckBox(clientChartControlsGroupBox);
        clientToggleLegendCheckBox->setObjectName("clientToggleLegendCheckBox");
        clientToggleLegendCheckBox->setChecked(true);

        clientChartControlsLayout->addWidget(clientToggleLegendCheckBox);

        clientChartRefreshButton = new QPushButton(clientChartControlsGroupBox);
        clientChartRefreshButton->setObjectName("clientChartRefreshButton");

        clientChartControlsLayout->addWidget(clientChartRefreshButton);


        clientStatsLayout->addWidget(clientChartControlsGroupBox);

        clientChartView = new QChartView(clientStatisticsTab);
        clientChartView->setObjectName("clientChartView");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(clientChartView->sizePolicy().hasHeightForWidth());
        clientChartView->setSizePolicy(sizePolicy2);
        clientChartView->setMinimumSize(QSize(0, 400));

        clientStatsLayout->addWidget(clientChartView);

        clientHoverDescriptionLabel = new QLabel(clientStatisticsTab);
        clientHoverDescriptionLabel->setObjectName("clientHoverDescriptionLabel");
        clientHoverDescriptionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        clientStatsLayout->addWidget(clientHoverDescriptionLabel);

        clientTabWidget->addTab(clientStatisticsTab, QString());

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
        trainingStatisticsTab = new QWidget();
        trainingStatisticsTab->setObjectName("trainingStatisticsTab");
        trainingStatsLayout = new QVBoxLayout(trainingStatisticsTab);
        trainingStatsLayout->setObjectName("trainingStatsLayout");
        trainingChartControlsGroupBox = new QGroupBox(trainingStatisticsTab);
        trainingChartControlsGroupBox->setObjectName("trainingChartControlsGroupBox");
        trainingChartControlsLayout = new QHBoxLayout(trainingChartControlsGroupBox);
        trainingChartControlsLayout->setObjectName("trainingChartControlsLayout");
        trainingChartTypeLabel = new QLabel(trainingChartControlsGroupBox);
        trainingChartTypeLabel->setObjectName("trainingChartTypeLabel");

        trainingChartControlsLayout->addWidget(trainingChartTypeLabel);

        trainingChartTypeComboBox = new QComboBox(trainingChartControlsGroupBox);
        trainingChartTypeComboBox->addItem(QString());
        trainingChartTypeComboBox->addItem(QString());
        trainingChartTypeComboBox->setObjectName("trainingChartTypeComboBox");

        trainingChartControlsLayout->addWidget(trainingChartTypeComboBox);

        trainingChartFilterLabel = new QLabel(trainingChartControlsGroupBox);
        trainingChartFilterLabel->setObjectName("trainingChartFilterLabel");

        trainingChartControlsLayout->addWidget(trainingChartFilterLabel);

        trainingChartFilterComboBox = new QComboBox(trainingChartControlsGroupBox);
        trainingChartFilterComboBox->addItem(QString());
        trainingChartFilterComboBox->addItem(QString());
        trainingChartFilterComboBox->addItem(QString());
        trainingChartFilterComboBox->setObjectName("trainingChartFilterComboBox");

        trainingChartControlsLayout->addWidget(trainingChartFilterComboBox);

        trainingToggleLegendCheckBox = new QCheckBox(trainingChartControlsGroupBox);
        trainingToggleLegendCheckBox->setObjectName("trainingToggleLegendCheckBox");
        trainingToggleLegendCheckBox->setChecked(true);

        trainingChartControlsLayout->addWidget(trainingToggleLegendCheckBox);

        trainingChartRefreshButton = new QPushButton(trainingChartControlsGroupBox);
        trainingChartRefreshButton->setObjectName("trainingChartRefreshButton");

        trainingChartControlsLayout->addWidget(trainingChartRefreshButton);


        trainingStatsLayout->addWidget(trainingChartControlsGroupBox);

        trainingChartView = new QChartView(trainingStatisticsTab);
        trainingChartView->setObjectName("trainingChartView");
        sizePolicy2.setHeightForWidth(trainingChartView->sizePolicy().hasHeightForWidth());
        trainingChartView->setSizePolicy(sizePolicy2);
        trainingChartView->setMinimumSize(QSize(0, 400));

        trainingStatsLayout->addWidget(trainingChartView);

        trainingHoverDescriptionLabel = new QLabel(trainingStatisticsTab);
        trainingHoverDescriptionLabel->setObjectName("trainingHoverDescriptionLabel");
        trainingHoverDescriptionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        trainingStatsLayout->addWidget(trainingHoverDescriptionLabel);

        trainingTabWidget->addTab(trainingStatisticsTab, QString());

        trainingPageLayout->addWidget(trainingTabWidget);

        mainStackedWidget->addWidget(trainingPage);
        meetingPage = new QWidget();
        meetingPage->setObjectName("meetingPage");
        meetingPageLayout = new QVBoxLayout(meetingPage);
        meetingPageLayout->setObjectName("meetingPageLayout");
        meetingTabWidget = new QTabWidget(meetingPage);
        meetingTabWidget->setObjectName("meetingTabWidget");
        meetingAddTab = new QWidget();
        meetingAddTab->setObjectName("meetingAddTab");
        meetingAddGridLayout = new QGridLayout(meetingAddTab);
        meetingAddGridLayout->setObjectName("meetingAddGridLayout");
        meetingTitleLabel = new QLabel(meetingAddTab);
        meetingTitleLabel->setObjectName("meetingTitleLabel");

        meetingAddGridLayout->addWidget(meetingTitleLabel, 0, 0, 1, 1);

        meetingTitleInput = new QLineEdit(meetingAddTab);
        meetingTitleInput->setObjectName("meetingTitleInput");

        meetingAddGridLayout->addWidget(meetingTitleInput, 0, 2, 1, 1);

        meetingOrganiserLabel = new QLabel(meetingAddTab);
        meetingOrganiserLabel->setObjectName("meetingOrganiserLabel");

        meetingAddGridLayout->addWidget(meetingOrganiserLabel, 1, 0, 1, 1);

        meetingOrganiserInput = new QLineEdit(meetingAddTab);
        meetingOrganiserInput->setObjectName("meetingOrganiserInput");

        meetingAddGridLayout->addWidget(meetingOrganiserInput, 1, 2, 1, 1);

        meetingParticipantLabel = new QLabel(meetingAddTab);
        meetingParticipantLabel->setObjectName("meetingParticipantLabel");

        meetingAddGridLayout->addWidget(meetingParticipantLabel, 2, 0, 1, 1);

        meetingParticipantInput = new QLineEdit(meetingAddTab);
        meetingParticipantInput->setObjectName("meetingParticipantInput");

        meetingAddGridLayout->addWidget(meetingParticipantInput, 2, 2, 1, 1);

        meetingAgendaLabel = new QLabel(meetingAddTab);
        meetingAgendaLabel->setObjectName("meetingAgendaLabel");

        meetingAddGridLayout->addWidget(meetingAgendaLabel, 3, 0, 1, 1);

        meetingAgendaComboBox = new QComboBox(meetingAddTab);
        meetingAgendaComboBox->addItem(QString());
        meetingAgendaComboBox->addItem(QString());
        meetingAgendaComboBox->addItem(QString());
        meetingAgendaComboBox->setObjectName("meetingAgendaComboBox");

        meetingAddGridLayout->addWidget(meetingAgendaComboBox, 3, 2, 1, 1);

        meetingDateLabel = new QLabel(meetingAddTab);
        meetingDateLabel->setObjectName("meetingDateLabel");

        meetingAddGridLayout->addWidget(meetingDateLabel, 4, 0, 1, 1);

        meetingDateTimeEdit = new QDateTimeEdit(meetingAddTab);
        meetingDateTimeEdit->setObjectName("meetingDateTimeEdit");

        meetingAddGridLayout->addWidget(meetingDateTimeEdit, 4, 2, 1, 1);

        meetingDurationLabel = new QLabel(meetingAddTab);
        meetingDurationLabel->setObjectName("meetingDurationLabel");

        meetingAddGridLayout->addWidget(meetingDurationLabel, 5, 0, 1, 1);

        meetingDurationInput = new QLineEdit(meetingAddTab);
        meetingDurationInput->setObjectName("meetingDurationInput");

        meetingAddGridLayout->addWidget(meetingDurationInput, 5, 2, 1, 1);

        meetingAddButton = new QPushButton(meetingAddTab);
        meetingAddButton->setObjectName("meetingAddButton");

        meetingAddGridLayout->addWidget(meetingAddButton, 6, 2, 1, 1);

        meetingAddHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        meetingAddGridLayout->addItem(meetingAddHorizontalSpacer, 0, 3, 7, 1);

        meetingTabWidget->addTab(meetingAddTab, QString());
        meetingManageTab = new QWidget();
        meetingManageTab->setObjectName("meetingManageTab");
        meetingManageLayout = new QVBoxLayout(meetingManageTab);
        meetingManageLayout->setObjectName("meetingManageLayout");
        meetingSearchGroupBox = new QGroupBox(meetingManageTab);
        meetingSearchGroupBox->setObjectName("meetingSearchGroupBox");
        meetingSearchLayout = new QHBoxLayout(meetingSearchGroupBox);
        meetingSearchLayout->setObjectName("meetingSearchLayout");
        meetingSearchCriteriaComboBox = new QComboBox(meetingSearchGroupBox);
        meetingSearchCriteriaComboBox->addItem(QString());
        meetingSearchCriteriaComboBox->addItem(QString());
        meetingSearchCriteriaComboBox->addItem(QString());
        meetingSearchCriteriaComboBox->addItem(QString());
        meetingSearchCriteriaComboBox->addItem(QString());
        meetingSearchCriteriaComboBox->setObjectName("meetingSearchCriteriaComboBox");

        meetingSearchLayout->addWidget(meetingSearchCriteriaComboBox);

        meetingSearchInput = new QLineEdit(meetingSearchGroupBox);
        meetingSearchInput->setObjectName("meetingSearchInput");

        meetingSearchLayout->addWidget(meetingSearchInput);

        meetingResetSearchButton = new QPushButton(meetingSearchGroupBox);
        meetingResetSearchButton->setObjectName("meetingResetSearchButton");

        meetingSearchLayout->addWidget(meetingResetSearchButton);


        meetingManageLayout->addWidget(meetingSearchGroupBox);

        meetingTableWidget = new QTableWidget(meetingManageTab);
        if (meetingTableWidget->columnCount() < 7)
            meetingTableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        meetingTableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        meetingTableWidget->setObjectName("meetingTableWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(meetingTableWidget->sizePolicy().hasHeightForWidth());
        meetingTableWidget->setSizePolicy(sizePolicy3);
        meetingTableWidget->setMinimumSize(QSize(0, 300));
        meetingTableWidget->setAlternatingRowColors(true);
        meetingTableWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        meetingTableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        meetingTableWidget->setShowGrid(true);
        meetingTableWidget->setGridStyle(Qt::PenStyle::SolidLine);
        meetingTableWidget->setSortingEnabled(true);
        meetingTableWidget->horizontalHeader()->setDefaultSectionSize(120);
        meetingTableWidget->horizontalHeader()->setStretchLastSection(true);
        meetingTableWidget->verticalHeader()->setVisible(false);

        meetingManageLayout->addWidget(meetingTableWidget);

        meetingManageButtonLayout = new QHBoxLayout();
        meetingManageButtonLayout->setObjectName("meetingManageButtonLayout");
        meetingManageHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        meetingManageButtonLayout->addItem(meetingManageHorizontalSpacer);

        meetingDeleteButton = new QPushButton(meetingManageTab);
        meetingDeleteButton->setObjectName("meetingDeleteButton");

        meetingManageButtonLayout->addWidget(meetingDeleteButton);

        meetingUpdateButton = new QPushButton(meetingManageTab);
        meetingUpdateButton->setObjectName("meetingUpdateButton");

        meetingManageButtonLayout->addWidget(meetingUpdateButton);

        meetingGenerateQRCodeButton = new QPushButton(meetingManageTab);
        meetingGenerateQRCodeButton->setObjectName("meetingGenerateQRCodeButton");

        meetingManageButtonLayout->addWidget(meetingGenerateQRCodeButton);

        meetingExportPdfButton = new QPushButton(meetingManageTab);
        meetingExportPdfButton->setObjectName("meetingExportPdfButton");

        meetingManageButtonLayout->addWidget(meetingExportPdfButton);


        meetingManageLayout->addLayout(meetingManageButtonLayout);

        meetingTabWidget->addTab(meetingManageTab, QString());
        meetingChatTab = new QWidget();
        meetingChatTab->setObjectName("meetingChatTab");
        meetingChatLayout = new QVBoxLayout(meetingChatTab);
        meetingChatLayout->setObjectName("meetingChatLayout");
        meetingChatTextEdit = new QTextEdit(meetingChatTab);
        meetingChatTextEdit->setObjectName("meetingChatTextEdit");
        meetingChatTextEdit->setReadOnly(true);

        meetingChatLayout->addWidget(meetingChatTextEdit);

        meetingChatInputLayout = new QHBoxLayout();
        meetingChatInputLayout->setObjectName("meetingChatInputLayout");
        meetingChatInputLineEdit = new QLineEdit(meetingChatTab);
        meetingChatInputLineEdit->setObjectName("meetingChatInputLineEdit");

        meetingChatInputLayout->addWidget(meetingChatInputLineEdit);

        meetingChatSendButton = new QPushButton(meetingChatTab);
        meetingChatSendButton->setObjectName("meetingChatSendButton");

        meetingChatInputLayout->addWidget(meetingChatSendButton);

        meetingChatClearButton = new QPushButton(meetingChatTab);
        meetingChatClearButton->setObjectName("meetingChatClearButton");

        meetingChatInputLayout->addWidget(meetingChatClearButton);


        meetingChatLayout->addLayout(meetingChatInputLayout);

        meetingTabWidget->addTab(meetingChatTab, QString());
        meetingStatisticsTab = new QWidget();
        meetingStatisticsTab->setObjectName("meetingStatisticsTab");
        meetingStatsLayout = new QVBoxLayout(meetingStatisticsTab);
        meetingStatsLayout->setObjectName("meetingStatsLayout");
        meetingChartControlsGroupBox = new QGroupBox(meetingStatisticsTab);
        meetingChartControlsGroupBox->setObjectName("meetingChartControlsGroupBox");
        meetingChartControlsLayout = new QHBoxLayout(meetingChartControlsGroupBox);
        meetingChartControlsLayout->setObjectName("meetingChartControlsLayout");
        meetingChartTypeLabel = new QLabel(meetingChartControlsGroupBox);
        meetingChartTypeLabel->setObjectName("meetingChartTypeLabel");

        meetingChartControlsLayout->addWidget(meetingChartTypeLabel);

        meetingChartTypeComboBox = new QComboBox(meetingChartControlsGroupBox);
        meetingChartTypeComboBox->addItem(QString());
        meetingChartTypeComboBox->addItem(QString());
        meetingChartTypeComboBox->setObjectName("meetingChartTypeComboBox");

        meetingChartControlsLayout->addWidget(meetingChartTypeComboBox);

        meetingChartFilterLabel = new QLabel(meetingChartControlsGroupBox);
        meetingChartFilterLabel->setObjectName("meetingChartFilterLabel");

        meetingChartControlsLayout->addWidget(meetingChartFilterLabel);

        meetingChartFilterComboBox = new QComboBox(meetingChartControlsGroupBox);
        meetingChartFilterComboBox->addItem(QString());
        meetingChartFilterComboBox->addItem(QString());
        meetingChartFilterComboBox->addItem(QString());
        meetingChartFilterComboBox->setObjectName("meetingChartFilterComboBox");

        meetingChartControlsLayout->addWidget(meetingChartFilterComboBox);

        meetingToggleLegendCheckBox = new QCheckBox(meetingChartControlsGroupBox);
        meetingToggleLegendCheckBox->setObjectName("meetingToggleLegendCheckBox");
        meetingToggleLegendCheckBox->setChecked(true);

        meetingChartControlsLayout->addWidget(meetingToggleLegendCheckBox);

        meetingChartRefreshButton = new QPushButton(meetingChartControlsGroupBox);
        meetingChartRefreshButton->setObjectName("meetingChartRefreshButton");

        meetingChartControlsLayout->addWidget(meetingChartRefreshButton);


        meetingStatsLayout->addWidget(meetingChartControlsGroupBox);

        meetingChartView = new QChartView(meetingStatisticsTab);
        meetingChartView->setObjectName("meetingChartView");
        sizePolicy2.setHeightForWidth(meetingChartView->sizePolicy().hasHeightForWidth());
        meetingChartView->setSizePolicy(sizePolicy2);
        meetingChartView->setMinimumSize(QSize(0, 400));

        meetingStatsLayout->addWidget(meetingChartView);

        meetingHoverDescriptionLabel = new QLabel(meetingStatisticsTab);
        meetingHoverDescriptionLabel->setObjectName("meetingHoverDescriptionLabel");
        meetingHoverDescriptionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        meetingStatsLayout->addWidget(meetingHoverDescriptionLabel);

        meetingTabWidget->addTab(meetingStatisticsTab, QString());

        meetingPageLayout->addWidget(meetingTabWidget);

        mainStackedWidget->addWidget(meetingPage);
        employeePage = new QWidget();
        employeePage->setObjectName("employeePage");
        employeePageLayout = new QVBoxLayout(employeePage);
        employeePageLayout->setObjectName("employeePageLayout");
        employeeTabWidget = new QTabWidget(employeePage);
        employeeTabWidget->setObjectName("employeeTabWidget");
        AddEmployee = new QWidget();
        AddEmployee->setObjectName("AddEmployee");
        gridLayout = new QGridLayout(AddEmployee);
        gridLayout->setObjectName("gridLayout");
        hiddenId = new QLineEdit(AddEmployee);
        hiddenId->setObjectName("hiddenId");
        hiddenId->setVisible(false);

        gridLayout->addWidget(hiddenId, 5, 4, 1, 1);

        lineEdit_salaire = new QLineEdit(AddEmployee);
        lineEdit_salaire->setObjectName("lineEdit_salaire");

        gridLayout->addWidget(lineEdit_salaire, 2, 3, 1, 1);

        dateEdit_hiring = new QDateEdit(AddEmployee);
        dateEdit_hiring->setObjectName("dateEdit_hiring");

        gridLayout->addWidget(dateEdit_hiring, 3, 3, 1, 1);

        label_8 = new QLabel(AddEmployee);
        label_8->setObjectName("label_8");

        gridLayout->addWidget(label_8, 2, 2, 1, 1);

        label_3 = new QLabel(AddEmployee);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit_email = new QLineEdit(AddEmployee);
        lineEdit_email->setObjectName("lineEdit_email");

        gridLayout->addWidget(lineEdit_email, 5, 1, 1, 1);

        dateEdit_birth = new QDateEdit(AddEmployee);
        dateEdit_birth->setObjectName("dateEdit_birth");

        gridLayout->addWidget(dateEdit_birth, 3, 1, 1, 1);

        label = new QLabel(AddEmployee);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_Prenom = new QLineEdit(AddEmployee);
        lineEdit_Prenom->setObjectName("lineEdit_Prenom");

        gridLayout->addWidget(lineEdit_Prenom, 2, 1, 1, 1);

        label_9 = new QLabel(AddEmployee);
        label_9->setObjectName("label_9");

        gridLayout->addWidget(label_9, 3, 2, 1, 1);

        label_5 = new QLabel(AddEmployee);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_4 = new QLabel(AddEmployee);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        addButton = new QPushButton(AddEmployee);
        addButton->setObjectName("addButton");

        gridLayout->addWidget(addButton, 9, 3, 1, 1);

        label_11 = new QLabel(AddEmployee);
        label_11->setObjectName("label_11");

        gridLayout->addWidget(label_11, 4, 2, 1, 1);

        comboBox = new QComboBox(AddEmployee);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 4, 3, 1, 1);

        label_2 = new QLabel(AddEmployee);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        radioButton_H = new QRadioButton(AddEmployee);
        radioButton_H->setObjectName("radioButton_H");

        gridLayout->addWidget(radioButton_H, 0, 3, 1, 1);

        lineEdit_Nom = new QLineEdit(AddEmployee);
        lineEdit_Nom->setObjectName("lineEdit_Nom");

        gridLayout->addWidget(lineEdit_Nom, 1, 1, 1, 1);

        label_6 = new QLabel(AddEmployee);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        lineEdit_CIN = new QLineEdit(AddEmployee);
        lineEdit_CIN->setObjectName("lineEdit_CIN");

        gridLayout->addWidget(lineEdit_CIN, 0, 1, 1, 1);

        label_7 = new QLabel(AddEmployee);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 0, 2, 1, 1);

        lineEdit_phone = new QLineEdit(AddEmployee);
        lineEdit_phone->setObjectName("lineEdit_phone");

        gridLayout->addWidget(lineEdit_phone, 4, 1, 1, 1);

        radioButton_F = new QRadioButton(AddEmployee);
        radioButton_F->setObjectName("radioButton_F");

        gridLayout->addWidget(radioButton_F, 1, 3, 1, 1);

        label_image = new QLabel(AddEmployee);
        label_image->setObjectName("label_image");

        gridLayout->addWidget(label_image, 6, 0, 1, 1);

        imageLayout = new QHBoxLayout();
        imageLayout->setObjectName("imageLayout");
        imagePathLineEdit = new QLineEdit(AddEmployee);
        imagePathLineEdit->setObjectName("imagePathLineEdit");

        imageLayout->addWidget(imagePathLineEdit);

        selectImageButton = new QPushButton(AddEmployee);
        selectImageButton->setObjectName("selectImageButton");

        imageLayout->addWidget(selectImageButton);


        gridLayout->addLayout(imageLayout, 6, 1, 1, 3);

        label_role = new QLabel(AddEmployee);
        label_role->setObjectName("label_role");

        gridLayout->addWidget(label_role, 7, 0, 1, 1);

        roleComboBox = new QComboBox(AddEmployee);
        roleComboBox->addItem(QString());
        roleComboBox->addItem(QString());
        roleComboBox->addItem(QString());
        roleComboBox->setObjectName("roleComboBox");

        gridLayout->addWidget(roleComboBox, 7, 1, 1, 3);

        employeeTabWidget->addTab(AddEmployee, QString());
        listEmployee = new QWidget();
        listEmployee->setObjectName("listEmployee");
        verticalLayout_6 = new QVBoxLayout(listEmployee);
        verticalLayout_6->setObjectName("verticalLayout_6");
        searchGroupBox = new QGroupBox(listEmployee);
        searchGroupBox->setObjectName("searchGroupBox");
        searchLayout = new QHBoxLayout(searchGroupBox);
        searchLayout->setObjectName("searchLayout");
        searchCriteriaComboBox = new QComboBox(searchGroupBox);
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

        tableView = new QTableView(listEmployee);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout_6->addWidget(tableView);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        deleteBtn = new QPushButton(listEmployee);
        deleteBtn->setObjectName("deleteBtn");

        horizontalLayout_4->addWidget(deleteBtn);

        modifyBtn = new QPushButton(listEmployee);
        modifyBtn->setObjectName("modifyBtn");

        horizontalLayout_4->addWidget(modifyBtn);

        saveUpdateBtn = new QPushButton(listEmployee);
        saveUpdateBtn->setObjectName("saveUpdateBtn");

        horizontalLayout_4->addWidget(saveUpdateBtn);

        downloadBtn = new QPushButton(listEmployee);
        downloadBtn->setObjectName("downloadBtn");

        horizontalLayout_4->addWidget(downloadBtn);

        generateQRCodeBtn = new QPushButton(listEmployee);
        generateQRCodeBtn->setObjectName("generateQRCodeBtn");

        horizontalLayout_4->addWidget(generateQRCodeBtn);


        verticalLayout_6->addLayout(horizontalLayout_4);

        employeeTabWidget->addTab(listEmployee, QString());
        employeeStatsTab = new QWidget();
        employeeStatsTab->setObjectName("employeeStatsTab");
        verticalLayoutStats = new QVBoxLayout(employeeStatsTab);
        verticalLayoutStats->setObjectName("verticalLayoutStats");
        chartsContainer = new QWidget(employeeStatsTab);
        chartsContainer->setObjectName("chartsContainer");
        chartsLayout = new QVBoxLayout(chartsContainer);
        chartsLayout->setObjectName("chartsLayout");

        verticalLayoutStats->addWidget(chartsContainer);

        employeeHoverLabel = new QLabel(employeeStatsTab);
        employeeHoverLabel->setObjectName("employeeHoverLabel");
        employeeHoverLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayoutStats->addWidget(employeeHoverLabel);

        employeeTotalCountLabel = new QLabel(employeeStatsTab);
        employeeTotalCountLabel->setObjectName("employeeTotalCountLabel");

        verticalLayoutStats->addWidget(employeeTotalCountLabel);

        employeeAverageSalaryLabel = new QLabel(employeeStatsTab);
        employeeAverageSalaryLabel->setObjectName("employeeAverageSalaryLabel");

        verticalLayoutStats->addWidget(employeeAverageSalaryLabel);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer2);

        refreshStatsButton = new QPushButton(employeeStatsTab);
        refreshStatsButton->setObjectName("refreshStatsButton");

        buttonLayout->addWidget(refreshStatsButton);


        verticalLayoutStats->addLayout(buttonLayout);

        employeeTabWidget->addTab(employeeStatsTab, QString());

        employeePageLayout->addWidget(employeeTabWidget);

        mainStackedWidget->addWidget(employeePage);
        resourcePage = new QWidget();
        resourcePage->setObjectName("resourcePage");
        resourcePageLayout = new QVBoxLayout(resourcePage);
        resourcePageLayout->setObjectName("resourcePageLayout");
        resourceTabWidget = new QTabWidget(resourcePage);
        resourceTabWidget->setObjectName("resourceTabWidget");
        resourceAddTab = new QWidget();
        resourceAddTab->setObjectName("resourceAddTab");
        resourceAddTab->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        gridLayout_2 = new QGridLayout(resourceAddTab);
        gridLayout_2->setObjectName("gridLayout_2");
        label_brand = new QLabel(resourceAddTab);
        label_brand->setObjectName("label_brand");

        gridLayout_2->addWidget(label_brand, 2, 0, 1, 1);

        quantityLineEdit = new QLineEdit(resourceAddTab);
        quantityLineEdit->setObjectName("quantityLineEdit");

        gridLayout_2->addWidget(quantityLineEdit, 3, 2, 1, 1);

        confirmFormButton = new QPushButton(resourceAddTab);
        confirmFormButton->setObjectName("confirmFormButton");

        gridLayout_2->addWidget(confirmFormButton, 6, 2, 1, 1);

        lblImagePreview = new QLabel(resourceAddTab);
        lblImagePreview->setObjectName("lblImagePreview");

        gridLayout_2->addWidget(lblImagePreview, 5, 1, 1, 2);

        resourceHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(resourceHorizontalSpacer, 0, 3, 7, 1);

        label_quantity = new QLabel(resourceAddTab);
        label_quantity->setObjectName("label_quantity");

        gridLayout_2->addWidget(label_quantity, 3, 0, 1, 1);

        cancelFormButton = new QPushButton(resourceAddTab);
        cancelFormButton->setObjectName("cancelFormButton");

        gridLayout_2->addWidget(cancelFormButton, 7, 2, 1, 1);

        label_name = new QLabel(resourceAddTab);
        label_name->setObjectName("label_name");

        gridLayout_2->addWidget(label_name, 0, 0, 1, 1);

        typeComboBox = new QComboBox(resourceAddTab);
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->addItem(QString());
        typeComboBox->setObjectName("typeComboBox");

        gridLayout_2->addWidget(typeComboBox, 1, 2, 1, 1);

        nameLineEdit = new QLineEdit(resourceAddTab);
        nameLineEdit->setObjectName("nameLineEdit");

        gridLayout_2->addWidget(nameLineEdit, 0, 2, 1, 1);

        label_type = new QLabel(resourceAddTab);
        label_type->setObjectName("label_type");

        gridLayout_2->addWidget(label_type, 1, 0, 1, 1);

        btnSelectImage = new QPushButton(resourceAddTab);
        btnSelectImage->setObjectName("btnSelectImage");

        gridLayout_2->addWidget(btnSelectImage, 5, 0, 1, 1);

        brandLineEdit = new QLineEdit(resourceAddTab);
        brandLineEdit->setObjectName("brandLineEdit");

        gridLayout_2->addWidget(brandLineEdit, 2, 2, 1, 1);

        label_date = new QLabel(resourceAddTab);
        label_date->setObjectName("label_date");

        gridLayout_2->addWidget(label_date, 4, 0, 1, 1);

        purchaseDateEdit = new QDateEdit(resourceAddTab);
        purchaseDateEdit->setObjectName("purchaseDateEdit");

        gridLayout_2->addWidget(purchaseDateEdit, 4, 2, 1, 1);

        resourceTabWidget->addTab(resourceAddTab, QString());
        resourceManageTab = new QWidget();
        resourceManageTab->setObjectName("resourceManageTab");
        resourceManageLayout = new QVBoxLayout(resourceManageTab);
        resourceManageLayout->setObjectName("resourceManageLayout");
        resourceSearchGroupBox = new QGroupBox(resourceManageTab);
        resourceSearchGroupBox->setObjectName("resourceSearchGroupBox");
        resourceSearchLayout = new QHBoxLayout(resourceSearchGroupBox);
        resourceSearchLayout->setObjectName("resourceSearchLayout");
        searchLineEdit = new QLineEdit(resourceSearchGroupBox);
        searchLineEdit->setObjectName("searchLineEdit");

        resourceSearchLayout->addWidget(searchLineEdit);

        resetResourceSearchButton = new QPushButton(resourceSearchGroupBox);
        resetResourceSearchButton->setObjectName("resetResourceSearchButton");

        resourceSearchLayout->addWidget(resetResourceSearchButton);


        resourceManageLayout->addWidget(resourceSearchGroupBox);

        tableWidget = new QTableWidget(resourceManageTab);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem13);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setStyleSheet(QString::fromUtf8(""));

        resourceManageLayout->addWidget(tableWidget);

        resourceButtonLayout = new QHBoxLayout();
        resourceButtonLayout->setObjectName("resourceButtonLayout");
        exportPdfButton = new QPushButton(resourceManageTab);
        exportPdfButton->setObjectName("exportPdfButton");

        resourceButtonLayout->addWidget(exportPdfButton);

        downloadHistoryButton = new QPushButton(resourceManageTab);
        downloadHistoryButton->setObjectName("downloadHistoryButton");

        resourceButtonLayout->addWidget(downloadHistoryButton);

        clearHistoryButton = new QPushButton(resourceManageTab);
        clearHistoryButton->setObjectName("clearHistoryButton");

        resourceButtonLayout->addWidget(clearHistoryButton);

        btnLookForResource = new QPushButton(resourceManageTab);
        btnLookForResource->setObjectName("btnLookForResource");

        resourceButtonLayout->addWidget(btnLookForResource);

        resourceHorizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        resourceButtonLayout->addItem(resourceHorizontalSpacer2);

        deleteButton = new QPushButton(resourceManageTab);
        deleteButton->setObjectName("deleteButton");

        resourceButtonLayout->addWidget(deleteButton);

        updateButton = new QPushButton(resourceManageTab);
        updateButton->setObjectName("updateButton");

        resourceButtonLayout->addWidget(updateButton);


        resourceManageLayout->addLayout(resourceButtonLayout);

        resourceTabWidget->addTab(resourceManageTab, QString());
        resourceStatisticsTab = new QWidget();
        resourceStatisticsTab->setObjectName("resourceStatisticsTab");
        resourceStatsLayout = new QVBoxLayout(resourceStatisticsTab);
        resourceStatsLayout->setObjectName("resourceStatsLayout");
        statsTextGroup = new QGroupBox(resourceStatisticsTab);
        statsTextGroup->setObjectName("statsTextGroup");
        resourceVerticalLayoutStats = new QVBoxLayout(statsTextGroup);
        resourceVerticalLayoutStats->setObjectName("resourceVerticalLayoutStats");
        chartView = new QChartView(statsTextGroup);
        chartView->setObjectName("chartView");
        sizePolicy2.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
        chartView->setSizePolicy(sizePolicy2);

        resourceVerticalLayoutStats->addWidget(chartView);

        stackedBarChartView = new QChartView(statsTextGroup);
        stackedBarChartView->setObjectName("stackedBarChartView");
        sizePolicy2.setHeightForWidth(stackedBarChartView->sizePolicy().hasHeightForWidth());
        stackedBarChartView->setSizePolicy(sizePolicy2);

        resourceVerticalLayoutStats->addWidget(stackedBarChartView);


        resourceStatsLayout->addWidget(statsTextGroup);

        resourceStatsButtonLayout = new QHBoxLayout();
        resourceStatsButtonLayout->setObjectName("resourceStatsButtonLayout");
        horizontalSpacer21 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        resourceStatsButtonLayout->addItem(horizontalSpacer21);

        resourceRefreshStatsButton = new QPushButton(resourceStatisticsTab);
        resourceRefreshStatsButton->setObjectName("resourceRefreshStatsButton");

        resourceStatsButtonLayout->addWidget(resourceRefreshStatsButton);


        resourceStatsLayout->addLayout(resourceStatsButtonLayout);

        resourceTabWidget->addTab(resourceStatisticsTab, QString());

        resourcePageLayout->addWidget(resourceTabWidget);

        mainStackedWidget->addWidget(resourcePage);

        verticalLayout_5->addWidget(mainStackedWidget);


        horizontalLayout->addWidget(frame_4);


        verticalLayout->addWidget(frame_2);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        mainStackedWidget->setCurrentIndex(4);
        clientTabWidget->setCurrentIndex(1);
        trainingTabWidget->setCurrentIndex(2);
        meetingTabWidget->setCurrentIndex(1);
        employeeTabWidget->setCurrentIndex(1);
        resourceTabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Client, Training, and Meeting Management System", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        headerLabel->setText(QCoreApplication::translate("MainWindow", "Client, Training, and Meeting Management System", nullptr));
        trainingNotificationLabel->setText(QCoreApplication::translate("MainWindow", "notifications", nullptr));
        clientSectionButton->setText(QCoreApplication::translate("MainWindow", "Clients", nullptr));
        trainingSectionButton->setText(QCoreApplication::translate("MainWindow", "Training", nullptr));
        meetingSectionButton->setText(QCoreApplication::translate("MainWindow", "Meetings", nullptr));
        employeeSectionButton->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        resourceSectionButton->setText(QCoreApplication::translate("MainWindow", "Resource", nullptr));
        settingsButton->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
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
        clientChartControlsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Chart Options", nullptr));
        clientChartTypeLabel->setText(QCoreApplication::translate("MainWindow", "Chart Type:", nullptr));
        clientChartTypeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Pie Chart", nullptr));
        clientChartTypeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Bar Chart", nullptr));

        clientChartFilterLabel->setText(QCoreApplication::translate("MainWindow", "Filter By:", nullptr));
        clientChartFilterComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Sector", nullptr));
        clientChartFilterComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Consultant", nullptr));
        clientChartFilterComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Date", nullptr));

        clientToggleLegendCheckBox->setText(QCoreApplication::translate("MainWindow", "Show Legend", nullptr));
        clientChartRefreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        clientHoverDescriptionLabel->setText(QCoreApplication::translate("MainWindow", "Hover over a chart element to see details", nullptr));
        clientTabWidget->setTabText(clientTabWidget->indexOf(clientStatisticsTab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
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
        trainingChartControlsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Chart Options", nullptr));
        trainingChartTypeLabel->setText(QCoreApplication::translate("MainWindow", "Chart Type:", nullptr));
        trainingChartTypeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Pie Chart", nullptr));
        trainingChartTypeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Bar Chart", nullptr));

        trainingChartFilterLabel->setText(QCoreApplication::translate("MainWindow", "Filter By:", nullptr));
        trainingChartFilterComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Trainer", nullptr));
        trainingChartFilterComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Price", nullptr));
        trainingChartFilterComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Date", nullptr));

        trainingToggleLegendCheckBox->setText(QCoreApplication::translate("MainWindow", "Show Legend", nullptr));
        trainingChartRefreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        trainingHoverDescriptionLabel->setText(QCoreApplication::translate("MainWindow", "Hover over a chart element to see details", nullptr));
        trainingTabWidget->setTabText(trainingTabWidget->indexOf(trainingStatisticsTab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        meetingTitleLabel->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        meetingOrganiserLabel->setText(QCoreApplication::translate("MainWindow", "Organiser", nullptr));
        meetingParticipantLabel->setText(QCoreApplication::translate("MainWindow", "Participant", nullptr));
        meetingAgendaLabel->setText(QCoreApplication::translate("MainWindow", "Agenda", nullptr));
        meetingAgendaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "car advice", nullptr));
        meetingAgendaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "life advice", nullptr));
        meetingAgendaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "future advice", nullptr));

        meetingDateLabel->setText(QCoreApplication::translate("MainWindow", "Date and Time:", nullptr));
        meetingDurationLabel->setText(QCoreApplication::translate("MainWindow", "Duration (min)", nullptr));
        meetingAddButton->setText(QCoreApplication::translate("MainWindow", "Add Meeting", nullptr));
        meetingTabWidget->setTabText(meetingTabWidget->indexOf(meetingAddTab), QCoreApplication::translate("MainWindow", "Add Meeting", nullptr));
        meetingSearchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        meetingSearchCriteriaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Title", nullptr));
        meetingSearchCriteriaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Organiser", nullptr));
        meetingSearchCriteriaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Participant", nullptr));
        meetingSearchCriteriaComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Agenda", nullptr));
        meetingSearchCriteriaComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Date Time", nullptr));

        meetingSearchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        meetingResetSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        QTableWidgetItem *___qtablewidgetitem = meetingTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = meetingTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = meetingTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Organiser", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = meetingTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Participant", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = meetingTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Agenda", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = meetingTableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Duration", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = meetingTableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Date and Time", nullptr));
        meetingDeleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        meetingUpdateButton->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        meetingGenerateQRCodeButton->setText(QCoreApplication::translate("MainWindow", "QR Code", nullptr));
        meetingExportPdfButton->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        meetingTabWidget->setTabText(meetingTabWidget->indexOf(meetingManageTab), QCoreApplication::translate("MainWindow", "Manage Meetings", nullptr));
        meetingChatInputLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type your message here...", nullptr));
        meetingChatSendButton->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        meetingChatClearButton->setText(QCoreApplication::translate("MainWindow", "Clear Chat", nullptr));
        meetingTabWidget->setTabText(meetingTabWidget->indexOf(meetingChatTab), QCoreApplication::translate("MainWindow", "Chat Assistant", nullptr));
        meetingChartControlsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Chart Options", nullptr));
        meetingChartTypeLabel->setText(QCoreApplication::translate("MainWindow", "Chart Type:", nullptr));
        meetingChartTypeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Pie Chart", nullptr));
        meetingChartTypeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Bar Chart", nullptr));

        meetingChartFilterLabel->setText(QCoreApplication::translate("MainWindow", "Filter By:", nullptr));
        meetingChartFilterComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Organiser", nullptr));
        meetingChartFilterComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Agenda", nullptr));
        meetingChartFilterComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Date", nullptr));

        meetingToggleLegendCheckBox->setText(QCoreApplication::translate("MainWindow", "Show Legend", nullptr));
        meetingChartRefreshButton->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        meetingHoverDescriptionLabel->setText(QCoreApplication::translate("MainWindow", "Hover over a chart element to see details", nullptr));
        meetingTabWidget->setTabText(meetingTabWidget->indexOf(meetingStatisticsTab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        dateEdit_hiring->setDisplayFormat(QCoreApplication::translate("MainWindow", "d/M/yyyy", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Salary", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "First Name", nullptr));
        dateEdit_birth->setDisplayFormat(QCoreApplication::translate("MainWindow", "d/M/yyyy", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Date of Hiring", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Phone Number", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Date of Birth", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Add Employee", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Specialty", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Informatique", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Finance", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Communication", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Last Name", nullptr));
        radioButton_H->setText(QCoreApplication::translate("MainWindow", "Homme", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Gender", nullptr));
        radioButton_F->setText(QCoreApplication::translate("MainWindow", "Femme", nullptr));
        label_image->setText(QCoreApplication::translate("MainWindow", "Image Path:", nullptr));
        selectImageButton->setText(QCoreApplication::translate("MainWindow", "Select Image", nullptr));
        label_role->setText(QCoreApplication::translate("MainWindow", "Role:", nullptr));
        roleComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Admin", nullptr));
        roleComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Employee", nullptr));
        roleComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Manager", nullptr));

        employeeTabWidget->setTabText(employeeTabWidget->indexOf(AddEmployee), QCoreApplication::translate("MainWindow", "Add Employee", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        searchCriteriaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Full name", nullptr));
        searchCriteriaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "CIN", nullptr));
        searchCriteriaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Specialty", nullptr));

        searchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        resetSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        modifyBtn->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        saveUpdateBtn->setText(QCoreApplication::translate("MainWindow", "Save Update", nullptr));
        downloadBtn->setText(QCoreApplication::translate("MainWindow", "Download", nullptr));
        generateQRCodeBtn->setText(QCoreApplication::translate("MainWindow", "Generate QR Code", nullptr));
        employeeTabWidget->setTabText(employeeTabWidget->indexOf(listEmployee), QCoreApplication::translate("MainWindow", "Manage Employees", nullptr));
        employeeHoverLabel->setText(QCoreApplication::translate("MainWindow", "Hover over a chart element to see details", nullptr));
        employeeTotalCountLabel->setText(QCoreApplication::translate("MainWindow", "Total Employees: 0", nullptr));
        employeeAverageSalaryLabel->setText(QCoreApplication::translate("MainWindow", "Average Salary: $0.00", nullptr));
        refreshStatsButton->setText(QCoreApplication::translate("MainWindow", "Refresh Statistics", nullptr));
        employeeTabWidget->setTabText(employeeTabWidget->indexOf(employeeStatsTab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        label_brand->setText(QCoreApplication::translate("MainWindow", "Brand", nullptr));
        confirmFormButton->setText(QCoreApplication::translate("MainWindow", "Add Resource", nullptr));
        lblImagePreview->setText(QString());
        label_quantity->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        cancelFormButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        label_name->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        typeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Electronics ", nullptr));
        typeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Cleaning Supplies", nullptr));
        typeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Kitchen Supplies", nullptr));
        typeComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Stationery ", nullptr));

        label_type->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        btnSelectImage->setText(QCoreApplication::translate("MainWindow", "Select Image", nullptr));
        label_date->setText(QCoreApplication::translate("MainWindow", "Purchase Date", nullptr));
        resourceTabWidget->setTabText(resourceTabWidget->indexOf(resourceAddTab), QCoreApplication::translate("MainWindow", "Add Resources", nullptr));
        resourceSearchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        resetResourceSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Brand", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Purchase Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Image", nullptr));
        exportPdfButton->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        downloadHistoryButton->setText(QCoreApplication::translate("MainWindow", "History", nullptr));
        clearHistoryButton->setText(QCoreApplication::translate("MainWindow", "Clear History", nullptr));
        btnLookForResource->setText(QCoreApplication::translate("MainWindow", "Look For Resource", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        resourceTabWidget->setTabText(resourceTabWidget->indexOf(resourceManageTab), QCoreApplication::translate("MainWindow", "Manage Resources", nullptr));
        statsTextGroup->setTitle(QCoreApplication::translate("MainWindow", "Statistical Summary", nullptr));
        resourceRefreshStatsButton->setText(QCoreApplication::translate("MainWindow", "Refresh Statistics", nullptr));
        resourceTabWidget->setTabText(resourceTabWidget->indexOf(resourceStatisticsTab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
