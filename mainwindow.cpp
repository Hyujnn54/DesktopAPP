#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
#include "client.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QInputDialog>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QHoverEvent>
#include <QToolTip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isDarkTheme(true)
{
    ui->setupUi(this);

    // Set up the datetime edit widget for consultations
    QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(this);
    dateTimeEdit->setObjectName("consultation_datetime");
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
    dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->ClientForm->layout());
    if (gridLayout) {
        int row = 0, column = 0;
        for (int i = 0; i < gridLayout->count(); ++i) {
            int r, c, rs, cs;
            gridLayout->getItemPosition(i, &r, &c, &rs, &cs);
            if (gridLayout->itemAtPosition(r, c) &&
                gridLayout->itemAtPosition(r, c)->widget() == ui->searchDateButton_2) {
                row = r;
                column = c;
                break;
            }
        }
        gridLayout->removeWidget(ui->searchDateButton_2);
        ui->searchDateButton_2->deleteLater();
        gridLayout->addWidget(dateTimeEdit, row, column);
    }

    // Update search date widget to search date time
    QDateTimeEdit *searchDateTimeEdit = new QDateTimeEdit(this);
    searchDateTimeEdit->setObjectName("searchDateTimeEdit");
    searchDateTimeEdit->setCalendarPopup(true);
    searchDateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
    searchDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Connect existing buttons
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->add, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    connect(ui->Delet, &QPushButton::clicked, this, &MainWindow::on_removeButtonClicked);
    connect(ui->update, &QPushButton::clicked, this, &MainWindow::on_updateButtonClicked);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButton_clicked);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::tableViewHeaderClicked);

    // Connect dynamic search signals
    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::on_searchInput_textChanged);
    connect(ui->searchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_searchCriteriaComboBox_currentIndexChanged);

    // Setup calendar functionality
    setupCalendarView();
    QSqlQueryModel *model = Etmp.afficher();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    // Ensure all columns are visible
    for (int i = 0; i < model->columnCount(); ++i) {
        ui->tableView->showColumn(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButtonclicked() {
    if (!validateInputs()) {
        return;
    }

    QString Name = ui->Name->text().trimmed();
    QString Sector = ui->Sector->text().trimmed();
    QString Contact = ui->Contact_info->text().trimmed();
    QString Email = ui->Email->text().trimmed(); // New field
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime Cdate = dateTimeEdit ? dateTimeEdit->dateTime() : QDateTime::currentDateTime();
    int Consultant = ui->Consultant->text().toInt();

    Client c(Name, Sector, Contact, Email, Cdate, Consultant); // Updated constructor call

    if (c.ajouter()) {
        QMessageBox::information(this, "Success", "Client added successfully!");
        QSqlQueryModel *model = Etmp.afficher();
        ui->tableView->setModel(model);
        ui->Name->clear();
        ui->Sector->clear();
        ui->Contact_info->clear();
        ui->Email->clear(); // Clear new field
        if (dateTimeEdit) dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->Consultant->clear();
        updateCalendarConsultations();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add client.");
    }
}

void MainWindow::on_removeButtonClicked() {
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    qDebug() << "Selected Rows Count:" << selectedRows.count(); // Debug output

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a client to remove.");
        return;
    }

    int row = selectedRows.first().row();
    QString clientName = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();

    Client c;
    if (c.removeByName(clientName)) {
        QMessageBox::information(this, "Success", "Client removed successfully!");

        // Refresh the table view
        QSqlQueryModel *model = Etmp.afficher();
        ui->tableView->setModel(model);

        // Update calendar view after removal
        updateCalendarConsultations();
    } else {
        QMessageBox::critical(this, "Error", "Failed to remove the client.");
    }
}

void MainWindow::toggleSidebar() {
    // Check if the sidebar is currently visible
    bool isVisible = ui->sideMenu->isVisible();

    // Toggle visibility
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::on_updateButtonClicked() {
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a client to update.");
        return;
    }

    int row = selectedRows.first().row();
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());

    if (!model) {
        QMessageBox::critical(this, "Error", "Failed to get model data.");
        return;
    }

    // Get the data from the selected row
    QString name = model->data(model->index(row, 0)).toString();
    QString sector = model->data(model->index(row, 1)).toString();
    QString contact = model->data(model->index(row, 2)).toString();
    QString email = model->data(model->index(row, 3)).toString(); // New field
    QDateTime dateTime = model->data(model->index(row, 4)).toDateTime();
    int consultant = model->data(model->index(row, 5)).toInt();

    UpdateClientDialog updateDialog(this);
    updateDialog.setClientData(name, sector, contact, email, dateTime, consultant); // Updated

    if (updateDialog.exec() == QDialog::Accepted) {
        QString newName = updateDialog.getName();
        QString newSector = updateDialog.getSector();
        QString newContact = updateDialog.getContactInfo();
        QString newEmail = updateDialog.getEmail(); // New field
        QDateTime newDateTime = updateDialog.getConsultationDateTime();
        int newConsultant = updateDialog.getConsultant();

        Client c;
        if (c.updateClient(name, newName, newSector, newContact, newEmail, newDateTime, newConsultant)) { // Updated
            QMessageBox::information(this, "Success", "Client updated successfully!");
            QSqlQueryModel *updatedModel = Etmp.afficher();
            ui->tableView->setModel(updatedModel);
            updateCalendarConsultations();
        } else {
            QMessageBox::critical(this, "Error", "Failed to update the client.");
        }
    }
}

