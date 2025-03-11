#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include<Qstring>
#include<QDate>
#include<QSqlQuery>
#include<QSqlQueryModel>
    class Employee {
    public:
        Employee();
        Employee(QString& cin,QString& lastName,QString& firstName, QDate& dateOfBirth,
                 QString& phoneNumber, QString& email,  QString& gender,
                int& salary, QDate& dateOfHire, QString& field, QString& imagePath, QString& role);
        int getId() const;
        QString getCIN() const;
        QString getLastName() const;
        QString getFirstName() const;
        QDate getDateOfBirth() const;
        QString getPhoneNumber() const;
        QString getEmail() const;
        QString getGender() const;
        int getSalary() const;
        QDate getDateOfHire() const;
        QString getField() const;
        QString getImagePath() const;
        QString getRole() const;


        void setCIN(const QString& cin);
        void setLastName(const QString& lastName);
        void setFirstName(const QString& firstName);
        void setDateOfBirth(const QDate& dateOfBirth);
        void setPhoneNumber(const QString& phoneNumber);
        void setEmail(const QString& email);
        void setGender(const QString& gender);
        void setSalary(int salary);
        void setDateOfHire(const QDate& dateOfHire);
        void setField(const QString& field);
        void setImagePath(const QString& imagePath);
        void setRole(const QString& role);
        bool ajouter();
        QSqlQueryModel* loadEmployees();
        bool updateEmployee(int id, QString cin, QString lastName, QString firstName, QDate dateBirth,
                            QString phone, QString email, QString gender, int salary, QDate dateHiring, QString specialty, QString imagePath, QString role);

        bool deleteEmployee(int id);

    private:
        int id;
        QString cin;
        QString lastName;
        QString firstName;
        QDate dateOfBirth;
        QString phoneNumber;
        QString email;
        QString gender;
        int salary;
        QDate dateOfHire;
        QString field;
        QString imagePath;
        QString role;
    };

#endif
