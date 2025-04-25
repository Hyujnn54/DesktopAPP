#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "formations.h"
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
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QRegularExpression>

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
    // Client Management Slots
    void on_clientAddButton_clicked();
    void on_clientDeleteButton_clicked();
    void on_clientUpdateButtonClicked();
    void on_clientSearchInput_textChanged();
    void on_clientSearchCriteriaComboBox_currentIndexChanged();
    void on_clientResetSearchButton_clicked();
    void on_clientTableViewHeaderClicked(int logicalIndex);
    void on_clientConsultationCalendar_selectionChanged();
    void on_clientConsultationCalendar_activated(const QDate &date);
    void on_clientExportPdfButton_clicked();
    void on_clientOpenChartButton_clicked();
    void on_clientRefreshStatsButton_clicked();

    // Training Management Slots
    void on_trainingAddButtonClicked();
    void on_trainingDeleteButtonClicked();
    void on_trainingUpdateButtonClicked();
    void on_trainingSearchInput_textChanged(const QString &text);
    void on_trainingResetSearchButton_clicked();
    void on_trainingExportButtonClicked();
    void on_trainingRefreshStatsButton_clicked();
    void on_trainingSmsRequestFinished(QNetworkReply *reply);

    // Shared Slots
    void toggleSidebar();
    void toggleTheme();
    void sendConsultationReminders();

    // Navigation Slots
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();

private:
    Ui::MainWindow *ui;
    Client client;
    formations formation;
    QWidget *sidebarWidget;
    bool isDarkTheme;
    bool m_dbConnected;

    // Client Management Members
    EmailSender *emailSender;
    int emailAttempts;
    int emailSuccesses;
    QSqlQueryModel *clientTableModel;
    QSortFilterProxyModel *clientProxyModel;
    QMap<QDate, int> consultationCountMap;
    QMap<QDateTime, int> consultationDateTimeMap;
    QDate lastHoveredDate;
    QMap<QString, QString> employeeMap;

    // Training Management Members
    QSqlQueryModel *trainingTableModel;
    QSortFilterProxyModel *trainingProxyModel;
    int notificationCount;
    QStringList changeHistory;
    QNetworkAccessManager *networkManager;
    int totalFormations;
    double totalCost;
    double avgCost;

    // Shared Methods
    void applyDarkTheme();
    void applyLightTheme();

    // Client Management Methods
    bool validateClientInputs();
    bool isValidName(const QString &name);
    bool isValidDate(const QDate &date);
    bool isValidDateTime(const QDateTime &dateTime);
    bool isValidConsultant(const QString &consultant);
    void setupCalendarView();
    void highlightDatesWithConsultations();
    void updateSelectedDateInfo(const QDate &date);
    void performClientSearch();
    void checkAndSendReminders();
    bool calendarHoverEventFilter(QObject* watched, QEvent* event);
    bool eventFilter(QObject* watched, QEvent* event) override;
    void updateClientStatisticsDisplay();
    void loadEmployees();
    void refreshClientTable();
    bool updateClient(const QString &originalName, const QString &newName, const QString &sector,
                      const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime,
                      const QString &consultant);
    void updateCalendarConsultations();

    // Training Management Methods
    void refreshTrainingTableView();
    void updateTrainingStatistics();
    void sendSmsNotification(const QString &phoneNumber, const QString &formationName, const QDate &date);
    void addChangeToHistory(const QString &action, const QString &formationName);
};

#endif // MAINWINDOW_H
