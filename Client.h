#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMap>

class Client {
public:
    Client(); // Default constructor

    // Updated constructor with email parameter
    Client(const QString &name, const QString &sector, const QString &contact, const QString &email, const QDateTime &consultationDateTime, int consultant);

    int getClientId() const;
    QString getName() const;
    QString getSector() const;
    QString getContactInfo() const;
    QString getEmail() const; // New getter
    QDateTime getConsultationDateTime() const;
    int getConsultant() const;

    void setName(const QString& name);
    void setSector(const QString& sector);
    void setContactInfo(const QString& contactInfo);
    void setEmail(const QString& email); // New setter
    void setConsultationDateTime(const QDateTime& consultationDateTime);
    void setConsultant(int consultant);

    bool ajouter();
    bool removeByName(const QString &name);
    bool updateClient(const QString &oldName, const QString &newName, const QString &newSector, const QString &newContact, const QString &newEmail, const QDateTime &newConsultationDateTime, int newConsultant); // Updated
    QSqlQueryModel* afficher();
    QSqlQueryModel* sortByColumn(int column, Qt::SortOrder order);

    QSqlQueryModel* searchByName(const QString &name);
    QSqlQueryModel* searchBySector(const QString &sector);
    QSqlQueryModel* searchByDateTime(const QDateTime &dateTime);
    QSqlQueryModel* searchByEmail(const QString &email); // New
    QSqlQueryModel* searchByContact(const QString &contact); // New
    QSqlQueryModel* searchByConsultant(int consultant); // New
    QSqlQueryModel* getUpcomingConsultations(const QDateTime &start, const QDateTime &end);

    QMap<QDateTime, int> getConsultationCountsByDateTime();
    QMap<QDate, int> getConsultationCountsByDate();
    QSqlQueryModel* getConsultationsForDateTime(const QDateTime& dateTime);
    QSqlQueryModel* getConsultationsForDate(const QDate& date);

private:
    int clientId;
    QString name;
    QString sector;
    QString contactInfo;
    QString email; // New member
    QDateTime consultationDateTime;
    int consultant;
};

#endif // CLIENT_H
