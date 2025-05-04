#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QDebug>
#include <QSqlError>
#include "managers/resources/resource.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog),
    serialPort(new QSerialPort(this))
{
    ui->setupUi(this);

    // Configure serial port
    bool portFound = false;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        if (info.description().contains("Arduino", Qt::CaseInsensitive)) {
            serialPort->setPortName(info.portName());
            serialPort->setBaudRate(QSerialPort::Baud9600);
            if (serialPort->open(QIODevice::ReadWrite)) {
                qDebug() << "Connected to Arduino on" << info.portName();
                portFound = true;
            } else {
                qDebug() << "Failed to open serial port:" << serialPort->errorString();
                QMessageBox::warning(this, "Serial Error", "Failed to connect to Arduino: " + serialPort->errorString());
            }
            break;
        }
    }
    if (!portFound) {
        qDebug() << "No Arduino found. Running in simulation mode.";
        ui->lblStatus->setText("No Arduino detected. Commands will be logged for simulation.");
    }
}

SearchDialog::~SearchDialog()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete ui;
}

void SearchDialog::on_btnSearch_clicked()
{
    QString resourceName = ui->txtResourceName->text().trimmed();
    if (resourceName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a resource name.");
        return;
    }

    int resourceId = -1;
    Resource resource;
    bool resourceExists = queryDatabase(resourceName, resourceId, resource);

    if (resourceExists) {
        int currentQuantity = resource.getQuantity();
        if (currentQuantity <= 0) {
            QMessageBox::warning(this, "Quantity Error", "Resource '" + resourceName + "' has zero quantity and cannot be reduced further.");
            ui->lblStatus->setText("Resource '" + resourceName + "' has zero quantity.");
            sendToArduino(false);
            return;
        }

        QString employeeName = "DefaultEmployee"; // Replace with dynamic employee name if available
        if (updateResourceQuantity(resourceId, currentQuantity, employeeName)) {
            ui->lblStatus->setText("Resource '" + resourceName + "' found! Quantity reduced by 1.");
            QMessageBox::information(this, "Search Result", "Resource '" + resourceName + "' found! Quantity reduced to " + QString::number(currentQuantity - 1) + ".");
            sendToArduino(true);
            emit resourceUpdated(); // Emit signal to refresh table
        } else {
            ui->lblStatus->setText("Failed to update quantity for '" + resourceName + "'.");
            QMessageBox::warning(this, "Database Error", "Failed to update resource quantity.");
            sendToArduino(false);
        }
    } else {
        ui->lblStatus->setText("Resource '" + resourceName + "' does not exist.");
        QMessageBox::information(this, "Search Result", "Resource '" + resourceName + "' does not exist.");
        sendToArduino(false);
    }
}

bool SearchDialog::queryDatabase(const QString &resourceName, int &resourceId, Resource &resource)
{
    QSqlQuery query;
    query.prepare("SELECT RESOURCE_ID, NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE "
                  "FROM RESOURCES WHERE UPPER(NAME) = UPPER(:name)");
    query.bindValue(":name", resourceName);
    if (query.exec() && query.next()) {
        resourceId = query.value("RESOURCE_ID").toInt();
        resource.setResourceId(resourceId);
        resource.setName(query.value("NAME").toString());
        resource.setType(query.value("TYPE").toString());
        resource.setBrand(query.value("BRAND").toString());
        resource.setQuantity(query.value("QUANTITY").toInt());
        resource.setPurchaseDate(query.value("PURCHASE_DATE").toDate());
        resource.setImage(query.value("IMAGE").toByteArray());
        qDebug() << "Database query result: Resource" << resourceName << "exists with ID" << resourceId;
        return true;
    } else {
        if (query.lastError().isValid()) {
            qDebug() << "Database query error:" << query.lastError().text();
            QMessageBox::warning(this, "Database Error", "Failed to query database: " + query.lastError().text());
        } else {
            qDebug() << "Resource" << resourceName << "does not exist.";
        }
        return false;
    }
}

bool SearchDialog::updateResourceQuantity(int resourceId, int currentQuantity, const QString &employeeName)
{
    Resource resource;
    resource.setResourceId(resourceId);
    resource.setQuantity(currentQuantity - 1);

    QSqlQuery query;
    query.prepare("SELECT NAME, TYPE, BRAND, PURCHASE_DATE, IMAGE FROM RESOURCES WHERE RESOURCE_ID = :id");
    query.bindValue(":id", resourceId);
    if (query.exec() && query.next()) {
        resource.setName(query.value("NAME").toString());
        resource.setType(query.value("TYPE").toString());
        resource.setBrand(query.value("BRAND").toString());
        resource.setPurchaseDate(query.value("PURCHASE_DATE").toDate());
        resource.setImage(query.value("IMAGE").toByteArray());
    } else {
        qDebug() << "Failed to retrieve resource data for update:" << query.lastError().text();
        return false;
    }

    bool success = resource.updateResource(employeeName);
    if (success) {
        qDebug() << "Resource quantity updated to" << (currentQuantity - 1) << "by" << employeeName;
    } else {
        qDebug() << "Failed to update resource quantity:" << query.lastError().text();
    }
    return success;
}

void SearchDialog::sendToArduino(bool resourceExists)
{
    if (serialPort->isOpen()) {
        QByteArray data = resourceExists ? "1" : "2";
        serialPort->write(data);
        serialPort->flush();
        qDebug() << "Sent to Arduino:" << data;
    } else {
        QString command = resourceExists ? "1" : "2";
        qDebug() << "Simulated Arduino command:" << command;
        ui->lblStatus->setText("Simulated command: " + QString(resourceExists ? "Play 'Resource exists'" : "Play 'Resource does not exist'"));
        QMessageBox::information(this, "Simulation", "Please send command '" + command + "' to Arduino Serial Monitor.");
    }
}
