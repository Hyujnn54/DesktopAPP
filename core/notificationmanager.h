// core/notificationmanager.h
#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QList>

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    struct Notification {
        QString action;
        QDateTime timestamp;
        QString location;
        QString details;
        int lineNumber;
    };

    explicit NotificationManager(QObject *parent = nullptr);

    void addNotification(const QString &action, const QString &location, const QString &details, int lineNumber = -1);
    const QList<Notification> &getNotifications() const;
    int getNotificationCount() const;

signals:
    void notificationAdded(int count);

private:
    QList<Notification> notifications;
};

#endif // NOTIFICATIONMANAGER_H
