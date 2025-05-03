#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QLabel>
#include <QtCharts>
#include <QMap>
#include "resource.h"

class QSqlQueryModel;
class QSqlTableModel;
class QTableView;

class ResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit ResourceManager(QObject *parent = nullptr);
    ~ResourceManager();

    // Interface management
    void setupTable(QTableWidget *tableWidget);
    void updateTable(QTableWidget *tableWidget, const QString &filter = "");
    void updateTable(QTableWidget *tableWidget, const QString &searchText, const QString &column);
    void clearForm();
    
    // Data operations
    bool addResource(const QString &name, const QString &type, 
                     const QString &brand, int quantity, 
                     const QDate &purchaseDate, const QByteArray &imageData);
    bool updateResource(int id, const QString &name, const QString &type, 
                       const QString &brand, int quantity, 
                       const QDate &purchaseDate, const QByteArray &imageData);
    bool deleteResource(int id);
    
    // Statistics and data analysis
    QMap<QString, int> getStatisticsByType();
    QMap<QString, int> getResourceCountByDate();
    int getTotalResourceCount();
    QDate getOldestResourceDate();
    QDate getNewestResourceDate();

    // Export and file operations
    bool exportToPdf(const QString &filePath);
    bool exportHistoryToFile(const QString &filePath);
    bool clearHistory();

    // Image handling
    static QByteArray loadImageData(const QString &filePath);
    static QPixmap byteArrayToPixmap(const QByteArray &imageData);

signals:
    void resourceAdded(int id);
    void resourceUpdated(int id);
    void resourceDeleted(int id);

private:
    Resource resourceObj;
    QString loggedInUserName;

    void setupHeaders(QTableWidget *tableWidget);
    void updateResourceData(QTableWidget *tableWidget, int row, const Resource &resource);
    QList<Resource> getResources(const QString &filter = "");
};

#endif // RESOURCEMANAGER_H
