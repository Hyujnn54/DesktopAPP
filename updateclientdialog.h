#ifndef UPDATECLIENTDIALOG_H
#define UPDATECLIENTDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class UpdateClientDialog;
}

class UpdateClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateClientDialog(QWidget *parent = nullptr);
    ~UpdateClientDialog();

    void setClientData(const QString &name, const QString &sector, const QString &contactInfo,
                       const QString &email, const QDateTime &consultationDateTime, const QString &consultantId);

    QString getName() const;
    QString getSector() const;
    QString getContactInfo() const;
    QString getEmail() const;
    QDateTime getConsultationDateTime() const;
    QString getConsultant() const;

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::UpdateClientDialog *ui;
};

#endif // UPDATECLIENTDIALOG_H
