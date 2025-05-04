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

meeting::meeting(int id, const QString &title, const QString &organiser, const QString &participant,
                const QString &agenda, int duration, const QDateTime &dateTime,
                const QVariant &employeeId, const QVariant &clientId, const QVariant &resourceId)
    : id(id), title(title), organiser(organiser), participant(participant),
    agenda(agenda), duration(duration), m_dateTime(dateTime),
    employeeId(employeeId), clientId(clientId), resourceId(resourceId)
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
    try {
        // Make sure required fields are not empty
        if (title.isEmpty() || organiser.isEmpty() || participant.isEmpty() || agenda.isEmpty()) {
            qDebug() << "Error: Required fields cannot be empty";
            return false;
        }
        
        QSqlQuery query;
        query.prepare("INSERT INTO AHMED.MEETING (ID, TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, EMPLOYEE_ID, CLIENT_ID, RESSOURCE_ID, DATEM) "
                      "VALUES (:id, :title, :organiser, :participant, :agenda, :duration, :employee_id, :client_id, :ressource_id, :datem)");
        query.bindValue(":id", id);
        query.bindValue(":title", title);
        query.bindValue(":organiser", organiser);
        query.bindValue(":participant", participant);
        query.bindValue(":agenda", agenda);
        query.bindValue(":duration", duration);

        // Properly handle NULL values for foreign keys
        if (employeeId.isValid() && !employeeId.isNull()) {
            bool ok;
            int empId = employeeId.toInt(&ok);
            if (ok) {
                query.bindValue(":employee_id", empId);
            } else {
                // Update to use QMetaType instead of deprecated QVariant::Int
                query.bindValue(":employee_id", QVariant(QMetaType(QMetaType::Int)));
            }
        } else {
            // Update to use QMetaType instead of deprecated QVariant::Int
            query.bindValue(":employee_id", QVariant(QMetaType(QMetaType::Int)));
        }
        
        if (clientId.isValid() && !clientId.isNull()) {
            bool ok;
            int clId = clientId.toInt(&ok);
            if (ok) {
                query.bindValue(":client_id", clId);
            } else {
                // Update to use QMetaType instead of deprecated QVariant::Int
                query.bindValue(":client_id", QVariant(QMetaType(QMetaType::Int)));
            }
        } else {
            // Update to use QMetaType instead of deprecated QVariant::Int
            query.bindValue(":client_id", QVariant(QMetaType(QMetaType::Int)));
        }
        
        if (resourceId.isValid() && !resourceId.isNull()) {
            bool ok;
            int resId = resourceId.toInt(&ok);
            if (ok) {
                query.bindValue(":ressource_id", resId);
            } else {
                // Update to use QMetaType instead of deprecated QVariant::Int
                query.bindValue(":ressource_id", QVariant(QMetaType(QMetaType::Int)));
            }
        } else {
            // Update to use QMetaType instead of deprecated QVariant::Int
            query.bindValue(":ressource_id", QVariant(QMetaType(QMetaType::Int)));
        }
        
        query.bindValue(":datem", m_dateTime);

        if (!query.exec()) {
            qDebug() << "Error executing query in meeting::add:" << query.lastError().text();
            return false;
        }

        // Get the last inserted ID using a separate query
        QSqlQuery idQuery("SELECT MAX(ID) FROM AHMED.MEETING");
        if (idQuery.next()) {
            id = idQuery.value(0).toInt();
            qDebug() << "Successfully added meeting with ID:" << id;
        } else {
            qDebug() << "Could not retrieve the ID of the newly inserted meeting";
        }
        
        return true;
    } catch (const std::exception& e) {
        qDebug() << "Exception in meeting::add:" << e.what();
        return false;
    } catch (...) {
        qDebug() << "Unknown exception in meeting::add";
        return false;
    }
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
                  "AGENDA = :agenda, DURATION = :duration, EMPLOYEE_ID = :employeeId, CLIENT_ID = :clientId, "
                  "RESSOURCE_ID = :resourceId, DATEM = :datem WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":title", title);
    query.bindValue(":organiser", organiser);
    query.bindValue(":participant", participant);
    query.bindValue(":agenda", agenda);
    query.bindValue(":duration", duration);
    
    // Properly handle NULL values for foreign keys using the same approach as in add()
    if (employeeId.isValid() && !employeeId.isNull()) {
        bool ok;
        int empId = employeeId.toInt(&ok);
        if (ok) {
            query.bindValue(":employeeId", empId);
        } else {
            query.bindValue(":employeeId", QVariant(QMetaType(QMetaType::Int)));
        }
    } else {
        query.bindValue(":employeeId", QVariant(QMetaType(QMetaType::Int)));
    }
    
    if (clientId.isValid() && !clientId.isNull()) {
        bool ok;
        int clId = clientId.toInt(&ok);
        if (ok) {
            query.bindValue(":clientId", clId);
        } else {
            query.bindValue(":clientId", QVariant(QMetaType(QMetaType::Int)));
        }
    } else {
        query.bindValue(":clientId", QVariant(QMetaType(QMetaType::Int)));
    }
    
    if (resourceId.isValid() && !resourceId.isNull()) {
        bool ok;
        int resId = resourceId.toInt(&ok);
        if (ok) {
            query.bindValue(":resourceId", resId);
        } else {
            query.bindValue(":resourceId", QVariant(QMetaType(QMetaType::Int)));
        }
    } else {
        query.bindValue(":resourceId", QVariant(QMetaType(QMetaType::Int)));
    }
    
    query.bindValue(":datem", m_dateTime);

    if (!query.exec()) {
        qDebug() << "Error executing query in meeting::update:" << query.lastError().text();
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
