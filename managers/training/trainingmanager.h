// managers/training/trainingmanager.h
#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <QObject>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QDateTime>
#include "formations.h"
#include "core/notificationmanager.h"
#include "dialog/updatetrainingdialog/updatetrainingdialog.h"

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
    void setNotificationManager(NotificationManager *manager);

private slots:
    void on_trainingAddButton_clicked();
    void on_trainingDeleteButton_clicked();
    void on_trainingUpdateButton_clicked();
    void on_trainingSearchInput_textChanged(const QString &text);
    void on_trainingSearchCriteriaComboBox_currentIndexChanged(int index);
    void on_trainingResetSearchButton_clicked();
    void on_trainingTableViewHeader_clicked(int logicalIndex);
    void on_trainingExportPdfButton_clicked();

private:
    bool m_dbConnected;
    Ui::MainWindow *ui;
    formations *formations;
    QSqlQueryModel *trainingTableModel;
    QSortFilterProxyModel *trainingProxyModel;
    NotificationManager *notificationManager;
    int currentSortColumn;
    Qt::SortOrder currentSortOrder;

    bool validateTrainingInputs();
    bool isValidName(const QString &name);
    void clearTrainingInputs();
    void performTrainingSearch();
    void refreshTrainingTable();
    void exportTrainingsToPdf();
};

#endif // TRAININGMANAGER_H
