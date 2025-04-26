#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QMap>
#include <QDateTime>
#include <QList>
#include "Client.h"
#include "EmailSender.h"
#include "ChartWindow.h"

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
    void on_trainingNotificationLabel_clicked();

private:
    struct Notification {
        QString action;
        QDateTime timestamp;
        QString location;
        QString details;
        int lineNumber;
    };

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
    QList<Notification> notifications;

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
    bool updateClient(const QString &originalName, const QString &newName, const QString sector,
                      const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime,
                      const QString &consultant);
    void updateCalendarConsultations();
    void exportClientsToPdf();
    void logNotification(const QString &action, const QString &location, const QString &details, int lineNumber);
};

#endif // CLIENTMANAGER_H
