#ifndef FORMATIONWINDOW_H
#define FORMATIONWINDOW_H
#include "formations.h"
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
namespace Ui { class formationwindow; } // Use lowercase 'formationwindow'
QT_END_NAMESPACE

class FormationWindow : public QMainWindow
{
    Q_OBJECT

public:
    FormationWindow(QWidget *parent = nullptr);
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

private:
    Ui::formationwindow *ui; // Use lowercase 'formationwindow'
    formations formation;
    bool isDarkTheme;
    QSqlQueryModel *tableModel;
    QSortFilterProxyModel *proxyModel;
    int notificationCount;
    QLabel *notificationLabel;
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
