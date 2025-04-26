// managers/client/clientmanager.h
#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QMap>
#include <QDateTime>
#include "client.h"
#include "core/emailsender.h"
#include "ui/chartwindow/chartwindow.h"
#include "core/notificationmanager.h"

namespace Ui {
class MainWindow;
}

class QCalendarWidget;
class QDate;

class ClientManager : public QObject
{
    Q_OBJECT

public:
    explicit ClientManager(bool dbConnected, QObject *parent = nullptr);
    ~ClientManager();

    void initialize(Ui::MainWindow *ui);
    void refresh();
    void showStatistics();
    void setNotificationManager(NotificationManager *manager);

private slots:
    void on_clientAddButton_clicked();
    void on_clientDeleteButton_clicked();
    void on_clientUpdateButton_clicked();
    void on_clientSearchInput_textChanged(const QString &text);
    void on_clientSearchCriteriaComboBox_currentIndexChanged(int index);
    void on_clientResetSearchButton_clicked();
    void on_clientTableViewHeader_clicked(int logicalIndex);
    void on_clientConsultationCalendar_selectionChanged();
    void on_clientConsultationCalendar_activated(const QDate &date);
    void on_clientExportPdfButton_clicked();
    void sendConsultationReminders();

private:
    bool m_dbConnected;
    Ui::MainWindow *ui;
    Client *client;
    ChartWindow *chartWindow;
    EmailSender *emailSender;
    QSqlQueryModel *clientTableModel;
    QSortFilterProxyModel *clientProxyModel;
    QMap<QString, QString> employeeMap;
    int emailAttempts;
    int emailSuccesses;
    QDateTime lastCalendarUpdate;
    NotificationManager *notificationManager;

    bool validateClientInputs();
    bool isValidName(const QString &name);
    bool isValidEmail(const QString &email);
    bool isValidDateTime(const QDateTime &dateTime);
    void setupCalendarView();
    void highlightDatesWithConsultations();
    void updateSelectedDateInfo(const QDate &date);
    void performClientSearch();
    void checkAndSendReminders();
    bool eventFilter(QObject *watched, QEvent *event);
    void loadEmployees();
    void refreshClientTable();
    bool updateClient(const QString &originalName, const QString &newName, const QString &sector,
                      const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime,
                      const QString &consultant);
    void updateCalendarConsultations();
    void exportClientsToPdf();
};

#endif // CLIENTMANAGER_H
