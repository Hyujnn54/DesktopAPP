// ui/mainwindow/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "managers/client/clientmanager.h"
#include "managers/training/trainingmanager.h"
#include "core/notificationmanager.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool dbConnected, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();
    void on_statisticsButton_clicked();
    void toggleSidebar();
    void toggleTheme();
    void on_notificationLabel_clicked();
    void updateNotificationLabel(int count);

private:
    void setupUiConnections();
    void applyLightTheme();
    void applyDarkTheme();

    Ui::MainWindow *ui;
    bool m_dbConnected;
    ClientManager *clientManager;
    TrainingManager *trainingManager;
    NotificationManager *notificationManager;
};

#endif // MAINWINDOW_H
