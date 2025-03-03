/********************************************************************************
** Form generated from reading UI file 'statisticsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTICSDIALOG_H
#define UI_STATISTICSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatisticsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *summaryTab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *summaryGroup;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *totalClientsLabel;
    QSpacerItem *verticalSpacer;
    QWidget *sectorTab;
    QVBoxLayout *verticalLayout_3;
    QWidget *consultantTab;
    QVBoxLayout *verticalLayout_4;
    QWidget *dateTab;
    QVBoxLayout *verticalLayout_5;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *StatisticsDialog)
    {
        if (StatisticsDialog->objectName().isEmpty())
            StatisticsDialog->setObjectName("StatisticsDialog");
        StatisticsDialog->resize(800, 600);
        verticalLayout = new QVBoxLayout(StatisticsDialog);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(StatisticsDialog);
        tabWidget->setObjectName("tabWidget");
        summaryTab = new QWidget();
        summaryTab->setObjectName("summaryTab");
        verticalLayout_2 = new QVBoxLayout(summaryTab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        summaryGroup = new QGroupBox(summaryTab);
        summaryGroup->setObjectName("summaryGroup");
        gridLayout = new QGridLayout(summaryGroup);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(summaryGroup);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        totalClientsLabel = new QLabel(summaryGroup);
        totalClientsLabel->setObjectName("totalClientsLabel");
        totalClientsLabel->setFont(font);

        gridLayout->addWidget(totalClientsLabel, 0, 1, 1, 1);


        verticalLayout_2->addWidget(summaryGroup);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        tabWidget->addTab(summaryTab, QString());
        sectorTab = new QWidget();
        sectorTab->setObjectName("sectorTab");
        verticalLayout_3 = new QVBoxLayout(sectorTab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        tabWidget->addTab(sectorTab, QString());
        consultantTab = new QWidget();
        consultantTab->setObjectName("consultantTab");
        verticalLayout_4 = new QVBoxLayout(consultantTab);
        verticalLayout_4->setObjectName("verticalLayout_4");
        tabWidget->addTab(consultantTab, QString());
        dateTab = new QWidget();
        dateTab->setObjectName("dateTab");
        verticalLayout_5 = new QVBoxLayout(dateTab);
        verticalLayout_5->setObjectName("verticalLayout_5");
        tabWidget->addTab(dateTab, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(StatisticsDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Close);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(StatisticsDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, StatisticsDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, StatisticsDialog, qOverload<>(&QDialog::reject));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StatisticsDialog);
    } // setupUi

    void retranslateUi(QDialog *StatisticsDialog)
    {
        StatisticsDialog->setWindowTitle(QCoreApplication::translate("StatisticsDialog", "Client Statistics", nullptr));
        summaryGroup->setTitle(QCoreApplication::translate("StatisticsDialog", "Client Statistics Summary", nullptr));
        label->setText(QCoreApplication::translate("StatisticsDialog", "Total Clients:", nullptr));
        totalClientsLabel->setText(QCoreApplication::translate("StatisticsDialog", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(summaryTab), QCoreApplication::translate("StatisticsDialog", "Summary", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(sectorTab), QCoreApplication::translate("StatisticsDialog", "Sectors", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(consultantTab), QCoreApplication::translate("StatisticsDialog", "Consultants", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(dateTab), QCoreApplication::translate("StatisticsDialog", "Dates", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatisticsDialog: public Ui_StatisticsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICSDIALOG_H
