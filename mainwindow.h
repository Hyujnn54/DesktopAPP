#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ClientManager.h"
#include "TrainingManager.h"
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

private:
    void setupUiConnections();
    void applyLightTheme();
    void applyDarkTheme();

    Ui::MainWindow *ui;
    bool m_dbConnected;
    ClientManager *clientManager;
    TrainingManager *trainingManager;
};

#endif // MAINWINDOW_H
