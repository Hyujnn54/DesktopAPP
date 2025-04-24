#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QMap>
#include <QPair>

class Client {
public:
    Client();
    Client(const QString& name, const QString& sector, const QString& contactInfo,
           const QString& email, const QDateTime& consultationDateTime, const QString& consultant);

    // Getters
    int getClientId() const;
    QString getName() const;
    QString getSector() const;
    QString getContactInfo() const;
    QString getEmail() const;
    QDateTime getConsultationDateTime() const;
    QString getConsultant() const;

    // Setters
    void setName(const QString& name);
    void setSector(const QString& sector);
    void setContactInfo(const QString& contactInfo);
    void setEmail(const QString& email);
    void setConsultationDateTime(const QDateTime& consultationDateTime);
    void setConsultant(const QString& consultant);

    QPair<bool, QString> ajouter(QString name, QString sector, QString contact, QString email, QDateTime dateTime, QString consultant);
    QSqlQueryModel* afficher();
    bool updateClient(const QString& originalName, const QString& newName, const QString& sector,
                      const QString& contactInfo, const QString& email, const QDateTime& consultationDateTime,
                      const QString& consultant);
    QSqlQueryModel* searchByName(const QString& name);
    QSqlQueryModel* searchBySector(const QString& sector);
    QSqlQueryModel* searchByDateTime(const QDateTime& dateTime);
    QSqlQueryModel* sortByColumn(int column, Qt::SortOrder order);
    QMap<QDateTime, int> getConsultationCountsByDateTime();
    QMap<QDate, int> getConsultationCountsByDate();
    QSqlQueryModel* getConsultationsForDate(const QDate& date);
    QSqlQueryModel* searchByEmail(const QString& email);
    QSqlQueryModel* searchByContact(const QString& contact);
    QSqlQueryModel* searchByConsultant(const QString& consultant);
    QSqlQueryModel* getUpcomingConsultations(const QDateTime& start, const QDateTime& end);
    int getTotalConsultations(const QDateTime& start, const QDateTime& end);
    int getUpcomingConsultationsCount(const QDateTime& start, const QDateTime& end);
    QMap<QDate, int> getConsultationsPerDay(const QDateTime& start, const QDateTime& end);
    int getUniqueClients(const QDateTime& start, const QDateTime& end);
    bool removeByName(const QString& name);

private:
    int clientId;
    QString name;
    QString sector;
    QString contactInfo;
    QString email;
    QDateTime consultationDateTime;
    QString consultantId; // Renamed from consultant to match client.cpp
};

#endif // CLIENT_H
