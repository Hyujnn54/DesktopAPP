#include "Client.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

// Default constructor
Client::Client()
    : clientId(0), name(""), sector(""), contactInfo(""), email(""),
    consultationDateTime(QDateTime::currentDateTime()), consultantId("") {}

// Updated parameterized constructor
Client::Client(const QString& name, const QString& sector,
               const QString& contactInfo, const QString& email,
               const QDateTime& consultationDateTime, const QString& consultant)
    : clientId(1),
    name(name),
    sector(sector),
    contactInfo(contactInfo),
    email(email),
    consultationDateTime(consultationDateTime),
    consultantId(consultant) {}

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

QString Client::getConsultant() const {
    return consultantId;
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

void Client::setConsultant(const QString& consultant) {
    this->consultantId = consultant;
}

QPair<bool, QString> Client::ajouter(QString name, QString sector, QString contact, QString email, QDateTime dateTime, QString consultant) {
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.CLIENTS (CLIENT_ID, NAME, SECTOR, CONTACT_INFO, CONSULTATION_DATE, EMAIL, CONSULTANT_ID) "
                  "VALUES (CLIENTS_SEQ.NEXTVAL, :name, :sector, :contact_info, TO_TIMESTAMP(:consultation_timestamp, 'YYYY-MM-DD HH24:MI:SS'), :email, :consultant_id)");
    query.bindValue(":name", name);
    query.bindValue(":sector", sector);
    query.bindValue(":contact_info", contact);
    query.bindValue(":consultation_timestamp", dateTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":email", email);
    query.bindValue(":consultant_id", consultant.toInt());

    qDebug() << "Inserting client with values:"
             << "Name:" << name
             << "Sector:" << sector
             << "Contact Info:" << contact
             << "Consultation Timestamp:" << dateTime.toString()
             << "Email:" << email
             << "Consultant ID:" << consultant;

    if (!query.exec()) {
        QString error = query.lastError().text();
        qDebug() << "Error inserting client:" << error;
        return qMakePair(false, error);
    }
    qDebug() << "Client inserted successfully.";
    return qMakePair(true, QString("Client added successfully"));
}

QSqlQueryModel* Client::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID");
    if (!query.exec()) {
        qDebug() << "Error in Client::afficher():" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));

    if (model->lastError().isValid()) {
        qDebug() << "Model error in Client::afficher():" << model->lastError().text();
    }

    qDebug() << "Client::afficher() rowCount:" << model->rowCount();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));

    return model;
}

bool Client::updateClient(const QString& originalName, const QString& newName, const QString& sector,
                          const QString& contactInfo, const QString& email, const QDateTime& consultationDateTime,
                          const QString& consultant) {
    QSqlQuery query;
    query.prepare("UPDATE AHMED.CLIENTS SET NAME = :newName, SECTOR = :sector, "
                  "CONTACT_INFO = :contactInfo, EMAIL = :email, CONSULTATION_DATE = TO_TIMESTAMP(:consultationDateTime, 'YYYY-MM-DD HH24:MI:SS'), "
                  "CONSULTANT_ID = :consultant WHERE NAME = :originalName");
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
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.NAME LIKE :name");
    query.bindValue(":name", "%" + name + "%");

    if (!query.exec()) {
        qDebug() << "Error searching by name:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QSqlQueryModel* Client::searchBySector(const QString& sector) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.SECTOR LIKE :sector");
    query.bindValue(":sector", "%" + sector + "%");

    if (!query.exec()) {
        qDebug() << "Error searching by sector:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QSqlQueryModel* Client::searchByDateTime(const QDateTime& dateTime) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.CONSULTATION_DATE = TO_TIMESTAMP(:dateTime, 'YYYY-MM-DD HH24:MI:SS')");
    query.bindValue(":dateTime", dateTime.toString("yyyy-MM-dd HH:mm:ss"));

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
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QSqlQueryModel* Client::sortByColumn(int column, Qt::SortOrder order) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QStringList columnNames = {"c.NAME", "c.SECTOR", "c.CONTACT_INFO", "c.EMAIL", "c.CONSULTATION_DATE", "e.FIRST_NAME || ' ' || e.LAST_NAME"};
    if (column < 0 || column >= columnNames.size()) {
        qDebug() << "Invalid column index for sorting";
        return model;
    }
    QString sortOrder = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
    QString queryText = QString("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                                "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                                "FROM AHMED.CLIENTS c "
                                "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                                "ORDER BY %1 %2").arg(columnNames[column], sortOrder);

    if (!query.exec(queryText)) {
        qDebug() << "Error sorting by column:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QMap<QDateTime, int> Client::getConsultationCountsByDateTime() {
    QMap<QDateTime, int> countMap;
    QSqlQuery query;

    query.prepare("SELECT CONSULTATION_DATE, COUNT(*) FROM AHMED.CLIENTS GROUP BY CONSULTATION_DATE");

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
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.CONSULTATION_DATE BETWEEN TO_TIMESTAMP(:startDate, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_TIMESTAMP(:endDate, 'YYYY-MM-DD HH24:MI:SS') "
                  "ORDER BY c.CONSULTATION_DATE");
    query.bindValue(":startDate", startDateStr);
    query.bindValue(":endDate", endDateStr);

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Sector"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date & Time"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    } else {
        qDebug() << "Get consultations for date failed: " << query.lastError().text();
    }
    return model;
}

QSqlQueryModel* Client::searchByEmail(const QString &email) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.EMAIL LIKE :email");
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
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QSqlQueryModel* Client::searchByContact(const QString &contact) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.CONTACT_INFO LIKE :contact");
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
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QSqlQueryModel* Client::searchByConsultant(const QString &consultant) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.CONSULTANT_ID = :consultant");
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
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

QSqlQueryModel* Client::getUpcomingConsultations(const QDateTime &start, const QDateTime &end) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT c.NAME, c.SECTOR, c.CONTACT_INFO, c.EMAIL, c.CONSULTATION_DATE, "
                  "e.FIRST_NAME || ' ' || e.LAST_NAME AS CONSULTANT_NAME "
                  "FROM AHMED.CLIENTS c "
                  "LEFT JOIN AHMED.EMPLOYEE e ON c.CONSULTANT_ID = e.ID "
                  "WHERE c.CONSULTATION_DATE BETWEEN TO_TIMESTAMP(:start, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_TIMESTAMP(:end, 'YYYY-MM-DD HH24:MI:SS')");
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
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Consultant Name"));
    return model;
}

