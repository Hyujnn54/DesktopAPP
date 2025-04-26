#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QDateTime>
#include <QList>
#include "formations.h"

namespace Ui {
class MainWindow;
}

class QDate;

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
    void on_trainingSearchCriteriaComboBox_currentIndexChanged(int index);
    void on_trainingResetSearchButton_clicked();
    void on_trainingTableViewHeader_clicked(int logicalIndex);
    void on_trainingExportPdfButton_clicked(); // New slot for PDF export
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
    formations *formations;
    QSqlQueryModel *trainingTableModel;
    QSortFilterProxyModel *trainingProxyModel;
    QList<Notification> notifications;
    int currentSortColumn; // Track the current sort column
    Qt::SortOrder currentSortOrder; // Track the current sort order

    bool validateTrainingInputs();
    bool isValidName(const QString &name);
    void clearTrainingInputs();
    void performTrainingSearch();
    void refreshTrainingTable();
    void logNotification(const QString &action, const QString &location, const QString &details, int lineNumber);
    void exportTrainingsToPdf(); // New method for PDF export
};

#endif // TRAININGMANAGER_H
