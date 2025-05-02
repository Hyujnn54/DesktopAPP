#include "resource.h"
#include "qdir.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

Resource::Resource() : resource_id(0), name(""), type(""), brand(""), quantity(0), purchase_date(QDate::currentDate()), image(QByteArray()) {}

Resource::Resource(QString& name, QString& type, QString& brand, int& quantity, QDate& purchase_date, QByteArray& image)
    : resource_id(0), name(name), type(type), brand(brand), quantity(quantity), purchase_date(purchase_date), image(image) {}

int Resource::getResourceId() const { return resource_id; }
QString Resource::getName() const { return name; }
QString Resource::getType() const { return type; }
QString Resource::getBrand() const { return brand; }
int Resource::getQuantity() const { return quantity; }
QDate Resource::getPurchaseDate() const { return purchase_date; }
QByteArray Resource::getImage() const { return image; }

void Resource::setResourceId(const int id) { resource_id = id; }
void Resource::setName(const QString& name) { this->name = name; }
void Resource::setType(const QString& type) { this->type = type; }
void Resource::setBrand(const QString& brand) { this->brand = brand; }
void Resource::setQuantity(const int& quantity) { this->quantity = quantity; }
void Resource::setPurchaseDate(const QDate& purchaseDate) { this->purchase_date = purchaseDate; }
void Resource::setImage(const QByteArray& image) { this->image = image; }

void Resource::logAction(const QString& action, const QString& employeeName) {
    QFile file("resource_history.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        out << timestamp << " | Employee: " << employeeName << " | Action: " << action
            << " | Resource ID: " << resource_id << " | Name: " << name
            << " | Type: " << type << " | Brand: " << brand
            << " | Quantity: " << quantity << " | Purchase Date: " << purchase_date.toString("yyyy-MM-dd") << "\n";
        file.close();
    } else {
        qDebug() << "Failed to open history file for writing: " << file.errorString();
    }
}

bool Resource::addResource(const QString& employeeName) {
    QSqlQuery query;

    qDebug() << "Adding resource with values: ";
    qDebug() << "Name: " << name;
    qDebug() << "Type: " << type;
    qDebug() << "Brand: " << brand;
    qDebug() << "Quantity: " << quantity;
    qDebug() << "Purchase Date: " << purchase_date.toString("yyyy-MM-dd");
    qDebug() << "Image Size: " << image.size();

    // Step 1: Insert with EMPTY_BLOB() and retrieve RESOURCE_ID using RETURNING
    query.prepare("INSERT INTO RESOURCES (NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE) "
                  "VALUES (:name, :type, :brand, :quantity, TO_DATE(:purchase_date, 'YYYY-MM-DD'), EMPTY_BLOB()) "
                  "RETURNING RESOURCE_ID INTO :id");

    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":brand", brand);
    query.bindValue(":quantity", quantity);
    query.bindValue(":purchase_date", purchase_date.toString("yyyy-MM-dd"));
    query.bindValue(":id", 0, QSql::Out); // Bind as an output parameter

    if (query.exec()) {
        int id = query.boundValue(":id").toInt();
        resource_id = id; // Set the resource_id for logging
        qDebug() << "Inserted resource ID:" << id;

        // Step 2: Update the IMAGE column with the actual data if an image is provided
        if (id > 0 && !image.isEmpty()) {
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE RESOURCES SET IMAGE = :image WHERE RESOURCE_ID = :id");
            updateQuery.bindValue(":image", image);
            updateQuery.bindValue(":id", id);

            if (!updateQuery.exec()) {
                qDebug() << "Failed to update image: " << updateQuery.lastError().text();
                return false;
            }
            qDebug() << "Image updated successfully for resource ID:" << id;
        }

        // Log the action after successful addition
        logAction("Added", employeeName);
        qDebug() << "Resource added successfully!";
        return true;
    } else {
        qDebug() << "Error adding resource: " << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Resource::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT RESOURCE_ID, NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE FROM RESOURCES"); // Explicit columns

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error when displaying a resource:" << model->lastError().text();
    }

    return model;
}

bool Resource::delet(int resource_id, const QString& employeeName)
{
    QSqlQuery query;

    // Step 1: Retrieve current data for logging before deletion
    query.prepare("SELECT NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE FROM RESOURCES WHERE RESOURCE_ID = :resource_id");
    query.bindValue(":resource_id", resource_id);
    if (query.exec() && query.next()) {
        this->resource_id = resource_id; // Set the ID
        name = query.value("NAME").toString();
        type = query.value("TYPE").toString();
        brand = query.value("BRAND").toString();
        quantity = query.value("QUANTITY").toInt();
        purchase_date = query.value("PURCHASE_DATE").toDate();
        image = query.value("IMAGE").toByteArray();
    } else {
        qDebug() << "Failed to retrieve resource data for logging: " << query.lastError().text();
    }

    // Step 2: Perform the deletion
    query.prepare("DELETE FROM RESOURCES WHERE RESOURCE_ID = :resource_id");
    query.bindValue(":resource_id", resource_id);

    bool success = query.exec();

    if (success) {
        logAction("Deleted", employeeName); // Log the action after successful deletion
        qDebug() << "Resource deleted successfully!";
    } else {
        qDebug() << "SQL Error when deleting a resource:" << query.lastError().text();
    }

    return success;
}

bool Resource::updateResource(const QString& employeeName)
{
    if (resource_id <= 0) {
        qDebug() << "Error: resource_id is not set for update.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE RESOURCES SET NAME = :name, TYPE = :type, BRAND = :brand, "
                  "QUANTITY = :quantity, PURCHASE_DATE = TO_DATE(:purchase_date, 'YYYY-MM-DD'), "
                  "IMAGE = :image WHERE RESOURCE_ID = :resource_id");

    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":brand", brand);
    query.bindValue(":quantity", quantity);
    query.bindValue(":purchase_date", purchase_date.toString("yyyy-MM-dd"));
    query.bindValue(":image", image);
    query.bindValue(":resource_id", resource_id);

    bool success = query.exec();

    if (success) {
        logAction("Updated", employeeName); // Log the action after successful update
        qDebug() << "Resource updated successfully!";
    } else {
        qDebug() << "Error updating resource: " << query.lastError().text();
    }

    return success;
}