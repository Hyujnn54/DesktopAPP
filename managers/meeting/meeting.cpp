// managers/meeting/meeting.cpp
#include "meeting.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include "lib/qrcodegen/qrcodegen.hpp"
#include <QPainter>
#include <QDebug>

meeting::meeting()
    : id(-1),
    title(""),
    organiser(""),
    participant(""),
    agenda(""),
    duration(0),
    m_dateTime(QDateTime::currentDateTime())
{
}

meeting::meeting(QString title, QString organiser, QString participant, QString agenda, int duration, QDateTime dateTime,
                 QVariant employeeId, QVariant clientId, QVariant resourceId)
    : id(-1),
    title(title),
    organiser(organiser),
    participant(participant),
    agenda(agenda),
    duration(duration),
    m_dateTime(dateTime),
    employeeId(employeeId),
    clientId(clientId),
    resourceId(resourceId)
{
}

// Getters
int meeting::getId() const { return id; }
QString meeting::getTitle() const { return title; }
QString meeting::getOrganiser() const { return organiser; }
QString meeting::getParticipant() const { return participant; }
QString meeting::getAgenda() const { return agenda; }
int meeting::getDuration() const { return duration; }
QDateTime meeting::getDatem() const { return m_dateTime; }
QVariant meeting::getEmployeeId() const { return employeeId; }
QVariant meeting::getClientId() const { return clientId; }
QVariant meeting::getResourceId() const { return resourceId; }

// Setters
void meeting::setId(const int &id) { this->id = id; }
void meeting::setTitle(const QString &title) { this->title = title; }
void meeting::setOrganiser(const QString &organiser) { this->organiser = organiser; }
void meeting::setParticipant(const QString &participant) { this->participant = participant; }
void meeting::setAgenda(const QString &agenda) { this->agenda = agenda; }
void meeting::setDuration(int duration) { this->duration = duration; }
void meeting::setDatem(const QDateTime &dateTime) { this->m_dateTime = dateTime; }
void meeting::setEmployeeId(const QVariant &employeeId) { this->employeeId = employeeId; }
void meeting::setClientId(const QVariant &clientId) { this->clientId = clientId; }
void meeting::setResourceId(const QVariant &resourceId) { this->resourceId = resourceId; }

// Functionalities
bool meeting::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.MEETING (TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, EMPLOYEE_ID, CLIENT_ID, RESSOURCE_ID, DATEM) "
                  "VALUES (:title, :organiser, :participant, :agenda, :duration, :employee_id, :client_id, :ressource_id, :datem)");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":employee_id", employeeId.isNull() ? QVariant() : employeeId);
    query.bindValue(":client_id", clientId.isNull() ? QVariant() : clientId);
    query.bindValue(":ressource_id", resourceId.isNull() ? QVariant() : resourceId);
    query.bindValue(":datem", m_dateTime);

    if (!query.exec()) {
        qDebug() << "Error executing query in meeting::add:" << query.lastError().text();
        return false;
    }
    return true;
}

QPixmap meeting::generateQRCode() const
{
    using namespace qrcodegen;

    QString qrData = QString("Meeting ID: %1\nTitle: %2\nOrganiser: %3\nParticipant: %4\nAgenda: %5\nDuration: %6 min\nDate: %7")
                         .arg(id)
                         .arg(title)
                         .arg(organiser)
                         .arg(participant)
                         .arg(agenda)
                         .arg(duration)
                         .arg(m_dateTime.toString("yyyy-MM-dd hh:mm"));

    try {
        QrCode qr = QrCode::encodeText(qrData.toUtf8().constData(), QrCode::Ecc::MEDIUM);
        const int scale = 10;
        const int size = qr.getSize() * scale;
        if (size <= 0) {
            qDebug() << "QR code size is invalid";
            return QPixmap();
        }

        QPixmap pixmap(size, size);
        pixmap.fill(Qt::white);

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
    } catch (const std::exception &e) {
        qDebug() << "QR code generation failed:" << e.what();
        return QPixmap();
    }
}

QSqlQueryModel* meeting::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query("SELECT ID, TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, DATEM FROM AHMED.MEETING");
    model->setQuery(std::move(query));
    if (model->lastError().isValid()) {
        qDebug() << "SQL Error in afficher:" << model->lastError().text();
    } else {
        qDebug() << "afficher: Query successful, returned" << model->rowCount() << "rows";
        for (int row = 0; row < model->rowCount(); ++row) {
            qDebug() << "Row" << row << ": ID =" << model->data(model->index(row, 0)).toInt()
            << ", Title =" << model->data(model->index(row, 1)).toString()
            << ", Organiser =" << model->data(model->index(row, 2)).toString()
            << ", Participant =" << model->data(model->index(row, 3)).toString()
            << ", Agenda =" << model->data(model->index(row, 4)).toString()
            << ", Duration =" << model->data(model->index(row, 5)).toInt()
            << ", DATEM =" << model->data(model->index(row, 6)).toDateTime();
        }
    }
    return model;
}

bool meeting::deleteMeeting(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM AHMED.MEETING WHERE ID = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Delete meeting failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool meeting::update()
{
    QSqlQuery query;
    query.prepare("UPDATE AHMED.MEETING SET TITLE = :title, ORGANISER = :organiser, PARTICIPANT = :participant, "
                  "AGENDA = :agenda, DURATION = :duration, DATEM = :datem, EMPLOYEE_ID = :employeeId, "
                  "CLIENT_ID = :clientId, RESSOURCE_ID = :resourceId WHERE ID = :id");
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    query.bindValue(":datem", m_dateTime);
    // Explicitly convert to int or NULL
    query.bindValue(":employeeId", employeeId.isNull() ? QVariant(QVariant::Int) : employeeId.toInt());
    query.bindValue(":clientId", clientId.isNull() ? QVariant(QVariant::Int) : clientId.toInt());
    query.bindValue(":resourceId", resourceId.isNull() ? QVariant(QVariant::Int) : resourceId.toInt());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to update meeting: " << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* meeting::searchByTitle(const QString& title)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM AHMED.MEETING WHERE TITLE LIKE '%" + title + "%'");
    if (model->lastError().isValid()) {
        qDebug() << "Search meeting failed: " << model->lastError().text();
    }
    return model;
}
