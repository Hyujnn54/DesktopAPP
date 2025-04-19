#include "sign.h"
#include "ui_sign.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"
#include <QSettings>
#include <opencv2/opencv.hpp>
#include <QFile>
sign::sign(QWidget *parent) : QMainWindow(parent), ui(new Ui::sign)
{
    ui->setupUi(this);
    connect(ui->signBtn, &QPushButton::clicked, this, &sign::handleLogin);
    bool connected = connect(ui->faceIdBtn, &QPushButton::clicked, this, &sign::faceId);
    qDebug() << "Face ID button connection status:" << (connected ? "Success" : "Failed");
    loadLoginCredentials();

    // Load Haar cascade
    QString cascadePath = "C:/opencv/build/install/etc/haarcascades/haarcascade_frontalface_default.xml";
    if (!faceCascade.load(cascadePath.toStdString())) {
        qDebug() << "Failed to load Haar cascade from" << cascadePath;
        QMessageBox::critical(this, "Error", "Failed to load face detection cascade.");
    }

    // Initialize the recognizer
    recognizer = cv::face::LBPHFaceRecognizer::create();
    // Load face data
    if (!loadFaceData()) {
        qDebug() << "Failed to load face data from database";
        QMessageBox::warning(this, "Warning", "Face recognition unavailable due to database error.");
    }
}

sign::~sign()
{
    delete ui;
}



