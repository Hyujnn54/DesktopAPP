// ui/mainwindow/mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "managers/meeting/meeting.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbConnected(dbConnected),
    clientManager(new ClientManager(dbConnected, this)),
    trainingManager(new TrainingManager(dbConnected, this)),
    meetingManager(new MeetingManager(dbConnected, this)),
    notificationManager(new NotificationManager(this)),
    networkManager(new QNetworkAccessManager(this)),
    chartWindow(new ChartWindow(this))
{
    qDebug() << "Entering MainWindow constructor";

    // Block signals during UI setup
    this->blockSignals(true);

    ui->setupUi(this);
    applyLightTheme();
    setAttribute(Qt::WA_DeleteOnClose);

    clientManager->setNotificationManager(notificationManager);
    trainingManager->setNotificationManager(notificationManager);
    meetingManager->setNotificationManager(notificationManager);

    setupUiConnections();

    // Connect networkManager and AI chat signals after UI setup
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAIResponseReceived);
    connect(ui->trainingNotificationLabel, &QPushButton::clicked, this, &MainWindow::handleNotificationLabelClicked);

    // Move appendChatMessage after all setup
    appendChatMessage("Hello! I'm your Meeting Assistant. How can I help you today?", true);

    if (!m_dbConnected) {
        ui->clientSectionButton->setEnabled(false);
        ui->trainingSectionButton->setEnabled(false);
        ui->meetingSectionButton->setEnabled(false);
        ui->statisticsButton->setEnabled(false);
        statusBar()->showMessage("Database not connected. Some features are disabled.");
    } else {
        clientManager->initialize(ui);
        trainingManager->initialize(ui);
        meetingManager->initialize(ui);
        on_meetingSectionButton_clicked();
    }

    // Re-enable signals after all setup
    this->blockSignals(false);

    qDebug() << "Exiting MainWindow constructor";
}

MainWindow::~MainWindow()
{
    delete clientManager;
    delete trainingManager;
    delete meetingManager;
    delete notificationManager;
    delete networkManager;
    delete chartWindow;
    delete ui;
}

void MainWindow::setupUiConnections()
{
    qDebug() << "Setting up UI connections";
    connect(ui->clientSectionButton, &QPushButton::clicked, this, &MainWindow::on_clientSectionButton_clicked);
    connect(ui->trainingSectionButton, &QPushButton::clicked, this, &MainWindow::on_trainingSectionButton_clicked);
    connect(ui->meetingSectionButton, &QPushButton::clicked, this, &MainWindow::on_meetingSectionButton_clicked);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::on_statisticsButton_clicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->meetingChatSendButton, &QPushButton::clicked, this, &MainWindow::on_chatSendButton_clicked);
    connect(ui->meetingChatClearButton, &QPushButton::clicked, this, &MainWindow::on_chatClearButton_clicked);
}

void MainWindow::on_clientSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->clientPage);
    clientManager->refresh();
}

void MainWindow::on_trainingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->trainingPage);
    trainingManager->refresh();
}

void MainWindow::on_meetingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->meetingPage);
    meetingManager->refreshTableWidget();
}

