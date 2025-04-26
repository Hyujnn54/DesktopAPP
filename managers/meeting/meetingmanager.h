// managers/meeting/meetingmanager.h
#ifndef MEETINGMANAGER_H
#define MEETINGMANAGER_H

#include <QObject>
#include <QWidget>
#include "meeting.h"
#include "../../core/notificationmanager.h"

namespace Ui {
class MainWindow;
}

class MeetingManager : public QObject
{
    Q_OBJECT

public:
    explicit MeetingManager(bool dbConnected, QObject *parent = nullptr);
    ~MeetingManager();

    void setNotificationManager(NotificationManager *manager);
    void initialize(Ui::MainWindow *ui);
    void refreshTableWidget();
    // Add this declaration
    QMap<QString, int> getStatisticsByCategory(const QString &category);

public slots:
    void handleAddButtonClick();
    void handleDeleteButtonClick();
    void handleUpdateButtonClick();
    void handleSearchButtonClick();
    void handleSearchTextChanged(const QString &text);
    void handleSortCriteriaChanged(int index);
    void handleGenerateQRCodeButtonClick();
    void handleExportPdfButtonClick();
    void handleStatisticsButtonClick();
    void handleRefreshStatsButtonClick();
    void handleTabChanged(int index);
    void updateInputFields();

private:
    bool m_dbConnected;
    Ui::MainWindow *ui;
    NotificationManager *notificationManager;

    void applyThemeStyles();
};

#endif // MEETINGMANAGER_H
