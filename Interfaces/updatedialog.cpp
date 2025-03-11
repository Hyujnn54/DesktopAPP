#include "updatedialog.h"
#include "ui_updatemeetingdialog.h"

UpdateDialog::UpdateDialog(const meeting& m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateMeetingDialog),
    currentMeeting(m)
{
    ui->setupUi(this);
    setWindowTitle("Update Meeting");
    ui->nameLineEdit->setText(m.getTitle());
    ui->sectorLineEdit->setText(m.getOrganiser());
    ui->contactLineEdit->setText(m.getParticipant());
    ui->emailLineEdit->setText(m.getAgenda());
    ui->consultationDateTime->setDate(m.getDate());
    ui->consultantLineEdit->setText(QString::number(m.getDuration()));
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

meeting UpdateDialog::getUpdatedMeeting() const
{
    return currentMeeting;
}

void UpdateDialog::on_confirmButton_clicked()
{
    currentMeeting.setTitle(ui->nameLineEdit->text());
    currentMeeting.setOrganiser(ui->sectorLineEdit->text());
    currentMeeting.setParticipant(ui->contactLineEdit->text());
    currentMeeting.setAgenda(ui->emailLineEdit->text());
    currentMeeting.setDate(ui->consultationDateTime->date());
    bool ok;
    int duration = ui->consultantLineEdit->text().toInt(&ok);
    if (ok) {
        currentMeeting.setDuration(duration);
    }
    accept();
}
