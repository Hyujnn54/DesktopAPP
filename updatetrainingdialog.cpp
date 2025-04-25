#include "updatetrainingdialog.h"
#include "ui_updatetrainingdialog.h"
#include <QMessageBox>

UpdateTrainingDialog::UpdateTrainingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTrainingDialog),
    trainingId(0)
{
    ui->setupUi(this);
    setWindowTitle("Update Training");

    connect(ui->confirmButton, &QPushButton::clicked, this, &UpdateTrainingDialog::on_confirmButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &UpdateTrainingDialog::on_cancelButton_clicked);
}

UpdateTrainingDialog::~UpdateTrainingDialog()
{
    delete ui;
}

void UpdateTrainingDialog::setTrainingData(int idfor, const formations &trainingData)
{
    this->trainingId = idfor;
    this->trainingData = trainingData;

    ui->formationLineEdit->setText(trainingData.getFormation());
    ui->descriptionLineEdit->setText(trainingData.getDescription());
    ui->trainerLineEdit->setText(trainingData.getTrainer());
    ui->dateEdit->setDate(trainingData.getDatef());
    ui->timeSpinBox->setValue(trainingData.getTime());
    ui->prixSpinBox->setValue(trainingData.getPrix());
}

QString UpdateTrainingDialog::getFormation() const
{
    return ui->formationLineEdit->text().trimmed();
}

QString UpdateTrainingDialog::getDescription() const
{
    return ui->descriptionLineEdit->text().trimmed();
}

QString UpdateTrainingDialog::getTrainer() const
{
    return ui->trainerLineEdit->text().trimmed();
}

QDate UpdateTrainingDialog::getDate() const
{
    return ui->dateEdit->date();
}

int UpdateTrainingDialog::getTime() const
{
    return ui->timeSpinBox->value();
}

double UpdateTrainingDialog::getPrix() const
{
    return ui->prixSpinBox->value();
}

int UpdateTrainingDialog::getTrainingId() const
{
    return trainingId;
}

void UpdateTrainingDialog::on_confirmButton_clicked()
{
    if (ui->formationLineEdit->text().trimmed().isEmpty() ||
        ui->descriptionLineEdit->text().trimmed().isEmpty() ||
        ui->trainerLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all required fields.");
        return;
    }

    if (ui->dateEdit->date() < QDate::currentDate()) {
        QMessageBox::warning(this, "Invalid Date", "Please select a future date.");
        return;
    }

    accept();
}

void UpdateTrainingDialog::on_cancelButton_clicked()
{
    reject();
}
