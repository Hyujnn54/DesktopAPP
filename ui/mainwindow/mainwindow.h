// ui/mainwindow/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managers/client/clientmanager.h"
#include "managers/training/trainingmanager.h"
#include "managers/meeting/meetingmanager.h"
#include "managers/resources/resourcemanager.h"
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
    void setLoggedInRole(const QString &role); // Méthode pour définir le rôle de l'utilisateur connecté

private slots:
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();
    void on_meetingSectionButton_clicked();
    void on_employeeSectionButton_clicked();
    void on_resourceSectionButton_clicked();
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

    // Employee chart handlers
    void on_employeeChartRefreshButton_clicked();
    void on_employeeChartTypeComboBox_currentIndexChanged(int index);
    void on_employeeChartFilterComboBox_currentIndexChanged(int index);
    void on_employeeToggleLegendCheckBox_toggled(bool checked);

    // Resource management
    void on_confirmFormButton_clicked();
    void on_btnSelectImage_clicked();
    void on_cancelFormButton_clicked();
    void on_updateButton_clicked();
    void on_deleteButton_clicked();
    void on_exportPdfButton_clicked();
    void on_searchTextChanged(const QString &text);
    void on_resetResourceSearchButton_clicked(); // Corrected from resetSearchButton_2
    void on_downloadHistoryButton_clicked();
    void on_clearHistoryButton_clicked();
    void on_btnLookForResource_clicked();
    void on_searchTimeout();
    void on_resourceSearchColumnChanged(int index);

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
    void setupResourceChart();
    void updateClientChart();
    void updateTrainingChart();
    void updateMeetingChart();
    void updateEmployeeChart();
    void updateResourceChart();

    Ui::MainWindow *ui;
    bool m_dbConnected;
    QString m_loggedInRole; // Rôle de l'utilisateur connecté
    ClientManager *clientManager;
    TrainingManager *trainingManager;
    MeetingManager *meetingManager;
    EmployeeManager *employeeManager;
    ResourceManager *resourceManager;
    NotificationManager *notificationManager;
    QNetworkAccessManager *networkManager;
    
    // Resource management variables
    int selectedResourceId;
    QByteArray imageData;
    int currentSortColumn;
    Qt::SortOrder currentSortOrder;
    QTimer *searchTimer;
    QPieSeries *pieSeries;
    QBarSeries *barSeries;
};

#endif // MAINWINDOW_H