#ifndef SIGN_H
#define SIGN_H

#include <QMainWindow>
// Commented out OpenCV includes
// #include <opencv2/opencv.hpp>
// #include <opencv2/face.hpp>
#include <QSettings>
#include <QVector>
#include <QMap>
#include <QString>

namespace Ui {
class sign;
}

class sign : public QMainWindow
{
    Q_OBJECT

public:
    explicit sign(QWidget *parent = nullptr);
    ~sign();
    void saveLoginCredentials(const QString &email, const QString &cin, bool remember); // Updated
    void loadLoginCredentials();
    QString getLoggedInRole() const { return loggedInRole; }
private slots :
    void handleLogin();
    void faceId();

private:
    Ui::sign *ui;
    // Commented out OpenCV related members
    // cv::CascadeClassifier faceCascade;
    // cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer;
    QMap<int, QString> idToCinMap;
    QString loggedInRole;
    bool loadFaceData();
};

#endif // SIGN_H
