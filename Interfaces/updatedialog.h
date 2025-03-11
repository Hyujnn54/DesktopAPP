#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include "meeting.h"

namespace Ui {
class UpdateMeetingDialog;  // Changed from UpdateClientDialog
}

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(const meeting& m, QWidget *parent = nullptr);
    ~UpdateDialog();
    meeting getUpdatedMeeting() const;

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::UpdateMeetingDialog *ui;  // Changed from Ui::UpdateClientDialog
    meeting currentMeeting;
};

#endif // UPDATEDIALOG_H
