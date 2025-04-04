#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "formations.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QLabel> // Include for QLabel
#include <QStringList>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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


private:
    Ui::MainWindow *ui;
    formations formation;
    bool isDarkTheme;
    QSqlQueryModel *tableModel;
    QSortFilterProxyModel *proxyModel;
    int notificationCount; // New: Track unread notifications
    QLabel *notificationLabel; // New: Display notification count
    void applyDarkTheme();
    void applyLightTheme();
    QWidget *sidebarWidget;
    void updateNotificationCount(int change, const QString &changeDescription = "");
    void resetNotificationCount();
    void refreshTableView(); // New: Reset count when viewed
    QStringList changeHistory;
};

#endif // MAINWINDOW_H
