// managers/meeting/meeting.h
#ifndef MEETING_H
#define MEETING_H

#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QVariant>

class meeting {
private:
    int id;
    QString title;
    QString organiser;
    QString participant;
    QString agenda;
    int duration;
    QDateTime m_dateTime;
    QVariant employeeId;
    QVariant clientId;
    QVariant resourceId;

public:
    meeting();
    meeting(QString title, QString organiser, QString participant, QString agenda, int duration, QDateTime dateTime,
            QVariant employeeId = QVariant(), QVariant clientId = QVariant(), QVariant resourceId = QVariant());

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getOrganiser() const;
    QString getParticipant() const;
    QString getAgenda() const;
    int getDuration() const;
    QDateTime getDatem() const;
    QVariant getEmployeeId() const;
    QVariant getClientId() const;
    QVariant getResourceId() const;

    // Setters
    void setId(const int &id);
    void setTitle(const QString &title);
    void setOrganiser(const QString &organiser);
    void setParticipant(const QString &participant);
    void setAgenda(const QString &agenda);
    void setDuration(int duration);
    void setDatem(const QDateTime &dateTime);
    void setEmployeeId(const QVariant &employeeId);
    void setClientId(const QVariant &clientId);
    void setResourceId(const QVariant &resourceId);

    // Functionalities
    bool add();
    QPixmap generateQRCode() const;
    QSqlQueryModel* afficher();
    bool deleteMeeting(int id);
    bool update();
    QSqlQueryModel* searchByTitle(const QString& title);
};

#endif // MEETING_H
