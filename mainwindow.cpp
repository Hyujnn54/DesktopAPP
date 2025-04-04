#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include <QStandardItemModel> // For setting the model
#include <QTableView>
#include "updatetrainingdialog.h"
#include <QRegularExpression>
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), isDarkTheme(false),
    tableModel(nullptr), proxyModel(new QSortFilterProxyModel(this)),
    notificationCount(0)
{
    ui->setupUi(this);
    applyLightTheme();

    ui->date->setDate(QDate::currentDate());

    // Initialize notification label (assuming you add it to UI in Qt Designer)
    ui->notificationLabel->setText("Notifications: 0");
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;"); // Add to status bar

    // Existing connections...
    QObject::connect(ui->add, SIGNAL(clicked()), this, SLOT(on_addButtonclicked()));
    QObject::connect(ui->deletef, SIGNAL(clicked()), this, SLOT(on_deleteButtonClicked()));
    QObject::connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(on_updateButtonClicked()));
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportToPdf);

    QRegularExpression formationRegex("^[A-Za-z]+$");
    QRegularExpressionValidator *formationValidator = new QRegularExpressionValidator(formationRegex, this);
    ui->format->setValidator(formationValidator);

    QRegularExpression descriptionRegex("^[A-Za-z]+$");
    QRegularExpressionValidator *descriptionValidator = new QRegularExpressionValidator(descriptionRegex, this);
    ui->des->setValidator(descriptionValidator);

    QRegularExpression trainerRegex("^[A-Za-z]+$");
    QRegularExpressionValidator *trainerValidator = new QRegularExpressionValidator(trainerRegex, this);
    ui->tr->setValidator(trainerValidator);

    ui->timeb->setRange(1, 30);
    ui->prixb->setRange(1, 1000);

    ui->tabtr->setSortingEnabled(true);


    ui->notificationLabel->setText("Notifications: 0");
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");
    ui->notificationLabel->setTextInteractionFlags(Qt::TextBrowserInteraction); // Enable clicking
    ui->notificationLabel->setCursor(Qt::PointingHandCursor); // Hand cursor
    connect(ui->notificationLabel, &QLabel::linkActivated, this, &MainWindow::onNotificationLabelClicked);


    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::on_searchInput_textChanged);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButton_clicked);

    refreshTableView();
}

