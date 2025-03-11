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
    dateOfHire(QDate::currentDate()), field(""), imagePath(""), role("Employee") {}

Employee::Employee(QString& cin, QString& lastName, QString& firstName, QDate& dateOfBirth,
                   QString& phoneNumber, QString& email, QString& gender,
                   int& salary, QDate& dateOfHire, QString& field, QString& imagePath, QString& role)
    : cin(cin), lastName(lastName), firstName(firstName), dateOfBirth(dateOfBirth),
    phoneNumber(phoneNumber), email(email), gender(gender),
    salary(salary), dateOfHire(dateOfHire), field(field), imagePath(imagePath), role(role) {}

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
QString Employee::getImagePath() const {
    return imagePath;
}
QString Employee::getRole() const {
    return role;
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
void Employee::setImagePath(const QString& imagePath){
    this->imagePath = imagePath;
}
void Employee::setRole(const QString& role) {
    this->role = role;
}
bool Employee::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEE (CIN, LAST_NAME, FIRST_NAME,DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY,DATE_HIRING, SPECIALITY,IMAGE,ROLE)"
    "VALUES (:cin,:lastName, :firstName,:dateOfBirth, :phoneNumber, :email, :gender, :salary,:dateOfHire, :field, :imagePath, :role)");

    query.bindValue(":cin", cin);
    query.bindValue(":lastName",lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":dateOfBirth", dateOfBirth.toString("dd/MM/yyyy"));
    query.bindValue(":phoneNumber", phoneNumber);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);
    query.bindValue(":salary", salary);
    query.bindValue(":dateOfHire", dateOfHire.toString("dd/MM/yyyy"));
    query.bindValue(":field", field);
    query.bindValue(":imagePath", imagePath);
    query.bindValue(":role", role);

    return query.exec(); // Execute query
}
QSqlQueryModel *Employee::loadEmployees(){
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY,ROLE FROM EMPLOYEE");

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last Name")); // Fixed order
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("First Name"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date of Birth"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Phone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Gender"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Salary"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Date of Hiring"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Speciality"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Role"));
    } else {
        qDebug() << "Error loading employee list:" << query.lastError().text();
        delete model; // Clean up if query fails
        return nullptr; // Return nullptr on failure
    }

    return model;

}
bool Employee::updateEmployee(int id, QString cin, QString lastName, QString firstName, QDate dateBirth,
                              QString phone, QString email, QString gender, int salary, QDate dateHiring,
                              QString specialty, QString imagePath, QString role) {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEE SET CIN = :cin, LAST_NAME = :lastName, FIRST_NAME = :firstName, "
                  "DATE_BIRTH = :dateBirth, PHONE = :phone, EMAIL = :email, GENDER = :gender, "
                  "SALARY = :salary, DATE_HIRING = :dateHiring, SPECIALITY = :specialty, "
                  "IMAGE = :imagePath, ROLE = :role WHERE ID = :id");

    query.bindValue(":id", id);
    query.bindValue(":cin", cin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":dateOfBirth", dateOfBirth.toString("dd/MM/yyyy"));
    query.bindValue(":phone", phone);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);
    query.bindValue(":salary", salary);
    query.bindValue(":dateOfHire", dateOfHire.toString("dd/MM/yyyy"));
    query.bindValue(":specialty", specialty);
    query.bindValue(":imagePath", imagePath);
    query.bindValue(":role", role);

    return query.exec();
}
bool Employee::deleteEmployee(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}
