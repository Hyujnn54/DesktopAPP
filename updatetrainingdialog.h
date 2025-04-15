#ifndef UPDATETRAININGDIALOG_H
#define UPDATETRAININGDIALOG_H

#include <QDialog>
#include <QDate>
#include "formations.h"

class QLineEdit;
class QDateEdit;
class QSpinBox;
class QDoubleSpinBox;

class UpdateTrainingDialog : public QDialog
{
    Q_OBJECT


public:
    explicit UpdateTrainingDialog(int idfor, const formations &currentData, QWidget *parent = nullptr);
    ~UpdateTrainingDialog();

    // Getters for the updated values
    QString getFormation() const;
    QString getDescription() const;
    QString getTrainer() const;
    QDate getDate() const;
    int getTime() const;
    double getPrix() const;
    int getId() const;

private slots:
    void onConfirmClicked();

private:
    int trainingId;
    QLineEdit *formationEdit;
    QLineEdit *descriptionEdit;
    QLineEdit *trainerEdit;
    QDateEdit *dateEdit;
    QSpinBox *timeSpin;
    QDoubleSpinBox *prixSpin;
};

#endif // UPDATETRAININGDIALOG_H
