// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "meeting.h"
#include <QMessageBox>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QJsonDocument>  // Already included for QJsonDocument
#include <QJsonObject>    // Add this for QJsonObject
#include <QJsonArray>     // Add this for QJsonArray
#include <QFileDialog>
#include <QFontMetrics>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isDarkTheme(false)
{
    ui->setupUi(this);
    applyLightTheme();
    // Set up the table headers and properties first
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Populate the sortComboBox with sorting options
    ui->sortComboBox->addItem("Title");
    ui->sortComboBox->addItem("Duration");

    // Connect signals and slots
    connect(ui->AddButton, &QPushButton::clicked, this, &MainWindow::handleAddButtonClick);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClick);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::handleUpdateButtonClick);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::handleSearchButtonClick);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::handleSearchTextChanged);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::updateInputFields);
    connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::handleSortCriteriaChanged);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::handleStatisticsButtonClick);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->generateQRCodeButton, &QPushButton::clicked, this, &MainWindow::handleGenerateQRCodeButtonClick);
    connect(ui->chatSendButton, &QPushButton::clicked, this, &MainWindow::on_chatSendButton_clicked);
    connect(ui->chatClearButton, &QPushButton::clicked, this, &MainWindow::on_chatClearButton_clicked);
    connect(ui->refreshStatsButton, &QPushButton::clicked, this, &MainWindow::handleRefreshStatsButtonClick);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAIResponseReceived); // Connect API response
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &MainWindow::handleExportPdfButtonClick);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::handleTabChanged);
    // Initialize chatbot with a welcome message
    appendChatMessage("Hello! I'm your Meeting Assistant. How can I help you today?", true);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAIResponseReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete networkManager;
}
void MainWindow::handleTabChanged(int index) {
    // Check if the "Add Meeting" tab is selected (index 0)
    if (index == 0) {
        // Clear all input fields
        ui->title->clear();
        ui->organiser->clear();
        ui->participant->clear();
        ui->agenda->setCurrentIndex(0); // Reset to default agenda
        ui->duration->clear();
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime()); // Reset to current date/time
    }
}
void MainWindow::handleExportPdfButtonClick() {
    // Check if a meeting is selected in the table
    int selectedRow = ui->tableWidget->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to export.");
        return;
    }

    // Prompt user to save the PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "Meeting_" + ui->tableWidget->item(selectedRow, 0)->text() + ".pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return; // User canceled the dialog
    }

    // Set up the PDF writer
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300); // High resolution for clarity

    // Start painting on the PDF
    QPainter painter(&pdfWriter);
    painter.setPen(Qt::black);

    // Define layout constants (in device units: 1/300 inch at 300 DPI)
    const int margin = 500;       // Margin from edges
    const int lineSpacing = 200;  // Space between lines
    const int qrSize = 500;       // QR code size (1 inch at 300 DPI)
    const int padding = 250;      // Padding between label and value
    const int pageWidth = pdfWriter.width() - 2 * margin;
    int yPos = margin;

    // Title styling
    QFont titleFont("Arial", 16, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor("#3A5DAE")); // Blue color for title
    painter.drawText(margin, yPos, "Meeting Management System - Selected Meeting");
    yPos += 300; // Space after title

    // Label and value fonts
    QFont labelFont("Arial", 12, QFont::Bold);   // For labels (e.g., "ID:")
    QFont valueFont("Arial", 10);                // For values (e.g., "65")

    // Extract the selected meeting's details from the table
    QString id = ui->tableWidget->item(selectedRow, 0)->text();
    QString title = ui->tableWidget->item(selectedRow, 1)->text();
    QString organiser = ui->tableWidget->item(selectedRow, 2)->text();
    QString participant = ui->tableWidget->item(selectedRow, 3)->text();
    QString agenda = ui->tableWidget->item(selectedRow, 4)->text();
    QString duration = ui->tableWidget->item(selectedRow, 5)->text(); // Already includes " min"
    QString dateTimeStr = ui->tableWidget->item(selectedRow, 6)->text();

    // Draw meeting details vertically
    QStringList labels = {"ID:", "Title:", "Organiser:", "Participant:", "Agenda:", "Duration:", "Date and Time:"};
    QStringList values = {id, title, organiser, participant, agenda, duration, dateTimeStr};

    // Find the widest label to align values consistently
    int maxLabelWidth = 0;
    QFontMetrics fmLabel(labelFont);
    for (const QString& label : labels) {
        int labelWidth = fmLabel.horizontalAdvance(label);
        if (labelWidth > maxLabelWidth) {
            maxLabelWidth = labelWidth;
        }
    }
    maxLabelWidth += padding; // Add padding after the widest label

    // Draw each label-value pair
    for (int i = 0; i < labels.size(); ++i) {
        // Draw the label
        painter.setFont(labelFont);
        painter.setPen(QColor("#3A5DAE")); // Blue for labels
        painter.drawText(margin, yPos, labels[i]);

        // Draw the value aligned after the widest label
        painter.setFont(valueFont);
        painter.setPen(Qt::black); // Black for values
        painter.drawText(margin + maxLabelWidth, yPos, values[i]);

        yPos += lineSpacing;
    }

    // Generate and draw QR code for this meeting
    meeting currentMeeting(title, organiser, participant, agenda,
                           duration.replace(" min", "").toInt(),
                           QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm"));
    currentMeeting.setId(id.toInt());
    QPixmap qrCode = currentMeeting.generateQRCode();
    painter.setFont(valueFont);
    painter.setPen(Qt::black);
    painter.drawText(margin, yPos, "QR Code for Meeting ID " + id + ":");
    yPos += lineSpacing;
    painter.drawPixmap(margin, yPos, qrCode.scaled(qrSize, qrSize, Qt::KeepAspectRatio));
    yPos += qrSize + lineSpacing;

    // Draw a separator line after the meeting
    painter.setPen(QPen(Qt::gray, 20));
    painter.drawLine(margin, yPos, pageWidth + margin, yPos);

    // Clean up
    painter.end();

    QMessageBox::information(this, "Success", "Selected meeting exported to PDF successfully!");
}
void MainWindow::handleRefreshStatsButtonClick() {
    meeting m;
    QSqlQueryModel* model = m.afficher();

    // --- Calculate Statistics ---
    int totalMeetings = model->rowCount();
    if (totalMeetings == 0) {
        // If there are no meetings, clear the charts and show a message
        agendaChart = new QChart();
        agendaChart->setTitle("No Meetings Available");
        ui->agendaChartView->setChart(agendaChart);

        durationChart = new QChart();
        durationChart->setTitle("No Meetings Available");
        ui->durationChartView->setChart(durationChart);

        delete model;
        return;
    }

    // --- Agenda Distribution ---
    QMap<QString, int> agendaCount;
    QStringList agendas = {"car advice", "life advice", "future advice"};
    for (const QString &agenda : agendas) {
        agendaCount[agenda] = 0;
    }

    for (int row = 0; row < model->rowCount(); ++row) {
        QString agenda = model->data(model->index(row, 4)).toString();
        if (agendaCount.contains(agenda)) {
            agendaCount[agenda]++;
        }
    }

    QPieSeries *agendaSeries = new QPieSeries();
    agendaSeries->setName("Meetings by Agenda");
    for (const QString &agenda : agendaCount.keys()) {
        if (agendaCount[agenda] > 0) {
            agendaSeries->append(agenda, agendaCount[agenda]);
        }
    }

    // Customize agenda pie slices
    for (QPieSlice *slice : agendaSeries->slices()) {
        slice->setLabel(QString("%1: %2 (%3%)")
                            .arg(slice->label())
                            .arg(static_cast<int>(slice->value()))
                            .arg(slice->percentage() * 100, 0, 'f', 1));
        slice->setLabelVisible();
    }

    // Add colors to agenda slices
    QList<QColor> agendaColors = {QColor("#FF6F61"), QColor("#6B5B95"), QColor("#88B04B")};
    int colorIndex = 0;
    for (QPieSlice *slice : agendaSeries->slices()) {
        slice->setColor(agendaColors[colorIndex % agendaColors.size()]);
        colorIndex++;
    }

    // --- Duration Distribution ---
    QMap<QString, int> durationRanges;
    durationRanges["Short (0-30 mins)"] = 0;
    durationRanges["Medium (31-60 mins)"] = 0;
    durationRanges["Long (>60 mins)"] = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        int duration = model->data(model->index(row, 5)).toInt();
        if (duration <= 30) {
            durationRanges["Short (0-30 mins)"]++;
        } else if (duration <= 60) {
            durationRanges["Medium (31-60 mins)"]++;
        } else {
            durationRanges["Long (>60 mins)"]++;
        }
    }

    QPieSeries *durationSeries = new QPieSeries();
    durationSeries->setName("Meetings by Duration");
    for (const QString &range : durationRanges.keys()) {
        if (durationRanges[range] > 0) {
            durationSeries->append(range, durationRanges[range]);
        }
    }

    // Customize duration pie slices
    for (QPieSlice *slice : durationSeries->slices()) {
        slice->setLabel(QString("%1: %2 (%3%)")
                            .arg(slice->label())
                            .arg(static_cast<int>(slice->value()))
                            .arg(slice->percentage() * 100, 0, 'f', 1));
        slice->setLabelVisible();
    }

    // Add colors to duration slices
    QList<QColor> durationColors = {QColor("#FFD700"), QColor("#4682B4"), QColor("#FF4500")};
    colorIndex = 0;
    for (QPieSlice *slice : durationSeries->slices()) {
        slice->setColor(durationColors[colorIndex % durationColors.size()]);
        colorIndex++;
    }

    // --- Create the Agenda Chart ---
    agendaChart = new QChart();
    agendaChart->setTitle("Agenda Distribution");
    agendaChart->addSeries(agendaSeries);
    agendaChart->legend()->setVisible(ui->toggleLegendCheckBox->isChecked());
    agendaChart->legend()->setAlignment(Qt::AlignBottom);
    agendaChart->setMargins(QMargins(10, 10, 10, 10));
    ui->agendaChartView->setChart(agendaChart);
    ui->agendaChartView->setRenderHint(QPainter::Antialiasing);

    // --- Create the Duration Chart ---
    durationChart = new QChart();
    durationChart->setTitle("Duration Distribution");
    durationChart->addSeries(durationSeries);
    durationChart->legend()->setVisible(ui->toggleLegendCheckBox->isChecked());
    durationChart->legend()->setAlignment(Qt::AlignBottom);
    durationChart->setMargins(QMargins(10, 10, 10, 10));
    ui->durationChartView->setChart(durationChart);
    ui->durationChartView->setRenderHint(QPainter::Antialiasing);

    delete model;
}
void MainWindow::on_toggleLegendCheckBox_stateChanged(int state) {
    bool showLegend = (state == Qt::Checked);
    if (agendaChart) {
        agendaChart->legend()->setVisible(showLegend);
    }
    if (durationChart) {
        durationChart->legend()->setVisible(showLegend);
    }
}

