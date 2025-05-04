#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSerialPort>
#include "managers/resources/resource.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

signals:
    void resourceUpdated(); // New signal for quantity update

private slots:
    void on_btnSearch_clicked();

private:
    Ui::SearchDialog *ui;
    QSerialPort *serialPort;

    bool queryDatabase(const QString &resourceName, int &resourceId, Resource &resource);
    bool updateResourceQuantity(int resourceId, int currentQuantity, const QString &employeeName);
    void sendToArduino(bool resourceExists);
};

#endif // SEARCHDIALOG_H