int Client::getTotalConsultations(const QDateTime &start, const QDateTime &end) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM AHMED.CLIENTS "
                  "WHERE CONSULTATION_DATE BETWEEN TO_TIMESTAMP(:start, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_TIMESTAMP(:end, 'YYYY-MM-DD HH24:MI:SS')");
    query.bindValue(":start", start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":end", end.toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    qDebug() << "Error fetching total consultations:" << query.lastError().text();
    return 0;
}

int Client::getUpcomingConsultationsCount(const QDateTime &start, const QDateTime &end) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM AHMED.CLIENTS "
                  "WHERE CONSULTATION_DATE BETWEEN TO_TIMESTAMP(:start, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_TIMESTAMP(:end, 'YYYY-MM-DD HH24:MI:SS')");
    query.bindValue(":start", start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":end", end.toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    qDebug() << "Error fetching upcoming consultations count:" << query.lastError().text();
    return 0;
}

QMap<QDate, int> Client::getConsultationsPerDay(const QDateTime &start, const QDateTime &end) {
    QMap<QDate, int> result;
    QSqlQuery query;
    query.prepare("SELECT TRUNC(CONSULTATION_DATE), COUNT(*) FROM AHMED.CLIENTS "
                  "WHERE CONSULTATION_DATE BETWEEN TO_TIMESTAMP(:start, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_TIMESTAMP(:end, 'YYYY-MM-DD HH24:MI:SS') "
                  "GROUP BY TRUNC(CONSULTATION_DATE)");
    query.bindValue(":start", start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":end", end.toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec()) {
        while (query.next()) {
            QDate date = query.value(0).toDate();
            int count = query.value(1).toInt();
            result[date] = count;
        }
    } else {
        qDebug() << "Error fetching consultations per day:" << query.lastError().text();
    }
    return result;
}

int Client::getUniqueClients(const QDateTime &start, const QDateTime &end) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(DISTINCT NAME) FROM AHMED.CLIENTS "
                  "WHERE CONSULTATION_DATE BETWEEN TO_TIMESTAMP(:start, 'YYYY-MM-DD HH24:MI:SS') "
                  "AND TO_TIMESTAMP(:end, 'YYYY-MM-DD HH24:MI:SS')");
    query.bindValue(":start", start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":end", end.toString("yyyy-MM-dd HH:mm:ss"));

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    qDebug() << "Error fetching unique clients:" << query.lastError().text();
    return 0;
}

bool Client::removeByName(const QString &name) {
    QSqlQuery query;
    query.prepare("DELETE FROM AHMED.CLIENTS WHERE NAME = :name");
    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "Error removing client by name:" << query.lastError().text();
        return false;
    }

    qDebug() << "Client removed successfully:" << name;
    return true;
}
