// managers/meeting/meetingmanager.cpp
#include "meetingmanager.h"
#include "ui_mainwindow.h"
#include "updatemeeting.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFontMetrics>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>

MeetingManager::MeetingManager(bool dbConnected, QObject *parent)
    : QObject(parent), m_dbConnected(dbConnected), ui(nullptr), notificationManager(nullptr)
{
}

MeetingManager::~MeetingManager()
{
}

void MeetingManager::setNotificationManager(NotificationManager *manager)
{
    notificationManager = manager;
}

void MeetingManager::initialize(Ui::MainWindow *ui)
{
    this->ui = ui;

    // Connect signals and slots
    connect(ui->meetingAddButton, &QPushButton::clicked, this, &MeetingManager::handleAddButtonClick);
    connect(ui->meetingDeleteButton, &QPushButton::clicked, this, &MeetingManager::handleDeleteButtonClick);
    connect(ui->meetingUpdateButton, &QPushButton::clicked, this, &MeetingManager::handleUpdateButtonClick);
    connect(ui->meetingResetSearchButton, &QPushButton::clicked, this, &MeetingManager::handleSearchButtonClick);
    connect(ui->meetingSearchInput, &QLineEdit::textChanged, this, &MeetingManager::handleSearchTextChanged);
    connect(ui->meetingTableWidget, &QTableWidget::itemSelectionChanged, this, &MeetingManager::updateInputFields);
    connect(ui->meetingGenerateQRCodeButton, &QPushButton::clicked, this, &MeetingManager::handleGenerateQRCodeButtonClick);
    connect(ui->meetingExportPdfButton, &QPushButton::clicked, this, &MeetingManager::handleExportPdfButtonClick);
    connect(ui->meetingTabWidget, &QTabWidget::currentChanged, this, &MeetingManager::handleTabChanged);
    connect(ui->meetingSearchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MeetingManager::handleSortCriteriaChanged);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MeetingManager::handleStatisticsButtonClick);
    // connect(ui->meetingRefreshStatsButton, &QPushButton::clicked, this, &MeetingManager::handleRefreshStatsButtonClick); // Commented out: Button not in UI

    // Set up table properties
    ui->meetingTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->meetingTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->meetingTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Refresh table initially
    refreshTableWidget();

    // Apply theme styles
    applyThemeStyles();
}

void MeetingManager::handleAddButtonClick()
{
    QString title = ui->meetingTitleInput->text();
    QString organiser = ui->meetingOrganiserInput->text();
    QString participant = ui->meetingParticipantInput->text();
    QString agenda = ui->meetingAgendaComboBox->currentText();
    QDateTime datem = ui->meetingDateTimeEdit->dateTime();
    bool conversionOk = false;
    int duration = ui->meetingDurationInput->text().toInt(&conversionOk);

    QStringList validationErrors;
    if (title.isEmpty()) validationErrors << "Title cannot be empty";
    if (organiser.isEmpty()) validationErrors << "Organiser cannot be empty";
    if (!conversionOk || duration <= 0) validationErrors << "Duration must be a valid positive number";
    if (datem < QDateTime::currentDateTime()) validationErrors << "Date and time must be in the future";

    if (!validationErrors.isEmpty()) {
        QMessageBox::warning(nullptr, "Validation Error",
                             "Please correct the following errors:\n• " + validationErrors.join("\n• "));
        return;
    }

    meeting m(title, organiser, participant, agenda, duration, datem);
    if (m.add()) {
        QMessageBox::information(nullptr, "Success", "Meeting added successfully.");
        ui->meetingTitleInput->clear();
        ui->meetingOrganiserInput->clear();
        ui->meetingParticipantInput->clear();
        ui->meetingDurationInput->clear();
        ui->meetingAgendaComboBox->setCurrentIndex(0);
        ui->meetingDateTimeEdit->setDateTime(QDateTime::currentDateTime());
        refreshTableWidget();
        if (notificationManager) {
            notificationManager->addNotification("Meeting Added", "Meeting: " + title, "Added meeting with ID " + QString::number(m.getId()), -1);
        }
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to add meeting.");
    }
}

void MeetingManager::handleDeleteButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to delete.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    QString title = ui->meetingTableWidget->item(row, 1)->text();

    meeting m;
    if (m.deleteMeeting(id)) {
        QMessageBox::information(nullptr, "Success", "Meeting deleted successfully.");
        refreshTableWidget();
        if (notificationManager) {
            notificationManager->addNotification("Meeting Deleted", "Meeting: " + title, "Deleted meeting with ID " + QString::number(id), -1);
        }
    } else {
        QMessageBox::warning(nullptr, "Error", "Failed to delete meeting. Check the database connection.");
    }
}

