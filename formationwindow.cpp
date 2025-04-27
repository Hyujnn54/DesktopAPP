#include "formationwindow.h"
#include "ui_formationwindow.h"
#include "formations.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTableView>
#include "updatetrainingdialog.h"
#include <QRegularExpression>
#include <QDateTime>
#include <QSqlError>
#include <QGridLayout>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>

FormationWindow::FormationWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::formationwindow), isDarkTheme(false),
    tableModel(nullptr), proxyModel(new QSortFilterProxyModel(this)),
    notificationCount(0), arduino(nullptr), arduinoTimer(new QTimer(this)),
    arduinoDataBuffer("")
{
    ui->setupUi(this);
    applyLightTheme();

    ui->date->setDate(QDate::currentDate());

    // Initialize notification label
    ui->notificationLabel->setText("Notifications: 0");
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");

    // Initialize setText QPlainTextEdit
    ui->setText->setPlainText("Waiting Room: 0");
    ui->setText->setStyleSheet("font-weight: bold; color: #3A5DAE; background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; padding: 4px;");

    // Connections
    QObject::connect(ui->add, SIGNAL(clicked()), this, SLOT(on_addButtonclicked()));
    QObject::connect(ui->deletef, SIGNAL(clicked()), this, SLOT(on_deleteButtonClicked()));
    QObject::connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(on_updateButtonClicked()));
    connect(ui->themeButton, &QPushButton::clicked, this, &FormationWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &FormationWindow::toggleSidebar);
    connect(ui->exportButton, &QPushButton::clicked, this, &FormationWindow::exportToPdf);
    connect(ui->notificationLabel, &QLabel::linkActivated, this, &FormationWindow::onNotificationLabelClicked);
    connect(ui->refreshStatsButton, &QPushButton::clicked, this, &FormationWindow::on_refreshStatsButton_clicked);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &FormationWindow::on_searchInput_textChanged);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &FormationWindow::on_resetSearchButton_clicked);
    connect(ui->wrr, &QPushButton::clicked, this, &FormationWindow::on_wrr_clicked);
    // New connection

    // Validators
    QRegularExpression formationRx("[A-Za-z0-9\\s]{1,50}");
    QRegularExpression descriptionRx("[A-Za-z0-9\\s,.!?]{1,200}");
    QRegularExpression trainerRx("[A-Za-z\\s]{1,50}");
    QRegularExpression phoneRx("\\+\\d{10,15}");

    QValidator *formationValidator = new QRegularExpressionValidator(formationRx, this);
    QValidator *descriptionValidator = new QRegularExpressionValidator(descriptionRx, this);
    QValidator *trainerValidator = new QRegularExpressionValidator(trainerRx, this);
    QValidator *phoneValidator = new QRegularExpressionValidator(phoneRx, this);

    ui->format->setValidator(formationValidator);
    ui->des->setValidator(descriptionValidator);
    ui->tr->setValidator(trainerValidator);
    ui->phoneNumberInput->setValidator(phoneValidator);

    ui->timeb->setRange(1, 30);
    ui->prixb->setRange(1, 1000);

    ui->tabtr->setSortingEnabled(true);

    // Initialize network manager
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &FormationWindow::onSmsRequestFinished);

    // Verify database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        qDebug() << "Database connection not established";
        QMessageBox::critical(this, "Database Error", "No active database connection. Please configure the database.");
    } else {
        qDebug() << "Database connected:" << db.databaseName();
    }

    // Initialize Arduino
    setupArduino();

    // Connect timer to read Arduino data every 5 seconds
    connect(arduinoTimer, &QTimer::timeout, this, &FormationWindow::readArduinoData);
    arduinoTimer->start(1000); // Poll every 1 second

    refreshTableView();
    updateStatistics();
    updateWaitingRoomCount();
}

FormationWindow::~FormationWindow()
{
    if (arduino) {
        arduino->closeArduino();
        delete arduino;
    }
    delete tableModel;
    delete ui;
}

