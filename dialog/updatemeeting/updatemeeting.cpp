#include "updatemeeting.h"
#include "ui_updatemeeting.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>

UpdateMeeting::UpdateMeeting(QWidget *parent, meeting *m) :
    QDialog(parent),
    ui(new Ui::UpdateMeetingDialog),
    currentMeeting(m)
{
    ui->setupUi(this);

    // Populate fields with current meeting data if provided
    if (currentMeeting) {
        ui->titleLineEdit->setText(currentMeeting->getTitle());
        ui->organiserLineEdit->setText(currentMeeting->getOrganiser());
        ui->participantLineEdit->setText(currentMeeting->getParticipant());
        ui->agendaComboBox->setCurrentText(currentMeeting->getAgenda());
        ui->durationLineEdit->setText(QString::number(currentMeeting->getDuration()));
        ui->dateTimeEdit->setDateTime(currentMeeting->getDatem());
    }

    // --- Resource area population ---
    QMap<int, int> usedResources; // resource_id -> quantity
    if (currentMeeting) {
        QSqlQuery usedQuery;
        usedQuery.prepare("SELECT RESOURCE_ID, QUANTITY FROM AHMED.MEETING_RESOURCES WHERE MEETING_ID = :mid");
        usedQuery.bindValue(":mid", currentMeeting->getId());
        if (usedQuery.exec()) {
            while (usedQuery.next()) {
                usedResources[usedQuery.value(0).toInt()] = usedQuery.value(1).toInt();
            }
        }
    }
    QSqlQuery resQuery("SELECT RESOURCE_ID, NAME FROM RESOURCES");
    QVBoxLayout* vbox = qobject_cast<QVBoxLayout*>(ui->resourcesGroupBox->layout());
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
        // Pre-check if used
        if (usedResources.contains(id)) {
            checkBox->setChecked(true);
            spinBox->setValue(usedResources[id]);
        }
    }
}

UpdateMeeting::~UpdateMeeting()
{
    delete ui;
}

void UpdateMeeting::on_confirmButton_clicked()
{
    // Get values from the UI
    QString title = ui->titleLineEdit->text();
    QString organiser = ui->organiserLineEdit->text();
    QString participant = ui->participantLineEdit->text();
    QString agenda = ui->agendaComboBox->currentText();
    QDateTime datem = ui->dateTimeEdit->dateTime();
    bool conversionOk = false;
    int duration = ui->durationLineEdit->text().toInt(&conversionOk);

    // Validate inputs
    QStringList validationErrors;
    if (title.isEmpty()) {
        validationErrors << "Title cannot be empty";
    }
    if (organiser.isEmpty()) {
        validationErrors << "Organiser cannot be empty";
    }
    if (!conversionOk || duration <= 0) {
        validationErrors << "Duration must be a valid positive number";
    }
    if (!datem.isValid()) {
        validationErrors << "Date and time must be valid";
    }

    if (!validationErrors.isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please correct the following errors:\n• " + validationErrors.join("\n• "));
        return;
    }

    // Update the meeting object
    currentMeeting->setTitle(title);
    currentMeeting->setOrganiser(organiser);
    currentMeeting->setParticipant(participant);
    currentMeeting->setAgenda(agenda);
    currentMeeting->setDuration(duration);
    currentMeeting->setDatem(datem);

    // --- Collect selected resources ---
    QList<QPair<int, int>> selectedResources;
    QVBoxLayout* vbox = qobject_cast<QVBoxLayout*>(ui->resourcesGroupBox->layout());
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
    // --- Update resources in DB ---
    if (currentMeeting) {
        QSqlQuery delQuery;
        delQuery.prepare("DELETE FROM AHMED.MEETING_RESOURCES WHERE MEETING_ID = :mid");
        delQuery.bindValue(":mid", currentMeeting->getId());
        delQuery.exec();
        for (const auto& pair : selectedResources) {
            int resourceId = pair.first;
            int quantity = pair.second;
            QSqlQuery linkQuery;
            linkQuery.prepare("INSERT INTO AHMED.MEETING_RESOURCES (MEETING_ID, RESOURCE_ID, QUANTITY) VALUES (:mid, :rid, :qty)");
            linkQuery.bindValue(":mid", currentMeeting->getId());
            linkQuery.bindValue(":rid", resourceId);
            linkQuery.bindValue(":qty", quantity);
            linkQuery.exec();
        }
    }

    // Attempt to save changes to the database
    if (currentMeeting->update()) {
        QMessageBox::information(this, "Success", "Meeting updated successfully.");
        accept(); // Close dialog with success
    } else {
        QMessageBox::warning(this, "Error", "Failed to update meeting. Check the database connection.");
    }
}
