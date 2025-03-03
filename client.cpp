#include "Client.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

// Default constructor
Client::Client()
    : clientId(0), name(""), sector(""), contactInfo(""), email(""),
    consultationDateTime(QDateTime::currentDateTime()), consultant(0) {}

// Updated parameterized constructor
Client::Client(const QString& name, const QString& sector,
               const QString& contactInfo, const QString& email,
               const QDateTime& consultationDateTime, int consultant)
    : clientId(1),
    name(name),
    sector(sector),
    contactInfo(contactInfo),
    email(email),
    consultationDateTime(consultationDateTime),
    consultant(consultant) {}

// Getters
int Client::getClientId() const {
    return clientId;
}

QString Client::getName() const {
    return name;
}

QString Client::getSector() const {
    return sector;
}

QString Client::getContactInfo() const {
    return contactInfo;
}

QString Client::getEmail() const {
    return email;
}

QDateTime Client::getConsultationDateTime() const {
    return consultationDateTime;
}

int Client::getConsultant() const {
    return consultant;
}

// Setters
void Client::setName(const QString& name) {
    this->name = name;
}

void Client::setSector(const QString& sector) {
    this->sector = sector;
}

void Client::setContactInfo(const QString& contactInfo) {
    this->contactInfo = contactInfo;
}

void Client::setEmail(const QString& email) {
    this->email = email;
}

void Client::setConsultationDateTime(const QDateTime& consultationDateTime) {
    this->consultationDateTime = consultationDateTime;
}

void Client::setConsultant(int consultant) {
    this->consultant = consultant;
}

bool Client::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO Clients (CLIENT_ID, NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT) "
                  "VALUES (clients_seq.NEXTVAL, :name, :sector, :contactInfo, :email, TO_DATE(:consultationDateTime, 'YYYY-MM-DD HH24:MI:SS'), :consultant)");
    query.bindValue(":name", name);
    query.bindValue(":sector", sector);
    query.bindValue(":contactInfo", contactInfo);
    query.bindValue(":email", email);
    query.bindValue(":consultationDateTime", consultationDateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":consultant", consultant);

    if (!query.exec()) {
        qDebug() << "Error inserting client:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Client::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    
    // Add EMAIL to the query
    model->setQuery("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients");
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));  // Add this line
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    
    return model;
}

bool Client::removeByName(const QString& clientName) {
    QSqlQuery query;
    query.prepare("DELETE FROM Clients WHERE NAME = :clientName");
    query.bindValue(":clientName", clientName);

    if (!query.exec()) {
        qDebug() << "Error deleting client:" << query.lastError().text();
        return false;
    }
    return true;
}




