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
#include <QDebug>

FormationWindow::FormationWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::formationwindow), isDarkTheme(false),
    tableModel(nullptr), proxyModel(new QSortFilterProxyModel(this)),
    notificationCount(0)
{
    ui->setupUi(this);
    applyLightTheme();

    ui->date->setDate(QDate::currentDate());

    // Initialize notification label
    ui->notificationLabel->setText("Notifications: 0");
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");

    // Connections
    QObject::connect(ui->add, SIGNAL(clicked()), this, SLOT(on_addButtonclicked()));
    QObject::connect(ui->deletef, SIGNAL(clicked()), this, SLOT(on_deleteButtonClicked()));
    QObject::connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(on_updateButtonClicked()));
    connect(ui->themeButton, &QPushButton::clicked, this, &FormationWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &FormationWindow::toggleSidebar);
    connect(ui->exportButton, &QPushButton::clicked, this, &FormationWindow::exportToPdf); // Fixed syntax
    connect(ui->notificationLabel, &QLabel::linkActivated, this, &FormationWindow::onNotificationLabelClicked);
    connect(ui->refreshStatsButton, &QPushButton::clicked, this, &FormationWindow::on_refreshStatsButton_clicked);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &FormationWindow::on_searchInput_textChanged);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &FormationWindow::on_resetSearchButton_clicked);

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

    refreshTableView();
    updateStatistics();
}

FormationWindow::~FormationWindow()
{
    delete tableModel;
    delete ui;
}

