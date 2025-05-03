#include "resourcemanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPainter>
#include <QPrinter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFile>

ResourceManager::ResourceManager(QObject *parent) : QObject(parent), loggedInUserName("Admin")
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::setupTable(QTableWidget *tableWidget)
{
    if (!tableWidget) return;

    // Clear existing content
    tableWidget->clear();
    tableWidget->setRowCount(0);

    // Setup headers
    setupHeaders(tableWidget);

    // Enable sorting
    tableWidget->setSortingEnabled(true);

    // Configure table behavior
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableWidget->setAlternatingRowColors(true);

    // Update table with fresh data
    updateTable(tableWidget);
}

void ResourceManager::setupHeaders(QTableWidget *tableWidget)
{
    QStringList headers = {
        "ID", "Name", "Type", "Brand", "Quantity", "Purchase Date", "Image"
    };

    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    // Set reasonable column widths
    tableWidget->setColumnWidth(0, 50);  // ID
    tableWidget->setColumnWidth(1, 150); // Name
    tableWidget->setColumnWidth(2, 120); // Type
    tableWidget->setColumnWidth(3, 120); // Brand
    tableWidget->setColumnWidth(4, 70);  // Quantity
    tableWidget->setColumnWidth(5, 120); // Purchase Date
    tableWidget->setColumnWidth(6, 120); // Image
}

void ResourceManager::updateTable(QTableWidget *tableWidget, const QString &filter)
{
    if (!tableWidget) return;

    tableWidget->setSortingEnabled(false);
    tableWidget->setRowCount(0);

    QList<Resource> resources = getResources(filter);
    
    tableWidget->setRowCount(resources.size());
    
    int row = 0;
    for (const Resource &resource : resources) {
        updateResourceData(tableWidget, row, resource);
        row++;
    }
    
    tableWidget->setSortingEnabled(true);
}

void ResourceManager::updateTable(QTableWidget* tableWidget, const QString& searchText, const QString& column)
{
    // Default: show all resources if no search text
    QString queryStr = "SELECT RESOURCE_ID, NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE FROM RESOURCES";
    QMap<QString, QString> columnMap = {
        {"Name", "NAME"},
        {"Type", "TYPE"},
        {"Brand", "BRAND"},
        {"Quantity", "QUANTITY"},
        {"Purchase Date", "PURCHASE_DATE"}
    };
    QList<QVariant> bindValues;
    if (!searchText.isEmpty() && columnMap.contains(column)) {
        queryStr += QString(" WHERE UPPER(%1) LIKE UPPER(?)").arg(columnMap[column]);
        bindValues << ("%" + searchText + "%");
    }
    QSqlQuery query;
    query.prepare(queryStr);
    for (const QVariant& val : bindValues) {
        query.addBindValue(val);
    }
    query.exec();
    tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        for (int col = 0; col < 6; ++col) {
            tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        // Handle image column if needed
        row++;
    }
}

void ResourceManager::updateResourceData(QTableWidget *tableWidget, int row, const Resource &resource)
{
    // Create items for each column
    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(resource.getResourceId()));
    QTableWidgetItem *nameItem = new QTableWidgetItem(resource.getName());
    QTableWidgetItem *typeItem = new QTableWidgetItem(resource.getType());
    QTableWidgetItem *brandItem = new QTableWidgetItem(resource.getBrand());
    QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(resource.getQuantity()));
    QTableWidgetItem *dateItem = new QTableWidgetItem(resource.getPurchaseDate().toString("yyyy-MM-dd"));
    
    // Create a label for the image
    QLabel *imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    
    QByteArray imageData = resource.getImage();
    if (!imageData.isEmpty()) {
        QPixmap pixmap = byteArrayToPixmap(imageData);
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(100, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(scaledPixmap);
        } else {
            imageLabel->setText("Invalid Image");
        }
    } else {
        imageLabel->setText("No Image");
    }
    
    // Store resource ID in item
    idItem->setData(Qt::UserRole, resource.getResourceId());
    
    // Set items to the tableWidget
    tableWidget->setItem(row, 0, idItem);
    tableWidget->setItem(row, 1, nameItem);
    tableWidget->setItem(row, 2, typeItem);
    tableWidget->setItem(row, 3, brandItem);
    tableWidget->setItem(row, 4, quantityItem);
    tableWidget->setItem(row, 5, dateItem);
    tableWidget->setCellWidget(row, 6, imageLabel);
}