bool Client::updateClient(const QString& originalName, const QString& newName, const QString& sector,
    const QString& contactInfo, const QString& email, const QDateTime& consultationDateTime, int consultant) {
    QSqlQuery query;
    query.prepare("UPDATE Clients SET NAME = :newName, SECTOR = :sector, "
                  "CONTACT_INFO = :contactInfo, EMAIL = :email, CONSULTATION_DATE = TO_DATE(:consultationDateTime, 'YYYY-MM-DD HH24:MI:SS'), "
                  "CONSULTANT = :consultant WHERE NAME = :originalName");
    query.bindValue(":newName", newName);
    query.bindValue(":sector", sector);
    query.bindValue(":contactInfo", contactInfo);
    query.bindValue(":email", email);
    query.bindValue(":consultationDateTime", consultationDateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":consultant", consultant);
    query.bindValue(":originalName", originalName);

    if (!query.exec()) {
        qDebug() << "Error updating client:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Client::searchByName(const QString& name) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE NAME LIKE :name");
    query.bindValue(":name", "%" + name + "%");

    if (!query.exec()) {
        qDebug() << "Error searching by name:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email")); // Added
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QSqlQueryModel* Client::searchBySector(const QString& sector) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE SECTOR LIKE :sector");
    query.bindValue(":sector", "%" + sector + "%"); // Using LIKE for partial matches

    if (!query.exec()) {
        qDebug() << "Error searching by sector:" << query.lastError().text();
        return model; // Return empty model
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email")); // Added
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QSqlQueryModel* Client::searchByDateTime(const QDateTime& dateTime) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE CONSULTATION_DATE = TO_DATE(:dateTime, 'YYYY-MM-DD HH24:MI:SS')");
    query.bindValue(":dateTime", dateTime.toString("yyyy-MM-dd HH:mm:ss"));

    // Debug info
    qDebug() << "Searching by date/time:" << dateTime.toString("yyyy-MM-dd HH:mm:ss");

    if (!query.exec()) {
        qDebug() << "Error searching by date and time:" << query.lastError().text();
        qDebug() << "Database error:" << query.lastError().databaseText();
        qDebug() << "Driver error:" << query.lastError().driverText();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email")); // Added
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QSqlQueryModel* Client::sortByColumn(int column, Qt::SortOrder order) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QStringList columnNames = {"NAME", "SECTOR", "CONTACT_INFO", "EMAIL", "CONSULTATION_DATE", "CONSULTANT"};
    if (column < 0 || column >= columnNames.size()) {
        qDebug() << "Invalid column index for sorting";
        return model;
    }
    QString sortOrder = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
    QString queryText = QString("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                                "ORDER BY %1 %2").arg(columnNames[column], sortOrder);

    if (!query.exec(queryText)) {
        qDebug() << "Error sorting by column:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email")); // Added
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QMap<QDateTime, int> Client::getConsultationCountsByDateTime() {
    QMap<QDateTime, int> countMap;
    QSqlQuery query;

    query.prepare("SELECT CONSULTATION_DATE, COUNT(*) FROM Clients GROUP BY CONSULTATION_DATE");

    if (query.exec()) {
        while (query.next()) {
            QDateTime dateTime = query.value(0).toDateTime();
            int count = query.value(1).toInt();
            countMap[dateTime] = count;
        }
    } else {
        qDebug() << "Get consultation counts query failed: " << query.lastError().text();
    }

    return countMap;
}

QMap<QDate, int> Client::getConsultationCountsByDate() {
    QMap<QDate, int> dateCountMap;
    QMap<QDateTime, int> dateTimeCountMap = getConsultationCountsByDateTime();

    // Convert QDateTime keys to QDate keys, combining counts for the same date
    for (auto it = dateTimeCountMap.begin(); it != dateTimeCountMap.end(); ++it) {
        QDate date = it.key().date();
        dateCountMap[date] += it.value();
    }

    return dateCountMap;
}


QSqlQueryModel* Client::getConsultationsForDate(const QDate& date) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QString startDateStr = date.toString("yyyy-MM-dd") + " 00:00:00";
    QString endDateStr = date.toString("yyyy-MM-dd") + " 23:59:59";
    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE CONSULTATION_DATE BETWEEN TO_DATE(:startDate, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_DATE(:endDate, 'YYYY-MM-DD HH24:MI:SS') "
                  "ORDER BY CONSULTATION_DATE");
    query.bindValue(":startDate", startDateStr);
    query.bindValue(":endDate", endDateStr);

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email")); // Added
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    } else {
        qDebug() << "Get consultations for date failed: " << query.lastError().text();
    }
    return model;
}

QSqlQueryModel* Client::searchByEmail(const QString &email) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE EMAIL LIKE :email");
    query.bindValue(":email", "%" + email + "%");

    if (!query.exec()) {
        qDebug() << "Error searching by email:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QSqlQueryModel* Client::searchByContact(const QString &contact) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE CONTACT_INFO LIKE :contact");
    query.bindValue(":contact", "%" + contact + "%");

    if (!query.exec()) {
        qDebug() << "Error searching by contact:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QSqlQueryModel* Client::searchByConsultant(int consultant) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE CONSULTANT = :consultant");
    query.bindValue(":consultant", consultant);

    if (!query.exec()) {
        qDebug() << "Error searching by consultant:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}

QSqlQueryModel* Client::getUpcomingConsultations(const QDateTime &start, const QDateTime &end)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT NAME, SECTOR, CONTACT_INFO, EMAIL, CONSULTATION_DATE, CONSULTANT FROM Clients "
                  "WHERE CONSULTATION_DATE BETWEEN TO_DATE(:start, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_DATE(:end, 'YYYY-MM-DD HH24:MI:SS')");
    query.bindValue(":start", start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":end", end.toString("yyyy-MM-dd HH:mm:ss"));

    if (!query.exec()) {
        qDebug() << "Error fetching upcoming consultations:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant"));
    return model;
}
