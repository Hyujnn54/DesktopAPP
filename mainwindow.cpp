#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
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
#include <QInputDialog>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QTime>
#include "emailsender.h"
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isDarkTheme(false)
    , emailSender(new EmailSender(this))
    , emailAttempts(0)
    , emailSuccesses(0)
{
    qDebug() << "Starting MainWindow constructor";
    ui->setupUi(this);
    applyLightTheme();

    // DateTimeEdit setup (unchanged)
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

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Connect buttons (remove refreshChartButton connection)
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->add, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    connect(ui->Delet, &QPushButton::clicked, this, &MainWindow::on_removeButtonClicked);
    connect(ui->update, &QPushButton::clicked, this, &MainWindow::on_updateButtonClicked);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButton_clicked);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::tableViewHeaderClicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::sendConsultationReminders);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(ui->refreshStatsButton, &QPushButton::clicked, this, &MainWindow::on_refreshStatsButton_clicked);

    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::on_searchInput_textChanged);
    connect(ui->searchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_searchCriteriaComboBox_currentIndexChanged);

    setupCalendarView();
    QSqlQueryModel *model = Etmp.afficher();
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    for (int i = 0; i < model->columnCount(); ++i) {
        ui->tableView->showColumn(i);
    }

    checkAndSendReminders();
    ui->consultationCalendar->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButtonclicked()
{
    if (!validateInputs()) {
        return;
    }

    QString Name = ui->Name->text().trimmed();
    QString Sector = ui->Sector->text().trimmed();
    QString Contact = ui->Contact_info->text().trimmed();
    QString Email = ui->Email->text().trimmed();
    QDateTimeEdit *dateTimeEdit = findChild<QDateTimeEdit*>("consultation_datetime");
    QDateTime Cdate = dateTimeEdit ? dateTimeEdit->dateTime() : QDateTime::currentDateTime();
    int Consultant = ui->Consultant->text().toInt();

    Client c(Name, Sector, Contact, Email, Cdate, Consultant);

    if (c.ajouter()) {
        QMessageBox::information(this, "Success", "Client added successfully!");
        QSqlQueryModel *model = Etmp.afficher();
        ui->tableView->setModel(model);
        ui->Name->clear();
        ui->Sector->clear();
        ui->Contact_info->clear();
        ui->Email->clear();
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

void MainWindow::applyLightTheme() {
    // Blueish white gradient (unchanged)
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #FFFFFF, stop:1 #A1B8E6);
            color: #333333;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        /* Buttons with rounded corners and subtle shadow (remove box-shadow if not supported) */
        QPushButton {
            background-color: #3A5DAE;
            color: white;
            border: 1px solid #2A4682;
            border-radius: 5px;
            padding: 6px;
            font-weight: bold;
            /* box-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1); */
        }
        QPushButton:hover {
            background-color: #4A70C2;
            /* box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.2); */
        }
        QPushButton:pressed {
            background-color: #2A4682;
        }
        /* Input fields with smooth borders */
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
        /* Table view styling */
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
        /* Calendar styling */
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
        /* Tooltip */
        QToolTip {
            color: #333333;
            background-color: #E6ECF5;
            border: 1px solid #3A5DAE;
            border-radius: 3px;
        }
        /* Tab widget */
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
        /* General Label Styling (all labels) */
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        /* Form Labels (e.g., Name:, Sector:) - Using custom property or specific names */
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #3A5DAE;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        /* Main Title ("Client Management System") */
        #label { /* Specific to ui->label in frame_5 */
            font-size: 18pt;
            font-weight: bold;
            color: #3A5DAE;
            /* text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2); */
            qproperty-alignment: AlignCenter;
        }
        /* Frames for distinguishing areas */
        QFrame#header {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            /* box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.1); */
        }
        QFrame#sideMenu {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            /* box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.1); */
            background-color: #E6ECF5; /* Slightly darker blueish background for contrast */
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #D3DCE6;
            border-radius: 5px;
            /* box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.1); */
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::applyDarkTheme() {
    // Lighter orange to soft dark gray gradient
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #F28C6F, stop:1 #5C5C5C);
            color: #F0F0F0;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        /* Buttons with rounded corners and subtle shadow */
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
        /* Input fields with smooth borders */
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
        /* Table view styling */
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
        /* Calendar styling */
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
        /* Tooltip */
        QToolTip {
            color: #F0F0F0;
            background-color: #F28C6F;
            border: 1px solid #D96C53;
            border-radius: 3px;
        }
        /* Tab widget */
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
        /* General Label Styling (all labels) */
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        /* Form Labels (e.g., Name:, Sector:) - Using custom property or specific names */
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #F28C6F;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        /* Main Title ("Client Management System") */
        #label { /* Specific to ui->label in frame_5 */
            font-size: 18pt;
            font-weight: bold;
            color: #F28C6F;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);
            qproperty-alignment: AlignCenter;
        }
        /* Frames for distinguishing areas */
        QFrame#header {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
        }
        QFrame#sideMenu {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            box-shadow: 2px 2px 6px rgba(0, 0, 0, 0.3);
            background-color: #7A7A7A; /* Slightly lighter gray for contrast */
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #4A4A4A;
            border-radius: 5px;
            box-shadow: 1px 1px 4px rgba(0, 0, 0, 0.3);
        }
    )";
    qApp->setStyleSheet(styleSheet);
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