void MainWindow::toggleTheme() {
    isDarkTheme = !isDarkTheme;

    if (isDarkTheme) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
}

void MainWindow::applyDarkTheme() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void MainWindow::applyLightTheme() {
    qApp->setPalette(QApplication::style()->standardPalette());
    qApp->setStyleSheet("");
}

void MainWindow::on_resetSearchButton_clicked()
{
    ui->searchInput->clear();
    QSqlQueryModel *model = Etmp.afficher();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    // Ensure all columns are visible
    for (int i = 0; i < model->columnCount(); ++i) {
        ui->tableView->showColumn(i);
    }
    ui->statusBar->showMessage("Showing all clients");
}

void MainWindow::tableViewHeaderClicked(int logicalIndex) {
    // Toggle sort order
    static Qt::SortOrder currentOrder = Qt::AscendingOrder;
    currentOrder = (currentOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;

    // Apply sorting
    QSqlQueryModel* sortedModel = Etmp.sortByColumn(logicalIndex, currentOrder);
    ui->tableView->setModel(sortedModel);

    // Update status message
    QString column;
    switch (logicalIndex) {
    case 0: column = "Name"; break;
    case 1: column = "Sector"; break;
    case 2: column = "Contact"; break;
    case 3: column = "Email"; break;  // Add this line
    case 4: column = "Date"; break;
    case 5: column = "Consultant"; break;
    default: column = "Unknown"; break;
    }

    QString order = (currentOrder == Qt::AscendingOrder) ? "ascending" : "descending";
    ui->statusBar->showMessage(QString("Sorted by %1 (%2)").arg(column, order));
}

bool MainWindow::validateInputs() {
    // Validate name
    QString name = ui->Name->text().trimmed();
    if (!isValidName(name)) {
        QMessageBox::warning(this, "Input Error", "Name cannot be empty or contain numbers.");
        ui->Name->setFocus();
        return false;
    }

    // Validate sector
    QString sector = ui->Sector->text().trimmed();
    if (sector.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Sector cannot be empty.");
        ui->Sector->setFocus();
        return false;
    }

    // Validate contact info
    QString contact = ui->Contact_info->text().trimmed();
    if (contact.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Contact information cannot be empty.");
        ui->Contact_info->setFocus();
        return false;
    }

    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    if (!dateTimeEdit) {
        QMessageBox::critical(this, "Error", "DateTime widget not found.");
        return false;
    }
    
    QDateTime dateTime = dateTimeEdit->dateTime();
    if (!isValidDateTime(dateTime)) {
        QMessageBox::warning(this, "Input Error", "Date and time must be current or in the future.");
        dateTimeEdit->setFocus();
        return false;
    }

    // Validate consultant ID
    QString consultant = ui->Consultant->text().trimmed();
    if (!isValidConsultant(consultant)) {
        QMessageBox::warning(this, "Input Error", "Consultant ID must be a valid number.");
        ui->Consultant->setFocus();
        return false;
    }
    QString email = ui->Email->text().trimmed();
    if (email.isEmpty() || !email.contains('@') || !email.contains('.')) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid email address.");
        ui->Email->setFocus();
        return false;
    }

    return true;
}

