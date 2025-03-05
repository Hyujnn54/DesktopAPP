//write the code for the employee class here
#include "employee.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>
Employee::Employee()
    : cin(""), lastName(""), firstName(""), dateOfBirth(QDate::currentDate()),
    phoneNumber(""), email(""), gender("Homme"), salary(0),
    dateOfHire(QDate::currentDate()), field("") {}

Employee::Employee(QString& cin, QString& lastName, QString& firstName, QDate& dateOfBirth,
                    QString& phoneNumber, QString& email, QString& gender,
                   int& salary, QDate& dateOfHire, QString& field)
    : cin(cin), lastName(lastName), firstName(firstName), dateOfBirth(dateOfBirth),
    phoneNumber(phoneNumber), email(email), gender(gender),
    salary(salary), dateOfHire(dateOfHire),field(field) {}

// Getters
int Employee::getId() const {
    return id;
}
QString Employee::getCIN() const {
    return cin;
}
QString Employee::getLastName() const {
    return lastName;
}

QString Employee::getFirstName() const {
    return firstName;
}

QDate Employee::getDateOfBirth() const {
    return dateOfBirth;
}

QString Employee::getPhoneNumber() const {
    return phoneNumber;
}

QString Employee::getEmail() const {
    return email;
}

QString Employee::getGender() const {
    return gender;
}

int Employee::getSalary() const {
    return salary;
}

QDate Employee::getDateOfHire() const {
    return dateOfHire;
}
QString Employee::getField() const {
    return field;
}

// Setters
void Employee::setCIN(const QString& cin) {
    this->cin = cin;
}
void Employee::setLastName(const QString& lastName) {
    this->lastName = lastName;
}

void Employee::setFirstName(const QString& firstName) {
    this->firstName = firstName;
}

void Employee::setDateOfBirth(const QDate& dateOfBirth) {
    this->dateOfBirth = dateOfBirth;
}

void Employee::setPhoneNumber(const QString& phoneNumber) {
    this->phoneNumber = phoneNumber;
}

void Employee::setEmail(const QString& email) {
    this->email = email;
}

void Employee::setGender(const QString& gender) {
    this->gender = gender;
}

void Employee::setSalary(int salary) {
    this->salary = salary;
}

void Employee::setDateOfHire(const QDate& dateOfHire) {
    this->dateOfHire = dateOfHire;
}
void Employee::setField(const QString& field) {
    this->field = field;
}
bool Employee::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEE (CIN, LAST_NAME, FIRST_NAME,DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY,DATE_HIRING, SPECIALITY)"
    "VALUES (:cin,:lastName, :firstName,:dateOfBirth, :phoneNumber, :email, :gender, :salary,:dateOfHire, :field)");

    query.bindValue(":cin", cin);
    query.bindValue(":lastName",lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":dateOfBirth", dateOfBirth);
    query.bindValue(":phoneNumber", phoneNumber);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);
    query.bindValue(":salary", salary);
    query.bindValue(":dateOfHire", dateOfHire);
    query.bindValue(":field", field);

    return query.exec(); // Execute query
}
QSqlQueryModel *Employee::loadEmployees(){
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID, CIN, LAST_NAME, FIRST_NAME,DATE_BIRTH,PHONE, EMAIL, GENDER, SALARY,DATE_HIRING, SPECIALITY FROM employees");

    if (!query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("First_Name"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Last_Name"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date_Birth"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Phone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Gender"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Salary"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date_Hiring"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Speciality"));
    } else {
        qDebug() << "Error loading employee list:" << query.lastError().text();
    }

    return model;

}
bool Employee::updateEmployee(int id, QString cin, QString lastName, QString firstName, QDate dateBirth,
                              QString phone, QString email, QString gender, int salary, QDate dateHiring, QString specialty) {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET CIN = :cin, LAST_NAME = :lastName, FIRST_NAME = :firstName, "
                  "DATE_BIRTH = :dateBirth, PHONE = :phone, EMAIL = :email, GENDER = :gender, "
                  "SALARY = :salary, DATE_HIRING = :dateHiring, SPECIALITY = :specialty WHERE ID = :id");

    query.bindValue(":id", id);
    query.bindValue(":cin", cin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":dateBirth", dateBirth);
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);
    query.bindValue(":salary", salary);
    query.bindValue(":dateHiring", dateHiring);
    query.bindValue(":specialty", specialty);

    return query.exec();  // Returns true if successful, false otherwise
}
bool Employee::deleteEmployee(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}