// Rest of the MainWindow implementation (unchanged)
void MainWindow::on_chatSendButton_clicked()
{
    QString userInput = ui->chatInputLineEdit->text().trimmed();
    if (!userInput.isEmpty()) {
        appendChatMessage(userInput);
        processUserInput(userInput);
        ui->chatInputLineEdit->clear();
    }
}
void MainWindow::on_chatClearButton_clicked()
{
    ui->chatTextEdit->clear();
    appendChatMessage("Chat cleared. How can I assist you now?", true);
}
void MainWindow::appendChatMessage(const QString &message, bool isBot)
{
    QString formattedMessage = isBot ? "<b>Bot:</b> " + message : "<b>You:</b> " + message;
    ui->chatTextEdit->append(formattedMessage);
}

void MainWindow::processUserInput(const QString &input)
{
    // Check if the input is a meeting scheduling request (contains commas)
    if (input.contains(",")) {
        QStringList parts = input.split(",");
        QString errorMessage;
        if (!validateMeetingInput(parts, errorMessage)) {
            appendChatMessage(errorMessage, true);
            return;
        }

        meeting m = createMeetingFromInput(input);
        if (m.add()) {
            appendChatMessage("Meeting scheduled successfully! Here's the QR code:", true);
            QPixmap qrCode = m.generateQRCode();
            QMessageBox qrDialog(this);
            qrDialog.setWindowTitle("Meeting QR Code");
            qrDialog.setText("Scan this QR code for meeting details:");
            qrDialog.setIconPixmap(qrCode.scaled(200, 200, Qt::KeepAspectRatio));
            qrDialog.exec();
            refreshTableWidget();
            return;
        } else {
            appendChatMessage("Failed to schedule the meeting.", true);
            return;
        }
    }

    // Handle specific commands locally
    QString trimmedInput = input.trimmed().toLower();
    if (trimmedInput == "show meetings") {
        meeting m;
        QSqlQueryModel* model = m.afficher();
        QString meetingList;
        for (int row = 0; row < model->rowCount(); ++row) {
            meetingList += QString("ID: %1, Title: %2, Organiser: %3, Participant: %4, Date: %5\n")
            .arg(model->data(model->index(row, 0)).toString())
                .arg(model->data(model->index(row, 1)).toString())
                .arg(model->data(model->index(row, 2)).toString())
                .arg(model->data(model->index(row, 3)).toString())
                .arg(model->data(model->index(row, 6)).toDateTime().toString("yyyy-MM-dd hh:mm"));
        }
        appendChatMessage(meetingList.isEmpty() ? "No meetings found." : meetingList, true);
        delete model;
        return;
    } else if (trimmedInput == "help") {
        appendChatMessage("I can help with:\n"
                          "- Scheduling a meeting: Provide details like 'Title, Organiser, Participant, Agenda, Duration, DateTime'.\n"
                          "- Listing meetings: Say 'show meetings'.\n"
                          "- Deleting a meeting: Say 'delete meeting <ID>' (e.g., 'delete meeting 9').\n"
                          "- Help: Say 'help' to see this message.\n"
                          "- Clear chat: Say 'clear chat' to clear the chat.", true);
        return;
    } else if (trimmedInput == "clear chat") {
        ui->chatTextEdit->clear();
        appendChatMessage("Chat cleared. How can I assist you now?", true);
        return;
    } else if (trimmedInput.startsWith("delete meeting ")) {
        // Extract the meeting ID from the input
        QString idStr = trimmedInput.mid(QString("delete meeting ").length()).trimmed();
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok || id <= 0) {
            appendChatMessage("Invalid meeting ID. Please provide a valid ID (e.g., 'delete meeting 9').", true);
            return;
        }

        // Delete the meeting
        meeting m;
        if (m.delet(id)) { // Note: Fix the typo 'delet' to 'delete' in your meeting class
            appendChatMessage(QString("Meeting with ID %1 deleted successfully.").arg(id), true);
            refreshTableWidget();
        } else {
            appendChatMessage("Failed to delete meeting. Check the database connection.", true);
        }
        return;
    }

    // For all other inputs, query the Hugging Face AI
    QNetworkRequest request;
    request.setUrl(QUrl("https://api-inference.huggingface.co/models/mistralai/Mixtral-8x7B-Instruct-v0.1"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer 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"); // Your new token

    // Prepare the JSON payload with a system prompt
    QJsonObject json;
    QString systemPrompt = "You are a friendly and helpful Meeting Assistant. Respond naturally and conversationally, as if you're a human assistant.";
    json["inputs"] = QString("[INST] %1\n\nUser: %2 [/INST]").arg(systemPrompt, input);

    QJsonObject parameters;
    parameters["max_new_tokens"] = 150;
    parameters["temperature"] = 0.7;
    parameters["top_p"] = 0.9;
    parameters["do_sample"] = true;
    json["parameters"] = parameters;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Send the request
    appendChatMessage("Processing...", true); // Show a loading message
    networkManager->post(request, data);
}
void MainWindow::onAIResponseReceived(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        // Get HTTP status code and response body for debugging
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString errorMsg = reply->errorString();
        QByteArray responseBody = reply->readAll();
        QString detailedError = QString("Error communicating with AI: %1 (HTTP %2)\nServer response: %3")
                                    .arg(errorMsg)
                                    .arg(statusCode)
                                    .arg(QString(responseBody));
        appendChatMessage(detailedError, true);
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);

    if (doc.isArray() && !doc.array().isEmpty()) {
        QString aiResponse = doc.array()[0].toObject()["generated_text"].toString();
        // Clean up the response (remove the [INST] prompt part)
        aiResponse = aiResponse.section("[/INST]", 1).trimmed();

        // Display the AI's natural response
        appendChatMessage(aiResponse, true);
    } else {
        appendChatMessage("Sorry, I couldn't process that. Please try again.\nResponse: " + QString(responseData), true);
    }

    reply->deleteLater();
}