bool MainWindow::isValidName(const QString &name) {
    if (name.isEmpty()) return false;

    // Check if name contains any digits
    for (const QChar &ch : name) {
        if (ch.isDigit()) return false;
    }

    return true;
}

bool MainWindow::isValidDateTime(const QDateTime &dateTime) {
    // Check if dateTime is current or in the future
    return dateTime >= QDateTime::currentDateTime();
}

bool MainWindow::isValidDate(const QDate &date) {
    // Check if date is current date or in the future
    return date >= QDate::currentDate();
}

bool MainWindow::isValidConsultant(const QString &consultant) {
    if (consultant.isEmpty()) return false;

    bool ok;
    consultant.toInt(&ok);
    return ok;
}

void MainWindow::setupCalendarView()
{
    // Connect calendar signals
    connect(ui->consultationCalendar, &QCalendarWidget::selectionChanged,
            this, &MainWindow::on_consultationCalendar_selectionChanged);
    connect(ui->consultationCalendar, &QCalendarWidget::activated,
            this, &MainWindow::on_consultationCalendar_activated);

    // Create and install event filter for hover tooltips
    CalendarHoverItem *hoverItem = new CalendarHoverItem(ui->consultationCalendar, &consultationCountMap);
    ui->consultationCalendar->installEventFilter(hoverItem);

    // Set first day of week to Monday
    ui->consultationCalendar->setFirstDayOfWeek(Qt::Monday);

    // Load consultations and update calendar
    updateCalendarConsultations();

    // Initialize selected date info
    updateSelectedDateInfo(ui->consultationCalendar->selectedDate());
}

void MainWindow::updateCalendarConsultations()
{
    QMap<QDateTime, int> dtMap = Etmp.getConsultationCountsByDateTime();
    consultationCountMap.clear();
    
    // Group by date (ignore time)
    for (auto it = dtMap.begin(); it != dtMap.end(); ++it) {
        QDate date = it.key().date();
        consultationCountMap[date] += it.value();
    }
    
    // Highlight dates with consultations on the calendar
    highlightDatesWithConsultations();
}

void MainWindow::highlightDatesWithConsultations()
{
    // Clear any existing formatting
    ui->consultationCalendar->setDateTextFormat(QDate(), QTextCharFormat());

    // Format for highlighting dates with consultations
    QTextCharFormat consultationFormat;
    consultationFormat.setBackground(QColor(100, 150, 255, 100)); // Light blue background

    // Apply formatting to all dates with consultations
    QMapIterator<QDate, int> i(consultationCountMap);
    while (i.hasNext()) {
        i.next();
        ui->consultationCalendar->setDateTextFormat(i.key(), consultationFormat);
    }
}

void MainWindow::on_consultationCalendar_selectionChanged()
{
    // Update the information for the newly selected date
    QDate selectedDate = ui->consultationCalendar->selectedDate();
    updateSelectedDateInfo(selectedDate);
}

