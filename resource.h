#ifndef RESOURCE_H
#define RESOURCE_H
#include <Qstring>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QByteArray>
#include <QSqlError>
#include <QDebug>

class Resource {
private:
    int resource_id;
    QString name;
    QString type;
    QString brand;
    int quantity;
    QDate purchase_date;
    QByteArray image;

    // New private method for logging
    //void logAction(const QString& action, const QString& employeeName);

public:

    Resource();
    Resource( QString& name, QString& type, QString& brand, int& quantity, QDate& purchaseDate, QByteArray& image);

    int getResourceId()const ;
    QString getName() const;
    QString getType() const;
    QString getBrand() const;
    int getQuantity()const;
    QDate getPurchaseDate() const;
    QByteArray getImage()const;


    void setResourceId(int id);
    void setName(const QString& name);
    void setType(const QString& type);
    void setBrand(const QString& brand);
    void setQuantity(const int& quantity);
    void setPurchaseDate(const QDate& purchaseDate);
    void setImage(const QByteArray& image);


    /*bool addResource();
    QSqlQueryModel* afficher();
    bool delet(int resource_id);
    bool updateResource();*/
    bool addResource(const QString& employeeName = "Unknown"); // Modified to include employeeName
    QSqlQueryModel* afficher();
    bool delet(int resource_id, const QString& employeeName = "Unknown"); // Modified
    bool updateResource(const QString& employeeName = "Unknown"); // Modified

   void logAction(const QString& action, const QString& employeeName);

};

#endif // RESOURCE_H