bool MainWindow::validateMeetingInput(const QStringList &parts, QString &errorMessage)
{
    if (parts.size() != 6) {
        errorMessage = "Invalid format. Please use: 'Title, Organiser, Participant, Agenda, Duration (min), DateTime (yyyy-MM-dd hh:mm)'";
        return false;
    }

    QString title = parts[0].trimmed();
    QString organiser = parts[1].trimmed();
    QString participant = parts[2].trimmed();
    QString agenda = parts[3].trimmed();
    bool durationOk;
    int duration = parts[4].trimmed().toInt(&durationOk);
    QDateTime datem = QDateTime::fromString(parts[5].trimmed(), "yyyy-MM-dd hh:mm");

    QStringList errors;
    if (title.isEmpty()) errors << "Title cannot be empty";
    if (organiser.isEmpty()) errors << "Organiser cannot be empty";
    if (participant.isEmpty()) errors << "Participant cannot be empty";
    if (agenda.isEmpty()) errors << "Agenda cannot be empty";
    if (!durationOk || duration <= 0) errors << "Duration must be a positive number";
    if (!datem.isValid()) errors << "DateTime must be in the format 'yyyy-MM-dd hh:mm'";
    if (datem.isValid() && datem < QDateTime::currentDateTime()) errors << "Date and time must be in the future";

    if (!errors.isEmpty()) {
        errorMessage = "Please correct the following errors:\n• " + errors.join("\n• ");
        return false;
    }

    return true;
}