void MainWindow::on_consultationCalendar_activated(const QDate &date)
{
    // Handle double-click on date
    // This could be used to add a new consultation on this date
    QDateTime currentDateTime = QDateTime(date, QTime::currentTime());

    // Find the consultation_datetime widget and set its value
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    if (dateTimeEdit) {
        dateTimeEdit->setDateTime(currentDateTime);
    }

    // Switch to the Add Client tab
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::updateSelectedDateInfo(const QDate &date) {
    // Update labels
    ui->selectedDateLabel->setText(QString("Selected date: %1").arg(date.toString("yyyy-MM-dd")));

    int count = consultationCountMap.value(date, 0);
    ui->consultationCountLabel->setText(QString("Consultations: %1").arg(count));

    // Get consultations for this date
    QSqlQueryModel *model = Etmp.getConsultationsForDate(date);
    ui->dateConsultationsView->setModel(model);

    // Resize columns to contents
    ui->dateConsultationsView->resizeColumnsToContents();
}

// Event filter implementation for calendar hover tooltips
bool CalendarHoverItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == calendar) {
        if (event->type() == QEvent::HoverMove) {
            QHoverEvent *hoverEvent = static_cast<QHoverEvent*>(event);
            QPoint pos = hoverEvent->position().toPoint();

            // Access the underlying table view of the calendar widget
            QTableView* tableView = calendar->findChild<QTableView*>();
            QDate hoverDate;

            if (tableView) {
                QModelIndex index = tableView->indexAt(pos);
                if (index.isValid()) {
                    // Convert the model index to a date
                    int row = index.row();
                    int col = index.column();

                    // Calculate the date based on row and column
                    QDate firstDayOfMonth = calendar->selectedDate().addDays(-calendar->selectedDate().day() + 1);
                    int offset = firstDayOfMonth.dayOfWeek() - 1; // Adjust for first day of week

                    int day = (row * 7) + col - offset + 1;
                    if (day > 0 && day <= firstDayOfMonth.daysInMonth()) {
                        hoverDate = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), day);
                    } else {
                        // Fallback to selected date if calculation fails
                        hoverDate = calendar->selectedDate();
                    }
                }
            } else {
                // Fallback to last activated date if table view isn't accessible
                hoverDate = calendar->selectedDate();
            }

            if (hoverDate.isValid()) {
                // Show consultation count in tooltip
                int count = consultationCounts->value(hoverDate, 0);

                if (count > 0) {
                    QString tooltipText = QString("%1: %2 consultation(s)")
                    .arg(hoverDate.toString("yyyy-MM-dd"))
                        .arg(count);
                    QToolTip::showText(calendar->mapToGlobal(hoverEvent->position().toPoint()),
                                       tooltipText, calendar);
                } else {
                    QToolTip::hideText();
                }
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

void MainWindow::on_exportPdfButton_clicked()
{
    static QElapsedTimer lastClickTime;
    if (lastClickTime.isValid() && lastClickTime.elapsed() < 1000) {
        qDebug() << "Ignoring rapid click";
        return;
    }
    lastClickTime.restart();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) {
        ui->exportPdfButton->setEnabled(true);
        return;
    }

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    qDebug() << "Exporting to:" << fileName;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    QAbstractItemModel* model = ui->tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Export Error", "No data available to export.");
        return;
    }

    const int rows = model->rowCount();
    const int columns = model->columnCount();
    const int margin = 100;
    const int cellPadding = 50;
    const int headerHeight = 300;
    const int rowHeight = 200;

    // Removed unused totalHeight
    int pageWidth = pdfWriter.width() - 2 * margin;
    int columnWidth = pageWidth / columns;

    QFont headerFont("Arial", 12, QFont::Bold);
    QFont dataFont("Arial", 10);

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(QRectF(margin, margin, pageWidth, 100),
                     Qt::AlignCenter, "Client Management Report");

    int yPos = margin + 200;

    painter.setFont(headerFont);
    for (int col = 0; col < columns; ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        QRectF cellRect(margin + col * columnWidth, yPos, columnWidth, headerHeight);
        painter.drawText(cellRect.adjusted(cellPadding, cellPadding, -cellPadding, -cellPadding),
                         Qt::AlignCenter, header);
        painter.drawRect(cellRect);
    }
    yPos += headerHeight;

    painter.setFont(dataFont);
    for (int row = 0; row < rows; ++row) {
        if (yPos + rowHeight > pdfWriter.height() - margin) {
            pdfWriter.newPage();
            yPos = margin;
            painter.setFont(headerFont);
            for (int col = 0; col < columns; ++col) {
                QString header = model->headerData(col, Qt::Horizontal).toString();
                QRectF cellRect(margin + col * columnWidth, yPos, columnWidth, headerHeight);
                painter.drawText(cellRect.adjusted(cellPadding, cellPadding, -cellPadding, -cellPadding),
                                 Qt::AlignCenter, header);
                painter.drawRect(cellRect);
            }
            yPos += headerHeight;
        }

        for (int col = 0; col < columns; ++col) {
            QString text = model->data(model->index(row, col)).toString();
            if (col == 4) { // Date & Time column
                QDateTime dt = model->data(model->index(row, col)).toDateTime();
                text = dt.toString("yyyy-MM-dd HH:mm");
            }
            QRectF cellRect(margin + col * columnWidth, yPos, columnWidth, rowHeight);
            painter.drawText(cellRect.adjusted(cellPadding, cellPadding, -cellPadding, -cellPadding),
                             Qt::AlignCenter, text);
            painter.drawRect(cellRect);
        }
        yPos += rowHeight;
    }

    painter.setFont(QFont("Arial", 8));
    QString footer = QString("Generated on: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm"));
    painter.drawText(QRectF(margin, pdfWriter.height() - margin, pageWidth, 100),
                     Qt::AlignRight, footer);

    painter.end();

    QMessageBox::information(this, "Success", "PDF exported successfully!");
    ui->statusBar->showMessage("PDF exported to: " + fileName);
    ui->exportPdfButton->setEnabled(false);
}

