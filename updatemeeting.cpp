#include "updatemeeting.h"
#include "ui_updatemeeting.h"
#include <QMessageBox>
#include <QDebug>

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

    // Attempt to save changes to the database
    if (currentMeeting->update()) {
        QMessageBox::information(this, "Success", "Meeting updated successfully.");
        accept(); // Close dialog with success
    } else {
        QMessageBox::warning(this, "Error", "Failed to update meeting. Check the database connection.");
    }
}
