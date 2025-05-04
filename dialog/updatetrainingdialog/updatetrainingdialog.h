#ifndef UPDATETRAININGDIALOG_H
#define UPDATETRAININGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QDate>

class UpdateTrainingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTrainingDialog(QWidget *parent = nullptr);
    void setTrainingData(const QString &name, const QString &description, const QString &trainer,
                         const QDate &date, int time, double prix);
    QString getName() const;
    QString getDescription() const;
    QString getTrainer() const;
    QDate getDate() const;
    int getTime() const;
    double getPrix() const;
    void setResourceData(int trainingId);
    QList<QPair<int, int>> getSelectedResources() const;

private:
    QLineEdit *nameEdit;
    QLineEdit *descriptionEdit;
    QLineEdit *trainerEdit;
    QDateEdit *dateEdit;
    QSpinBox *timeSpinBox;
    QDoubleSpinBox *prixSpinBox;
};

#endif // UPDATETRAININGDIALOG_H
