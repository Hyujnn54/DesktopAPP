// managers/meeting/meetingmanager.cpp
#include "meetingmanager.h"
#include "ui_mainwindow.h"
#include "../../dialog/updatemeeting/updatemeeting.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFontMetrics>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQuery>

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
    qDebug() << "Entering MeetingManager::initialize";
    this->ui = ui;
    if (!ui) {
        qDebug() << "Error: ui is null";
        return;
    }

    // Block signals during initialization
    ui->meetingTableWidget->blockSignals(true);

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

    // Set up table properties
    ui->meetingTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->meetingTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->meetingTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Refresh table
    refreshTableWidget();

    // Apply theme styles
    applyThemeStyles();

    // Re-enable signals
    ui->meetingTableWidget->blockSignals(false);
    qDebug() << "Exiting MeetingManager::initialize";
}

void MeetingManager::handleAddButtonClick()
{
    QString title = ui->meetingTitleInput->text();
    QString organiser = ui->meetingOrganiserInput->text();
    QString participant = ui->meetingParticipantInput->text();
    QString agenda = ui->meetingAgendaComboBox->currentText();
    int duration = ui->meetingDurationInput->text().toInt();
    QDateTime dateTime = ui->meetingDateTimeEdit->dateTime();

    if (title.isEmpty() || organiser.isEmpty() || participant.isEmpty() || agenda.isEmpty() || duration <= 0) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in all fields correctly.");
        return;
    }

    // Use -1 as a placeholder ID for new meetings
    meeting m(-1, title, organiser, participant, agenda, duration, dateTime);
    if (m.add()) {
        QMessageBox::information(nullptr, "Success", "Meeting added successfully!");
        refreshTableWidget();
        if (notificationManager) {
            notificationManager->addNotification("Meeting Added", "Title: " + title, "Organiser: " + organiser, -1);
        } else {
            qDebug() << "Warning: notificationManager is null in handleAddButtonClick";
        }
    } else {
        QMessageBox::critical(nullptr, "Error", "Failed to add meeting.");
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
    if (!m_dbConnected) {
        QMessageBox::warning(ui->centralwidget, "Database Error", "Cannot update meeting: Database is not connected.");
        return;
    }
    QTableWidget *table = ui->meetingTableWidget;
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::warning(ui->centralwidget, "Error", "Please select a meeting to update.");
        return;
    }

    bool ok;
    int meetingId = table->item(row, 0)->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(ui->centralwidget, "Error", "Invalid meeting ID.");
        return;
    }

    meeting m;
    QSqlQuery query;
    query.prepare("SELECT * FROM AHMED.MEETING WHERE ID = :id");
    query.bindValue(":id", meetingId);
    if (query.exec() && query.next()) {
        m.setId(query.value("ID").toInt());
        m.setTitle(query.value("TITLE").toString());
        m.setOrganiser(query.value("ORGANISER").toString());
        m.setParticipant(query.value("PARTICIPANT").toString());
        m.setAgenda(query.value("AGENDA").toString());
        m.setDuration(query.value("DURATION").toInt());
        m.setDatem(query.value("MEETING_DATE").toDateTime());
        m.setEmployeeId(query.value("EMPLOYEE_ID").isNull() ? QVariant() : query.value("EMPLOYEE_ID"));
        m.setClientId(query.value("CLIENT_ID").isNull() ? QVariant() : query.value("CLIENT_ID"));
        m.setResourceId(query.value("RESSOURCE_ID").isNull() ? QVariant() : query.value("RESSOURCE_ID"));
    } else {
        QMessageBox::warning(ui->centralwidget, "Error", "Failed to load meeting data: " + query.lastError().text());
        return;
    }

    UpdateMeeting dialog(ui->centralwidget, &m);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTableWidget();
        QMessageBox::information(ui->centralwidget, "Success", "Meeting updated successfully.");
        if (notificationManager) {
            notificationManager->addNotification("Meeting Updated", "Title: " + m.getTitle(), "Meeting ID: " + QString::number(meetingId), -1);
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
        ui->meetingTableWidget->setItem(row, 0, new QTableWidgetItem(model->data(model->index(row, 0)).toString()));
        ui->meetingTableWidget->setItem(row, 1, new QTableWidgetItem(model->data(model->index(row, 1)).toString()));
        ui->meetingTableWidget->setItem(row, 2, new QTableWidgetItem(model->data(model->index(row, 2)).toString()));
        ui->meetingTableWidget->setItem(row, 3, new QTableWidgetItem(model->data(model->index(row, 3)).toString()));
        ui->meetingTableWidget->setItem(row, 4, new QTableWidgetItem(model->data(model->index(row, 4)).toString()));
        ui->meetingTableWidget->setItem(row, 5, new QTableWidgetItem(model->data(model->index(row, 5)).toString() + " min"));
        ui->meetingTableWidget->setItem(row, 6, new QTableWidgetItem(model->data(model->index(row, 6)).toDateTime().toString("yyyy-MM-dd hh:mm")));
    }
    delete model;
}

