#include "UpdateTrainingDialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>

UpdateTrainingDialog::UpdateTrainingDialog(QWidget *parent)
    : QDialog(parent),
    nameEdit(new QLineEdit(this)),
    descriptionEdit(new QLineEdit(this)),
    trainerEdit(new QLineEdit(this)),
    dateEdit(new QDateEdit(this)),
    timeSpinBox(new QSpinBox(this)),
    prixSpinBox(new QDoubleSpinBox(this))
{
    setWindowTitle("Update Training");

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Description:", descriptionEdit);
    formLayout->addRow("Trainer:", trainerEdit);
    dateEdit->setCalendarPopup(true);
    formLayout->addRow("Date:", dateEdit);
    timeSpinBox->setRange(1, 1000);
    formLayout->addRow("Duration (hours):", timeSpinBox);
    prixSpinBox->setRange(0.0, 10000.0);
    prixSpinBox->setDecimals(2);
    formLayout->addRow("Price:", prixSpinBox);

    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void UpdateTrainingDialog::setTrainingData(const QString &name, const QString &description, const QString &trainer,
                                           const QDate &date, int time, double prix)
{
    nameEdit->setText(name);
    descriptionEdit->setText(description);
    trainerEdit->setText(trainer);
    dateEdit->setDate(date);
    timeSpinBox->setValue(time);
    prixSpinBox->setValue(prix);
}

QString UpdateTrainingDialog::getName() const
{
    return nameEdit->text().trimmed();
}

QString UpdateTrainingDialog::getDescription() const
{
    return descriptionEdit->text().trimmed();
}

QString UpdateTrainingDialog::getTrainer() const
{
    return trainerEdit->text().trimmed();
}

QDate UpdateTrainingDialog::getDate() const
{
    return dateEdit->date();
}

int UpdateTrainingDialog::getTime() const
{
    return timeSpinBox->value();
}

double UpdateTrainingDialog::getPrix() const
{
    return prixSpinBox->value();
}

// Add resource area population and update logic
void UpdateTrainingDialog::setResourceData(int trainingId) {
    // Get used resources for this training
    QMap<int, int> usedResources;
    QSqlQuery usedQuery;
    usedQuery.prepare("SELECT RESOURCE_ID, QUANTITY FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :tid");
    usedQuery.bindValue(":tid", trainingId);
    if (usedQuery.exec()) {
        while (usedQuery.next()) {
            usedResources[usedQuery.value(0).toInt()] = usedQuery.value(1).toInt();
        }
    }
    QSqlQuery resQuery("SELECT RESOURCE_ID, NAME FROM RESOURCES");
    QVBoxLayout* vbox = findChild<QVBoxLayout*>("resourcesVBoxLayout");
    if (!vbox) return;
    while (resQuery.next()) {
        int id = resQuery.value(0).toInt();
        QString name = resQuery.value(1).toString();
        QWidget* rowWidget = new QWidget();
        QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
        QCheckBox* checkBox = new QCheckBox(name);
        checkBox->setObjectName(QString("resourceCheckBox_%1").arg(id));
        QSpinBox* spinBox = new QSpinBox();
        spinBox->setMinimum(1);
        spinBox->setMaximum(1000);
        spinBox->setValue(1);
        spinBox->setObjectName(QString("resourceSpinBox_%1").arg(id));
        spinBox->setEnabled(false);
        rowLayout->addWidget(checkBox);
        rowLayout->addWidget(spinBox);
        rowWidget->setLayout(rowLayout);
        vbox->addWidget(rowWidget);
        QObject::connect(checkBox, &QCheckBox::toggled, spinBox, &QSpinBox::setEnabled);
        if (usedResources.contains(id)) {
            checkBox->setChecked(true);
            spinBox->setValue(usedResources[id]);
        }
    }
}

QList<QPair<int, int>> UpdateTrainingDialog::getSelectedResources() const {
    QList<QPair<int, int>> selectedResources;
    QVBoxLayout* vbox = findChild<QVBoxLayout*>("resourcesVBoxLayout");
    if (!vbox) return selectedResources;
    for (int i = 0; i < vbox->count(); ++i) {
        QWidget* rowWidget = vbox->itemAt(i)->widget();
        if (!rowWidget) continue;
        QCheckBox* checkBox = rowWidget->findChild<QCheckBox*>();
        QSpinBox* spinBox = rowWidget->findChild<QSpinBox*>();
        if (checkBox && spinBox && checkBox->isChecked()) {
            QString objName = checkBox->objectName();
            int resourceId = objName.section('_', 1, 1).toInt();
            int quantity = spinBox->value();
            selectedResources.append(qMakePair(resourceId, quantity));
        }
    }
    return selectedResources;
}
