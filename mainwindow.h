#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMessageBox>
#include "employee.h"
#include <QTableWidget>
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
    void on_listBtnClicked();
    void on_modifyBtnClicked();
    void on_saveUpdateBtnClicked();
    void on_deleteBtnClicked();
private:
    Ui::MainWindow *ui;
    Employee employee;
    QTableWidget *tableWidget;
};
#endif // MAINWINDOW_H
