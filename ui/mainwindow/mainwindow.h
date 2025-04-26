// ui/mainwindow/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managers/client/clientmanager.h"
#include "managers/training/trainingmanager.h"
#include "managers/meeting/meetingmanager.h"
#include "core/notificationmanager.h"
#include "ui/chartwindow/chartwindow.h"
#include <QMainWindow>
#include <QNetworkAccessManager>

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
    ChartWindow* getChartWindow() const;

private slots:
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();
    void on_meetingSectionButton_clicked();
    void on_statisticsButton_clicked();
    void toggleSidebar();
    void toggleTheme();
    void handleNotificationLabelClicked();
    void updateNotificationLabel(int count);
    void on_chatSendButton_clicked();
    void on_chatClearButton_clicked();
    void processUserInput(const QString &input);
    void onAIResponseReceived(QNetworkReply *reply);

private:
    void setupUiConnections();
    void applyLightTheme();
    void applyDarkTheme();
    void appendChatMessage(const QString &message, bool isBot = false);
    meeting createMeetingFromInput(const QString &input);
    bool validateMeetingInput(const QStringList &parts, QString &errorMessage);

    Ui::MainWindow *ui;
    bool m_dbConnected;
    ClientManager *clientManager;
    TrainingManager *trainingManager;
    MeetingManager *meetingManager;
    NotificationManager *notificationManager;
    QNetworkAccessManager *networkManager;
    ChartWindow *chartWindow;
};

#endif // MAINWINDOW_H