void MeetingManager::handleSearchTextChanged(const QString &searchText)
{
    if (!m_dbConnected) {
        return;
    }

    int column = ui->meetingSearchCriteriaComboBox->currentIndex();
    
    // If search box is empty, show all data
    if (searchText.isEmpty()) {
        for (int i = 0; i < ui->meetingTableWidget->rowCount(); i++) {
            ui->meetingTableWidget->setRowHidden(i, false);
        }
        return;
    }
    
    // Hide rows that don't match the search criteria and column
    for (int i = 0; i < ui->meetingTableWidget->rowCount(); i++) {
        bool matchFound = false;
        QTableWidgetItem* item = ui->meetingTableWidget->item(i, column);
        
        if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
            matchFound = true;
        }
        
        ui->meetingTableWidget->setRowHidden(i, !matchFound);
    }
}

void MeetingManager::handleGenerateQRCodeButtonClick()
{
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(nullptr, "Error", "Please select a meeting to generate a QR code.");
        return;
    }

    int id = ui->meetingTableWidget->item(row, 0)->text().toInt();
    meeting m(
        id,
        ui->meetingTableWidget->item(row, 1)->text(),
        ui->meetingTableWidget->item(row, 2)->text(),
        ui->meetingTableWidget->item(row, 3)->text(),
        ui->meetingTableWidget->item(row, 4)->text(),
        ui->meetingTableWidget->item(row, 5)->text().replace(" min", "").toInt(),
        QDateTime::fromString(ui->meetingTableWidget->item(row, 6)->text(), "yyyy-MM-dd hh:mm")
        );

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

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save QR Code", "Meeting_" + QString::number(id) + ".png", "PNG Files (*.png)");
    if (!filePath.isEmpty()) {
        qrCode.save(filePath);
        if (notificationManager) {
            notificationManager->addNotification("QR Code Generated", "Meeting ID: " + QString::number(id),
                                                 "QR code saved to " + filePath, -1);
        }
        QMessageBox::information(nullptr, "Success", "QR code saved successfully!");
    }
}

void MeetingManager::handleExportPdfButtonClick()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }
    exportAllMeetingsToPdf();
}