MainWindow::~MainWindow()
{
    delete tableModel;
    delete ui;
}
void MainWindow::onNotificationLabelClicked()
{
    if (notificationCount == 0) {
        QMessageBox::information(this, "Notifications", "No recent changes.");
    } else {
        QString message = "Recent Changes:\n" + changeHistory.join("\n");
        QMessageBox::information(this, "Notifications", message);
    }
    resetNotificationCount(); // Optional: Reset after viewing
}
void MainWindow::updateNotificationCount(int change, const QString &changeDescription)
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
void MainWindow::resetNotificationCount()
{
    notificationCount = 0;
    changeHistory.clear(); // Clear history when resetting
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");
    ui->notificationLabel->setText("<img src=':notification_icon.png' width='16' height='16'> <a href='#'>Notifications: 0</a>");
    qDebug() << "Notification count reset";
}
void MainWindow::exportToPdf()
{
    refreshTableView();

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString defaultFileName = QString("Formations_%1.pdf").arg(timestamp);

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), defaultFileName, tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty())
    {
        return;
    }

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
    {
        filePath += ".pdf";
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300); // High resolution for clarity

    QPainter painter(&pdfWriter);
    painter.setFont(QFont("Arial", 10));

    // Scale factor for 300 DPI (default is 72 DPI)
    const qreal scaleFactor = 300.0 / 72.0;
    painter.scale(scaleFactor, scaleFactor);

    // A4 dimensions in points at 72 DPI (595 x 842)
    int pageWidth = 595;  // Width in points
    int pageHeight = 842; // Height in points
    int margin = 40;      // Margin in points
    int lineHeight = 15;  // Row height in points
    // Column widths in points, adjusted to fit within pageWidth - 2*margin (595 - 80 = 515)
    QVector<int> columnWidths = {50, 100, 150, 80, 80, 50, 60}; // ID, Formation, Description, Trainer, Date, Time, Price
    int totalWidth = 0;
    for (int w : columnWidths)
        totalWidth += w;
    qDebug() << "Total table width:" << totalWidth << "Available width:" << (pageWidth - 2 * margin);

    // Title
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    int yPos = margin;
    painter.drawText(margin, yPos, "Formations List - Exported on " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    yPos += lineHeight * 2;

    qDebug() << "Export - Row count:" << (tableModel ? tableModel->rowCount() : 0) << "Columns:" << (tableModel ? tableModel->columnCount() : 0);

    if (!tableModel || tableModel->rowCount() == 0 || tableModel->columnCount() < 7)
    {
        painter.setFont(QFont("Arial", 10));
        painter.drawText(margin, yPos, "No data available or incomplete data to export.");
        painter.end();
        QMessageBox::information(this, "Export", "PDF exported with no/incomplete data at: " + filePath);
        return;
    }

    // Draw table headers
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    QStringList headers = {"ID", "Formation", "Description", "Trainer", "Date", "Time", "Price"};
    int xPos = margin;
    int headerYPos = yPos;
    for (int i = 0; i < headers.size(); ++i)
    {
        painter.drawText(xPos, yPos, headers[i]);
        xPos += columnWidths[i];
    }
    yPos += lineHeight;

    // Draw header underline
    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(margin, yPos, margin + totalWidth, yPos);
    yPos += 2;

    // Draw table data with grid lines
    painter.setFont(QFont("Arial", 10));
    for (int row = 0; row < tableModel->rowCount(); ++row)
    {
        xPos = margin;
        int rowYPos = yPos;
        for (int col = 0; col < 7; ++col)
        {
            QString data = tableModel->index(row, col).data().toString();
            painter.drawText(xPos, yPos, data);
            qDebug() << "Row" << row << "Col" << col << ":" << data;
            xPos += columnWidths[col];
        }
        yPos += lineHeight;

        // Draw horizontal line for each row
        painter.drawLine(margin, yPos, margin + totalWidth, yPos);

        // Draw vertical lines for columns
        xPos = margin;
        for (int i = 0; i <= 7; ++i)
        {
            painter.drawLine(xPos, rowYPos, xPos, yPos);
            if (i < 7)
                xPos += columnWidths[i];
        }

        // Check for new page
        if (yPos > (pageHeight - margin))
        {
            pdfWriter.newPage();
            yPos = margin;
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            xPos = margin;
            headerYPos = yPos;
            for (int i = 0; i < headers.size(); ++i)
            {
                painter.drawText(xPos, yPos, headers[i]);
                xPos += columnWidths[i];
            }
            yPos += lineHeight;
            painter.drawLine(margin, yPos, margin + totalWidth, yPos);
            yPos += 2;
            painter.setFont(QFont("Arial", 10));
        }
    }

    painter.end();
    QMessageBox::information(this, "Export", "Formations data exported to PDF successfully at: " + filePath);
}
void MainWindow::on_addButtonclicked()
{
    // Get input values
    QString formation = ui->format->text().trimmed();
    QString description = ui->des->text().trimmed();
    QString trainer = ui->tr->text().trimmed();
    QDate datef = ui->date->date();
    int time = ui->timeb->value();
    double prix = ui->prixb->value();
    // Check each field one by one
    if (formation.isEmpty())
    {
        QMessageBox::warning(this, "Missing Data", "Please enter a Formation.");
        return;
    }
    if (formation.length() < 4)
    {
        QMessageBox::warning(this, "Invalid Data", "Formation must be at least 4 letters long.");
        return;
    }
    if (description.isEmpty())
    {
        QMessageBox::warning(this, "Missing Data", "Please enter a Description.");
        return;
    }
    if (trainer.isEmpty())
    {
        QMessageBox::warning(this, "Missing Data", "Please enter a Trainer.");
        return;
    }
    if (trainer.length() < 4)
    {
        QMessageBox::warning(this, "Invalid Data", "Trainer must be at least 4 letters long.");
        return;
    }
    if (!datef.isValid())
    {
        QMessageBox::warning(this, "Missing Data", "Please select a valid Date.");
        return;
    }
    if (time == 0)
    {
        QMessageBox::warning(this, "Missing Data", "Please enter a valid Time (greater than 0).");
        return;
    }
    if (prix == 0.0)
    {
        QMessageBox::warning(this, "Missing Data", "Please enter a valid Price (greater than 0).");
        return;
    }

    // If all fields are filled and valid, proceed with adding the formation
    formations f(0, formation, description, trainer, datef, time, prix);

    if (f.ajoutforma())
    {
        QMessageBox::information(this, "Success", "Formation added successfully!");
        updateNotificationCount(1, QString("Added formation: %1").arg(formation));
        refreshTableView();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to add formation.");
    }
}

