#include <QSqlQuery>
#include <QSqlQueryModel>
#include "meeting.h"
#include "qrcodegen.hpp"
#include <QPainter>
// Parameterized constructor
meeting::meeting(QString title, QString organiser, QString participant, QString agenda, int duration, QDateTime datem)
    : title(title), organiser(organiser), participant(participant), agenda(agenda), duration(duration), datem(datem)
{}

// Getters
int meeting::getId() const { return id; }
QString meeting::getTitle() const { return title; }
QString meeting::getOrganiser() const { return organiser; }
QString meeting::getParticipant() const { return participant; }
QString meeting::getAgenda() const { return agenda; }
int meeting::getDuration() const { return duration; }
QDateTime meeting::getDatem() const { return datem; } // New getter

// Setters
void meeting::setId(const int &id) { this->id = id; }
void meeting::setTitle(const QString &title) { this->title = title; }
void meeting::setOrganiser(const QString &organiser) { this->organiser = organiser; }
void meeting::setParticipant(const QString &participant) { this->participant = participant; }
void meeting::setAgenda(const QString &agenda) { this->agenda = agenda; }
void meeting::setDuration(int duration) { this->duration = duration; }
void meeting::setDatem(const QDateTime &datem) { this->datem = datem; } // New setter

// Functionalities
bool meeting::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO meeting (title, organiser, participant, agenda, duration, datem) "
                  "VALUES (:title, :organiser, :participant, :agenda, :duration, :datem)");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", datem); // Bind new field

    if (!query.exec()) {
        qDebug() << "Add meeting failed: " << query.lastError();
        return false;
    }
    return true;
}
#include "meeting.h"
#include "qrcodegen.hpp" // Include the QR code library
#include <QPainter>

QPixmap meeting::generateQRCode() const {
    using namespace qrcodegen;

    // Create a string with all meeting details to encode in the QR code
    QString qrData = QString("Meeting ID: %1\nTitle: %2\nOrganiser: %3\nParticipant: %4\nAgenda: %5\nDuration: %6 min\nDate: %7")
                         .arg(id)
                         .arg(title)
                         .arg(organiser)
                         .arg(participant)
                         .arg(agenda)
                         .arg(duration)
                         .arg(datem.toString("yyyy-MM-dd hh:mm"));

    // Generate QR code
    QrCode qr = QrCode::encodeText(qrData.toUtf8().constData(), QrCode::Ecc::MEDIUM);

    // Convert QR code to QPixmap
    const int scale = 10; // Scale factor for QR code size
    const int size = qr.getSize() * scale;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::white); // White background

    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                painter.drawRect(x * scale, y * scale, scale, scale);
            }
        }
    }

    return pixmap;
}


QSqlQueryModel* meeting::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    // Explicitly select only the columns we want to display
    model->setQuery("SELECT ID, TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, DATEM FROM meeting");

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error when displaying meetings:" << model->lastError().text();
    } else {
        qDebug() << "Query successful, returned" << model->rowCount() << "rows";
    }

    return model;
}
bool meeting::delet(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM meeting WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool meeting::update()
{
    QSqlQuery query;
    query.prepare("UPDATE meeting SET title = :title, organiser = :organiser, participant = :participant, "
                  "agenda = :agenda, duration = :duration, datem = :datem WHERE id = :id");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", datem); // Bind new field
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to update meeting: " << query.lastError();
        return false;
    }
    return true;
}

QSqlQueryModel* meeting::searchByTitle(const QString& title)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM meeting WHERE title LIKE '%" + title + "%'");
    return model;
}