QList<Resource> ResourceManager::getResources(const QString &filter)
{
    QList<Resource> resources;
    
    QSqlQuery query;
    QString queryStr = "SELECT RESOURCE_ID, NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE FROM RESOURCES";
    
    if (!filter.isEmpty()) {
        queryStr += " WHERE UPPER(NAME) LIKE UPPER(:filter) OR UPPER(TYPE) LIKE UPPER(:filter) "
                    "OR UPPER(BRAND) LIKE UPPER(:filter)";
        query.prepare(queryStr);
        query.bindValue(":filter", "%" + filter + "%");
    } else {
        query.prepare(queryStr);
    }
    
    if (query.exec()) {
        while (query.next()) {
            Resource resource;
            resource.setResourceId(query.value("RESOURCE_ID").toInt());
            resource.setName(query.value("NAME").toString());
            resource.setType(query.value("TYPE").toString());
            resource.setBrand(query.value("BRAND").toString());
            resource.setQuantity(query.value("QUANTITY").toInt());
            resource.setPurchaseDate(query.value("PURCHASE_DATE").toDate());
            resource.setImage(query.value("IMAGE").toByteArray());
            
            resources.append(resource);
        }
    } else {
        qDebug() << "Error fetching resources:" << query.lastError().text();
    }
    
    return resources;
}

bool ResourceManager::addResource(const QString &name, const QString &type, 
                                 const QString &brand, int quantity, 
                                 const QDate &purchaseDate, const QByteArray &imageData)
{
    resourceObj.setName(name);
    resourceObj.setType(type);
    resourceObj.setBrand(brand);
    resourceObj.setQuantity(quantity);
    resourceObj.setPurchaseDate(purchaseDate);
    resourceObj.setImage(imageData);
    
    if (resourceObj.addResource(loggedInUserName)) {
        emit resourceAdded(resourceObj.getResourceId());
        return true;
    }
    
    return false;
}

bool ResourceManager::updateResource(int id, const QString &name, const QString &type, 
                                   const QString &brand, int quantity, 
                                   const QDate &purchaseDate, const QByteArray &imageData)
{
    resourceObj.setResourceId(id);
    resourceObj.setName(name);
    resourceObj.setType(type);
    resourceObj.setBrand(brand);
    resourceObj.setQuantity(quantity);
    resourceObj.setPurchaseDate(purchaseDate);
    resourceObj.setImage(imageData);
    
    if (resourceObj.updateResource(loggedInUserName)) {
        emit resourceUpdated(id);
        return true;
    }
    
    return false;
}

bool ResourceManager::deleteResource(int id)
{
    if (resourceObj.delet(id, loggedInUserName)) {
        emit resourceDeleted(id);
        return true;
    }
    
    return false;
}

QMap<QString, int> ResourceManager::getStatisticsByType()
{
    QMap<QString, int> typeCount;
    
    QSqlQuery query;
    query.prepare("SELECT TYPE, COUNT(*) as COUNT FROM RESOURCES GROUP BY TYPE");
    
    if (query.exec()) {
        while (query.next()) {
            QString type = query.value("TYPE").toString();
            int count = query.value("COUNT").toInt();
            typeCount.insert(type, count);
        }
    } else {
        qDebug() << "Error getting statistics by type:" << query.lastError().text();
    }
    
    return typeCount;
}

QMap<QString, int> ResourceManager::getResourceCountByDate()
{
    QMap<QString, int> dateCount;
    
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(PURCHASE_DATE, 'YYYY-MM') as MONTH, COUNT(*) as COUNT "
                  "FROM RESOURCES GROUP BY TO_CHAR(PURCHASE_DATE, 'YYYY-MM') "
                  "ORDER BY MONTH");
    
    if (query.exec()) {
        while (query.next()) {
            QString month = query.value("MONTH").toString();
            int count = query.value("COUNT").toInt();
            dateCount.insert(month, count);
        }
    } else {
        qDebug() << "Error getting resources by date:" << query.lastError().text();
    }
    
    return dateCount;
}

int ResourceManager::getTotalResourceCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM RESOURCES");
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    
    return 0;
}

QDate ResourceManager::getOldestResourceDate()
{
    QSqlQuery query;
    query.prepare("SELECT MIN(PURCHASE_DATE) FROM RESOURCES");
    
    if (query.exec() && query.next()) {
        return query.value(0).toDate();
    }
    
    return QDate();
}

QDate ResourceManager::getNewestResourceDate()
{
    QSqlQuery query;
    query.prepare("SELECT MAX(PURCHASE_DATE) FROM RESOURCES");
    
    if (query.exec() && query.next()) {
        return query.value(0).toDate();
    }
    
    return QDate();
}