meeting MainWindow::createMeetingFromInput(const QString &input)
{
    QStringList parts = input.split(",");
    QString title = parts[0].trimmed();
    QString organiser = parts[1].trimmed();
    QString participant = parts[2].trimmed();
    QString agenda = parts[3].trimmed();
    int duration = parts[4].trimmed().toInt();
    QDateTime datem = QDateTime::fromString(parts[5].trimmed(), "yyyy-MM-dd hh:mm");

    return meeting(title, organiser, participant, agenda, duration, datem);
}

void MainWindow::handleGenerateQRCodeButtonClick() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to generate a QR code.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    meeting m;
    m.setId(id);
    m.setTitle(ui->tableWidget->item(row, 1)->text());
    m.setOrganiser(ui->tableWidget->item(row, 2)->text());
    m.setParticipant(ui->tableWidget->item(row, 3)->text());
    m.setAgenda(ui->tableWidget->item(row, 4)->text());
    m.setDuration(ui->tableWidget->item(row, 5)->text().replace(" min", "").toInt());
    m.setDatem(QDateTime::fromString(ui->tableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm"));

    QPixmap qrCode = m.generateQRCode();

    QMessageBox qrDialog(this);
    qrDialog.setWindowTitle("Meeting QR Code");
    qrDialog.setText("Scan this QR code for full meeting details:");
    qrDialog.setIconPixmap(qrCode.scaled(200, 200, Qt::KeepAspectRatio));
    qrDialog.exec();
}

void MainWindow::handleAddButtonClick() {
    QString title = ui->title->text();
    QString organiser = ui->organiser->text();
    QString participant = ui->participant->text();
    QString agenda = ui->agenda->currentText();
    QDateTime datem = ui->dateTimeEdit->dateTime();
    bool conversionOk = false;
    int duration = ui->duration->text().toInt(&conversionOk);

    QStringList validationErrors;
    if (title.isEmpty()) validationErrors << "Title cannot be empty";
    if (organiser.isEmpty()) validationErrors << "Organiser cannot be empty";
    if (!conversionOk || duration <= 0) validationErrors << "Duration must be a valid positive number";
    if (datem < QDateTime::currentDateTime()) validationErrors << "Date and time must be in the future";

    if (!validationErrors.isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please correct the following errors:\n• " + validationErrors.join("\n• "));
        return;
    }

    meeting m(title, organiser, participant, agenda, duration, datem);
    if (m.add()) {
        QMessageBox::information(this, "Success", "Meeting added successfully.");
        ui->title->clear();
        ui->organiser->clear();
        ui->participant->clear();
        ui->duration->clear();
        ui->agenda->setCurrentIndex(0);
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add meeting.");
    }
}

void MainWindow::handleDeleteButtonClick() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to delete.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();

    meeting m;
    if (m.delet(id)) {
        QMessageBox::information(this, "Success", "Meeting deleted successfully.");
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete meeting. Check the database connection.");
    }
}