void MeetingManager::handleUpdateButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to update.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    meeting m;
    m.setId(id);
    m.setTitle(ui->meetingTableWidget->item(row, 1)->text());
    m.setOrganiser(ui->meetingTableWidget->item(row, 2)->text());
    m.setParticipant(ui->meetingTableWidget->item(row, 3)->text());
    m.setAgenda(ui->meetingTableWidget->item(row, 4)->text());
    m.setDuration(ui->meetingTableWidget->item(row, 5)->text().replace(" min", "").toInt());
    m.setDatem(QDateTime::fromString(ui->meetingTableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm"));

    UpdateMeeting dialog(nullptr, &m);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTableWidget();
        if (notificationManager) {
            notificationManager->addNotification("Meeting Updated", "Meeting: " + m.getTitle(), "Updated meeting with ID " + QString::number(id), -1);
        }
    }
}

void MeetingManager::handleSearchButtonClick()
{
    QString searchText = ui->meetingSearchInput->text();
    if (searchText.isEmpty()) {
        refreshTableWidget();
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);
    if (model->rowCount() == 0) {
        QMessageBox::information(nullptr, "No Results", "No meetings found with the given title.");
        delete model;
        return;
    }

    ui->meetingTableWidget->setRowCount(0);
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->meetingTableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->meetingTableWidget->setItem(row, col, item);
        }
    }
    delete model;
}

void MeetingManager::handleSearchTextChanged(const QString &searchText)
{
    if (searchText.isEmpty()) {
        refreshTableWidget();
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);
    ui->meetingTableWidget->setRowCount(0);
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->meetingTableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) {
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->meetingTableWidget->setItem(row, col, item);
        }
    }
    delete model;
}

void MeetingManager::handleGenerateQRCodeButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to generate a QR code.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    meeting m;
    m.setId(id);
    m.setTitle(ui->meetingTableWidget->item(row, 1)->text());
    m.setOrganiser(ui->meetingTableWidget->item(row, 2)->text());
    m.setParticipant(ui->meetingTableWidget->item(row, 3)->text());
    m.setAgenda(ui->meetingTableWidget->item(row, 4)->text());
    m.setDuration(ui->meetingTableWidget->item(row, 5)->text().replace(" min", "").toInt());
    m.setDatem(QDateTime::fromString(ui->meetingTableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm"));

    QPixmap qrCode = m.generateQRCode();
    if (qrCode.isNull()) {
        QMessageBox::critical(nullptr, "Error", "Failed to generate QR code.");
        return;
    }

    QMessageBox qrDialog(nullptr);
    qrDialog.setWindowTitle("Meeting QR Code");
    qrDialog.setText("Scan this QR code for full meeting details:");
    qrDialog.setIconPixmap(qrCode.scaled(200, 200, Qt::KeepAspectRatio));
    qrDialog.exec();

    // Optional: Save QR code to file
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save QR Code", "Meeting_" + QString::number(id) + ".png", "PNG Files (*.png)");
    if (!filePath.isEmpty()) {
        qrCode.save(filePath);
        if (notificationManager) {
            notificationManager->addNotification("QR Code Generated", "Meeting ID: " + QString::number(id), "QR code saved to " + filePath, -1);
        }
        QMessageBox::information(nullptr, "Success", "QR code saved successfully!");
    }
}

void MeetingManager::handleExportPdfButtonClick()
{
    int selectedRow = ui->meetingTableWidget->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to export.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "Meeting_" + ui->meetingTableWidget->item(selectedRow, 0)->text() + ".pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setPen(Qt::black);

    const int margin = 500;
    const int lineSpacing = 200;
    const int qrSize = 500;
    const int padding = 250;
    const int pageWidth = pdfWriter.width() - 2 * margin;
    int yPos = margin;

    QFont titleFont("Arial", 16, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor("#3A5DAE"));
    painter.drawText(margin, yPos, "Meeting Management System - Selected Meeting");
    yPos += 300;

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont valueFont("Arial", 10);

    QString id = ui->meetingTableWidget->item(selectedRow, 0)->text();
    QString title = ui->meetingTableWidget->item(selectedRow, 1)->text();
    QString organiser = ui->meetingTableWidget->item(selectedRow, 2)->text();
    QString participant = ui->meetingTableWidget->item(selectedRow, 3)->text();
    QString agenda = ui->meetingTableWidget->item(selectedRow, 4)->text();
    QString duration = ui->meetingTableWidget->item(selectedRow, 5)->text();
    QString dateTimeStr = ui->meetingTableWidget->item(selectedRow, 6)->text();

    QStringList labels = {"ID:", "Title:", "Organiser:", "Participant:", "Agenda:", "Duration:", "Date and Time:"};
    QStringList values = {id, title, organiser, participant, agenda, duration, dateTimeStr};

    int maxLabelWidth = 0;
    QFontMetrics fmLabel(labelFont);
    for (const QString& label : labels) {
        int labelWidth = fmLabel.horizontalAdvance(label);
        if (labelWidth > maxLabelWidth) {
            maxLabelWidth = labelWidth;
        }
    }
    maxLabelWidth += padding;

    for (int i = 0; i < labels.size(); ++i) {
        painter.setFont(labelFont);
        painter.setPen(QColor("#3A5DAE"));
        painter.drawText(margin, yPos, labels[i]);
        painter.setFont(valueFont);
        painter.setPen(Qt::black);
        painter.drawText(margin + maxLabelWidth, yPos, values[i]);
        yPos += lineSpacing;
    }

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

    painter.setPen(QPen(Qt::gray, 20));
    painter.drawLine(margin, yPos, pageWidth + margin, yPos);

    painter.end();
    QMessageBox::information(nullptr, "Success", "Selected meeting exported to PDF successfully!");
}

void MeetingManager::handleTabChanged(int index)
{
    if (index == 0) { // Add Meeting tab
        ui->meetingTitleInput->clear();
        ui->meetingOrganiserInput->clear();
        ui->meetingParticipantInput->clear();
        ui->meetingAgendaComboBox->setCurrentIndex(0);
        ui->meetingDurationInput->clear();
        ui->meetingDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }
}

void MeetingManager::handleSortCriteriaChanged(int index)
{
    if (!m_dbConnected) return;

    QString column;
    switch (index) {
    case 0: column = "id"; break;
    case 1: column = "title"; break;
    case 2: column = "organiser"; break;
    case 3: column = "participant"; break;
    case 4: column = "agenda"; break;
    case 5: column = "duration"; break;
    case 6: column = "datem"; break;
    default: return;
    }

    QString queryStr = QString("SELECT id, title, organiser, participant, agenda, duration, datem FROM meeting ORDER BY %1").arg(column);
    QSqlQuery query(queryStr);

    ui->meetingTableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->meetingTableWidget->insertRow(row);
        ui->meetingTableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->meetingTableWidget->setItem(row, 1, new QTableWidgetItem(query.value("title").toString()));
        ui->meetingTableWidget->setItem(row, 2, new QTableWidgetItem(query.value("organiser").toString()));
        ui->meetingTableWidget->setItem(row, 3, new QTableWidgetItem(query.value("participant").toString()));
        ui->meetingTableWidget->setItem(row, 4, new QTableWidgetItem(query.value("agenda").toString()));
        ui->meetingTableWidget->setItem(row, 5, new QTableWidgetItem(query.value("duration").toString() + " min"));
        ui->meetingTableWidget->setItem(row, 6, new QTableWidgetItem(query.value("datem").toDateTime().toString("yyyy-MM-dd hh:mm")));
        row++;
    }

    ui->meetingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->meetingTableWidget->setColumnWidth(6, 150);
}

