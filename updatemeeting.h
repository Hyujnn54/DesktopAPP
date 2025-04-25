#ifndef UPDATEMEETING_H
#define UPDATEMEETING_H

#include <QDialog>
#include "meeting.h"

namespace Ui {
class UpdateMeetingDialog;
}

class UpdateMeeting : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateMeeting(QWidget *parent = nullptr, meeting *m = nullptr);
    ~UpdateMeeting();

private slots:
    void on_confirmButton_clicked();

private:
    Ui::UpdateMeetingDialog *ui;
    meeting *currentMeeting; // Pointer to the meeting being updated
};

#endif // UPDATEMEETING_H