void MainWindow::handleUpdateButtonClick() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to update.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    meeting m;
    m.setId(id);
    m.setTitle(ui->tableWidget->item(row, 1)->text());
    m.setOrganiser(ui->tableWidget->item(row, 2)->text());
    m.setParticipant(ui->tableWidget->item(row, 3)->text());
    m.setAgenda(ui->tableWidget->item(row, 4)->text());
    m.setDuration(ui->tableWidget->item(row, 5)->text().replace(" min", "").toInt());
    m.setDatem(QDateTime::fromString(ui->tableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm"));

    UpdateMeeting dialog(this, &m);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTableWidget();
    }
}

void MainWindow::updateInputFields() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        return;
    }

    ui->title->setText(ui->tableWidget->item(row, 1)->text());
    ui->organiser->setText(ui->tableWidget->item(row, 2)->text());
    ui->participant->setText(ui->tableWidget->item(row, 3)->text());
    ui->agenda->setCurrentText(ui->tableWidget->item(row, 4)->text());
    ui->duration->setText(ui->tableWidget->item(row, 5)->text().replace(" min", ""));
}

void MainWindow::handleSortCriteriaChanged(int index) {
    int columnToSort = -1;
    switch (index) {
    case 0: // Title
        columnToSort = 1;
        break;
    case 1: // Duration
        columnToSort = 5;
        break;
    default:
        return;
    }

    ui->tableWidget->sortItems(columnToSort, Qt::AscendingOrder);
}

