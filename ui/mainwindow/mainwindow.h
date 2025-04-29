// ui/mainwindow/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managers/client/clientmanager.h"
#include "managers/training/trainingmanager.h"
#include "managers/meeting/meetingmanager.h"
#include "managers/employee/employeemanager.h"
#include "core/notificationmanager.h"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class meeting;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool dbConnected, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();
    void on_meetingSectionButton_clicked();
    void on_employeeSectionButton_clicked();
    void on_statisticsButton_clicked();
    void toggleSidebar();
    void toggleTheme();
    void handleNotificationLabelClicked();
    void updateNotificationLabel(int count);
    void on_chatSendButton_clicked();
    void on_chatClearButton_clicked();
    void processUserInput(const QString &input);
    void onAIResponseReceived(QNetworkReply *reply);
    
    // Employee slots
    void on_employeeSearchChanged(const QString &text);
    void on_resetSearchButton_clicked();
    void on_addButton_clicked();
    void on_deleteBtn_clicked();
    void on_modifyBtn_clicked();
    void on_downloadBtn_clicked();
    void on_selectImageButton_clicked();
    void on_generateQRCodeBtn_clicked();
    bool validateEmployeeInput();
    
    // Input validation slots
    void validateCIN(const QString &text);
    void validateName(const QString &text);
    void validatePhone(const QString &text);
    void validateEmail(const QString &text);
    void validateSalary(const QString &text);
    
    // New chart handlers for each section
    void on_clientChartRefreshButton_clicked();
    void on_trainingChartRefreshButton_clicked();
    void on_meetingChartRefreshButton_clicked();
    void on_clientChartTypeComboBox_currentIndexChanged(int index);
    void on_trainingChartTypeComboBox_currentIndexChanged(int index);
    void on_meetingChartTypeComboBox_currentIndexChanged(int index);
    void on_clientChartFilterComboBox_currentIndexChanged(int index);
    void on_trainingChartFilterComboBox_currentIndexChanged(int index);
    void on_meetingChartFilterComboBox_currentIndexChanged(int index);

private:
    void setupUiConnections();
    void setupInputValidators();
    void applyLightTheme();
    void applyDarkTheme();
    void appendChatMessage(const QString &message, bool isBot = false);
    meeting createMeetingFromInput(const QString &input);
    bool validateMeetingInput(const QStringList &parts, QString &errorMessage);
    
    // Table display improvement methods
    void improveTableDisplay(QTableView* tableView);
    void improveTableWidgetDisplay(QTableWidget* tableWidget);
    
    // Chart helper methods
    void setupChartConnections();
    void setupClientChart();
    void setupTrainingChart();
    void setupMeetingChart();
    void setupEmployeeChart();
    void updateClientChart();
    void updateTrainingChart();
    void updateMeetingChart();
    void updateEmployeeChart();

    Ui::MainWindow *ui;
    bool m_dbConnected;
    ClientManager *clientManager;
    TrainingManager *trainingManager;
    MeetingManager *meetingManager;
    EmployeeManager *employeeManager;
    NotificationManager *notificationManager;
    QNetworkAccessManager *networkManager;
};

#endif // MAINWINDOW_H