void MeetingManager::handleStatisticsButtonClick()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    // Defer to MainWindow's ChartWindow
    QMessageBox::information(nullptr, "Statistics", "Statistics are available via the main Statistics button.");
}

void MeetingManager::handleRefreshStatsButtonClick()
{
    // Placeholder: meetingRefreshStatsButton not in mainwindow.ui
    QMessageBox::information(nullptr, "Refresh Stats", "Statistics refresh is currently disabled.");
}

void MeetingManager::updateInputFields()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        return;
    }

    ui->meetingTitleInput->setText(ui->meetingTableWidget->item(row, 1)->text());
    ui->meetingOrganiserInput->setText(ui->meetingTableWidget->item(row, 2)->text());
    ui->meetingParticipantInput->setText(ui->meetingTableWidget->item(row, 3)->text());
    ui->meetingAgendaComboBox->setCurrentText(ui->meetingTableWidget->item(row, 4)->text());
    ui->meetingDurationInput->setText(ui->meetingTableWidget->item(row, 5)->text().replace(" min", ""));
}

void MeetingManager::refreshTableWidget()
{
    meeting m;
    QSqlQueryModel* model = m.afficher();

    ui->meetingTableWidget->setColumnCount(7);
    ui->meetingTableWidget->setHorizontalHeaderLabels({"ID", "Title", "Organiser", "Participant", "Agenda", "Duration", "Date and Time"});
    ui->meetingTableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->meetingTableWidget->insertRow(row);
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
            ui->meetingTableWidget->setItem(row, col, item);
        }
    }

    ui->meetingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->meetingTableWidget->setColumnWidth(6, 150);
    delete model;
}

void MeetingManager::applyThemeStyles()
{
    ui->meetingTableWidget->setStyleSheet("QTableView { background-color: #FFFFFF; border: 1px solid #D3DCE6; selection-background-color: #A1B8E6; }"
                                          "QHeaderView::section { background-color: #3A5DAE; color: white; }");
}