void MainWindow::on_searchInput_textChanged()
{
    performSearch();
}

void MainWindow::on_searchCriteriaComboBox_currentIndexChanged()
{
    performSearch();
}

void MainWindow::performSearch()
{
    QString searchText = ui->searchInput->text().trimmed();
    int criteriaIndex = ui->searchCriteriaComboBox->currentIndex();
    QSqlQueryModel *model = nullptr;

    if (searchText.isEmpty()) {
        model = Etmp.afficher();
    } else {
        switch (criteriaIndex) {
        case 0: // Name
            model = Etmp.searchByName(searchText);
            break;
        case 1: // Sector
            model = Etmp.searchBySector(searchText);
            break;
        case 2: // Email
            model = Etmp.searchByEmail(searchText);
            break;
        case 3: // Contact
            model = Etmp.searchByContact(searchText);
            break;
        case 4: // Date & Time
            {
                QDateTime dateTime = QDateTime::fromString(searchText, "yyyy-MM-dd HH:mm");
                if (dateTime.isValid()) {
                    model = Etmp.searchByDateTime(dateTime);
                } else {
                    model = new QSqlQueryModel();
                }
            }
            break;
        case 5: // Consultant
            {
                bool ok;
                int consultantId = searchText.toInt(&ok);
                if (ok) {
                    model = Etmp.searchByConsultant(consultantId);
                } else {
                    model = new QSqlQueryModel();
                }
            }
            break;
        default:
            model = Etmp.afficher();
            break;
        }
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    // Ensure all columns are visible
    for (int i = 0; i < model->columnCount(); ++i) {
        ui->tableView->showColumn(i);
    }
    ui->statusBar->showMessage(QString("Found %1 client(s)").arg(model->rowCount()));
}