void FormationWindow::setupArduino()
{
    arduino = new Arduino(this);
    int result = arduino->connectArduino();
    if (result == 0) {
        qDebug() << "Arduino connected successfully on port:" << arduino->getSerial()->portName();
        // Increase read buffer size to handle larger messages
        arduino->getSerial()->setReadBufferSize(1024);
    } else {
        qDebug() << "Failed to connect to Arduino. Result:" << result;
        ui->setText->setPlainText("Waiting Room: Arduino Disconnected");
        ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #D93025; border: 1px solid #D93025; border-radius: 4px; padding: 4px;");
        QMessageBox::warning(this, "Arduino Error", "Could not connect to Arduino. Check connection and try again.");
    }
}
void FormationWindow::readArduinoData()
{
    static QDateTime lastDataTime = QDateTime::currentDateTime();
    static QSet<unsigned long> processedMessageIds; // Track processed message IDs
    const int maxProcessedIds = 100; // Limit stored IDs

    if (!arduino || !arduino->getSerial()->isOpen()) {
        qDebug() << "Arduino not connected or serial port not open";
        ui->setText->setPlainText("Waiting Room: Arduino Disconnected");
        ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #D93025; border: 1px solid #D93025; border-radius: 4px; padding: 4px;");
        updateWaitingRoomCount();
        lastDataTime = QDateTime::currentDateTime();
        return;
    }

    QByteArray data = arduino->readFromArduino();
    if (data.isEmpty()) {
        qDebug() << "No data received from Arduino";
        if (lastDataTime.secsTo(QDateTime::currentDateTime()) > 30) {
            qDebug() << "Arduino communication timeout";
            updateNotificationCount(1, "Arduino communication timeout");
            ui->setText->setPlainText("Waiting Room: Arduino Timeout");
            ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #D93025; border: 1px solid #D93025; border-radius: 4px; padding: 4px;");
        }
        return;
    }

    lastDataTime = QDateTime::currentDateTime();
    arduinoDataBuffer.append(QString::fromUtf8(data));
    qDebug() << "Appended to buffer. Current Arduino data buffer:" << arduinoDataBuffer;

    while (true) {
        int newlineIndex = arduinoDataBuffer.indexOf('\n');
        if (newlineIndex == -1) {
            qDebug() << "No complete line found. Waiting for more data. Buffer:" << arduinoDataBuffer;
            break;
        }

        QString line = arduinoDataBuffer.left(newlineIndex).remove('\r').trimmed();
        arduinoDataBuffer.remove(0, newlineIndex + 1);

        if (line.isEmpty()) {
            qDebug() << "Empty line received, skipping";
            continue;
        }

        // Extract message ID
        unsigned long messageId = 0;
        bool hasValidId = false;
        if (line.contains(" ID: ")) {
            QStringList parts = line.split(" ID: ");
            if (parts.size() == 2) {
                bool ok;
                messageId = parts[1].toULong(&ok);
                if (ok) {
                    hasValidId = true;
                    line = parts[0]; // Process the main message
                }
            }
        }

        // Check for duplicate message ID
        if (hasValidId && processedMessageIds.contains(messageId)) {
            qDebug() << "Duplicate Arduino message ID ignored:" << messageId << "Message:" << line;
            continue;
        }

        // Store message ID
        if (hasValidId) {
            processedMessageIds.insert(messageId);
            if (processedMessageIds.size() > maxProcessedIds) {
                processedMessageIds.remove(*processedMessageIds.begin()); // Remove oldest
            }
        }

        qDebug() << "Processing Arduino message:" << line << "ID:" << (hasValidId ? QString::number(messageId) : "None");

        if (line.startsWith("Person detected. Total: ")) {
            QString countStr = line.mid(23).trimmed();
            bool ok;
            int count = countStr.toInt(&ok);
            if (ok && count >= 0 && count <= 5) {
                qDebug() << "Parsed person detected. Count:" << count;
                updateMeetingWR(count);
                updateWaitingRoomCount();
                updateNotificationCount(1, QString("Person detected. Waiting room count: %1").arg(count));
            } else {
                qDebug() << "Invalid count in Person detected message:" << line;
                updateNotificationCount(1, "Invalid Arduino count: " + line);
            }
        } else if (line.startsWith("Counter reset due to inactivity. Total: ")) {
            QString countStr = line.mid(39).trimmed();
            bool ok;
            int count = countStr.toInt(&ok);
            if (ok && count >= 0 && count <= 5) {
                qDebug() << "Parsed counter reset. Count:" << count;
                updateMeetingWR(count);
                updateWaitingRoomCount();
                updateNotificationCount(1, QString("Counter reset. Waiting room count: %1").arg(count));
            } else {
                qDebug() << "Invalid count in Counter reset message:" << line;
                updateNotificationCount(1, "Invalid Arduino count: " + line);
            }
        } else if (line.startsWith("Received WR count:") || line == "Green LED ON" || line == "Green LED OFF") {
            qDebug() << "Arduino response:" << line;
        } else if (line.startsWith("Invalid") || line.startsWith("Unknown command")) {
            qDebug() << "Arduino error:" << line;
            updateNotificationCount(1, "Arduino error: " + line);
        } else {
            qDebug() << "Unrecognized Arduino message:" << line;
            updateNotificationCount(1, "Unrecognized Arduino message: " + line);
        }
    }

    if (arduinoDataBuffer.size() > 1024) {
        qDebug() << "Buffer size exceeded 1KB, clearing. Buffer was:" << arduinoDataBuffer;
        updateNotificationCount(1, "Arduino buffer overflow, cleared");
        arduinoDataBuffer.clear();
    }

    if (!arduinoDataBuffer.isEmpty()) {
        qDebug() << "Partial Arduino data remaining:" << arduinoDataBuffer;
    }
}
void FormationWindow::updateMeetingWR(int count)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not connected";
        QMessageBox::critical(this, "Database Error", "Database connection lost. Please reconnect.");
        return;
    }

    QSqlQuery query;

    // Check if meeting with ID = 1 exists
    query.prepare("SELECT COUNT(*) FROM AHMED.MEETING WHERE ID = 1");
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to check existence of meeting ID 1:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to verify meeting existence: " + query.lastError().text());
        return;
    }

    int meetingCount = query.value(0).toInt();
    if (meetingCount == 0) {
        qDebug() << "Meeting with ID 1 does not exist";
        // Create a default record
        query.prepare("INSERT INTO AHMED.MEETING (ID, TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, WR) "
                      "VALUES (1, 'Default Meeting', 'System', 'None', 'Auto-generated', 60, 0)");
        if (!query.exec()) {
            qDebug() << "Failed to create default meeting ID 1:" << query.lastError().text();
            QMessageBox::critical(this, "Database Error", "Failed to create default meeting: " + query.lastError().text());
            return;
        }
        qDebug() << "Created default meeting with ID 1";
        updateNotificationCount(1, "Created default meeting ID 1");
    }

    // Update WR for meeting with ID = 1
    query.prepare("UPDATE AHMED.MEETING SET WR = :count WHERE ID = 1");
    query.bindValue(":count", count);

    if (!query.exec()) {
        qDebug() << "Failed to update WR in MEETING table for ID 1:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to update waiting room count: " + query.lastError().text());
        return;
    }

    qDebug() << "Successfully updated WR in MEETING table for ID 1 to:" << count;
    updateNotificationCount(1, QString("Updated waiting room count to %1 for meeting ID 1").arg(count));
}

