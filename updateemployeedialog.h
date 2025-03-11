#ifndef UPDATEEMPLOYEEDIALOG_H
#define UPDATEEMPLOYEEDIALOG_H

#include <QDialog>
#include "employee.h"

namespace Ui {
class UpdateEmployeeDialog;
}

class UpdateEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateEmployeeDialog(int employeeId, QWidget *parent = nullptr);
    ~UpdateEmployeeDialog();

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::UpdateEmployeeDialog *ui;
    int m_employeeId;
    Employee *employee;
    QString m_imagePath;
};

#endif // UPDATEEMPLOYEEDIALOG_H
