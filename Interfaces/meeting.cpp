#include <QSqlQuery>
#include <QSqlQueryModel>
#include"meeting.h"
// Parameterized constructor
meeting::meeting(QString title, QString organiser, QString participant, QString agenda, int duration)
    : title(title), organiser(organiser), participant(participant), agenda(agenda), duration(duration), date(QDate::currentDate()) {}
// Getters
int meeting::getId() const { return id; }
QString meeting::getTitle() const { return title; }
QString meeting::getOrganiser() const { return organiser; }
QString meeting::getParticipant() const { return participant; }
QString meeting::getAgenda() const { return agenda; }
int meeting::getDuration() const { return duration; }
QDate meeting::getDate() const { return date; }

// Setters
void meeting::setId(const int &id) { this->id = id; }
void meeting::setTitle(const QString &title) { this->title = title; }
void meeting::setOrganiser(const QString &organiser) { this->organiser = organiser; }
void meeting::setParticipant(const QString &participant) { this->participant = participant; }
void meeting::setAgenda(const QString &agenda) { this->agenda = agenda; }
void meeting::setDuration(int duration) { this->duration = duration; }
void meeting::setDate(const QDate &date) { this->date = date; }

// Functionalities
bool meeting::add() {
    QSqlQuery query;
    query.prepare("INSERT INTO meetings (title, organiser, participant, agenda, duration, datem) "
                  "VALUES (:title, :organiser, :participant, :agenda, :duration, :datem)");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", date); // Bind date value
    return query.exec();
}
QSqlQueryModel* meeting::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id, title, organiser, participant, agenda, duration, status, datem FROM meetings");
    return model;
}
bool meeting::delet(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM meetings WHERE id = :id");  // Changed "meeting" to "meetings"
    query.bindValue(":id", id);

    bool success = query.exec();

    if (!success) {
        qDebug() << "SQL Error when deleting meeting:" << query.lastError().text();
    }

    return success;
}

bool meeting::update() {
    QSqlQuery query;
    query.prepare("UPDATE meetings SET title = :title, organiser = :organiser, participant = :participant, agenda = :agenda, duration = :duration, datem = :datem "
                  "WHERE id = :id");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", date); // Bind date value
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* meeting::searchByTitle(const QString &title) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id, title, organiser, participant, agenda, duration, datem FROM meetings WHERE title LIKE :title");
    query.bindValue(":title", "%" + title + "%");
    query.exec();
    model->setQuery(query);
    return model;
}
