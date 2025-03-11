#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "meeting.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isDarkTheme(false)
{
    ui->setupUi(this);
    applyLightTheme();
    // Set up the table headers and properties first
   // ui->tableWidget->setColumnCount(10); // Update to 7 columns (removed Status)
    //ui->tableWidget->setHorizontalHeaderLabels({"ID", "Title", "Organiser", "Participants", "Agenda", "Duration"});
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
 // ui->tableWidget->setSortingEnabled(true); // Enable sorting (tlwj bel bar)

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
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::handleStatisticsButtonClick); // Connect statisticsButton signal
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleAddButtonClick() {
    // Get values from the UI
    QString title = ui->title->text();
    QString organiser = ui->organiser->text();
    QString participant = ui->participant->text();
    QString agenda = ui->agenda->currentText();

    bool conversionOk = false;
    int duration = ui->duration->text().toInt(&conversionOk);

    // Validate inputs with detailed feedback
    QStringList validationErrors;

    if (title.isEmpty()) {
        validationErrors << "Title cannot be empty";
    }

    if (organiser.isEmpty()) {
        validationErrors << "Organiser cannot be empty";
    }

    if (!conversionOk || duration <= 0) {
        validationErrors << "Duration must be a valid positive number";
    }

    if (!validationErrors.isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Please correct the following errors:\n• " + validationErrors.join("\n• "));
        return;
    }

    // Add debugging output
    qDebug() << "Adding meeting with values:"
             << "\nTitle:" << title << (title.isEmpty() ? " (EMPTY)" : "")
             << "\nOrganiser:" << organiser << (organiser.isEmpty() ? " (EMPTY)" : "")
             << "\nParticipant:" << participant << (participant.isEmpty() ? " (EMPTY)" : "")
             << "\nAgenda:" << agenda << (agenda.isEmpty() ? " (EMPTY)" : "")
             << "\nDuration:" << duration;

    // Create and add the meeting
    meeting m(title, organiser, participant, agenda, duration);

    if (m.add()) {
        QMessageBox::information(this, "Success", "Meeting added successfully.");

        // Clear the form fields after successful addition
        ui->title->clear();
        ui->organiser->clear();
        ui->participant->clear();
        ui->duration->clear();
        ui->agenda->setCurrentIndex(0);

        // Refresh the table to show the new meeting
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add meeting. Check the database connection.");
    }
}



void MainWindow::handleDeleteButtonClick() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a meeting to delete.");
        return;
    }

    // Get the ID of the selected meeting
    int id = ui->tableWidget->item(row, 0)->text().toInt();

    // Delete the meeting from the database
    meeting m;
    if (m.delet(id)) {
        QMessageBox::information(this, "Success", "Meeting deleted successfully.");

        // Refresh the table to remove the deleted meeting
        refreshTableWidget();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete meeting. Check the database connection.");
    }
}



void MainWindow::handleUpdateButtonClick() {
    // Vérifier si une ligne est sélectionnée
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une réunion à mettre à jour.");
        return;
    }

    // Récupérer les données de la ligne sélectionnée
    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    QTableWidgetItem *titleItem = ui->tableWidget->item(row, 1);
    QTableWidgetItem *organiserItem = ui->tableWidget->item(row, 2);
    QTableWidgetItem *participantItem = ui->tableWidget->item(row, 3);
    QTableWidgetItem *agendaItem = ui->tableWidget->item(row, 4);
    QTableWidgetItem *durationItem = ui->tableWidget->item(row, 5);

    // Vérifier que tous les éléments existent pour éviter les crashs
    if (!idItem || !titleItem || !organiserItem || !participantItem || !agendaItem || !durationItem) {
        QMessageBox::critical(this, "Erreur", "Les données de la réunion sélectionnée sont incomplètes.");
        return;
    }

    // Convertir l'ID en entier
    bool idOk;
    int id = idItem->text().toInt(&idOk);
    if (!idOk) {
        QMessageBox::critical(this, "Erreur", "L'ID de la réunion est invalide.");
        return;
    }

    // Convertir la durée en entier, en supprimant " min" si présent
    QString durationText = durationItem->text().replace(" min", "");
    bool durationOk;
    int duration = durationText.toInt(&durationOk);
    if (!durationOk || duration <= 0) {
        QMessageBox::warning(this, "Erreur", "La durée doit être un nombre positif valide.");
        return;
    }

    // Créer un objet meeting avec les données actuelles
    meeting m;
    m.setId(id);
    m.setTitle(titleItem->text());
    m.setOrganiser(organiserItem->text());
    m.setParticipant(participantItem->text());
    m.setAgenda(agendaItem->text());
    m.setDuration(duration);

    // Ouvrir le dialogue de mise à jour
    UpdateMeeting dialog(this, &m);
    if (dialog.exec() == QDialog::Accepted) {
        // Rafraîchir le tableau si la mise à jour est réussie
        refreshTableWidget();
    }
}
void MainWindow::updateInputFields() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        return;
    }

    // Update the input fields with the values from the selected row
    ui->title->setText(ui->tableWidget->item(row, 1)->text());
    ui->organiser->setText(ui->tableWidget->item(row, 2)->text());
    ui->participant->setText(ui->tableWidget->item(row, 3)->text());
    ui->agenda->setCurrentText(ui->tableWidget->item(row, 4)->text());
    ui->duration->setText(ui->tableWidget->item(row, 5)->text().replace(" min", ""));
}

