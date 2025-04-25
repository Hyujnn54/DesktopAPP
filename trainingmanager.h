#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include "formations.h" // Changed to lowercase formations.h
#include <QObject>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include <QRegularExpression>

namespace Ui { class MainWindow; }

class TrainingManager : public QObject
{
    Q_OBJECT

public:
    explicit TrainingManager(bool dbConnected, QObject *parent = nullptr);
    ~TrainingManager();
    void initialize(Ui::MainWindow *ui);
    void refresh();

private slots:
    void on_trainingAddButton_clicked();
    void on_trainingDeleteButton_clicked();
    void on_trainingUpdateButton_clicked();
    void on_trainingSearchInput_textChanged(const QString &text);
    void on_trainingResetSearchButton_clicked();
    void on_trainingExportButton_clicked();
    void on_trainingSmsRequest_finished(QNetworkReply *reply);
    void handleTrainingNotificationLabel_clicked();

private:
    void refreshTrainingTableView();
    void sendSmsNotification(const QString &phoneNumber, const QString &formationName, const QDate &date);
    void addNotification(const QString &action, const QString &details);
    void updateNotificationLabel();
    void exportTrainingsToPdf();
    bool validateTrainingInputs();

    struct Notification {
        QString action;
        QString details;
        QDateTime timestamp;
    };
    QList<Notification> notifications;

    bool m_dbConnected;
    Ui::MainWindow *ui;
    formations *formation; // Changed to lowercase formations
    QNetworkAccessManager *networkManager;
    QSqlQueryModel *trainingTableModel;
    QSortFilterProxyModel *trainingProxyModel;
    QDateTime lastTrainingTableRefresh;
    static const int TRAINING_REFRESH_INTERVAL_MS = 1000;
};

#endif // TRAININGMANAGER_H
