#ifndef FORMATIONWINDOW_H
#define FORMATIONWINDOW_H
#include "formations.h"
#include "ui_formationwindow.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QStringList>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow;

class FormationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormationWindow(bool dbConnected, QWidget *parent = nullptr);
    ~FormationWindow();

private slots:
    void on_addButtonclicked();
    void toggleTheme();
    void toggleSidebar();
    void on_deleteButtonClicked();
    void on_updateButtonClicked();
    void on_searchInput_textChanged(const QString &text);
    void on_resetSearchButton_clicked();
    void exportToPdf();
    void onNotificationLabelClicked();
    void on_refreshStatsButton_clicked();
    void onSmsRequestFinished(QNetworkReply *reply);
    void on_pushButton_2_clicked();

private:
    Ui::formationwindow *ui;
    formations formation;
    bool isDarkTheme;
    QSqlQueryModel *tableModel;
    QSortFilterProxyModel *proxyModel;
    int notificationCount;
    QLabel *notificationLabel;
    bool m_dbConnected; // Store the database connection status
    void applyDarkTheme();
    void applyLightTheme();
    QWidget *sidebarWidget;
    void updateNotificationCount(int change, const QString &changeDescription = "");
    void resetNotificationCount();
    void refreshTableView();
    QStringList changeHistory;
    QNetworkAccessManager *networkManager;
    int totalFormations;
    double totalCost;
    double avgCost;
    void updateStatistics();
};

#endif // FORMATIONWINDOW_H
