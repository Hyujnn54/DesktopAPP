#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "formations.h"
#include "emailsender.h"
#include "ChartWindow.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlTableModel> // Added this include
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
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

    // Training Management Slots
    void on_trainingAddButtonClicked();
    void on_trainingDeleteButtonClicked();
    void on_trainingUpdateButtonClicked();
    void on_trainingSearchInput_textChanged(const QString &text);
    void on_trainingResetSearchButton_clicked();
    void on_trainingExportButtonClicked();
    void on_trainingSmsRequestFinished(QNetworkReply *reply);
    void handleTrainingNotificationLabelClick();

    // Shared Slots
    void toggleSidebar();
    void toggleTheme();
    void sendConsultationReminders();
    void on_statisticsButton_clicked(); // Opens ChartWindow

    // Navigation Slots
    void on_clientSectionButton_clicked();
    void on_trainingSectionButton_clicked();

private:
    Ui::MainWindow *ui;
    bool m_dbConnected;
    Client client;
    formations formation;
    ChartWindow *chartWindow;
    EmailSender *emailSender;
    QNetworkAccessManager *networkManager;
    QSqlQueryModel *clientTableModel;
    QSortFilterProxyModel *clientProxyModel;
    QSqlQueryModel *trainingTableModel;
    QSortFilterProxyModel *trainingProxyModel;
    QMap<QString, QString> employeeMap;
    int emailAttempts;
    int emailSuccesses;

    // Notification system
    struct Notification {
        QString action; // e.g., "Added", "Updated", "Deleted"
        QString details; // e.g., "Client: John Doe" or "Training: Python Course"
        QDateTime timestamp;
    };
    QList<Notification> notifications;
    void addNotification(const QString &action, const QString &details);
    void updateNotificationLabel();

    // Shared Methods
    void applyDarkTheme();
    void applyLightTheme();

    // Client Management Methods
    bool validateClientInputs();
    bool isValidName(const QString &name);
    bool isValidDateTime(const QDateTime &dateTime);
    void setupCalendarView();
    void highlightDatesWithConsultations();
    void updateSelectedDateInfo(const QDate &date);
    void performClientSearch();
    void checkAndSendReminders();
    bool eventFilter(QObject* watched, QEvent* event) override;
    void loadEmployees();
    void refreshClientTable();
    bool updateClient(const QString &originalName, const QString &newName, const QString &sector,
                      const QString &contactInfo, const QString &email, const QDateTime &consultationDateTime,
                      const QString &consultant);
    void updateCalendarConsultations();

    // Training Management Methods
    void refreshTrainingTableView();
    void sendSmsNotification(const QString &phoneNumber, const QString &formationName, const QDate &date);

    QSqlTableModel *consultationTableModel;
};

#endif // MAINWINDOW_H
