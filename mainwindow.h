#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMessageBox>
#include "employee.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_addButtonclicked();
    void loadEmployees();
    void on_modifyBtnClicked();
    void on_saveUpdateBtnClicked();
    void on_deleteBtnClicked();
    void on_resetSearchButtonclicked();
    void searchEmployees();
    void sortEmployees(int column);
    void toggleTheme();
    void toggleSidebar();

private:
    Ui::MainWindow *ui;
    Employee *employee;
    QSqlQueryModel *employeeModel;
    void setupConnections();
    QMap<int, Qt::SortOrder> sortOrders; // Track sort order per column
    void handleHeaderClicked(int column);
    bool isDarkTheme;
    void applyDarkTheme();
    void applyLightTheme();
};
#endif // MAINWINDOW_H