void MainWindow::handleStatisticsButtonClick() {
    ui->tabWidget->setCurrentWidget(ui->tab);
    handleRefreshStatsButtonClick();
}

void MainWindow::handleSearchButtonClick() {
    QString searchText = ui->searchInput->text();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Search Error", "Please enter a title to search.");
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "No Results", "No meetings found with the given title.");
        delete model;
        return;
    }

    ui->tableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model;
}

void MainWindow::handleSearchTextChanged(const QString &searchText) {
    if (searchText.isEmpty()) {
        refreshTableWidget();
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);

    ui->tableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model;
}

void MainWindow::refreshTableWidget() {
    meeting m;
    QSqlQueryModel* model = m.afficher();

    qDebug() << "Refreshing table widget. Model has" << model->rowCount() << "rows";

    if (model->rowCount() == 0) {
        qDebug() << "No data in model. Last error:" << model->lastError().text();
    }

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Title", "Organiser", "Participant", "Agenda", "Duration", "Date and Time"});
    ui->tableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < 7; ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            } else if (col == 6) {
                QDateTime dateTime = model->data(model->index(row, col)).toDateTime();
                if (dateTime.isValid()) {
                    data = dateTime.toString("yyyy-MM-dd hh:mm");
                } else {
                    data = "Invalid Date";
                    qDebug() << "Invalid DateTime at row" << row << "col" << col;
                }
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(6, 150);

    delete model;
}

void MainWindow::applyLightTheme() {
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

void MainWindow::applyDarkTheme() {
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
        }
        QPushButton:hover {
            background-color: #F5A38A;
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

void MainWindow::toggleSidebar() {
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::toggleTheme() {
    if (isDarkTheme) {
        applyLightTheme();
        isDarkTheme = false;
        ui->themeButton->setText("Switch to Dark Theme");
    } else {
        applyDarkTheme();
        isDarkTheme = true;
        ui->themeButton->setText("Switch to Light Theme");
    }
}
