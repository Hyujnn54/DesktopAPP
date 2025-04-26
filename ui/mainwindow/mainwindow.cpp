// ui/mainwindow/mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

MainWindow::MainWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbConnected(dbConnected),
    clientManager(new ClientManager(dbConnected, this)),
    trainingManager(new TrainingManager(dbConnected, this)),
    notificationManager(new NotificationManager(this))
{
    ui->setupUi(this);
    applyLightTheme();
    setAttribute(Qt::WA_DeleteOnClose);

    // Pass notificationManager to managers
    clientManager->setNotificationManager(notificationManager);
    trainingManager->setNotificationManager(notificationManager);

    setupUiConnections();

    // Connect notification signal
    connect(notificationManager, &NotificationManager::notificationAdded, this, &MainWindow::updateNotificationLabel);
    connect(ui->trainingNotificationLabel, &QPushButton::clicked, this, &MainWindow::on_notificationLabel_clicked);

    if (!m_dbConnected) {
        ui->clientSectionButton->setEnabled(false);
        ui->trainingSectionButton->setEnabled(false);
        ui->statisticsButton->setEnabled(false);
        statusBar()->showMessage("Database not connected. Some features are disabled.");
    } else {
        clientManager->initialize(ui);
        trainingManager->initialize(ui);
        on_clientSectionButton_clicked(); // Default to client section
    }
}

MainWindow::~MainWindow()
{
    delete clientManager;
    delete trainingManager;
    delete notificationManager;
    delete ui;
}

void MainWindow::setupUiConnections()
{
    connect(ui->clientSectionButton, &QPushButton::clicked, this, &MainWindow::on_clientSectionButton_clicked);
    connect(ui->trainingSectionButton, &QPushButton::clicked, this, &MainWindow::on_trainingSectionButton_clicked);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::on_statisticsButton_clicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
}

void MainWindow::on_clientSectionButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open client section: Database is not connected.");
        return;
    }
    ui->mainStackedWidget->setCurrentWidget(ui->clientPage);
    clientManager->refresh();
}

void MainWindow::on_trainingSectionButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open training section: Database is not connected.");
        return;
    }
    ui->mainStackedWidget->setCurrentWidget(ui->trainingPage);
    trainingManager->refresh();
}

void MainWindow::on_statisticsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    clientManager->showStatistics();
}

void MainWindow::toggleSidebar()
{
    ui->sideMenu->setVisible(!ui->sideMenu->isVisible());
}

void MainWindow::toggleTheme()
{
    static bool isDarkTheme = false;
    isDarkTheme = !isDarkTheme;
    isDarkTheme ? applyDarkTheme() : applyLightTheme();
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #A1B8E6); color: #333333; font-family: 'Segoe UI', Arial, sans-serif; }
        QPushButton { background-color: #3A5DAE; color: white; border: 1px solid #2A4682; border-radius: 5px; padding: 6px; font-weight: bold; }
        QPushButton:hover { background-color: #4A70C2; }
        QPushButton:pressed { background-color: #2A4682; }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; padding: 4px; color: #333333; }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus { border: 2px solid #3A5DAE; }
        QTableView { background-color: #FFFFFF; border: 1px solid #D3DCE6; border-radius: 4px; selection-background-color: #A1B8E6; selection-color: #333333; }
        QHeaderView::section { background-color: #3A5DAE; color: white; padding: 5px; border: none; }
        QCalendarWidget { background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; }
        QCalendarWidget QToolButton { background-color: #3A5DAE; color: white; border-radius: 3px; }
        QTabWidget::pane { border: 1px solid #3A5DAE; border-radius: 4px; }
        QTabBar::tab { background-color: #D3DCE6; color: #333333; padding: 6px; border-top-left-radius: 4px; border-top-right-radius: 4px; }
        QTabBar::tab:selected { background-color: #3A5DAE; color: white; }
        QLabel { font-size: 10pt; padding: 2px; }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel {
            font-size: 12pt; font-weight: bold; color: #3A5DAE; text-decoration: underline; padding: 2px; qproperty-alignment: AlignRight; }
        #headerLabel { font-size: 18pt; font-weight: bold; color: #3A5DAE; qproperty-alignment: AlignCenter; }
        #trainingNotificationLabel { font-size: 10pt; font-weight: bold; color: #3A5DAE; }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { border: 2px solid #3A5DAE; border-radius: 5px; }
    )");
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #F28C6F, stop:1 #5C5C5C); color: #F0F0F0; font-family: 'Segoe UI', Arial, sans-serif; }
        QPushButton { background-color: #F28C6F; color: white; border: 1px solid #D96C53; border-radius: 5px; padding: 6px; font-weight: bold; }
        QPushButton:hover { background-color: #F5A38A; }
        QPushButton:pressed { background-color: #D96C53; }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox { background-color: #6A6A6A; border: 1px solid #F28C6F; border-radius: 4px; padding: 4px; color: #F0F0F0; }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus { border: 2px solid #F28C6F; }
        QTableView { background-color: #6A6A6A; border: 1px solid #4A4A4A; border-radius: 4px; selection-background-color: #F28C6F; selection-color: #F0F0F0; }
        QHeaderView::section { background-color: #F28C6F; color: white; padding: 5px; border: none; }
        QCalendarWidget { background-color: #6A6A6A; border: 1px solid #F28C6F; border-radius: 4px; }
        QCalendarWidget QToolButton { background-color: #F28C6F; color: white; border-radius: 3px; }
        QTabWidget::pane { border: 1px solid #F28C6F; border-radius: 4px; }
        QTabBar::tab { background-color: #7A7A7A; color: #F0F0F0; padding: 6px; border-top-left-radius: 4px; border-top-right-radius: 4px; }
        QTabBar::tab:selected { background-color: #F28C6F; color: white; }
        QLabel { font-size: 10pt; padding: 2px; }
        QLabel[formLabel="true"], #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel {
            font-size: 12pt; font-weight: bold; color: #F28C6F; text-decoration: underline; padding: 2px; qproperty-alignment: AlignRight; }
        #headerLabel { font-size: 18pt; font-weight: bold; color: #F28C6F; qproperty-alignment: AlignCenter; }
        #trainingNotificationLabel { font-size: 10pt; font-weight: bold; color: #F28C6F; }
        QFrame#header, QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { border: 2px solid #F28C6F; border-radius: 5px; }
    )");
}

void MainWindow::updateNotificationLabel(int count)
{
    ui->trainingNotificationLabel->setText(QString("%1 modifications").arg(count));
}

void MainWindow::on_notificationLabel_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Notification History");
    dialog->resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QTextEdit *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    QPushButton *closeButton = new QPushButton("Close", dialog);

    layout->addWidget(textEdit);
    layout->addWidget(closeButton);

    QString notificationText;
    for (const auto &notification : notificationManager->getNotifications()) {
        notificationText += QString("[%1] %2\nLocation: %3\nLine: %4\nDetails: %5\n\n")
        .arg(notification.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(notification.action)
            .arg(notification.location)
            .arg(notification.lineNumber)
            .arg(notification.details);
    }
    textEdit->setText(notificationText.isEmpty() ? "No notifications available." : notificationText);

    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    dialog->exec();
    delete dialog;
}
