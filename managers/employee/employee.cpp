//implémentation de la classe Employee
#include "Employee.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

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
    
    // Générer un nouvel ID pour l'employé
    int newId = 1; // Valeur par défaut si aucun employé n'existe
    QSqlQuery idQuery("SELECT MAX(ID) + 1 FROM EMPLOYEE");
    if (idQuery.next() && !idQuery.value(0).isNull()) {
        newId = idQuery.value(0).toInt();
    }
    
    query.prepare("INSERT INTO EMPLOYEE (ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE)"
                 "VALUES (:id, :cin, :lastName, :firstName, :dateOfBirth, :phoneNumber, :email, :gender, :salary, :dateOfHire, :field, :image, :role)");
    
    // Définir les valeurs de base avec l'ID généré
    query.bindValue(":id", newId);
    query.bindValue(":cin", cin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":dateOfBirth", dateOfBirth);
    query.bindValue(":phoneNumber", phoneNumber);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);
    query.bindValue(":salary", salary);
    query.bindValue(":dateOfHire", dateOfHire);
    query.bindValue(":field", field);
    query.bindValue(":role", role);
    
    // Traitement de l'image avec gestion d'erreur améliorée
    QByteArray imageData;
    if (!imagePath.isEmpty()) {
        QFile file(imagePath);
        if (file.open(QIODevice::ReadOnly)) {
            imageData = file.readAll();
            file.close();
            qDebug() << "Image chargée avec succès, taille:" << imageData.size() << "bytes";
        } else {
            qDebug() << "Impossible d'ouvrir le fichier image:" << imagePath << "- Erreur:" << file.errorString();
            // On continue avec une image vide
            imageData.clear();
        }
    } else {
        qDebug() << "Aucun chemin d'image fourni, utilisation d'une image vide";
    }
    
    // Assigner les données d'image, même si elles sont vides
    query.bindValue(":image", imageData);

    // Journalisation des valeurs pour débogage
    qDebug() << "ID: " << newId;
    qDebug() << "CIN: " << cin;
    qDebug() << "Last Name: " << lastName;
    qDebug() << "First Name: " << firstName;
    qDebug() << "Date of Birth: " << dateOfBirth.toString("yyyy-MM-dd");
    qDebug() << "Phone: " << phoneNumber;
    qDebug() << "Email: " << email;
    qDebug() << "Gender: " << gender;
    qDebug() << "Salary: " << salary;
    qDebug() << "Date of Hire: " << dateOfHire.toString("yyyy-MM-dd");
    qDebug() << "Field: " << field;
    qDebug() << "Role: " << role;
    qDebug() << "Image Path: " << imagePath;
    
    // Exécuter la requête et récupérer le résultat
    bool success = query.exec();
    
    // En cas d'échec, journaliser l'erreur complète pour faciliter le débogage
    if (!success) {
        qDebug() << "ERREUR AJOUT EMPLOYÉ:";
        qDebug() << "  - Message d'erreur: " << query.lastError().text();
        qDebug() << "  - Type d'erreur: " << query.lastError().type();
        qDebug() << "  - Code d'erreur: " << query.lastError().nativeErrorCode();
        qDebug() << "  - Requête: " << query.lastQuery();
    } else {
        qDebug() << "Employé ajouté avec succès!";
    }
    
    return success;
}
QSqlQueryModel *Employee::loadEmployees(){
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    
    // Requête avec tous les champs explicitement listés
    query.prepare("SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE, RFID_UID FROM EMPLOYEE");

    if (query.exec()) {
        model->setQuery(std::move(query));
        
        // Définir les en-têtes des colonnes
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
        qDebug() << "Error loading employee list:" << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery();
        delete model; // Clean up if query fails
        return nullptr; // Return nullptr on failure
    }

    return model;
}
bool Employee::updateEmployee(int id, QString cin, QString lastName, QString firstName, QDate dateOfBirth,
                              QString phoneNumber, QString email, QString gender, int salary, QDate dateOfHire,
                              QString field, QString imagePath, QString role) {
    QSqlQuery query;
    QByteArray imageData;
    
    // Si un nouveau chemin d'image est fourni, lire le fichier
    if (!imagePath.isEmpty()) {
        QFile file(imagePath);
        if (file.open(QIODevice::ReadOnly)) {
            imageData = file.readAll();
            file.close();
            
            // Prépare la requête avec mise à jour de l'image
            query.prepare("UPDATE EMPLOYEE SET CIN = :cin, LAST_NAME = :lastName, FIRST_NAME = :firstName, "
                         "DATE_BIRTH = :dateOfBirth, PHONE = :phoneNumber, EMAIL = :email, GENDER = :gender, "
                         "SALARY = :salary, DATE_HIRING = :dateOfHire, SPECIALITY = :field, "
                         "IMAGE = :image, ROLE = :role WHERE ID = :id");
            
            query.bindValue(":image", imageData);
        } else {
            qDebug() << "Failed to open image file for reading:" << imagePath;
            // En cas d'échec d'ouverture du fichier, ne pas mettre à jour l'image
            query.prepare("UPDATE EMPLOYEE SET CIN = :cin, LAST_NAME = :lastName, FIRST_NAME = :firstName, "
                         "DATE_BIRTH = :dateOfBirth, PHONE = :phoneNumber, EMAIL = :email, GENDER = :gender, "
                         "SALARY = :salary, DATE_HIRING = :dateOfHire, SPECIALITY = :field, "
                         "ROLE = :role WHERE ID = :id");
        }
    } else {
        // Si aucune nouvelle image n'est fournie, ne pas mettre à jour la colonne IMAGE
        query.prepare("UPDATE EMPLOYEE SET CIN = :cin, LAST_NAME = :lastName, FIRST_NAME = :firstName, "
                     "DATE_BIRTH = :dateOfBirth, PHONE = :phoneNumber, EMAIL = :email, GENDER = :gender, "
                     "SALARY = :salary, DATE_HIRING = :dateOfHire, SPECIALITY = :field, "
                     "ROLE = :role WHERE ID = :id");
    }
    
    query.bindValue(":id", id);
    query.bindValue(":cin", cin);
    query.bindValue(":lastName", lastName);
    query.bindValue(":firstName", firstName);
    query.bindValue(":dateOfBirth", dateOfBirth);
    query.bindValue(":phoneNumber", phoneNumber);
    query.bindValue(":email", email);
    query.bindValue(":gender", gender);
    query.bindValue(":salary", salary);
    query.bindValue(":dateOfHire", dateOfHire);
    query.bindValue(":field", field);
    query.bindValue(":role", role);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Mise à jour employé échouée:" << query.lastError().text();
    }
    return success;
}
bool Employee::deleteEmployee(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}