void sign::handleLogin()
{
    QString email = ui->lineEdit_us->text();
    QString cin = ui->lineEdit_pwd->text();

    if (email.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both email and CIN.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ROLE FROM EMPLOYEE WHERE EMAIL = :email AND CIN = :cin");
    query.bindValue(":email", email);
    query.bindValue(":cin", cin);

    if (query.exec() && query.next()) {
        loggedInRole = query.value("ROLE").toString(); // Store the role
        QMessageBox::information(this, "Login Successful", "Welcome!");
        // Create MainWindow without passing 'this' as parent to avoid immediate deletion
        MainWindow *mainWindow = new MainWindow(nullptr); // Use nullptr as parent
        mainWindow->setLoggedInRole(loggedInRole); // Pass role to MainWindow
        mainWindow->show();
        // Hide sign window instead of closing to keep the application running
        this->hide(); // Use hide() instead of close()
    } else {
        QMessageBox::critical(this, "Login Failed", "Invalid email or CIN.");
    }

    if (ui->rememberMeCheckBox->isChecked()) {
        saveLoginCredentials(email, cin, true);
    } else {
        saveLoginCredentials("", "", false);
    }
}
bool sign::loadFaceData()
{
    QSqlQuery query;
    query.prepare("SELECT ID, CIN, IMAGE FROM EMPLOYEE WHERE IMAGE IS NOT NULL");
    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    std::vector<cv::Mat> images;
    std::vector<int> labels;

    while (query.next()) {
        int id = query.value("ID").toInt();
        QString cin = query.value("CIN").toString();
        QByteArray imageData = query.value("IMAGE").toByteArray();

        // Convert the raw bytes to a cv::Mat
        std::vector<uchar> buffer(imageData.begin(), imageData.end());
        cv::Mat image = cv::imdecode(buffer, cv::IMREAD_GRAYSCALE);
        if (image.empty()) {
            qDebug() << "Failed to decode image for ID:" << id << "CIN:" << cin;
            continue;
        }

        // Detect face in the image
        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(image, faces, 1.1, 3, 0, cv::Size(30, 30));
        if (faces.empty()) {
            qDebug() << "No face detected for ID:" << id << "CIN:" << cin;
            continue;
        }

        // Use the first detected face
        cv::Mat faceROI = image(faces[0]);
        images.push_back(faceROI);
        labels.push_back(id);
        idToCinMap[id] = cin;
    }

    if (images.empty()) {
        qDebug() << "No valid face images found in database";
        return false;
    }

    recognizer->train(images, labels);
    qDebug() << "Face recognizer trained with" << images.size() << "images";
    return true;
}
void sign::faceId()
{
    qDebug() << "faceId() called";

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        qDebug() << "Failed to open camera in faceId()";
        QMessageBox::critical(this, "Camera Error", "Failed to open the camera.");
        return;
    }
    qDebug() << "Camera opened successfully in faceId()";

    if (idToCinMap.isEmpty()) {
        qDebug() << "Face recognizer not trained, no valid data loaded";
        QMessageBox::critical(this, "Face ID Error", "Face recognition unavailable: no valid face data in database.");
        cap.release();
        return;
    }

    cv::Mat frame, gray;
    bool faceDetected = false;
    int id = -1;
    double confidence = 0.0;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            qDebug() << "Failed to capture frame in faceId()";
            QMessageBox::critical(this, "Camera Error", "Failed to capture an image from the camera.");
            break;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

        id = -1;
        confidence = 0.0;
        faceDetected = false;

        if (!faces.empty()) {
            cv::Mat faceROI = gray(faces[0]);
            recognizer->predict(faceROI, id, confidence);

            cv::rectangle(frame, faces[0], cv::Scalar(0, 255, 0), 2);
            QString label = QString("ID: %1, Confidence: %2").arg(id).arg(confidence);
            cv::putText(frame, label.toStdString(), cv::Point(faces[0].x, faces[0].y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);

            qDebug() << "Face detected - ID:" << id << "Confidence:" << confidence;

            if (confidence < 50.0) {
                faceDetected = true;
            }
        }

        cv::imshow("Face ID - Press 'q' to quit", frame);
        int key = cv::waitKey(30);
        if (key == 'q' || key == 27) {
            qDebug() << "User quit the camera feed";
            break;
        }

        if (faceDetected) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    qDebug() << "Camera feed closed in faceId()";

    if (faceDetected && idToCinMap.contains(id)) {
        QString cin = idToCinMap[id];
        QSqlQuery query;
        query.prepare("SELECT EMAIL, ROLE FROM EMPLOYEE WHERE CIN = :cin"); // Added ROLE
        query.bindValue(":cin", cin);
        if (query.exec() && query.next()) {
            QString email = query.value("EMAIL").toString();
            loggedInRole = query.value("ROLE").toString(); // Store the role
            qDebug() << "Logging in - CIN:" << cin << "Email:" << email << "Role:" << loggedInRole;
            QMessageBox::information(this, "Face ID Successful", "Welcome!");
            MainWindow *mainWindow = new MainWindow(nullptr); // Use nullptr as parent
            mainWindow->setLoggedInRole(loggedInRole); // Pass role to MainWindow
            mainWindow->show();
            this->hide(); // Use hide() to match handleLogin()

            if (ui->rememberMeCheckBox->isChecked()) {
                saveLoginCredentials(email, cin, true);
            }
        } else {
            qDebug() << "Face recognized but no matching employee found for CIN:" << cin;
            QMessageBox::critical(this, "Login Failed", "Face recognized but no matching employee found.");
        }
    } else {
        qDebug() << "Face ID failed - Detected:" << faceDetected << "ID in map:" << idToCinMap.contains(id);
        QMessageBox::critical(this, "Face ID Failed", "No matching face found in the database.");
    }
}

void sign::saveLoginCredentials(const QString &email, const QString &cin, bool remember)
{
    QSettings settings("YourCompany", "ConsultingApp");
    if (remember) {
        settings.setValue("email", email);
        settings.setValue("cin", cin);
    } else {
        settings.remove("email");
        settings.remove("cin");
    }
}

void sign::loadLoginCredentials()
{
    QSettings settings("YourCompany", "ConsultingApp");
    QString savedEmail = settings.value("email").toString();
    QString savedCin = settings.value("cin").toString();

    if (!savedEmail.isEmpty() && !savedCin.isEmpty()) {
        ui->lineEdit_us->setText(savedEmail);
        ui->lineEdit_pwd->setText(savedCin);
        ui->rememberMeCheckBox->setChecked(true);
    }
}