void FormationWindow::updateWaitingRoomCount()
{
    qDebug() << "Entering updateWaitingRoomCount";

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not connected";
        ui->setText->setPlainText("Waiting Room: Database Error");
        ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #D93025; border: 1px solid #D93025; border-radius: 4px; padding: 4px;");
        updateNotificationCount(1, "Database connection lost in updateWaitingRoomCount");
        return;
    }

    int waitingCount = 0;
    QSqlQuery query;
    query.prepare("SELECT WR FROM AHMED.MEETING WHERE ID = 1");
    if (query.exec() && query.next()) {
        waitingCount = query.value("WR").toInt();
        qDebug() << "Retrieved WR count for ID 1:" << waitingCount;
    } else {
        qDebug() << "Failed to retrieve WR count for ID 1:" << query.lastError().text() << "Query:" << query.lastQuery();
        waitingCount = 0;
        ui->setText->setPlainText("Waiting Room: Query Error");
        ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #D93025; border: 1px solid #D93025; border-radius: 4px; padding: 4px;");
        updateNotificationCount(1, "Failed to retrieve waiting room count: " + query.lastError().text());
        return;
    }

    // Update UI
    const int MAX_COUNT = 5;
    QString displayText = waitingCount >= MAX_COUNT ?
                              QString("Waiting Room: %1 (MAX)").arg(waitingCount) :
                              QString("Waiting Room: %1").arg(waitingCount);
    ui->setText->setPlainText(displayText);
    if (waitingCount >= MAX_COUNT) {
        ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #D93025; border: 1px solid #D93025; border-radius: 4px; padding: 4px;");
    } else if (waitingCount > 0) {
        ui->setText->setStyleSheet("font-weight: bold; color: white; background-color: #FF9800; border: 1px solid #FF9800; border-radius: 4px; padding: 4px;");
    } else {
        ui->setText->setStyleSheet(isDarkTheme ?
                                       "font-weight: bold; color: #F0F0F0; background-color: #6A6A6A; border: 1px solid #F28C6F; border-radius: 4px; padding: 4px;" :
                                       "font-weight: bold; color: #3A5DAE; background-color: #F5F7FA; border: 1px solid #3A5DAE; border-radius: 4px; padding: 4px;");
    }
    qDebug() << "Updated UI with waiting room count:" << waitingCount;

    // Send WR count to Arduino
    if (arduino && arduino->getSerial()->isOpen()) {
        QString command = QString("WR:%1\n").arg(waitingCount);
        if (arduino->writeToArduino(command.toUtf8())) {
            qDebug() << "Sent WR count to Arduino:" << command.trimmed();
        } else {
            qDebug() << "Failed to send WR count to Arduino";
            updateNotificationCount(1, "Failed to send WR count to Arduino");
        }
    } else {
        qDebug() << "Arduino not connected, cannot send WR count";
    }

    // Ensure widget is visible
    ui->setText->setVisible(true);
}

