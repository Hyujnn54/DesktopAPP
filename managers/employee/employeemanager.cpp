#include "employeemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>

EmployeeManager::EmployeeManager(QObject *parent)
    : QObject(parent)
    , employee(new Employee())
{
}

EmployeeManager::~EmployeeManager()
{
    delete employee;
}

void EmployeeManager::initialize(QWidget* parent)
{
    Q_UNUSED(parent);
    qDebug() << "Initialisation du gestionnaire d'employés";
    
    // Vérifier si la table existe et contient des données
    QSqlQuery checkQuery("SELECT COUNT(*) FROM EMPLOYEE");
    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "Table EMPLOYEE contient" << checkQuery.value(0).toInt() << "enregistrements";
    } else {
        qDebug() << "Table EMPLOYEE vide ou inexistante";
    }
}

bool EmployeeManager::addEmployee(const QString& cin, const QString& lastName, const QString& firstName, 
                                const QDate& dateOfBirth, const QString& phoneNumber, const QString& email, 
                                const QString& gender, int salary, const QDate& dateOfHire, 
                                const QString& field, const QString& imagePath, const QString& role)
{
    // Copier les valeurs dans l'objet employee
    employee->setCIN(cin);
    employee->setLastName(lastName);
    employee->setFirstName(firstName);
    employee->setDateOfBirth(dateOfBirth);
    employee->setPhoneNumber(phoneNumber);
    employee->setEmail(email);
    employee->setGender(gender);
    employee->setSalary(salary);
    employee->setDateOfHire(dateOfHire);
    employee->setField(field);
    employee->setImagePath(imagePath);
    employee->setRole(role);
    
    return employee->ajouter();
}

bool EmployeeManager::updateEmployee(int id, const QString& cin, const QString& lastName, const QString& firstName, 
                                  const QDate& dateOfBirth, const QString& phoneNumber, const QString& email, 
                                  const QString& gender, int salary, const QDate& dateOfHire, 
                                  const QString& field, const QString& imagePath, const QString& role)
{
    return employee->updateEmployee(id, cin, lastName, firstName, dateOfBirth, phoneNumber, email, 
                                    gender, salary, dateOfHire, field, imagePath, role);
}

bool EmployeeManager::deleteEmployee(int id)
{
    return employee->deleteEmployee(id);
}

QSqlQueryModel* EmployeeManager::getAllEmployees()
{
    return employee->loadEmployees();
}

QSqlQueryModel* EmployeeManager::searchEmployees(const QString& criteria, const QString& searchText)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE, RFID_UID FROM EMPLOYEE";
    bool hasFilter = !searchText.isEmpty();

    if (hasFilter) {
        if (criteria == "Full name") {
            queryStr += " WHERE UPPER(TRIM(LAST_NAME) || ' ' || TRIM(FIRST_NAME)) LIKE UPPER(:searchText)";
        } else if (criteria == "CIN") {
            queryStr += " WHERE UPPER(CIN) LIKE UPPER(:searchText)";
        } else if (criteria == "Specialty") {
            queryStr += " WHERE UPPER(SPECIALITY) LIKE UPPER(:searchText)";
        }
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (hasFilter) {
        query.bindValue(":searchText", "%" + searchText + "%");
    }

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("First Name"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date of Birth"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Phone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Gender"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Salary"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date of Hiring"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Specialty"));
        model->setHeaderData(11, Qt::Horizontal, QObject::tr("Image"));
        model->setHeaderData(12, Qt::Horizontal, QObject::tr("Role"));
        model->setHeaderData(13, Qt::Horizontal, QObject::tr("RFID UID"));
    } else {
        qDebug() << "Search error:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

QSqlQueryModel* EmployeeManager::sortEmployees(int column, Qt::SortOrder order)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    // Map column index to column name based on table structure
    QString columnName;
    switch (column) {
        case 0: columnName = "ID"; break;
        case 1: columnName = "CIN"; break;
        case 2: columnName = "LAST_NAME"; break;
        case 3: columnName = "FIRST_NAME"; break;
        case 4: columnName = "DATE_BIRTH"; break;
        case 5: columnName = "PHONE"; break;
        case 6: columnName = "EMAIL"; break;
        case 7: columnName = "GENDER"; break;
        case 8: columnName = "SALARY"; break;
        case 9: columnName = "DATE_HIRING"; break;
        case 10: columnName = "SPECIALITY"; break;
        case 11: columnName = "IMAGE"; break; // Ajout du tri par image (peu utile mais pour être complet)
        case 12: columnName = "ROLE"; break; // Ajout du tri par rôle
        case 13: columnName = "RFID_UID"; break; // Ajout du tri par RFID UID
        default: columnName = "ID"; break; // Default to ID if invalid column
    }
    
    QString queryStr = "SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE, RFID_UID FROM EMPLOYEE";
    
    // Déterminer l'ordre de tri
    QString orderDirection = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
    
    // Add ORDER BY clause
    // Handle date columns with Oracle's TO_CHAR for sorting
    if (column == 4 || column == 9) { // DATE_BIRTH or DATE_HIRING
        queryStr += QString(" ORDER BY TO_CHAR(%1, 'YYYY-MM-DD') %2").arg(columnName, orderDirection);
    } else if (column == 8) { // SALARY - tri numérique
        queryStr += QString(" ORDER BY TO_NUMBER(%1) %2").arg(columnName, orderDirection);
    } else if (column == 11) { // IMAGE - on ne peut pas vraiment trier par image, donc on maintient l'ordre par ID
        queryStr += QString(" ORDER BY ID %1").arg(orderDirection);
    } else {
        queryStr += QString(" ORDER BY %1 %2").arg(columnName, orderDirection);
    }
    
    QSqlQuery query;
    if (query.exec(queryStr)) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("First Name"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date of Birth"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Phone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Gender"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Salary"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date of Hiring"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Specialty"));
        model->setHeaderData(11, Qt::Horizontal, QObject::tr("Image"));
        model->setHeaderData(12, Qt::Horizontal, QObject::tr("Role"));
        model->setHeaderData(13, Qt::Horizontal, QObject::tr("RFID UID"));
    } else {
        qDebug() << "Sort error:" << query.lastError().text();
        qDebug() << "Query used:" << queryStr;
        delete model;
        return nullptr;
    }
    
    return model;
}

QMap<QString, int> EmployeeManager::getEmployeeCountByRole()
{
    QMap<QString, int> roleCounts;
    QSqlQuery query("SELECT ROLE, COUNT(*) as count FROM EMPLOYEE GROUP BY ROLE");
    
    while (query.next()) {
        QString role = query.value("ROLE").toString();
        int count = query.value("count").toInt();
        if (!role.isEmpty()) {
            roleCounts[role] = count;
        }
    }
    
    return roleCounts;
}

QMap<QString, int> EmployeeManager::getEmployeeCountBySpecialty()
{
    QMap<QString, int> specialtyCounts;
    QSqlQuery query("SELECT SPECIALITY, COUNT(*) as count FROM EMPLOYEE GROUP BY SPECIALITY");
    
    while (query.next()) {
        QString specialty = query.value("SPECIALITY").toString();
        if (specialty.isEmpty()) specialty = "Unknown";
        int count = query.value("count").toInt();
        specialtyCounts[specialty] = count;
    }
    
    return specialtyCounts;
}
