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
#include <QtWidgets/QSpacerItem>
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
    QHBoxLayout *controlLayout;
    QLabel *statsTypeLabel;
    QComboBox *statsTypeComboBox;
    QLabel *filterLabel;
    QComboBox *filterComboBox;
    QLabel *chartTypeLabel;
    QComboBox *chartTypeComboBox;
    QPushButton *refreshChartButton;
    QPushButton *resetChartButton;
    QSpacerItem *controlSpacer;
    QChartView *statsChartView;
    QLabel *hoverDescriptionLabel;
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
        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName("controlLayout");
        statsTypeLabel = new QLabel(chartGroupBox);
        statsTypeLabel->setObjectName("statsTypeLabel");

        controlLayout->addWidget(statsTypeLabel);

        statsTypeComboBox = new QComboBox(chartGroupBox);
        statsTypeComboBox->addItem(QString());
        statsTypeComboBox->addItem(QString());
        statsTypeComboBox->setObjectName("statsTypeComboBox");

        controlLayout->addWidget(statsTypeComboBox);

        filterLabel = new QLabel(chartGroupBox);
        filterLabel->setObjectName("filterLabel");

        controlLayout->addWidget(filterLabel);

        filterComboBox = new QComboBox(chartGroupBox);
        filterComboBox->setObjectName("filterComboBox");

        controlLayout->addWidget(filterComboBox);

        chartTypeLabel = new QLabel(chartGroupBox);
        chartTypeLabel->setObjectName("chartTypeLabel");

        controlLayout->addWidget(chartTypeLabel);

        chartTypeComboBox = new QComboBox(chartGroupBox);
        chartTypeComboBox->addItem(QString());
        chartTypeComboBox->addItem(QString());
        chartTypeComboBox->setObjectName("chartTypeComboBox");

        controlLayout->addWidget(chartTypeComboBox);

        refreshChartButton = new QPushButton(chartGroupBox);
        refreshChartButton->setObjectName("refreshChartButton");

        controlLayout->addWidget(refreshChartButton);

        resetChartButton = new QPushButton(chartGroupBox);
        resetChartButton->setObjectName("resetChartButton");

        controlLayout->addWidget(resetChartButton);

        controlSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(controlSpacer);


        chartLayout->addLayout(controlLayout);

        statsChartView = new QChartView(chartGroupBox);
        statsChartView->setObjectName("statsChartView");
        statsChartView->setMinimumSize(QSize(0, 400));

        chartLayout->addWidget(statsChartView);

        hoverDescriptionLabel = new QLabel(chartGroupBox);
        hoverDescriptionLabel->setObjectName("hoverDescriptionLabel");
        hoverDescriptionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        chartLayout->addWidget(hoverDescriptionLabel);


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
        ChartWindow->setWindowTitle(QCoreApplication::translate("ChartWindow", "Statistics Chart", nullptr));
        centralwidget->setStyleSheet(QCoreApplication::translate("ChartWindow", "background-image: url(:/imgs/background.jpg);", nullptr));
        chartGroupBox->setTitle(QCoreApplication::translate("ChartWindow", "Statistics", nullptr));
        statsTypeLabel->setText(QCoreApplication::translate("ChartWindow", "Statistics Type:", nullptr));
        statsTypeComboBox->setItemText(0, QCoreApplication::translate("ChartWindow", "Client Statistics", nullptr));
        statsTypeComboBox->setItemText(1, QCoreApplication::translate("ChartWindow", "Training Statistics", nullptr));

        filterLabel->setText(QCoreApplication::translate("ChartWindow", "Filter:", nullptr));
        chartTypeLabel->setText(QCoreApplication::translate("ChartWindow", "Chart Type:", nullptr));
        chartTypeComboBox->setItemText(0, QCoreApplication::translate("ChartWindow", "Bar Chart", nullptr));
        chartTypeComboBox->setItemText(1, QCoreApplication::translate("ChartWindow", "Pie Chart", nullptr));

        refreshChartButton->setText(QCoreApplication::translate("ChartWindow", "Refresh", nullptr));
        resetChartButton->setText(QCoreApplication::translate("ChartWindow", "Reset", nullptr));
        hoverDescriptionLabel->setText(QCoreApplication::translate("ChartWindow", "Hover over a chart element to see details", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartWindow: public Ui_ChartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTWINDOW_H
