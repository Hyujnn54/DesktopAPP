#ifndef MEETING_H
#define MEETING_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDateTime> // Add this for QDateTime
#include <QPixmap>

class meeting
{
    QString title, organiser, participant, agenda;
    int id, duration;
    QDateTime datem; // New datetime field

public:
    // Constructors
    meeting() {}
    meeting(QString, QString, QString, QString, int, QDateTime); // Updated constructor

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getOrganiser() const;
    QString getParticipant() const;
    QString getAgenda() const;
    int getDuration() const;
    QDateTime getDatem() const; // New getter

    // Setters
    void setId(const int &id);
    void setTitle(const QString &title);
    void setOrganiser(const QString &organiser);
    void setParticipant(const QString &participant);
    void setAgenda(const QString &agenda);
    void setDuration(int duration);
    void setDatem(const QDateTime &datem); // New setter
    // New method to generate QR code
    QPixmap generateQRCode() const;
    // Functionalities
    bool add();
    QSqlQueryModel* afficher();
    bool delet(int id);
    bool update();
    QSqlQueryModel* searchByTitle(const QString& title);
};

#endif // MEETING_H