void MeetingManager::exportAllMeetingsToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "Meetings.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&pdfWriter);
    QFont regularFont("Arial", 9);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont titleFont("Arial", 16, QFont::Bold);

    // Set up metrics
    int pageWidth = pdfWriter.width();
    int tableWidth = pageWidth - 40;
    int rowHeight = 30;
    
    // Draw title
    painter.setFont(titleFont);
    painter.drawText(20, 30, "Meeting List");
    
    // Define column headers and widths
    QStringList headers = {"ID", "Title", "Organiser", "Participant", "Agenda", "Duration", "Date & Time"};
    QVector<qreal> columnWidths = {0.05, 0.2, 0.15, 0.15, 0.15, 0.1, 0.2}; // Proportional widths
    
    int y = 50;
    painter.setFont(headerFont);
    
    // Draw table header
    int x = 20;
    QRect headerRect(20, y, tableWidth, rowHeight);
    painter.fillRect(headerRect, QColor(230, 230, 230));
    painter.setPen(QPen(Qt::black));
    painter.drawRect(headerRect);
    
    // Draw header cells with borders
    for (int i = 0; i < headers.size(); ++i) {
        int colWidth = tableWidth * columnWidths[i];
        QRect cellRect(x, y, colWidth, rowHeight);
        
        // Draw cell border
        painter.drawRect(cellRect);
        
        // Draw header text
        painter.drawText(cellRect, Qt::AlignCenter, headers[i]);
        x += colWidth;
    }
    
    y += rowHeight;
    painter.setFont(regularFont);
    
    // Count visible rows
    int visibleRowCount = 0;
    for (int row = 0; row < ui->meetingTableWidget->rowCount(); ++row) {
        if (!ui->meetingTableWidget->isRowHidden(row)) {
            visibleRowCount++;
        }
    }
    
    if (visibleRowCount == 0) {
        QRect noDataRect(20, y, tableWidth, rowHeight);
        painter.drawRect(noDataRect);
        painter.drawText(noDataRect, Qt::AlignCenter, "No meetings to display.");
        painter.end();
        
        // Show notification
        QMessageBox::information(nullptr, "Export Complete", "PDF file created, but no meetings were available to display.");
        return;
    }

    // Alternate row colors
    QColor altRowColor(245, 245, 245);
    int visibleRowIndex = 0;
    
    for (int row = 0; row < ui->meetingTableWidget->rowCount(); ++row) {
        // Skip hidden rows (filtered out by search)
        if (ui->meetingTableWidget->isRowHidden(row)) {
            continue;
        }
        
        // Set alternating row colors
        if (visibleRowIndex % 2 == 1) {
            QRect rowRect(20, y, tableWidth, rowHeight);
            painter.fillRect(rowRect, altRowColor);
        }
        visibleRowIndex++;
        
        x = 20;
        for (int col = 0; col < headers.size(); ++col) {
            int colWidth = tableWidth * columnWidths[col];
            QRect cellRect(x, y, colWidth, rowHeight);
            
            // Draw cell border
            painter.drawRect(cellRect);
            
            // Get and format cell data
            QString text = ui->meetingTableWidget->item(row, col) ? ui->meetingTableWidget->item(row, col)->text() : "";
            
            // Format duration column
            if (col == 5 && !text.contains("min")) {
                text += " min";
            }
            
            // Draw cell text with padding
            painter.drawText(cellRect.adjusted(5, 5, -5, -5), Qt::AlignVCenter | Qt::AlignLeft, text);
            x += colWidth;
        }
        
        y += rowHeight;
        
        // Check if we need a new page
        if (y > pdfWriter.height() - 40) {
            pdfWriter.newPage();
            y = 50;
            
            // Redraw header on new page
            painter.setFont(headerFont);
            
            // Draw table header
            x = 20;
            QRect headerRect(20, y, tableWidth, rowHeight);
            painter.fillRect(headerRect, QColor(230, 230, 230));
            painter.drawRect(headerRect);
            
            for (int i = 0; i < headers.size(); ++i) {
                int colWidth = tableWidth * columnWidths[i];
                QRect cellRect(x, y, colWidth, rowHeight);
                painter.drawRect(cellRect);
                painter.drawText(cellRect, Qt::AlignCenter, headers[i]);
                x += colWidth;
            }
            
            y += rowHeight;
            painter.setFont(regularFont);
        }
    }

    painter.end();

    // Show success message
    QMessageBox::information(nullptr, "Success", "Meeting list exported to PDF successfully!");
    
    if (notificationManager) {
        notificationManager->addNotification("PDF Exported", "Meeting Section", 
                                         "Meeting list exported to " + fileName, -1);
    }
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
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot sort meetings: Database is not connected.");
        return;
    }
    
    // Get the current search text and criteria
    QString searchText = ui->meetingSearchInput->text().trimmed();
    int column = ui->meetingSearchCriteriaComboBox->currentIndex();
    
    // First reset the table to show all data
    refreshTableWidget();
    
    // If there's search text, filter the table
    if (!searchText.isEmpty()) {
        QList<QTableWidgetItem*> items = ui->meetingTableWidget->findItems(searchText, Qt::MatchContains);
        
        // Hide rows that don't match the search criteria and column
        for (int i = 0; i < ui->meetingTableWidget->rowCount(); i++) {
            bool matchFound = false;
            QTableWidgetItem* item = ui->meetingTableWidget->item(i, column);
            
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                matchFound = true;
            }
            
            ui->meetingTableWidget->setRowHidden(i, !matchFound);
        }
    }
    
    // Sort the table based on the selected column
    ui->meetingTableWidget->sortItems(index, Qt::AscendingOrder);
}

void MeetingManager::handleStatisticsButtonClick()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    QMessageBox::information(nullptr, "Statistics", "Statistics are available via the main Statistics button.");
}

void MeetingManager::handleRefreshStatsButtonClick()
{
    QMessageBox::information(nullptr, "Refresh Stats", "Statistics refresh is currently disabled.");
}

void MeetingManager::updateInputFields()
{
    qDebug() << "Entering updateInputFields";
    int row = ui->meetingTableWidget->currentRow();
    if (row < 0) {
        qDebug() << "No row selected in updateInputFields";
        return;
    }

    auto getItemText = [&](int col, const QString& defaultValue = "") {
        QTableWidgetItem* item = ui->meetingTableWidget->item(row, col);
        return item ? item->text() : defaultValue;
    };

    ui->meetingTitleInput->setText(getItemText(1));
    ui->meetingOrganiserInput->setText(getItemText(2));
    ui->meetingParticipantInput->setText(getItemText(3));
    ui->meetingAgendaComboBox->setCurrentText(getItemText(4));
    ui->meetingDurationInput->setText(getItemText(5).replace(" min", ""));
    qDebug() << "Exiting updateInputFields";
}