// ... rest of the methods remain as provided, with class name updated to FormationWindow

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

    // Calculate percentage for Average Cost relative to Total Cost
    double percentage = totalCost > 0 ? (avgCost / totalCost * 100.0) : 0.0;

    // Create bar sets with updated labels including percentages
    qDebug() << "Creating bar sets";
    QBarSet *countSet = new QBarSet(QString("[%1] Total Formations").arg(totalFormations));
    QBarSet *costSet = new QBarSet(QString("[100.00%] Total Cost"));
    QBarSet *avgCostSet = new QBarSet(QString("[%1%] Average Cost").arg(percentage, 0, 'f', 2));

    *countSet << totalFormations;
    *costSet << totalCost;
    *avgCostSet << avgCost;

    // Adjust bar appearance to create spacing effect
    QPen pen(Qt::black, 1);
    countSet->setPen(pen);
    costSet->setPen(pen);
    avgCostSet->setPen(pen);

    qDebug() << "Bar sets created";

    // Create series
    QBarSeries *mainSeries = new QBarSeries();
    mainSeries->append(countSet);
    mainSeries->append(costSet);
    mainSeries->append(avgCostSet);
    qDebug() << "Series created";

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(mainSeries);
    chart->setTitle("Training Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    qDebug() << "Chart created";

    // Set up X-axis
    QStringList categories;
    categories << "Metrics";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    mainSeries->attachAxis(axisX);
    qDebug() << "X-axis set";

    // Primary Y-axis for count, cost, and avg cost
    QValueAxis *axisY = new QValueAxis();
    double maxValue = qMax(totalCost, qMax(static_cast<double>(totalFormations), avgCost));
    axisY->setRange(0, maxValue * 1.2);
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    mainSeries->attachAxis(axisY);
    qDebug() << "Y-axis set";

    // Ensure legend is visible
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    qDebug() << "Legend set";

    // Increase chart margins to create visual spacing around bars
    chart->setMargins(QMargins(50, 20, 50, 50)); // Left, Top, Right, Bottom
    qDebug() << "Margins set";

    // Set chart to view
    ui->statsChartView->setChart(chart);
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);
    qDebug() << "Chart displayed";
}
void FormationWindow::onNotificationLabelClicked()
{
    if (notificationCount == 0) {
        QMessageBox::information(this, "Notifications", "No recent changes.");
    } else {
        QString message = "Recent Changes:\n" + changeHistory.join("\n");
        QMessageBox::information(this, "Notifications", message);
    }
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
    if (notificationCount > 0) {
        ui->notificationLabel->setStyleSheet("background-color: #D93025; color: white; border-radius: 10px; padding: 2px 6px;");
    } else {
        ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");
    }
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

    // Generate timestamp for filename
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString defaultFileName = QString("Formations_%1.pdf").arg(timestamp);

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), defaultFileName, tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    // Set up PDF writer
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);
    pdfWriter.setTitle("Formations Report");
    pdfWriter.setCreator("Training Management System");
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define constants
    const qreal scaleFactor = 300.0 / 72.0;
    painter.scale(scaleFactor, scaleFactor);
    const int pageWidth = 595;  // A4 width in points at 72 DPI
    const int pageHeight = 842; // A4 height in points at 72 DPI
    const int margin = 30;
    const int lineHeight = 20;
    const int headerHeight = 60;
    const int footerHeight = 30;
    int pageCount = 1; // Manual page counter

    // Adjusted column widths for better balance
    QVector<int> columnWidths = {50, 100, 200, 80, 80, 50, 60};
    int totalWidth = 0;
    for (int w : columnWidths) totalWidth += w;

    // Fonts
    QFont titleFont("Helvetica", 12, QFont::Bold);
    QFont headerFont("Helvetica", 8, QFont::Bold);
    QFont bodyFont("Helvetica", 8);
    QFont footerFont("Helvetica", 6);

    // Colors
    QColor headerBg(60, 93, 174); // #3A5DAE
    QColor alternateRowBg(240, 240, 240); // Light gray
    QColor textColor(50, 50, 50);
    QColor borderColor(150, 150, 150);

    // Cover page
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
    pageCount++; // Increment page count for cover page

    // Data page
    int yPos = margin + headerHeight;
    int tableTop = yPos;

    // Header section
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

    // Table header
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

    // Table data
    painter.setFont(bodyFont);
    painter.setPen(borderColor);
    for (int row = 0; row < tableModel->rowCount(); ++row) {
        xPos = margin;
        // Alternate row background
        if (row % 2 == 0) {
            painter.setBrush(alternateRowBg);
        } else {
            painter.setBrush(Qt::white);
        }

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

        // Page break if necessary
        if (yPos > (pageHeight - margin - footerHeight) && row < tableModel->rowCount() - 1) {
            // Draw table borders
            xPos = margin;
            for (int i = 0; i <= 7; ++i) {
                painter.drawLine(xPos, tableTop, xPos, yPos);
                if (i < 7) xPos += columnWidths[i];
            }
            painter.drawLine(margin, yPos, margin + totalWidth, yPos);

            // Footer
            painter.setFont(footerFont);
            painter.setPen(textColor);
            painter.drawText(QRect(margin, pageHeight - margin, pageWidth - 2 * margin, 20),
                             Qt::AlignRight, QString("Page %1").arg(pageCount));
            painter.drawLine(margin, pageHeight - margin - 10, pageWidth - margin, pageHeight - margin - 10);

            pdfWriter.newPage();
            pageCount++; // Increment page count for new page

            yPos = margin + headerHeight;
            tableTop = yPos;

            // Redraw table header
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

    // Final table borders
    xPos = margin;
    for (int i = 0; i <= 7; ++i) {
        painter.drawLine(xPos, tableTop, xPos, yPos);
        if (i < 7) xPos += columnWidths[i];
    }
    painter.drawLine(margin, yPos, margin + totalWidth, yPos);

    // Footer
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
    QDate date = ui->date->date(); // Keep as QDate
    int timeValue = ui->timeb->value();
    double price = ui->prixb->value();
    QString phoneNumber = ui->phoneNumberInput->text().trimmed();

    // Validate inputs
    if (formation.isEmpty() || description.isEmpty() || trainer.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    // Validate phone number
    QRegularExpression phoneRegex("^\\+?[1-9]\\d{1,14}$");
    if (!phoneRegex.match(phoneNumber).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Phone number must be in international format (e.g., +1234567890).");
        return;
    }

    // Insert formation into database
    QSqlQuery query;
    query.prepare("INSERT INTO AHMED.FORMATIONS (FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX) "
                  "VALUES (:formation, :description, :trainer, :datef, :time, :prix)");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", date); // Bind QDate directly
    query.bindValue(":time", timeValue);
    query.bindValue(":prix", price);

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to add formation: " + query.lastError().text());
        return;
    }

    // Retrieve formation ID
    query.prepare("SELECT IDFORM FROM AHMED.FORMATIONS WHERE FORMATION = :formation AND DESCRIPTION = :description AND TRAINER = :trainer AND DATEF = :datef AND TIME = :time AND PRIX = :prix");
    query.bindValue(":formation", formation);
    query.bindValue(":description", description);
    query.bindValue(":trainer", trainer);
    query.bindValue(":datef", date); // Bind QDate directly
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

    // Format SMS message
    QString timeDisplay = QString("%1 hours").arg(timeValue);
    QString message = QString("New Formation Added: ID=%1, Formation=%2, Desc=%3, Trainer=%4, Date=%5, Time=%6, Price=%7 DT")
                          .arg(formationId)
                          .arg(formation)
                          .arg(description)
                          .arg(trainer)
                          .arg(date.toString("yyyy-MM-dd")) // Format for SMS display
                          .arg(timeDisplay)
                          .arg(price);

    const int maxSmsLength = 160;
    if (message.length() > maxSmsLength) {
        message = message.left(maxSmsLength - 3) + "...";
    }

    // Send SMS via Infobip
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
            box影子: 2px 2px 6px rgba(0, 0, 0, 0.3);
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
