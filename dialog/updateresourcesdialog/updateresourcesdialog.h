#ifndef UPDATERESOURCESDIALOG_H
#define UPDATERESOURCESDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class UpdateResourceDialog;
}

class UpdateResourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateResourceDialog(QWidget *parent = nullptr);
    ~UpdateResourceDialog();

    void setResourceData(int resourceId, const QString &name, const QString &type,
                         const QString &brand, int quantity, const QDate &purchaseDate,
                         const QByteArray &imageData);

    // Getters for the updated values
    QString getName() const;
    QString getType() const;
    QString getBrand() const;
    int getQuantity() const;
    QDate getPurchaseDate() const;
    QByteArray getImageData() const;

private slots:
    void on_selectImageButton_clicked();
    void onAccepted();

private:
    Ui::UpdateResourceDialog *ui;
    QByteArray imageData;
    bool validate();
};

#endif // UPDATERESOURCESDIALOG_H