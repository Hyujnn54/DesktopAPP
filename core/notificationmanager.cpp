// core/notificationmanager.cpp
#include "notificationmanager.h"

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent)
{
}

void NotificationManager::addNotification(const QString &action, const QString &location, const QString &details, int lineNumber)
{
    Notification notification;
    notification.action = action;
    notification.timestamp = QDateTime::currentDateTime();
    notification.location = location;
    notification.details = details;
    notification.lineNumber = lineNumber;
    notifications.append(notification);
    emit notificationAdded(notifications.size());
}

const QList<NotificationManager::Notification> &NotificationManager::getNotifications() const
{
    return notifications;
}

int NotificationManager::getNotificationCount() const
{
    return notifications.size();
}
