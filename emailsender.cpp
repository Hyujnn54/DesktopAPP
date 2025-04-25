#include "emailsender.h"
#include <QDebug>
#include <QTextStream>
#include <QDateTime>

EmailSender::EmailSender(QObject *parent) : QObject(parent)
{
}

bool EmailSender::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    lastSendSuccessful = false; // Reset before attempting to send
    QSslSocket socket;

    // Connect to SMTP server
    if (!connectToServer(socket)) {
        qDebug() << "Failed to connect to SMTP server";
        return false;
    }

    // Authenticate
    if (!authenticate(socket)) {
        qDebug() << "Authentication failed";
        socket.disconnectFromHost();
        return false;
    }

    // Send email
    QString mailFrom = "MAIL FROM:<" + senderEmail + ">\r\n";
    if (!sendData(socket, mailFrom) || !checkResponse(socket, "250")) {
        qDebug() << "MAIL FROM failed";
        socket.disconnectFromHost();
        return false;
    }

    QString rcptTo = "RCPT TO:<" + to + ">\r\n";
    if (!sendData(socket, rcptTo) || !checkResponse(socket, "250")) {
        qDebug() << "RCPT TO failed";
        socket.disconnectFromHost();
        return false;
    }

    if (!sendData(socket, "DATA\r\n") || !checkResponse(socket, "354")) {
        qDebug() << "DATA command failed";
        socket.disconnectFromHost();
        return false;
    }

    QString emailData = "From: " + senderEmail + "\r\n"
                                                 "To: " + to + "\r\n"
                               "Subject: " + subject + "\r\n"
                                    "\r\n" + body + "\r\n"
                                 ".\r\n";
    if (!sendData(socket, emailData) || !checkResponse(socket, "250")) {
        qDebug() << "Email data send failed";
        socket.disconnectFromHost();
        return false;
    }

    if (!sendData(socket, "QUIT\r\n") || !checkResponse(socket, "221")) {
        qDebug() << "QUIT command failed";
        socket.disconnectFromHost();
        return false;
    }

    socket.disconnectFromHost();
    lastSendSuccessful = true; // Email sent successfully
    return true;
}

bool EmailSender::connectToServer(QSslSocket &socket)
{
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForEncrypted(10000)) {
        qDebug() << "Connection error:" << socket.errorString();
        return false;
    }
    if (!checkResponse(socket, "220")) {
        qDebug() << "Invalid server greeting";
        return false;
    }
    return true;
}

bool EmailSender::authenticate(QSslSocket &socket)
{
    if (!sendData(socket, "EHLO localhost\r\n") || !checkResponse(socket, "250")) {
        qDebug() << "EHLO failed";
        return false;
    }

    if (!sendData(socket, "AUTH LOGIN\r\n") || !checkResponse(socket, "334")) {
        qDebug() << "AUTH LOGIN failed";
        return false;
    }

    QByteArray emailBase64 = senderEmail.toUtf8().toBase64();
    if (!sendData(socket, emailBase64 + "\r\n") || !checkResponse(socket, "334")) {
        qDebug() << "Email authentication failed";
        return false;
    }

    QByteArray passwordBase64 = senderPassword.toUtf8().toBase64();
    if (!sendData(socket, passwordBase64 + "\r\n") || !checkResponse(socket, "235")) {
        qDebug() << "Password authentication failed";
        return false;
    }

    return true;
}

bool EmailSender::sendData(QSslSocket &socket, const QString &data)
{
    QTextStream stream(&socket);
    stream << data;
    stream.flush();
    return socket.waitForBytesWritten(5000);
}

bool EmailSender::checkResponse(QSslSocket &socket, const QString &expectedCode)
{
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "No response from server";
        return false;
    }
    QByteArray response = socket.readAll();
    qDebug() << "Server response:" << response;
    return response.startsWith(expectedCode.toUtf8());
}
