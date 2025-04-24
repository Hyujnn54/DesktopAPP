#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "qcalendarwidget.h"
#include "qdatetimeedit.h"
#include "updateclientdialog.h"
#include "emailsender.h"
#include "ChartWindow.h"
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMainWindow>
#include <QPalette>
#include <QHoverEvent>
#include <QToolTip>
#include <QMap>

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
    void on_removeButtonClicked();
    void on_updateButtonClicked();
    void toggleSidebar();
    void toggleTheme();
    void on_searchInput_textChanged();
    void on_searchCriteriaComboBox_currentIndexChanged();
    void on_resetSearchButton_clicked();
    void tableViewHeaderClicked(int logicalIndex);
    void on_consultationCalendar_selectionChanged();
    void on_consultationCalendar_activated(const QDate &date);
    void updateCalendarConsultations();
    bool isValidDateTime(const QDateTime &dateTime);
    void on_exportPdfButton_clicked();
    void sendConsultationReminders();
    void showStatistics();
    void on_refreshStatsButton_clicked();
    void on_openChartButton_clicked();

private:
    Ui::MainWindow *ui;
    Client Etmp;
    QWidget *sidebarWidget;
    bool isDarkTheme;
    QMap<QDate, int> consultationCountMap;
    QMap<QDateTime, int> consultationDateTimeMap;
    QDate lastHoveredDate;
    QMap<QString, QString> employeeMap; // Map employee names to IDs

    void applyDarkTheme();
    void applyLightTheme();
    bool validateInputs();
    bool isValidName(const QString &name);
    bool isValidDate(const QDate &date);
    bool isValidConsultant(const QString &consultant);
    void setupCalendarView();
    void highlightDatesWithConsultations();
    void updateSelectedDateInfo(const QDate &date);
    void performSearch();
    EmailSender *emailSender;
    void checkAndSendReminders();
    bool calendarHoverEventFilter(QObject* watched, QEvent* event);
    bool eventFilter(QObject* watched, QEvent* event) override;
    void updateStatisticsDisplay();
    void loadEmployees(); // New method to load employees

    int emailAttempts = 0;
    int emailSuccesses = 0;
};
#endif // MAINWINDOW_H
