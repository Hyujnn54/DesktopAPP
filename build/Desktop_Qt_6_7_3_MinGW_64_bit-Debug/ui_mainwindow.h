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
    QPushButton *themeButton;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *AddEmployee;
    QGridLayout *gridLayout;
    QLineEdit *hiddenId;
    QLineEdit *lineEdit_salaire;
    QDateEdit *dateEdit_hiring;
    QLabel *label_8;
    QLabel *label_3;
    QLineEdit *lineEdit_email;
    QDateEdit *dateEdit_birth;
    QLabel *label1;
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
    QWidget *tab;
    QVBoxLayout *verticalLayoutStats;
    QGroupBox *statsTextGroup;
    QVBoxLayout *verticalLayoutText;
    QTextEdit *statsDisplay;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer2;
    QPushButton *refreshStatsButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1387, 773);
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
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        frame_3 = new QFrame(header);
        frame_3->setObjectName("frame_3");
        frame_3->setMinimumSize(QSize(50, 0));
        frame_3->setMaximumSize(QSize(151, 16777215));
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        menuButton = new QPushButton(frame_3);
        menuButton->setObjectName("menuButton");

        horizontalLayout_3->addWidget(menuButton);


        horizontalLayout_2->addWidget(frame_3);

        frame_5 = new QFrame(header);
        frame_5->setObjectName("frame_5");
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
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setObjectName("horizontalLayout");
        sideMenu = new QFrame(frame_2);
        sideMenu->setObjectName("sideMenu");
        sideMenu->setMinimumSize(QSize(150, 0));
        sideMenu->setMaximumSize(QSize(150, 16777215));
        verticalLayout_2 = new QVBoxLayout(sideMenu);
        verticalLayout_2->setObjectName("verticalLayout_2");
        frame_6 = new QFrame(sideMenu);
        frame_6->setObjectName("frame_6");
        frame_6->setMinimumSize(QSize(125, 600));
        verticalLayout_3 = new QVBoxLayout(frame_6);
        verticalLayout_3->setObjectName("verticalLayout_3");
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

        themeButton = new QPushButton(frame_6);
        themeButton->setObjectName("themeButton");

        verticalLayout_3->addWidget(themeButton);


        verticalLayout_2->addWidget(frame_6);


        horizontalLayout->addWidget(sideMenu);

        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName("frame_4");
        verticalLayout_5 = new QVBoxLayout(frame_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        tabWidget = new QTabWidget(frame_4);
        tabWidget->setObjectName("tabWidget");
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

        label1 = new QLabel(AddEmployee);
        label1->setObjectName("label1");

        gridLayout->addWidget(label1, 0, 0, 1, 1);

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

        tabWidget->addTab(AddEmployee, QString());
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


        verticalLayout_6->addLayout(horizontalLayout_4);

        tabWidget->addTab(listEmployee, QString());
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

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Employee Management System", nullptr));
        menuButton->setText(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Employee Management System", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Employees", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Reports", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        themeButton->setText(QCoreApplication::translate("MainWindow", "Toggle Theme", nullptr));
        dateEdit_hiring->setDisplayFormat(QCoreApplication::translate("MainWindow", "d/M/yyyy", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Salary", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "First Name", nullptr));
        dateEdit_birth->setDisplayFormat(QCoreApplication::translate("MainWindow", "d/M/yyyy", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
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

        tabWidget->setTabText(tabWidget->indexOf(AddEmployee), QCoreApplication::translate("MainWindow", "Add Employee", nullptr));
        searchGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Search", nullptr));
        searchCriteriaComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Full name", nullptr));
        searchCriteriaComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "CIN", nullptr));
        searchCriteriaComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Specialty", nullptr));

        searchInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type to search...", nullptr));
        resetSearchButton->setText(QCoreApplication::translate("MainWindow", "Reset Search", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        modifyBtn->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
        saveUpdateBtn->setText(QCoreApplication::translate("MainWindow", "Save Update", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(listEmployee), QCoreApplication::translate("MainWindow", "Manage Employees", nullptr));
        statsTextGroup->setTitle(QCoreApplication::translate("MainWindow", "Statistical Summary", nullptr));
        refreshStatsButton->setText(QCoreApplication::translate("MainWindow", "Refresh Statistics", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Statistics", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
