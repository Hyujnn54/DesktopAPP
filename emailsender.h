#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QSslSocket>
#include <QString>
#include <QObject>

class EmailSender : public QObject
{
    Q_OBJECT
public:
    explicit EmailSender(QObject *parent = nullptr);
    bool sendEmail(const QString &to, const QString &subject, const QString &body);

private:
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465; // SSL port for Gmail
    QString senderEmail = "your-email@gmail.com"; // Replace with your Gmail
    QString senderPassword = "your-app-password"; // Replace with your App Password

    bool connectToServer(QSslSocket &socket);
    bool authenticate(QSslSocket &socket);
    bool sendData(QSslSocket &socket, const QString &data);
};

#endif // EMAILSENDER_H