void MainWindow::handleSortCriteriaChanged(int index) {
    // Determine the column to sort by based on the selected index
    int columnToSort = -1;
    switch (index) {
    case 0: // Title
        columnToSort = 1;
        break;
    case 1: // Duration
        columnToSort = 5;
        break;
    default:
        return; // Invalid index, do nothing
    }

    // Sort the tableWidget by the selected column
    ui->tableWidget->sortItems(columnToSort, Qt::AscendingOrder);
}

void MainWindow::handleStatisticsButtonClick() {
    meeting m;
    QSqlQueryModel* model = m.afficher();

    int totalMeetings = model->rowCount();
    int totalDuration = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        totalDuration += model->data(model->index(row, 5)).toInt(); // Assuming duration is in column 5
    }

    double averageDuration = totalMeetings > 0 ? static_cast<double>(totalDuration) / totalMeetings : 0;

    QString statisticsMessage = QString("Total Meetings: %1\nTotal Duration: %2 minutes\nAverage Duration: %3 minutes")
                                    .arg(totalMeetings)
                                    .arg(totalDuration)
                                    .arg(averageDuration, 0, 'f', 2);

    QMessageBox::information(this, "Meeting Statistics", statisticsMessage);

    delete model; // Clean up the model
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

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) { // Duration column
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model; // Clean up the model
}

void MainWindow::handleSearchTextChanged(const QString &searchText) {
    if (searchText.isEmpty()) {
        refreshTableWidget();
        return;
    }

    meeting m;
    QSqlQueryModel* model = m.searchByTitle(searchText);

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) { // Duration column
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model; // Clean up the model
}

void MainWindow::refreshTableWidget() {
    meeting m;
    QSqlQueryModel* model = m.afficher();

    // Debug output
    qDebug() << "Refreshing table widget. Model has" << model->rowCount() << "rows";

    // If there are no rows, try to debug the SQL issue
    if (model->rowCount() == 0) {
        qDebug() << "No data in model. Last error:" << model->lastError().text();
    }

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows

    // Copy data from model to tableWidget
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 5) { // Duration column
                data += " min";
            }
            QTableWidgetItem* item = new QTableWidgetItem(data);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model; // Clean up the model
}
void MainWindow::applyLightTheme() {
    // Blueish white gradient
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #FFFFFF, stop:1 #A1B8E6);
            color: #333333;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        /* Buttons with rounded corners and subtle shadow */
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
        /* General Label Styling */
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        /* Form Labels */
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #3A5DAE;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        /* Main Title */
        #label {
            font-size: 18pt;
            font-weight: bold;
            color: #3A5DAE;
            qproperty-alignment: AlignCenter;
        }
        /* Frames */
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
        }
        QPushButton:hover {
            background-color: #F5A38A;
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
        /* General Label Styling */
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        /* Form Labels */
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #F28C6F;
            text-decoration: underline;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        /* Main Title */
        #label {
            font-size: 18pt;
            font-weight: bold;
            color: #F28C6F;
            text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);
            qproperty-alignment: AlignCenter;
        }
        /* Frames */
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
    // Check if the sidebar is currently visible
    bool isVisible = ui->sideMenu->isVisible();

    // Toggle visibility
    ui->sideMenu->setVisible(!isVisible);
}
void MainWindow::toggleTheme() {
    if (isDarkTheme) {
        applyLightTheme();
        isDarkTheme = false;
        ui->themeButton->setText("Switch to Dark Theme"); // Optional: Update button text
    } else {
        applyDarkTheme();
        isDarkTheme = true;
        ui->themeButton->setText("Switch to Light Theme"); // Optional: Update button text
    }
}
