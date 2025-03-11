#ifndef MEETING_H
#define MEETING_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError> // Add for error handling
#include <QDate> // Add for date handling

class meeting
{
    QString title, organiser, participant, agenda;
    int id, duration;
    QDate date; // Add date member

public:
    //constructeurs
    meeting(){}
    meeting(QString,QString,QString,QString,int) ;
    meeting(const QString &title, const QString &organiser, const QString &participant, const QString &agenda, int duration, const QDate &date); // Add this line

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getOrganiser() const;
    QString getParticipant() const;
    QString getAgenda() const;
    int getDuration() const;
    QDate getDate() const;


    // Setters
    void setId(const int &id);
    void setTitle(const QString &title);
    void setOrganiser(const QString &organiser);
    void setParticipant(const QString &participant);
    void setAgenda(const QString &agenda);
    void setDuration(int duration);
    void setDate(const QDate &date);

    // Functionalities
    bool add();
    QSqlQueryModel* afficher();
    bool delet(int id);
    bool update();
    QSqlQueryModel* searchByTitle(const QString& title); // Add this line

};

#endif // MEETING_H
