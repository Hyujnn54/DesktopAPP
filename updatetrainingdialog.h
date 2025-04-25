#ifndef UPDATETRAININGDIALOG_H
#define UPDATETRAININGDIALOG_H

#include <QDialog>
#include <QDate>
#include "formations.h"

namespace Ui {
class UpdateTrainingDialog;
}

class UpdateTrainingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTrainingDialog(QWidget *parent = nullptr);
    ~UpdateTrainingDialog();

    void setTrainingData(int idfor, const formations &trainingData);

    QString getFormation() const;
    QString getDescription() const;
    QString getTrainer() const;
    QDate getDate() const;
    int getTime() const;
    double getPrix() const;
    int getTrainingId() const;

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::UpdateTrainingDialog *ui;
    int trainingId;
    formations trainingData;
};

#endif // UPDATETRAININGDIALOG_H
