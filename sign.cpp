#include "sign.h"
#include "ui_sign.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"
#include <QSettings>
// Commented out OpenCV includes
// #include <opencv2/opencv.hpp>
#include <QFile>
sign::sign(QWidget *parent) : QMainWindow(parent), ui(new Ui::sign)
{
    ui->setupUi(this);
    connect(ui->signBtn, &QPushButton::clicked, this, &sign::handleLogin);
    // Disable face ID button connection
    // connect(ui->faceIdBtn, &QPushButton::clicked, this, &sign::faceId);
    loadLoginCredentials();

    // Disabled OpenCV code
    /*
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
    */
    
    // Disable Face ID button to prevent user from attempting to use it
    if (ui->faceIdBtn) {
        ui->faceIdBtn->setEnabled(false);
        ui->faceIdBtn->setToolTip("Face ID temporarily disabled");
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
        // Create MainWindow with proper parameters
        MainWindow *mainWindow = new MainWindow(true, nullptr); // Correction: passer true comme premier paramètre
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

// Disabled OpenCV-dependent functions
/*
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
*/

// Replace with dummy implementation
bool sign::loadFaceData()
{
    // Disabled implementation
    return false;
}

// Replace with dummy implementation
void sign::faceId()
{
    QMessageBox::information(this, "Face ID Disabled", "Face ID functionality is temporarily disabled.");
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
