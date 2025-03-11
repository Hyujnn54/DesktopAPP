#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "meeting.h"
#include "updatemeeting.h" // Add this include
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshTableWidget(); // Already public

private slots:
    void handleAddButtonClick();
    void handleDeleteButtonClick();
    void handleUpdateButtonClick(); // We'll modify this
    void updateInputFields();
    void handleSearchButtonClick();
    void handleSearchTextChanged(const QString& text);
    void handleSortCriteriaChanged(int index);
    void handleStatisticsButtonClick();
    void toggleTheme();
    void toggleSidebar();

private:
    Ui::MainWindow *ui;
    bool isDarkTheme;
    void applyDarkTheme();
    void applyLightTheme();
};

#endif // MAINWINDOW_H
