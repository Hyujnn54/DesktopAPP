#ifndef MEETING_H
#define MEETING_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError> // Add for error handling

class meeting
{
    QString title, organiser, participant, agenda;
    int id, duration;

public:
    //constructeurs
    meeting(){}
    meeting(QString,QString,QString,QString,int) ;

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getOrganiser() const;
    QString getParticipant() const;
    QString getAgenda() const;
    int getDuration() const;

    // Setters
    void setId(const int &id);
    void setTitle(const QString &title);
    void setOrganiser(const QString &organiser);
    void setParticipant(const QString &participant);
    void setAgenda(const QString &agenda);
    void setDuration(int duration);

    // Functionalities
    bool add();
    QSqlQueryModel* afficher();
    bool delet(int id);
    bool update();
    QSqlQueryModel* searchByTitle(const QString& title); // Add this line

};

#endif // MEETING_H
