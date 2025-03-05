#ifndef SIGN_H
#define SIGN_H

#include <QMainWindow>

namespace Ui {
class sign;
}

class sign : public QMainWindow
{
    Q_OBJECT

public:
    explicit sign(QWidget *parent = nullptr);
    ~sign();
private slots :
    void handleLogin();
private:
    Ui::sign *ui;
};

#endif // SIGN_H