void MainWindow::setupCalendarView() {
    // Connect calendar signals
    connect(ui->consultationCalendar, &QCalendarWidget::selectionChanged,
            this, &MainWindow::on_consultationCalendar_selectionChanged);
    connect(ui->consultationCalendar, &QCalendarWidget::activated,
            this, &MainWindow::on_consultationCalendar_activated);

    // Install event filter for hover tooltips directly on MainWindow
    ui->consultationCalendar->installEventFilter(this);

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

// Implement hover event filter directly in MainWindow
bool MainWindow::calendarHoverEventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->consultationCalendar && event->type() == QEvent::HoverMove) {
        QHoverEvent* hoverEvent = static_cast<QHoverEvent*>(event);
        QPoint pos = hoverEvent->position().toPoint();

        QTableView* tableView = ui->consultationCalendar->findChild<QTableView*>();
        QDate hoverDate;

        if (tableView) {
            QModelIndex index = tableView->indexAt(pos);
            if (index.isValid()) {
                int row = index.row();
                int col = index.column();

                QDate firstDayOfMonth = ui->consultationCalendar->selectedDate().addDays(-ui->consultationCalendar->selectedDate().day() + 1);
                int offset = firstDayOfMonth.dayOfWeek() - 1; // Adjust for first day of week

                int day = (row * 7) + col - offset + 1;
                if (day > 0 && day <= firstDayOfMonth.daysInMonth()) {
                    hoverDate = QDate(firstDayOfMonth.year(), firstDayOfMonth.month(), day);
                } else {
                    hoverDate = ui->consultationCalendar->selectedDate();
                }
            }
        } else {
            hoverDate = ui->consultationCalendar->selectedDate();
        }

        if (hoverDate.isValid()) {
            int count = consultationCountMap.value(hoverDate, 0);
            if (count > 0) {
                QString tooltipText = QString("%1: %2 consultation(s)")
                .arg(hoverDate.toString("yyyy-MM-dd"))
                    .arg(count);
                QToolTip::showText(ui->consultationCalendar->mapToGlobal(hoverEvent->position().toPoint()), tooltipText, ui->consultationCalendar);
            } else {
                QToolTip::hideText();
            }
        }
    }
    return false; // Return false to allow further event processing
}

// Override eventFilter to call our custom hover function
bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (calendarHoverEventFilter(watched, event)) {
        return true; // Event handled, stop propagation
    }
    return QMainWindow::eventFilter(watched, event); // Pass to base class for unhandled events
}