bool ResourceManager::exportToPdf(const QString &filePath)
{
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30));
    
    QPainter painter(&pdfWriter);
    painter.setPen(Qt::black);
    
    // Configure font
    QFont titleFont = painter.font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    
    QFont headerFont = painter.font();
    headerFont.setPointSize(12);
    headerFont.setBold(true);
    
    QFont contentFont = painter.font();
    contentFont.setPointSize(10);
    
    // Draw title
    painter.setFont(titleFont);
    painter.drawText(QRect(0, 100, pdfWriter.width(), 50), 
                    Qt::AlignHCenter, "Resources Report");
    
    // Subtitle with date
    painter.setFont(headerFont);
    painter.drawText(QRect(0, 150, pdfWriter.width(), 30), 
                    Qt::AlignHCenter, "Generated on " + QDate::currentDate().toString("yyyy-MM-dd"));
    
    // Get resources
    QList<Resource> resources = getResources();
    
    // Calculate row height and column widths
    int rowHeight = 40;
    int startY = 250;
    QList<int> columnWidths = {80, 150, 120, 120, 80, 120};
    int totalWidth = 0;
    for (int width : columnWidths) {
        totalWidth += width;
    }
    
    int startX = (pdfWriter.width() - totalWidth) / 2;
    
    // Draw table headers
    painter.setFont(headerFont);
    int currentX = startX;
    QStringList headers = {"ID", "Name", "Type", "Brand", "Quantity", "Purchase Date"};
    
    for (int i = 0; i < headers.size(); ++i) {
        QRect headerRect(currentX, startY, columnWidths[i], rowHeight);
        painter.drawRect(headerRect);
        painter.drawText(headerRect, Qt::AlignCenter, headers[i]);
        currentX += columnWidths[i];
    }
    
    // Draw table content
    painter.setFont(contentFont);
    int currentY = startY + rowHeight;
    
    for (const Resource &resource : resources) {
        if (currentY + rowHeight > pdfWriter.height() - 100) {
            // Start a new page
            pdfWriter.newPage();
            currentY = 100;
            
            // Redraw headers on new page
            painter.setFont(headerFont);
            currentX = startX;
            for (int i = 0; i < headers.size(); ++i) {
                QRect headerRect(currentX, currentY, columnWidths[i], rowHeight);
                painter.drawRect(headerRect);
                painter.drawText(headerRect, Qt::AlignCenter, headers[i]);
                currentX += columnWidths[i];
            }
            
            painter.setFont(contentFont);
            currentY += rowHeight;
        }
        
        // Fill row data
        currentX = startX;
        
        // ID
        QRect idRect(currentX, currentY, columnWidths[0], rowHeight);
        painter.drawRect(idRect);
        painter.drawText(idRect, Qt::AlignCenter, QString::number(resource.getResourceId()));
        currentX += columnWidths[0];
        
        // Name
        QRect nameRect(currentX, currentY, columnWidths[1], rowHeight);
        painter.drawRect(nameRect);
        painter.drawText(nameRect.adjusted(5, 0, -5, 0), Qt::AlignVCenter, resource.getName());
        currentX += columnWidths[1];
        
        // Type
        QRect typeRect(currentX, currentY, columnWidths[2], rowHeight);
        painter.drawRect(typeRect);
        painter.drawText(typeRect.adjusted(5, 0, -5, 0), Qt::AlignVCenter, resource.getType());
        currentX += columnWidths[2];
        
        // Brand
        QRect brandRect(currentX, currentY, columnWidths[3], rowHeight);
        painter.drawRect(brandRect);
        painter.drawText(brandRect.adjusted(5, 0, -5, 0), Qt::AlignVCenter, resource.getBrand());
        currentX += columnWidths[3];
        
        // Quantity
        QRect quantityRect(currentX, currentY, columnWidths[4], rowHeight);
        painter.drawRect(quantityRect);
        painter.drawText(quantityRect, Qt::AlignCenter, QString::number(resource.getQuantity()));
        currentX += columnWidths[4];
        
        // Purchase Date
        QRect dateRect(currentX, currentY, columnWidths[5], rowHeight);
        painter.drawRect(dateRect);
        painter.drawText(dateRect, Qt::AlignCenter, resource.getPurchaseDate().toString("yyyy-MM-dd"));
        
        currentY += rowHeight;
    }
    
    // Draw footer
    int footerY = pdfWriter.height() - 50;
    painter.drawLine(30, footerY, pdfWriter.width() - 30, footerY);
    painter.setFont(contentFont);
    painter.drawText(QRect(30, footerY + 10, pdfWriter.width() - 60, 30), 
                    Qt::AlignCenter, "Total Resources: " + QString::number(resources.size()));
    
    return true;
}

bool ResourceManager::exportHistoryToFile(const QString &filePath)
{
    QFile file("resource_history.txt");
    if (!file.exists()) {
        return false;
    }
    
    if (file.copy(filePath)) {
        return true;
    }
    
    return false;
}

bool ResourceManager::clearHistory()
{
    QFile file("resource_history.txt");
    if (file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.close();
            return true;
        }
    }
    
    return false;
}

QByteArray ResourceManager::loadImageData(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        return file.readAll();
    }
    
    return QByteArray();
}

QPixmap ResourceManager::byteArrayToPixmap(const QByteArray &imageData)
{
    QPixmap pixmap;
    pixmap.loadFromData(imageData);
    return pixmap;
}

void ResourceManager::clearForm()
{
    // This will be implemented in the MainWindow class
    // since it directly interacts with the UI elements
}