void MainWindow::on_statisticsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    
    try {
        // Show chart window with error handling
        chartWindow->show();
        
        // Find the combo box with error handling
        QComboBox* statsComboBox = chartWindow->findChild<QComboBox*>("statsTypeComboBox");
        if (!statsComboBox) {
            qDebug() << "Error: statsTypeComboBox not found in ChartWindow";
            return;
        }
        
        // Set to default statistics type that's less likely to crash
        int meetingStatsIndex = statsComboBox->findText("Meeting Statistics");
        if (meetingStatsIndex >= 0) {
            statsComboBox->setCurrentIndex(meetingStatsIndex);
        } else {
            // If "Meeting Statistics" not found, use the first item
            if (statsComboBox->count() > 0) {
                statsComboBox->setCurrentIndex(0);
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in on_statisticsButton_clicked: " << e.what();
        QMessageBox::critical(this, "Error", "An error occurred opening statistics: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in on_statisticsButton_clicked";
        QMessageBox::critical(this, "Error", "An unknown error occurred opening statistics");
    }
}

void MainWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::toggleTheme()
{
    if (ui->themeButton->text() == "Dark Theme") {
        applyDarkTheme();
        ui->themeButton->setText("Light Theme");
    } else {
        applyLightTheme();
        ui->themeButton->setText("Dark Theme");
    }
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #A1B8E6); color: #333333; font-family: 'Segoe UI', Arial, sans-serif; }
        QPushButton { background-color: #3A5DAE; color: white; border: 1px solid #2A4682; border-radius: 5px; padding: 6px; font-weight: bold; }
        QPushButton:hover { background-color: #4A70C2; }
        QPushButton:pressed { background-color: #2A4682; }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox, QDoubleSpinBox { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; padding: 4px; color: #333333; }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #3A5DAE; }
        QTableView, QTableWidget { background-color: #FFFFFF; border: 1px solid #D3DCE6; border-radius: 4px; selection-background-color: #A1B8E6; selection-color: #333333; }
        QHeaderView::section { background-color: #3A5DAE; color: white; padding: 5px; border: none; }
        QCalendarWidget { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; }
        QCalendarWidget QToolButton { background-color: #3A5DAE; color: white; border-radius: 3px; }
        QTabWidget::pane { border: 1px solid #3A5DAE; border-radius: 4px; }
        QTabBar::tab { background-color: #D3DCE6; color: #333333; padding: 6px; border-top-left-radius: 4px; border-top-right-radius: 4px; }
        QTabBar::tab:selected { background-color: #3A5DAE; color: white; }
        QTextEdit { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; color: #333333; }
        QChartView { background-color: #FFFFFF; border: 1px solid #D3DCE6; border-radius: 4px; }
        QLabel { font-size: 10pt; padding: 2px; }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel,
        #meetingTitleLabel, #meetingOrganiserLabel, #meetingParticipantLabel, #meetingAgendaLabel, #meetingDurationLabel, #meetingDateLabel {
            font-size: 12pt; font-weight: bold; color: #3A5DAE; text-decoration: underline; padding: 2px; qproperty-alignment: AlignRight; }
        #headerLabel { font-size: 18pt; font-weight: bold; color: #3A5DAE; qproperty-alignment: AlignCenter; }
        #trainingNotificationLabel { font-size: 10pt; font-weight: bold; color: #3A5DAE; }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { border: 2px solid #3A5DAE; border-radius: 5px; }
        QFrame#sideMenu { background-color: #E6ECF5; }
    )");
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #F28C6F, stop:1 #5C5C5C); color: #F0F0F0; font-family: 'Segoe UI', Arial, sans-serif; }
        QPushButton { background-color: #F28C6F; color: white; border: 1px solid #D96C53; border-radius: 5px; padding: 6px; font-weight: bold; }
        QPushButton:hover { background-color: #F5A38A; }
        QPushButton:pressed { background-color: #D96C53; }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox, QDoubleSpinBox { background-color: #6A6A6A; border: 1px solid #F28C6F; border-radius: 4px; padding: 4px; color: #F0F0F0; }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 2px solid #F28C6F; }
        QTableView, QTableWidget { background-color: #6A6A6A; border: 1px solid #4A4A4A; border-radius: 4px; selection-background-color: #F28C6F; selection-color: #F0F0F0; }
        QHeaderView::section { background-color: #F28C6F; color: white; padding: 5px; border: none; }
        QCalendarWidget { background-color: #6A6A6A; border: 1px solid #F28C6F; border-radius: 4px; }
        QCalendarWidget QToolButton { background-color: #F28C6F; color: white; border-radius: 3px; }
        QTabWidget::pane { border: 1px solid #F28C6F; border-radius: 4px; }
        QTabBar::tab { background-color: #7A7A7A; color: #F0F0F0; padding: 6px; border-top-left-radius: 4px; border-top-right-radius: 4px; }
        QTabBar::tab:selected { background-color: #F28C6F; color: white; }
        QTextEdit { background-color: #6A6A6A; border: 1px solid #F28C6F; border-radius: 4px; color: #F0F0F0; }
        QChartView { background-color: #6A6A6A; border: 1px solid #4A4A4A; border-radius: 4px; }
        QLabel { font-size: 10pt; padding: 2px; }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel,
        #meetingTitleLabel, #meetingOrganiserLabel, #meetingParticipantLabel, #meetingAgendaLabel, #meetingDurationLabel, #meetingDateLabel {
            font-size: 12pt; font-weight: bold; color: #F28C6F; text-decoration: underline; padding: 2px; qproperty-alignment: AlignRight; }
        #headerLabel { font-size: 18pt; font-weight: bold; color: #F28C6F; qproperty-alignment: AlignCenter; }
        #trainingNotificationLabel { font-size: 10pt; font-weight: bold; color: #F28C6F; }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { border: 2px solid #F28C6F; border-radius: 5px; }
        QFrame#sideMenu { background-color: #7A7A7A; }
    )");
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

void MainWindow::handleNotificationLabelClicked()
{
    const QVector<NotificationManager::Notification>& notifications = notificationManager->getNotifications();
    if (notifications.isEmpty()) {
        QMessageBox::information(this, "Notifications", "No new notifications.");
        return;
    }

    // Create a dialog to display notifications
    QDialog dialog(this);
    dialog.setWindowTitle("Notifications");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Add a scrollable area for notifications
    QScrollArea *scrollArea = new QScrollArea(&dialog);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    for (const NotificationManager::Notification &notif : notifications) {
        QString notificationText = QString("%1\n%2\n%3")
        .arg(notif.title)
            .arg(notif.description)
            .arg(notif.details);
        QLabel *label = new QLabel(notificationText, scrollWidget);
        label->setWordWrap(true);
        label->setStyleSheet("QLabel { border-bottom: 1px solid #d3d3d3; padding: 5px; }");
        scrollLayout->addWidget(label);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    // Add a "Clear Notifications" button
    QPushButton *clearButton = new QPushButton("Clear Notifications", &dialog);
    connect(clearButton, &QPushButton::clicked, this, [this, &dialog]() {
        notificationManager->clearNotifications();
        dialog.accept();
    });
    layout->addWidget(clearButton);

    // Add a "Close" button
    QPushButton *closeButton = new QPushButton("Close", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog.setLayout(layout);
    dialog.resize(400, 300);
    dialog.exec();

    // Update the notification label after the dialog is closed
    updateNotificationLabel(notificationManager->getNotificationCount());
}

void MainWindow::updateNotificationLabel(int count)
{
    ui->trainingNotificationLabel->setText(QString("Notifications: %1").arg(count));
}

void MainWindow::on_chatSendButton_clicked()
{
    QString input = ui->meetingChatInputLineEdit->text().trimmed();
    if (input.isEmpty()) {
        return;
    }
    appendChatMessage(input);
    processUserInput(input);
    ui->meetingChatInputLineEdit->clear();
}

void MainWindow::on_chatClearButton_clicked()
{
    ui->meetingChatInputLineEdit->clear();
    ui->meetingChatTextEdit->clear();
}

void MainWindow::appendChatMessage(const QString &message, bool isBot)
{
    qDebug() << "Entering appendChatMessage, message:" << message << ", isBot:" << isBot;
    if (!ui || !ui->meetingChatTextEdit) {
        qDebug() << "Error: ui or meetingChatTextEdit is null";
        return;
    }

    QString formattedMessage = QString("[%1] %2: %3\n")
                                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss"),
                                        isBot ? "Assistant" : "User", message);
    ui->meetingChatTextEdit->append(formattedMessage);

    if (!isBot) {
        qDebug() << "Network request disabled for testing";
        ui->meetingChatTextEdit->append("[System] Network requests are disabled for testing.");
        // Uncomment and configure when API is ready
        /*
        QUrl url("https://api.example.com/chat");
        if (!url.isValid()) {
            qDebug() << "Error: Invalid URL in appendChatMessage:" << url.toString();
            ui->meetingChatTextEdit->append("[Error] Invalid API URL. Please check the configuration.");
            return;
        }

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["message"] = message;
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        QNetworkReply *reply = networkManager->post(request, data);
        if (!reply) {
            qDebug() << "Error: Failed to create network reply";
            ui->meetingChatTextEdit->append("[Error] Failed to send message to API.");
        } else {
            connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError code) {
                qDebug() << "Network error in appendChatMessage:" << reply->errorString();
                ui->meetingChatTextEdit->append("[Error] Network error: " + reply->errorString());
            });
        }
        */
    }
    qDebug() << "Exiting appendChatMessage";
}

void MainWindow::processUserInput(const QString &input)
{
    static const QRegularExpression addMeetingRegex(R"(add meeting\s+(.+?)\s+by\s+(.+?)\s+with\s+(.+?)\s+about\s+(.+?)\s+for\s+(\d+)\s+min\s+on\s+(.+))", QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression deleteMeetingRegex(R"(delete meeting\s+(\d+))", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch addMatch = addMeetingRegex.match(input);
    QRegularExpressionMatch deleteMatch = deleteMeetingRegex.match(input);

    if (addMatch.hasMatch()) {
        QStringList parts = addMatch.capturedTexts();
        QString errorMessage;
        if (!validateMeetingInput(parts, errorMessage)) {
            appendChatMessage("Error: " + errorMessage, true);
            return;
        }

        meeting m = createMeetingFromInput(input);
        if (m.add()) {
            meetingManager->refreshTableWidget();
            appendChatMessage("Meeting added successfully!", true);
            if (notificationManager) {
                notificationManager->addNotification("Meeting Added", "Meeting: " + m.getTitle(), "Added meeting via chat", -1);
            }
        } else {
            appendChatMessage("Failed to add meeting. Check database connection.", true);
        }
    } else if (deleteMatch.hasMatch()) {
        int id = deleteMatch.captured(1).toInt();
        meeting m;
        if (m.deleteMeeting(id)) {
            meetingManager->refreshTableWidget();
            appendChatMessage("Meeting deleted successfully!", true);
            if (notificationManager) {
                notificationManager->addNotification("Meeting Deleted", "Meeting ID: " + QString::number(id), "Deleted meeting via chat", -1);
            }
        } else {
            appendChatMessage("Failed to delete meeting. Check database connection.", true);
        }
    } else {
        QNetworkRequest request(QUrl("http://api.example.com/chatbot"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["message"] = input;
        QJsonDocument doc(json);
        networkManager->post(request, doc.toJson());
    }
}

meeting MainWindow::createMeetingFromInput(const QString &input)
{
    static const QRegularExpression addMeetingRegex(R"(add meeting\s+(.+?)\s+by\s+(.+?)\s+with\s+(.+?)\s+about\s+(.+?)\s+for\s+(\d+)\s+min\s+on\s+(.+))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = addMeetingRegex.match(input);

    QString title = match.captured(1);
    QString organiser = match.captured(2);
    QString participant = match.captured(3);
    QString agenda = match.captured(4);
    int duration = match.captured(5).toInt();
    QDateTime dateTime = QDateTime::fromString(match.captured(6), "yyyy-MM-dd hh:mm");

    // Use -1 as temporary ID, using the constructor compatible with meeting.h
    return meeting(-1, title, organiser, participant, agenda, duration, dateTime);
}

bool MainWindow::validateMeetingInput(const QStringList &parts, QString &errorMessage)
{
    if (parts.size() != 7) {
        errorMessage = "Invalid input format. Use: add meeting <title> by <organiser> with <participant> about <agenda> for <duration> min on <date>";
        return false;
    }

    bool ok;
    int duration = parts[5].toInt(&ok);
    if (!ok || duration <= 0) {
        errorMessage = "Duration must be a positive number.";
        return false;
    }

    QDateTime dateTime = QDateTime::fromString(parts[6], "yyyy-MM-dd hh:mm");
    if (!dateTime.isValid() || dateTime < QDateTime::currentDateTime()) {
        errorMessage = "Date and time must be valid and in the future.";
        return false;
    }

    return true;
}

void MainWindow::onAIResponseReceived(QNetworkReply *reply)
{
    qDebug() << "Entering onAIResponseReceived";
    if (!reply) {
        qDebug() << "Error: Network reply is null";
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error:" << reply->errorString();
        appendChatMessage("Error: " + reply->errorString(), true);
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    if (doc.isNull()) {
        qDebug() << "Error: Failed to parse JSON response";
        appendChatMessage("Error: Failed to parse API response.", true);
        reply->deleteLater();
        return;
    }

    QJsonObject json = doc.object();
    QString botResponse = json.value("response").toString();
    if (botResponse.isEmpty()) {
        qDebug() << "Error: Empty response from API";
        appendChatMessage("Error: Empty response from API.", true);
    } else {
        appendChatMessage(botResponse, true);
    }

    reply->deleteLater();
    qDebug() << "Exiting onAIResponseReceived";
}

ChartWindow* MainWindow::getChartWindow() const
{
    return chartWindow;
}