void MeetingManager::refreshTableWidget()
{
    if (!m_dbConnected) {
        return;
    }

    ui->meetingTableWidget->clearContents();
    ui->meetingTableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT ID, TITLE, ORGANISER, PARTICIPANT, AGENDA, DURATION, DATEM FROM AHMED.MEETING ORDER BY ID");
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Database Error",
                             "Failed to load meetings: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->meetingTableWidget->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(query.value("ID").toString());
        QTableWidgetItem *titleItem = new QTableWidgetItem(query.value("TITLE").toString());
        QTableWidgetItem *organiserItem = new QTableWidgetItem(query.value("ORGANISER").toString());
        QTableWidgetItem *participantItem = new QTableWidgetItem(query.value("PARTICIPANT").toString());
        QTableWidgetItem *agendaItem = new QTableWidgetItem(query.value("AGENDA").toString());
        QTableWidgetItem *durationItem = new QTableWidgetItem(query.value("DURATION").toString());
        QTableWidgetItem *dateItem = new QTableWidgetItem(query.value("DATEM").toDateTime().toString("yyyy-MM-dd HH:mm"));

        // Store the original data for sorting
        idItem->setData(Qt::UserRole, query.value("ID").toInt());
        durationItem->setData(Qt::UserRole, query.value("DURATION").toInt());
        dateItem->setData(Qt::UserRole, query.value("DATEM").toDateTime());

        ui->meetingTableWidget->setItem(row, 0, idItem);
        ui->meetingTableWidget->setItem(row, 1, titleItem);
        ui->meetingTableWidget->setItem(row, 2, organiserItem);
        ui->meetingTableWidget->setItem(row, 3, participantItem);
        ui->meetingTableWidget->setItem(row, 4, agendaItem);
        ui->meetingTableWidget->setItem(row, 5, durationItem);
        ui->meetingTableWidget->setItem(row, 6, dateItem);

        row++;
    }

    // Enable sorting
    ui->meetingTableWidget->setSortingEnabled(true);

    // Set custom sort role to handle different data types
    ui->meetingTableWidget->horizontalHeader()->setSortIndicatorShown(true);
}

void MeetingManager::applyThemeStyles()
{
    ui->meetingTableWidget->setStyleSheet("QTableView { background-color: #FFFFFF; border: 1px solid #D3DCE6; selection-background-color: #A1B8E6; }"
                                          "QHeaderView::section { background-color: #3A5DAE; color: white; }");
}

QMap<QString, int> MeetingManager::getStatisticsByCategory(const QString &category)
{
    QMap<QString, int> stats;
    if (!m_dbConnected) {
        qDebug() << "Database not connected in getStatisticsByCategory";
        return stats;
    }

    QSqlQuery query;
    
    if (category == "Organiser") {
        query.prepare("SELECT ORGANISER, COUNT(*) AS count FROM AHMED.MEETING GROUP BY ORGANISER ORDER BY count DESC");
    } 
    else if (category == "Participant") {
        query.prepare("SELECT PARTICIPANT, COUNT(*) AS count FROM AHMED.MEETING GROUP BY PARTICIPANT ORDER BY count DESC");
    } 
    else if (category == "Agenda") {
        query.prepare("SELECT AGENDA, COUNT(*) AS count FROM AHMED.MEETING GROUP BY AGENDA ORDER BY count DESC");
    } 
    else if (category == "Date") {
        query.prepare("SELECT TO_CHAR(DATEM, 'YYYY-MM-DD') as meeting_date, COUNT(*) AS count "
                     "FROM AHMED.MEETING "
                     "GROUP BY TO_CHAR(DATEM, 'YYYY-MM-DD') "
                     "ORDER BY meeting_date ASC");
    } 
    else {
        // Default fallback - by organizer
        query.prepare("SELECT ORGANISER, COUNT(*) AS count FROM AHMED.MEETING GROUP BY ORGANISER ORDER BY count DESC");
    }

    if (!query.exec()) {
        qDebug() << "Query failed in getStatisticsByCategory:" << query.lastError().text();
        return stats;
    }

    while (query.next()) {
        QString key = query.value(0).toString().trimmed();
        if (key.isEmpty()) {
            key = "Unknown " + category;
        }
        int count = query.value(1).toInt();
        stats[key] = count;
    }

    return stats;
}
