#include "updatetrainingdialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>

UpdateTrainingDialog::UpdateTrainingDialog(int idfor, const formations &currentData, QWidget *parent)
    : QDialog(parent), trainingId(idfor)
{
    setWindowTitle("Update Training");

    QFormLayout *form = new QFormLayout(this);


    // Create input fields with current data
    formationEdit = new QLineEdit(currentData.getFormation());
    descriptionEdit = new QLineEdit(currentData.getDescription());
    trainerEdit = new QLineEdit(currentData.getTrainer());
    dateEdit = new QDateEdit(currentData.getDatef());
    timeSpin = new QSpinBox;
    prixSpin = new QDoubleSpinBox;

    // Set ranges
    timeSpin->setRange(1, 30);
    timeSpin->setValue(currentData.getTime());
    prixSpin->setRange(1, 1000);
    prixSpin->setValue(currentData.getPrix());

    // Add to form
    form->addRow("Training:", formationEdit);
    form->addRow("Description:", descriptionEdit);
    form->addRow("Trainer:", trainerEdit);
    form->addRow("Date:", dateEdit);
    form->addRow("Time (hours):", timeSpin);
    form->addRow("Prix:", prixSpin);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(buttonBox);

    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &UpdateTrainingDialog::onConfirmClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

UpdateTrainingDialog::~UpdateTrainingDialog()
{
    // Qt handles memory management of child widgets
}

void UpdateTrainingDialog::onConfirmClicked()
{
    // Basic validation could be added here
    if (formationEdit->text().isEmpty() || descriptionEdit->text().isEmpty() || trainerEdit->text().isEmpty())
    {
        return; // Don't accept if fields are empty
    }
    accept();
}

QString UpdateTrainingDialog::getFormation() const { return formationEdit->text(); }
QString UpdateTrainingDialog::getDescription() const { return descriptionEdit->text(); }
QString UpdateTrainingDialog::getTrainer() const { return trainerEdit->text(); }
QDate UpdateTrainingDialog::getDate() const { return dateEdit->date(); }
int UpdateTrainingDialog::getTime() const { return timeSpin->value(); }
double UpdateTrainingDialog::getPrix() const { return prixSpin->value(); }
int UpdateTrainingDialog::getId() const { return trainingId; }