void FormationWindow::on_wrr_clicked()
{
    qDebug() << "wrr button clicked";
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not connected";
        QMessageBox::critical(this, "Database Error", "Database connection lost. Please reconnect.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT WR FROM AHMED.MEETING WHERE ID = 1");
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to retrieve WR count for ID 1:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to retrieve waiting room count: " + query.lastError().text());
        return;
    }

    int currentCount = query.value("WR").toInt();
    qDebug() << "Current waiting room count:" << currentCount;

    if (currentCount <= 0) {
        QMessageBox::information(this, "Waiting Room", "The waiting room is already empty!");
        return;
    }

    int newCount = currentCount - 1;
    updateMeetingWR(newCount); // Update database
    updateWaitingRoomCount(); // Refresh UI and send WR count to Arduino
}

void FormationWindow::on_refreshStatsButton_clicked()
{
    updateStatistics();
}

void FormationWindow::updateStatistics()
{
    qDebug() << "Starting updateStatistics";

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) AS total_count, SUM(PRIX) AS total_cost FROM AHMED.FORMATIONS");

    if (!query.exec()) {
        qDebug() << "Statistics query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to retrieve statistics: " + query.lastError().text());
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
        return;
    }

    if (query.next()) {
        totalFormations = query.value("total_count").toInt();
        QVariant totalCostVariant = query.value("total_cost");
        totalCost = totalCostVariant.isNull() ? 0.0 : totalCostVariant.toDouble();
        avgCost = totalFormations > 0 ? totalCost / totalFormations : 0.0;

        qDebug() << "Stats - Total Formations:" << totalFormations
                 << "Total Cost:" << totalCost
                 << "Avg Cost:" << avgCost;
    } else {
        qDebug() << "No data found for statistics.";
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
        return;
    }

    double percentage = totalCost > 0 ? (avgCost / totalCost * 100.0) : 0.0;

    qDebug() << "Creating bar sets";
    QBarSet *countSet = new QBarSet(QString("[%1] Total Formations").arg(totalFormations));
    QBarSet *costSet = new QBarSet(QString("[100.00%] Total Cost"));
    QBarSet *avgCostSet = new QBarSet(QString("[%1%] Average Cost").arg(percentage, 0, 'f', 2));

    *countSet << totalFormations;
    *costSet << totalCost;
    *avgCostSet << avgCost;

    QPen pen(Qt::black, 1);
    countSet->setPen(pen);
    costSet->setPen(pen);
    avgCostSet->setPen(pen);

    qDebug() << "Bar sets created";

    QBarSeries *mainSeries = new QBarSeries();
    mainSeries->append(countSet);
    mainSeries->append(costSet);
    mainSeries->append(avgCostSet);
    qDebug() << "Series created";

    QChart *chart = new QChart();
    chart->addSeries(mainSeries);
    chart->setTitle("Training Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    qDebug() << "Chart created";

    QStringList categories;
    categories << "Metrics";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    mainSeries->attachAxis(axisX);
    qDebug() << "X-axis set";

    QValueAxis *axisY = new QValueAxis();
    double maxValue = qMax(totalCost, qMax(static_cast<double>(totalFormations), avgCost));
    axisY->setRange(0, maxValue * 1.2);
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    mainSeries->attachAxis(axisY);
    qDebug() << "Y-axis set";

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    qDebug() << "Legend set";

    chart->setMargins(QMargins(50, 20, 50, 50));
    qDebug() << "Margins set";

    ui->statsChartView->setChart(chart);
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);
    qDebug() << "Chart displayed";
}

void FormationWindow::onNotificationLabelClicked()
{
    QString message = notificationCount == 0 ? "No recent changes." : "Recent Changes:\n" + changeHistory.join("\n");
    QMessageBox::information(this, "Notifications", message);
    resetNotificationCount();
}

void FormationWindow::updateNotificationCount(int change, const QString &changeDescription)
{
    notificationCount += change;
    if (notificationCount < 0) notificationCount = 0;

    if (!changeDescription.isEmpty()) {
        changeHistory.append(changeDescription);
        if (changeHistory.size() > 10) changeHistory.removeFirst();
    }

    QString labelText = QString("<img src=':/images/notification_icon.png' width='16' height='16'> <a href='#'>Notifications: %1</a>").arg(notificationCount);
    ui->notificationLabel->setStyleSheet(notificationCount > 0 ?
                                             "background-color: #D93025; color: white; border-radius: 10px; padding: 2px 6px;" :
                                             "font-weight: bold; color: #3A5DAE;");
    ui->notificationLabel->setText(labelText);
    qDebug() << "Notification count updated to:" << notificationCount << "Change:" << changeDescription;
}

void FormationWindow::resetNotificationCount()
{
    notificationCount = 0;
    changeHistory.clear();
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");
    ui->notificationLabel->setText("<img src=':notification_icon.png' width='16' height='16'> <a href='#'>Notifications: 0</a>");
    qDebug() << "Notification count reset";
}