void MainWindow::on_exportPdfButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty())
        return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20)); // 20mm margins
    pdfWriter.setResolution(300); // Higher resolution for clarity

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // Scaling factor to convert from PDF points to painter coordinates
    const qreal scaleFactor = pdfWriter.logicalDpiX() / 72.0; // Assuming 72 points per inch as base
    const qreal mmToPoints = 2.83465; // 1 mm = 2.83465 points at 72 DPI
    const qreal margin = 20 * mmToPoints * scaleFactor; // 20mm margin in scaled points
    const qreal pageWidth = pdfWriter.width() - 2 * margin;
    const qreal pageHeight = pdfWriter.height() - 2 * margin;

    // Fonts
    QFont titleFont("Arial", 14, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont bodyFont("Arial", 9);
    QFont summaryFont("Arial", 11, QFont::Bold);

    // Title
    painter.setFont(titleFont);
    painter.setPen(Qt::darkBlue);
    QString title = "Client Management System Report";
    QString date = QDate::currentDate().toString("MMMM d, yyyy"); // e.g., "March 11, 2025"
    QString headerText = QString("%1\nGenerated on: %2").arg(title, date);
    painter.drawText(QRectF(margin, margin, pageWidth, 50 * scaleFactor), Qt::AlignCenter, headerText);

    // Move to table area
    qreal yPos = margin + 60 * scaleFactor;

    // Table Headers
    QStringList headers = {"Name", "Sector", "Contact", "Email", "Date & Time", "Consultant"};
    qreal columnWidths[] = {1.5, 1.2, 1.2, 2.0, 1.5, 0.6}; // Proportional widths
    qreal totalWidthUnits = 0;
    for (qreal width : columnWidths) totalWidthUnits += width;
    qreal tableWidth = pageWidth;
    qreal rowHeight = 30 * scaleFactor; // Increased for readability

    // Calculate actual column widths
    QVector<qreal> actualWidths(headers.size());
    qreal accumulatedWidth = margin;
    for (int i = 0; i < headers.size(); ++i) {
        actualWidths[i] = (columnWidths[i] / totalWidthUnits) * tableWidth;
    }

    // Draw table headers
    painter.setFont(headerFont);
    painter.setBrush(QBrush(QColor(200, 220, 255))); // Light blue background for headers
    painter.setPen(Qt::black);
    painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                         Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
        accumulatedWidth += actualWidths[i];
    }
    yPos += rowHeight;

    // Draw table data and track page number
    QSqlQueryModel *model = Etmp.afficher();
    int rowCount = model->rowCount();
    int colCount = model->columnCount();
    int pageNumber = 1;

    painter.setFont(bodyFont);
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < rowCount; ++row) {
        if (yPos + rowHeight > pageHeight - margin) { // Page break
            // Draw page number
            painter.setFont(bodyFont);
            painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                             Qt::AlignRight, QString("Page %1").arg(pageNumber++));

            pdfWriter.newPage();
            yPos = margin;

            // Redraw headers on new page
            painter.setFont(headerFont);
            painter.setBrush(QBrush(QColor(200, 220, 255)));
            accumulatedWidth = margin;
            painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
            for (int i = 0; i < headers.size(); ++i) {
                painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                                 Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
                accumulatedWidth += actualWidths[i];
            }
            yPos += rowHeight;
        }

        accumulatedWidth = margin;
        painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
        for (int col = 0; col < colCount; ++col) {
            QString text = model->data(model->index(row, col)).toString();
            QRectF textRect(accumulatedWidth + 2 * scaleFactor, yPos, actualWidths[col] - 4 * scaleFactor, rowHeight);
            painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
            accumulatedWidth += actualWidths[col];
        }
        yPos += rowHeight;
    }

    // Draw page number on the last page
    painter.setFont(bodyFont);
    painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                     Qt::AlignRight, QString("Page %1").arg(pageNumber));

    // Summary Section
    yPos += 20 * scaleFactor; // Extra spacing
    if (yPos + 60 * scaleFactor < pageHeight - margin) {
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QDateTime start = QDateTime::currentDateTime().addDays(-30);
        QDateTime end = QDateTime::currentDateTime();
        int totalConsultations = Etmp.getTotalConsultations(start, end);
        int uniqueClients = Etmp.getUniqueClients(start, end);
        QString summary = QString("Summary (Last 30 Days):\n"
                                  "Total Consultations: %1\n"
                                  "Unique Clients: %2")
                              .arg(totalConsultations)
                              .arg(uniqueClients);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
    } else {
        pdfWriter.newPage();
        yPos = margin;
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QDateTime start = QDateTime::currentDateTime().addDays(-30);
        QDateTime end = QDateTime::currentDateTime();
        int totalConsultations = Etmp.getTotalConsultations(start, end);
        int uniqueClients = Etmp.getUniqueClients(start, end);
        QString summary = QString("Summary (Last 30 Days):\n"
                                  "Total Consultations: %1\n"
                                  "Unique Clients: %2")
                              .arg(totalConsultations)
                              .arg(uniqueClients);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
        // Page number for summary page
        painter.setFont(bodyFont);
        painter.setPen(Qt::black);
        painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                         Qt::AlignRight, QString("Page %1").arg(pageNumber + 1));
    }

    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to " + fileName);
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

void MainWindow::sendConsultationReminders()
{
    checkAndSendReminders();
}

void MainWindow::showStatistics()
{
    ui->tabWidget->setCurrentIndex(3); // Switch to Statistics tab (index 3)
    updateStatisticsDisplay();
}

