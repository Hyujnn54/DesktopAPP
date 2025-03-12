/********************************************************************************
** Form generated from reading UI file 'ChartWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTWINDOW_H
#define UI_CHARTWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *chartGroupBox;
    QVBoxLayout *chartLayout;
    QHBoxLayout *filterLayout;
    QLabel *filterLabel;
    QComboBox *statsFilterComboBox;
    QComboBox *filterValueComboBox;
    QPushButton *refreshChartButton;
    QChartView *statsChartView;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ChartWindow)
    {
        if (ChartWindow->objectName().isEmpty())
            ChartWindow->setObjectName("ChartWindow");
        ChartWindow->resize(800, 600);
        centralwidget = new QWidget(ChartWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        chartGroupBox = new QGroupBox(centralwidget);
        chartGroupBox->setObjectName("chartGroupBox");
        chartLayout = new QVBoxLayout(chartGroupBox);
        chartLayout->setObjectName("chartLayout");
        filterLayout = new QHBoxLayout();
        filterLayout->setObjectName("filterLayout");
        filterLabel = new QLabel(chartGroupBox);
        filterLabel->setObjectName("filterLabel");

        filterLayout->addWidget(filterLabel);

        statsFilterComboBox = new QComboBox(chartGroupBox);
        statsFilterComboBox->addItem(QString());
        statsFilterComboBox->addItem(QString());
        statsFilterComboBox->addItem(QString());
        statsFilterComboBox->setObjectName("statsFilterComboBox");

        filterLayout->addWidget(statsFilterComboBox);

        filterValueComboBox = new QComboBox(chartGroupBox);
        filterValueComboBox->setObjectName("filterValueComboBox");

        filterLayout->addWidget(filterValueComboBox);

        refreshChartButton = new QPushButton(chartGroupBox);
        refreshChartButton->setObjectName("refreshChartButton");

        filterLayout->addWidget(refreshChartButton);


        chartLayout->addLayout(filterLayout);

        statsChartView = new QChartView(chartGroupBox);
        statsChartView->setObjectName("statsChartView");
        statsChartView->setMinimumSize(QSize(0, 400));

        chartLayout->addWidget(statsChartView);


        verticalLayout->addWidget(chartGroupBox);

        ChartWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(ChartWindow);
        statusBar->setObjectName("statusBar");
        ChartWindow->setStatusBar(statusBar);

        retranslateUi(ChartWindow);

        QMetaObject::connectSlotsByName(ChartWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChartWindow)
    {
        ChartWindow->setWindowTitle(QCoreApplication::translate("ChartWindow", "Consultation Statistics Chart", nullptr));
        chartGroupBox->setTitle(QCoreApplication::translate("ChartWindow", "Consultation Statistics", nullptr));
        filterLabel->setText(QCoreApplication::translate("ChartWindow", "Filter by:", nullptr));
        statsFilterComboBox->setItemText(0, QCoreApplication::translate("ChartWindow", "By Sector", nullptr));
        statsFilterComboBox->setItemText(1, QCoreApplication::translate("ChartWindow", "By Date", nullptr));
        statsFilterComboBox->setItemText(2, QCoreApplication::translate("ChartWindow", "By Consultant", nullptr));

        filterValueComboBox->setPlaceholderText(QCoreApplication::translate("ChartWindow", "Select filter value...", nullptr));
        refreshChartButton->setText(QCoreApplication::translate("ChartWindow", "Refresh Chart", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartWindow: public Ui_ChartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTWINDOW_H