void FormationWindow::exportToPdf()
{
    refreshTableView();

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString defaultFileName = QString("Formations_%1.pdf").arg(timestamp);

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), defaultFileName, tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Formations Report");
    pdfWriter.setCreator("Training Management System");
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    const qreal scaleFactor = 300.0 / 72.0;
    painter.scale(scaleFactor, scaleFactor);
    const int pageWidth = 595;
    const int pageHeight = 842;
    const int margin = 30;
    const int lineHeight = 20;
    const int headerHeight = 60;
    const int footerHeight = 30;
    int pageCount = 1;

    QVector<int> columnWidths = {50, 100, 200, 80, 80, 50, 60};
    int totalWidth = 0;
    for (int w : columnWidths) totalWidth += w;

    QFont titleFont("Helvetica", 12, QFont::Bold);
    QFont headerFont("Helvetica", 8, QFont::Bold);
    QFont bodyFont("Helvetica", 8);
    QFont footerFont("Helvetica", 6);

    QColor headerBg(60, 93, 174);
    QColor alternateRowBg(240, 240, 240);
    QColor textColor(50, 50, 50);
    QColor borderColor(150, 150, 150);

    painter.setFont(titleFont);
    painter.setPen(textColor);
    painter.drawText(QRect(margin, margin, pageWidth - 2 * margin, 100), Qt::AlignCenter,
                     "Formations Report");
    painter.setFont(bodyFont);
    painter.drawText(QRect(margin, margin + 120, pageWidth - 2 * margin, 50), Qt::AlignCenter,
                     QString("Generated on %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
    painter.drawText(QRect(margin, margin + 180, pageWidth - 2 * margin, 50), Qt::AlignCenter,
                     QString("Total Formations: %1").arg(totalFormations));
    painter.drawText(QRect(margin, margin + 200, pageWidth - 2 * margin, 50), Qt::AlignCenter,
                     QString("Total Cost: %1 DT").arg(totalCost, 0, 'f', 2));
    painter.drawLine(margin, pageHeight - margin - 10, pageWidth - margin, pageHeight - margin - 10);
    painter.setFont(footerFont);
    painter.drawText(QRect(margin, pageHeight - margin, pageWidth - 2 * margin, 20), Qt::AlignCenter,
                     "Training Management System");

    pdfWriter.newPage();
    pageCount++;

    int yPos = margin + headerHeight;
    int tableTop = yPos;

    painter.setFont(titleFont);
    painter.drawText(QRect(margin, margin, pageWidth - 2 * margin, 30), Qt::AlignLeft,
                     "Formations List");
    painter.setFont(bodyFont);
    painter.drawText(QRect(margin, margin + 20, pageWidth - 2 * margin, 20), Qt::AlignLeft,
                     QString("Exported on %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));

    if (!tableModel || tableModel->rowCount() == 0 || tableModel->columnCount() < 7) {
        painter.setFont(bodyFont);
        painter.drawText(margin, yPos, "No data available or incomplete data to export.");
        painter.end();
        QMessageBox::information(this, "Export", "PDF exported with no/incomplete data at: " + filePath);
        return;
    }

    painter.setFont(headerFont);
    painter.setPen(Qt::white);
    painter.setBrush(headerBg);
    QStringList headers = {"ID", "Formation", "Description", "Trainer", "Date", "Time", "Price"};
    int xPos = margin;
    for (int i = 0; i < headers.size(); ++i) {
        QRect headerRect(xPos, yPos, columnWidths[i], lineHeight);
        painter.drawRect(headerRect);
        painter.drawText(headerRect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
        xPos += columnWidths[i];
    }
    yPos += lineHeight;

    painter.setFont(bodyFont);
    painter.setPen(borderColor);
    for (int row = 0; row < tableModel->rowCount(); ++row) {
        xPos = margin;
        painter.setBrush(row % 2 == 0 ? alternateRowBg : Qt::white);

        for (int col = 0; col < 7; ++col) {
            QRect cellRect(xPos, yPos, columnWidths[col], lineHeight);
            painter.drawRect(cellRect);
            painter.setPen(textColor);
            QString data = tableModel->index(row, col).data().toString();
            painter.drawText(cellRect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, data);
            painter.setPen(borderColor);
            xPos += columnWidths[col];
        }
        yPos += lineHeight;

        if (yPos > (pageHeight - margin - footerHeight) && row < tableModel->rowCount() - 1) {
            xPos = margin;
            for (int i = 0; i <= 7; ++i) {
                painter.drawLine(xPos, tableTop, xPos, yPos);
                if (i < 7) xPos += columnWidths[i];
            }
            painter.drawLine(margin, yPos, margin + totalWidth, yPos);

            painter.setFont(footerFont);
            painter.setPen(textColor);
            painter.drawText(QRect(margin, pageHeight - margin, pageWidth - 2 * margin, 20),
                             Qt::AlignRight, QString("Page %1").arg(pageCount));
            painter.drawLine(margin, pageHeight - margin - 10, pageWidth - margin, pageHeight - margin - 10);

            pdfWriter.newPage();
            pageCount++;

            yPos = margin + headerHeight;
            tableTop = yPos;

            painter.setFont(headerFont);
            painter.setPen(Qt::white);
            painter.setBrush(headerBg);
            xPos = margin;
            for (int i = 0; i < headers.size(); ++i) {
                QRect headerRect(xPos, yPos, columnWidths[i], lineHeight);
                painter.drawRect(headerRect);
                painter.drawText(headerRect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
                xPos += columnWidths[i];
            }
            yPos += lineHeight;
            painter.setFont(bodyFont);
            painter.setPen(borderColor);
        }
    }

    xPos = margin;
    for (int i = 0; i <= 7; ++i) {
        painter.drawLine(xPos, tableTop, xPos, yPos);
        if (i < 7) xPos += columnWidths[i];
    }
    painter.drawLine(margin, yPos, margin + totalWidth, yPos);

    painter.setFont(footerFont);
    painter.setPen(textColor);
    painter.drawText(QRect(margin, pageHeight - margin, pageWidth - 2 * margin, 20),
                     Qt::AlignRight, QString("Page %1").arg(pageCount));
    painter.drawLine(margin, pageHeight - margin - 10, pageWidth - margin, pageHeight - margin - 10);
    painter.drawText(QRect(margin, pageHeight - margin, pageWidth - 2 * margin, 20),
                     Qt::AlignLeft, "Training Management System");

    painter.end();
    QMessageBox::information(this, "Export", "Formations data exported to PDF successfully at: " + filePath);
}

void FormationWindow::on_addButtonclicked()
{
    QString formation = ui->format->text().trimmed();
    QString description = ui->des->text().trimmed();
    QString trainer = ui->tr->text().trimmed();
    QDate date = ui->date->date();
    int timeValue = ui->timeb->value();
    double price = ui->prixb->value();
    QString phoneNumber = ui->phoneNumberInput->text().trimmed();

    if (formation.isEmpty() || description.isEmpty() || trainer.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    QRegularExpression phoneRegex("^\\+?[1-9]\\d{1,14}$");
    if (!phoneRegex.match(phoneNumber).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Phone number must be in international format (e.g., +1234567890).");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX) "
                  "VALUES (:formation, :description, :trainer, :datef, :time, :prix)");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", date);
    query.bindValue(":time", timeValue);
    query.bindValue(":prix", price);

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to add formation: " + query.lastError().text());
        return;
    }

    query.prepare("SELECT IDFORM FROM AHMED.FORMATIONS WHERE FORMATION = :formation AND DESCRIPTION = :description AND TRAINER = :trainer AND DATEF = :datef AND TIME = :time AND PRIX = :prix");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", date);
    query.bindValue(":time", timeValue);
    query.bindValue(":prix", price);

    int formationId = -1;
    if (query.exec() && query.next()) {
        formationId = query.value("IDFORM").toInt();
    } else {
        qDebug() << "Failed to retrieve formation ID:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to retrieve formation ID: " + query.lastError().text());
        return;
    }

    QString timeDisplay = QString("%1 hours").arg(timeValue);
    QString message = QString("New Formation Added: ID=%1, Formation=%2, Desc=%3, Trainer=%4, Date=%5, Time=%6, Price=%7 DT")
                          .arg(formationId)
                          .arg(formation)
                          .arg(description)
                          .arg(trainer)
                          .arg(date.toString("yyyy-MM-dd"))
                          .arg(timeDisplay)
                          .arg(price);

    const int maxSmsLength = 160;
    if (message.length() > maxSmsLength) {
        message = message.left(maxSmsLength - 3) + "...";
    }

    QSettings settings("config.ini", QSettings::IniFormat);
    QString apiKey = settings.value("Infobip/ApiKey", "08a7928d0d8d19a1ebbaa2f3c09a96a6-dab070dd-8d76-4da4-9148-1e6ac4c84434").toString();
    QString baseUrl = "ype9p9.api.infobip.com";
    QString sender = "447491163443";
    QString apiUrl = QString("https://%1/sms/2/text/advanced").arg(baseUrl);

    QJsonObject messageObj;
    messageObj["text"] = message;
    QJsonObject destinationObj;
    destinationObj["to"] = phoneNumber;
    QJsonArray destinationsArray;
    destinationsArray.append(destinationObj);
    messageObj["destinations"] = destinationsArray;
    messageObj["from"] = sender;

    QJsonArray messagesArray;
    messagesArray.append(messageObj);
    QJsonObject requestBody;
    requestBody["messages"] = messagesArray;

    QJsonDocument jsonDoc(requestBody);
    QByteArray jsonData = jsonDoc.toJson();

    QNetworkRequest request{QUrl(apiUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("App %1").arg(apiKey).toUtf8());

    qDebug() << "SMS Request JSON:" << jsonData;
    networkManager->post(request, jsonData);

    updateNotificationCount(1, "Added formation: " + formation);
    refreshTableView();
    QMessageBox::information(this, "Success", "Formation added successfully! SMS notification queued.");
}

void FormationWindow::onSmsRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "SMS Network Error:" << reply->errorString();
        QMessageBox::critical(this, "SMS Error", "Failed to send SMS: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    qDebug() << "SMS Response:" << responseData;
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject responseObj = doc.object();

    if (responseObj.contains("messages")) {
        QJsonArray messages = responseObj["messages"].toArray();
        if (!messages.isEmpty()) {
            QJsonObject message = messages[0].toObject();
            QJsonObject status = message["status"].toObject();
            QString statusName = status["name"].toString();
            QString statusDesc = status["description"].toString();

            if (statusName == "PENDING" || statusName == "DELIVERED") {
                qDebug() << "SMS sent successfully! Status:" << statusName;
            } else {
                qDebug() << "SMS Status:" << statusName << "Description:" << statusDesc;
                QMessageBox::warning(this, "SMS Warning", "SMS status: " + statusName + " (" + statusDesc + ")");
            }
        } else {
            qDebug() << "Empty messages array in response";
            QMessageBox::critical(this, "SMS Error", "Invalid SMS response: No message data.");
        }
    } else {
        qDebug() << "Invalid SMS response structure:" << responseData;
        QMessageBox::critical(this, "SMS Error", "Failed to send SMS: Invalid response from server.");
    }

    reply->deleteLater();
}

void FormationWindow::refreshTableView()
{
    formations f;
    if (tableModel) {
        delete tableModel;
        tableModel = nullptr;
    }
    tableModel = new QSqlQueryModel(this);
    QSqlQuery query;
    query.exec("SELECT IDFORM, FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS");
    tableModel->setQuery(std::move(query));
    if (tableModel->lastError().isValid()) {
        qDebug() << "SQL Error:" << tableModel->lastError().text();
        QMessageBox::warning(this, "Error", "Failed to load formation data: " + tableModel->lastError().text());
        return;
    }
    qDebug() << "RefreshTableView - Row count:" << tableModel->rowCount()
             << "Column count:" << tableModel->columnCount();
    if (!proxyModel) {
        qDebug() << "Error: proxyModel is not initialized";
        QMessageBox::critical(this, "Error", "Table view proxy model is not initialized.");
        return;
    }
    proxyModel->setSourceModel(tableModel);
    ui->tabtr->setModel(proxyModel);
    ui->tabtr->resizeColumnsToContents();
}

void FormationWindow::on_deleteButtonClicked()
{
    QItemSelectionModel *selectionModel = ui->tabtr->selectionModel();
    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une formation à supprimer !");
        return;
    }

    QModelIndexList selectedRows = selectionModel->selectedRows();
    int row = selectedRows.at(0).row();
    int idfor = ui->tabtr->model()->index(row, 0).data().toInt();

    formations f;
    if (!f.exists(idfor)) {
        QMessageBox::warning(this, "Erreur", "Cette formation n'existe pas !");
        return;
    }

    if (formations::deleteFormation(idfor)) {
        QMessageBox::information(this, "Succès", "Formation supprimée avec succès !");
        updateNotificationCount(1, QString("Deleted formation ID: %1").arg(idfor));
        refreshTableView();
        updateStatistics();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de la formation.");
    }
}

void FormationWindow::on_updateButtonClicked()
{
    QItemSelectionModel *selectionModel = ui->tabtr->selectionModel();
    if (!selectionModel->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une formation à modifier !");
        return;
    }

    QModelIndexList selectedRows = selectionModel->selectedRows();
    int row = selectedRows.at(0).row();
    int idfor = ui->tabtr->model()->index(row, 0).data().toInt();

    formations f;
    if (!f.exists(idfor)) {
        QMessageBox::warning(this, "Erreur", "Cette formation n'existe pas !");
        return;
    }

    QString currentFormation = ui->tabtr->model()->index(row, 1).data().toString();
    QString currentDescription = ui->tabtr->model()->index(row, 2).data().toString();
    QString currentTrainer = ui->tabtr->model()->index(row, 3).data().toString();
    QDate currentDate = ui->tabtr->model()->index(row, 4).data().toDate();
    int currentTime = ui->tabtr->model()->index(row, 5).data().toInt();
    double currentPrix = ui->tabtr->model()->index(row, 6).data().toDouble();

    formations currentData(idfor, currentFormation, currentDescription, currentTrainer,
                           currentDate, currentTime, currentPrix);

    UpdateTrainingDialog dialog(idfor, currentData, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newFormation = dialog.getFormation();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();

        if (f.updateFormation(idfor, newFormation, newDescription, newTrainer, newDate, newTime, newPrix)) {
            QMessageBox::information(this, "Succès", "Formation mise à jour avec succès !");
            updateNotificationCount(1, QString("Updated formation ID: %1 to %2").arg(idfor).arg(newFormation));
            refreshTableView();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la formation.");
        }
    }
}

void FormationWindow::on_searchInput_textChanged(const QString &text)
{
    if (!tableModel || !proxyModel) return;

    QString column = ui->searchCriteriaComboBox->currentText();
    if (column.isEmpty()) return;

    int columnIndex = -1;
    if (column == "formation") {
        columnIndex = 1;
    } else if (column == "trainer") {
        columnIndex = 3;
    } else if (column == "time") {
        columnIndex = 5;
    } else if (column == "prix") {
        columnIndex = 6;
    } else {
        qDebug() << "Invalid column selected for search:" << column;
        return;
    }

    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regExp);
    proxyModel->setFilterKeyColumn(columnIndex);

    qDebug() << "Search: column=" << column << "index=" << columnIndex << "text=" << text;
}

void FormationWindow::on_resetSearchButton_clicked()
{
    ui->searchInput->clear();
    ui->searchCriteriaComboBox->setCurrentIndex(0);
    proxyModel->setFilterRegularExpression("");
    ui->tabtr->resizeColumnsToContents();
}

void FormationWindow::toggleTheme()
{
    isDarkTheme = !isDarkTheme;
    if (isDarkTheme) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

void FormationWindow::applyLightTheme()
{
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #FFFFFF, stop:1 #A1B8E6);
            color: #333333;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QPushButton {
            background-color: #3A5DAE;
            color: white;
            border: 1px solid #2A4682;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #4A70C2;
        }
        QPushButton:pressed {
            background-color: #2A4682;
        }
        QLineEdit, QComboBox, QDateTimeEdit {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
            padding: 4px;
            color: #333333;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus {
            border: 2px solid #3A5DAE;
        }
        QTableView {
            background-color: #FFFFFF;
            border: 1px solid #D3DCE6;
            border-radius: 4px;
            selection-background-color: #A1B8E6;
            selection-color: #333333;
        }
        QHeaderView::section {
            background-color: #3A5DAE;
            color: white;
            padding: 5px;
            border: none;
            border-radius: 2px;
        }
        QCalendarWidget {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
        }
        QCalendarWidget QToolButton {
            background-color: #3A5DAE;
            color: white;
            border-radius: 3px;
        }
        QToolTip {
            color: #333333;
            background-color: #E6ECF5;
            border: 1px solid #3A5DAE;
            border-radius: 3px;
        }
        QTabWidget::pane {
            border: 1px solid #3A5DAE;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #D3DCE6;
            color: #333333;
            padding: 6px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #3A5DAE;
            color: white;
        }
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #3A5DAE;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        #label {
            font-size: 18pt;
            font-weight: bold;
            color: #3A5DAE;
            qproperty-alignment: AlignCenter;
        }
        QFrame#header {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
        }
        QFrame#sideMenu {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            background-color: #E6ECF5;
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #D3DCE6;
            border-radius: 5px;
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void FormationWindow::applyDarkTheme()
{
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #F28C6F, stop:1 #5C5C5C);
            color: #F0F0F0;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QPushButton {
            background-color: #F28C6F;
            color: white;
            border: 1px solid #D96C53;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
            box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.3);
        }
        QPushButton:hover {
            background-color: #F5A38A;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.4);
        }
        QPushButton:pressed {
            background-color: #D96C53;
        }
        QLineEdit, QComboBox, QDateTimeEdit {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
            padding: 4px;
            color: #F0F0F0;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus {
            border: 2px solid #F28C6F;
        }
        QTableView {
            background-color: #6A6A6A;
            border: 1px solid #4A4A4A;
            border-radius: 4px;
            selection-background-color: #F28C6F;
            selection-color: #F0F0F0;
        }
        QHeaderView::section {
            background-color: #F28C6F;
            color: white;
            padding: 5px;
            border: none;
            border-radius: 2px;
        }
        QCalendarWidget {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
        }
        QCalendarWidget QToolButton {
            background-color: #F28C6F;
            color: white;
            border-radius: 3px;
        }
        QToolTip {
            color: #F0F0F0;
            background-color: #F28C6F;
            border: 1px solid #D96C53;
            border-radius: 3px;
        }
        QTabWidget::pane {
            border: 1px solid #F28C6F;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #7A7A7A;
            color: #F0F0F0;
            padding: 6px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background-color: #F28C6F;
            color: white;
        }
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #F28C6F;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        #label {
            font-size: 18pt;
            font-weight: bold;
            color: #F28C6F;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);
            qproperty-alignment: AlignCenter;
        }
        QFrame#header {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
        }
        QFrame#sideMenu {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
            background-color: #7A7A7A;
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #4A4A4A;
            border-radius: 5px;
            box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.3);
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void FormationWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}