void MainWindow::on_refreshStatsButton_clicked()
{
    static QElapsedTimer lastRefresh;
    if (!lastRefresh.isValid() || lastRefresh.elapsed() > 1000) { // Throttle to 1 second
        lastRefresh.start();
        updateStatisticsDisplay();
    } else {
        qDebug() << "Throttling refresh, too frequent";
    }
}

void MainWindow::updateStatisticsDisplay() {
    QDate startDate = QDate::currentDate().addDays(-30);
    QDate endDate = QDate::currentDate();
    QDateTime start = QDateTime(startDate, QTime(0, 0, 0));
    QDateTime end = QDateTime(endDate, QTime(23, 59, 59));

    qDebug() << "Updating statistics for range:" << start.toString() << "to" << end.toString();

    // Declare and initialize variables
    int total = Etmp.getTotalConsultations(start, end);
    int upcoming = Etmp.getUpcomingConsultationsCount(start, end.addDays(1));
    int uniqueClients = Etmp.getUniqueClients(start, end);
    double successRate = (emailAttempts > 0) ? (double)emailSuccesses / emailAttempts * 100 : 0;

    QMap<QDate, int> consultationsPerDay = Etmp.getConsultationsPerDay(start, end);
    qDebug() << "Consultations per day size:" << consultationsPerDay.size();

    // Format statistics text with HTML for the text edit
    QString dayStats;
    for (QMap<QDate, int>::const_iterator it = consultationsPerDay.constBegin(); it != consultationsPerDay.constEnd(); ++it) {
        dayStats += QString("%1: %2 consultations<br>").arg(it.key().toString("yyyy-MM-dd"), QString::number(it.value()));
    }

    QString statsText = QString(
                            "<h3>Statistics (as of %1):</h3>"
                            "<p><b>Total Consultations:</b> %2</p>"
                            "<p><b>Upcoming Consultations (Next 24h):</b> %3</p>"
                            "<p><b>Unique Clients:</b> %4</p>"
                            "<p><b>Email Reminder Success Rate:</b> %5%</p>"
                            "<p><b>Consultations Per Day:</b><br>%6</p>"
                            ).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                            .arg(total)
                            .arg(upcoming)
                            .arg(uniqueClients)
                            .arg(successRate, 0, 'f', 2)
                            .arg(dayStats);

    if (ui->statsDisplay) {
        ui->statsDisplay->setHtml(statsText);
    } else {
        qDebug() << "statsDisplay is nullptr";
        QMessageBox::warning(this, "Error", "Statistics display text edit is not initialized.");
    }
}

void MainWindow::checkAndSendReminders()
{
    qDebug() << "Starting checkAndSendReminders()";

    QDateTime now = QDateTime::currentDateTime();
    QDateTime reminderEnd = now.addSecs(24 * 60 * 60);
    qDebug() << "Reminder range:" << now.toString() << "to" << reminderEnd.toString();

    QSqlQueryModel *model = Etmp.getUpcomingConsultations(now, reminderEnd);
    if (!model) {
        qDebug() << "Model is null in checkAndSendReminders()";
        return;
    }
    int rowCount = model->rowCount();
    qDebug() << "Upcoming consultations count:" << rowCount;

    for (int row = 0; row < rowCount; ++row) {
        QString name = model->data(model->index(row, 0)).toString();
        QString email = model->data(model->index(row, 3)).toString();
        QDateTime consultationDate = model->data(model->index(row, 4)).toDateTime();
        qDebug() << "Processing reminder for:" << name << "Email:" << email << "Date:" << consultationDate.toString();

        if (!email.isEmpty()) {
            QString subject = "Consultation Reminder";
            QString body = QString("Dear %1,\n\nThis is a reminder for your consultation scheduled on %2.\n\nBest regards,\nClient Management System")
                               .arg(name)
                               .arg(consultationDate.toString("yyyy-MM-dd HH:mm"));

            emailAttempts++;
            if (emailSender->sendEmail(email, subject, body)) {
                emailSuccesses++;
                qDebug() << "Reminder sent to:" << email;
                ui->statusBar->showMessage("Reminder sent to " + email);
            } else {
                qDebug() << "Failed to send reminder to:" << email;
                ui->statusBar->showMessage("Failed to send reminder to " + email);
            }
        }
    }

    if (rowCount == 0) {
        ui->statusBar->showMessage("No upcoming consultations found.");
    }

    qDebug() << "checkAndSendReminders() completed";
}
void MainWindow::on_openChartButton_clicked()
{
    ChartWindow *chartWindow = new ChartWindow(this);
    chartWindow->show();
}
