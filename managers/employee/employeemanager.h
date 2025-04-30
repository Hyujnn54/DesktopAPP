#ifndef EMPLOYEEMANAGER_H
#define EMPLOYEEMANAGER_H

#include <QObject>
#include <QSqlQueryModel>
#include "employee.h"

class QWidget;

class EmployeeManager : public QObject
{
    Q_OBJECT
public:
    explicit EmployeeManager(QObject *parent = nullptr);
    ~EmployeeManager();

    // Méthode d'initialisation
    void initialize(QWidget* parent);

    // Méthodes de base CRUD
    bool addEmployee(const QString& cin, const QString& lastName, const QString& firstName, 
                    const QDate& dateOfBirth, const QString& phoneNumber, const QString& email, 
                    const QString& gender, int salary, const QDate& dateOfHire, 
                    const QString& field, const QString& imagePath, const QString& role);
    
    bool updateEmployee(int id, const QString& cin, const QString& lastName, const QString& firstName, 
                      const QDate& dateOfBirth, const QString& phoneNumber, const QString& email, 
                      const QString& gender, int salary, const QDate& dateOfHire, 
                      const QString& field, const QString& imagePath, const QString& role);
    
    bool deleteEmployee(int id);
    
    // Méthodes de recherche et d'affichage
    QSqlQueryModel* getAllEmployees();
    QSqlQueryModel* searchEmployees(const QString& criteria, const QString& searchText);
    QSqlQueryModel* sortEmployees(int column, Qt::SortOrder order);
    
    // Statistiques
    QMap<QString, int> getEmployeeCountByRole();
    QMap<QString, int> getEmployeeCountBySpecialty();
    QMap<QString, int> getEmployeeCountByGender();
    QMap<QString, int> getEmployeeCountByCategory(const QString& category);
    double getAverageSalary();
    int getTotalEmployeeCount();


private:
    Employee *employee;
};

#endif // EMPLOYEEMANAGER_H
