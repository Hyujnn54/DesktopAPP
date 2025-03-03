#include "emailsender.h"
#include <QDebug>
#include <QTextStream>
#include <QDateTime>

EmailSender::EmailSender(QObject *parent) : QObject(parent)
{
}

bool EmailSender::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    QSslSocket socket;
    // Remove setProtocol or use a modern TLS version
    // socket.setProtocol(QSsl::TlsV1_2); // Optional: Explicitly use TLS 1.2
    // By default, QSslSocket will negotiate the best available protocol (TLS)

    // Connect to SMTP server
    if (!connectToServer(socket)) {
        qDebug() << "Failed to connect to SMTP server";
        return false;
    }

    // Authenticate
    if (!authenticate(socket)) {
        qDebug() << "Authentication failed";
        return false;
    }

    // Send email
    QString mailFrom = "MAIL FROM:<" + senderEmail + ">\r\n";
    if (!sendData(socket, mailFrom)) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "MAIL FROM response:" << socket.readAll();

    QString rcptTo = "RCPT TO:<" + to + ">\r\n";
    if (!sendData(socket, rcptTo)) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "RCPT TO response:" << socket.readAll();

    if (!sendData(socket, "DATA\r\n")) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "DATA response:" << socket.readAll();

    QString emailData = "From: " + senderEmail + "\r\n"
                                                 "To: " + to + "\r\n"
                               "Subject: " + subject + "\r\n"
                                    "\r\n" + body + "\r\n"
                                 ".\r\n";
    if (!sendData(socket, emailData)) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "Email data response:" << socket.readAll();

    if (!sendData(socket, "QUIT\r\n")) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "QUIT response:" << socket.readAll();

    socket.disconnectFromHost();
    return true;
}

bool EmailSender::connectToServer(QSslSocket &socket)
{
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForEncrypted(10000)) {
        qDebug() << "Connection error:" << socket.errorString();
        return false;
    }
    socket.waitForReadyRead(5000);
    qDebug() << "Server response:" << socket.readAll();
    return true;
}

bool EmailSender::authenticate(QSslSocket &socket)
{
    if (!sendData(socket, "EHLO localhost\r\n")) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "EHLO response:" << socket.readAll();

    if (!sendData(socket, "AUTH LOGIN\r\n")) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "AUTH LOGIN response:" << socket.readAll();

    QByteArray emailBase64 = senderEmail.toUtf8().toBase64();
    if (!sendData(socket, emailBase64 + "\r\n")) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "Email response:" << socket.readAll();

    QByteArray passwordBase64 = senderPassword.toUtf8().toBase64();
    if (!sendData(socket, passwordBase64 + "\r\n")) return false;
    socket.waitForReadyRead(5000);
    qDebug() << "Password response:" << socket.readAll();

    return true;
}

bool EmailSender::sendData(QSslSocket &socket, const QString &data)
{
    QTextStream stream(&socket);
    stream << data;
    stream.flush();
    return socket.waitForBytesWritten(5000);
}