void MainWindow::refreshTableView()
{
    formations f;
    if (tableModel) {
        delete tableModel;
        tableModel = nullptr;
    }
    tableModel = f.afficher();
    if (!tableModel) {
        qDebug() << "Error: Failed to retrieve table model";
        QMessageBox::warning(this, "Error", "Failed to load formation data.");
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
#include <QInputDialog>

void MainWindow::on_deleteButtonClicked()
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

    if (formations::deleteFormation(idfor))
    {
        QMessageBox::information(this, "Succès", "Formation supprimée avec succès !");
        updateNotificationCount(1, QString("Deleted formation ID: %1").arg(idfor));
        refreshTableView();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de la formation.");
    }
}

void MainWindow::on_updateButtonClicked()
{
    QItemSelectionModel *selectionModel = ui->tabtr->selectionModel();
    if (!selectionModel->hasSelection())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une formation à modifier !");
        return;
    }

    QModelIndexList selectedRows = selectionModel->selectedRows();
    int row = selectedRows.at(0).row();
    int idfor = ui->tabtr->model()->index(row, 0).data().toInt();

    formations f;
    if (!f.exists(idfor))
    {
        QMessageBox::warning(this, "Erreur", "Cette formation n'existe pas !");
        return;
    }

    QString currentFormation = ui->tabtr->model()->index(row, 1).data().toString();
    QString currentDescription = ui->tabtr->model()->index(row, 2).data().toString();
    QString currentTrainer = ui->tabtr->model()->index(row, 3).data().toString();
    QDate currentDate = ui->tabtr->model()->index(row, 4).data().toDate();
    int currentTime = ui->tabtr->model()->index(row, 5).data().toInt();
    double currentPrix = ui->tabtr->model()->index(row, 6).data().toDouble(); // Change to toDouble()

    formations currentData(idfor, currentFormation, currentDescription, currentTrainer,
                           currentDate, currentTime, currentPrix);

    UpdateTrainingDialog dialog(idfor, currentData, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString newFormation = dialog.getFormation();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();

        if (f.updateFormation(idfor, newFormation, newDescription, newTrainer, newDate, newTime, newPrix))
        {
            QMessageBox::information(this, "Succès", "Formation mise à jour avec succès !");
            updateNotificationCount(1, QString("Updated formation ID: %1 to %2").arg(idfor).arg(newFormation));
            refreshTableView();
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la formation.");
        }
    }
}


void MainWindow::on_searchInput_textChanged(const QString &text)
{
    if (!tableModel || !proxyModel)
        return;

    QString column = ui->searchCriteriaComboBox->currentText();
    if (column.isEmpty())
        return;

    int columnIndex = -1;
    if (column == "formation")
    {
        columnIndex = 1;
    }
    else if (column == "trainer")
    {
        columnIndex = 3;
    }
    else if (column == "prix")
    {
        columnIndex = 6;
    }
    else if (column == "date")
    {
        columnIndex = 4;
    }
    else
    {
        qDebug() << "Invalid column selected for search:" << column;
        return;
    }

    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regExp);
    proxyModel->setFilterKeyColumn(columnIndex);

    qDebug() << "Search: column=" << column << "index=" << columnIndex << "text=" << text;
}

void MainWindow::on_resetSearchButton_clicked()
{
    ui->searchInput->clear();
    ui->searchCriteriaComboBox->setCurrentIndex(0);
    proxyModel->setFilterRegularExpression("");
    ui->tabtr->resizeColumnsToContents();
}
void MainWindow::toggleTheme()
{
    isDarkTheme = !isDarkTheme;
    if (isDarkTheme)
    {
        applyDarkTheme();
    }
    else
    {
        applyLightTheme();
    }
}
#include <QSqlError>

void MainWindow::applyLightTheme()
{
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

void MainWindow::applyDarkTheme()
{
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
void MainWindow::toggleSidebar()
{
    // Check if the sidebar is currently visible
    bool isVisible = ui->sideMenu->isVisible();

    // Toggle visibility
    ui->sideMenu->setVisible(!isVisible);
}
