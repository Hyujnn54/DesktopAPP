#include "trainingmanager.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>

TrainingManager::TrainingManager(bool dbConnected, QObject *parent)
    : QObject(parent),
    m_dbConnected(dbConnected),
    ui(nullptr),
    formations(new class formations),
    trainingTableModel(new QSqlQueryModel(this)),
    trainingProxyModel(new QSortFilterProxyModel(this)),
    notificationManager(nullptr),
    currentSortColumn(-1),
    currentSortOrder(Qt::AscendingOrder)
{
}

TrainingManager::~TrainingManager()
{
    delete formations;
    delete trainingTableModel;
    delete trainingProxyModel;
}

void TrainingManager::setNotificationManager(NotificationManager *manager)
{
    notificationManager = manager;
}

void TrainingManager::initialize(Ui::MainWindow *ui)
{
    this->ui = ui;
    trainingProxyModel->setSourceModel(trainingTableModel);
    ui->trainingTableView->setModel(trainingProxyModel);

    // Initialize search criteria combo box
    ui->trainingSearchCriteriaComboBox->clear();
    ui->trainingSearchCriteriaComboBox->addItems({"Name", "Description", "Trainer", "Date", "Duration", "Price"});

    // Connect signals
    connect(ui->trainingTableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &TrainingManager::on_trainingTableViewHeader_clicked);
    connect(ui->trainingExportButton, &QPushButton::clicked, this, &TrainingManager::on_trainingExportPdfButton_clicked);
    connect(ui->trainingAddButton, &QPushButton::clicked, this, &TrainingManager::on_trainingAddButton_clicked);
    connect(ui->trainingDeleteButton, &QPushButton::clicked, this, &TrainingManager::on_trainingDeleteButton_clicked);
    connect(ui->trainingUpdateButton, &QPushButton::clicked, this, &TrainingManager::on_trainingUpdateButton_clicked);
    connect(ui->trainingSearchInput, &QLineEdit::textChanged, this, &TrainingManager::on_trainingSearchInput_textChanged);
    connect(ui->trainingSearchCriteriaComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TrainingManager::on_trainingSearchCriteriaComboBox_currentIndexChanged);
    connect(ui->trainingResetSearchButton, &QPushButton::clicked, this, &TrainingManager::on_trainingResetSearchButton_clicked);

    refresh();
}

void TrainingManager::refresh()
{
    refreshTrainingTable();
}

void TrainingManager::on_trainingAddButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot add training: Database is not connected.");
        return;
    }
    if (!validateTrainingInputs()) {
        return;
    }
    QString name = ui->trainingNameInput->text().trimmed();
    QString description = ui->trainingDescriptionInput->text().trimmed();
    QDate date = ui->trainingDateEdit->date();
    QString trainer = ui->trainingTrainerInput->text().trimmed();
    int time = ui->trainingTimeSpinBox->value();
    double prix = ui->trainingPhoneNumberInput->text().trimmed().toDouble();

    auto result = formations->ajouter(name, description, trainer, date, time, prix);
    if (result.first) {
        QMessageBox::information(nullptr, "Success", "Training added successfully!");
        refreshTrainingTable();
        trainingProxyModel->setFilterRegularExpression(""); // Reset filter
        trainingProxyModel->sort(-1); // Reset sorting
        ui->trainingTableView->viewport()->update(); // Force repaint
        int newRow = trainingTableModel->rowCount() - 1; // Last row
        if (notificationManager) {
            notificationManager->addNotification("Added Training", "Training Section", QString("Training: %1").arg(name), newRow);
        }
        clearTrainingInputs();
    } else {
        QMessageBox::critical(nullptr, "Error", "Failed to add training: " + result.second);
    }
}

void TrainingManager::on_trainingDeleteButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot delete training: Database is not connected.");
        return;
    }
    QModelIndexList selectedIndexes = ui->trainingTableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(nullptr, "Selection Error", "Please select a training to delete.");
        return;
    }
    int row = selectedIndexes.first().row();
    QString trainingName = trainingProxyModel->data(trainingProxyModel->index(row, 0)).toString();
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr,
        "Confirm Deletion",
        QString("Are you sure you want to delete the training '%1'?").arg(trainingName),
        QMessageBox::Yes | QMessageBox::No
        );
    if (reply == QMessageBox::Yes) {
        if (formations->supprimer(trainingName)) {
            QMessageBox::information(nullptr, "Success", "Training deleted successfully!");
            if (notificationManager) {
                notificationManager->addNotification("Deleted Training", "Training Section", QString("Training: %1").arg(trainingName), row);
            }
            refreshTrainingTable();
            trainingProxyModel->setFilterRegularExpression(""); // Reset filter
            trainingProxyModel->sort(-1); // Reset sorting
            ui->trainingTableView->viewport()->update(); // Force repaint
        } else {
            QMessageBox::critical(nullptr, "Error", "Failed to delete training.");
        }
    }
}

void TrainingManager::on_trainingUpdateButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot update training: Database is not connected.");
        return;
    }
    QModelIndexList selectedIndexes = ui->trainingTableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(nullptr, "Selection Error", "Please select a training to update.");
        return;
    }
    int row = selectedIndexes.first().row();
    QString name = trainingProxyModel->data(trainingProxyModel->index(row, 0)).toString();
    QString description = trainingProxyModel->data(trainingProxyModel->index(row, 1)).toString();
    QString trainer = trainingProxyModel->data(trainingProxyModel->index(row, 2)).toString();
    QDate date = trainingProxyModel->data(trainingProxyModel->index(row, 3)).toDate();
    int time = trainingProxyModel->data(trainingProxyModel->index(row, 4)).toInt();
    double prix = trainingProxyModel->data(trainingProxyModel->index(row, 5)).toDouble();

    UpdateTrainingDialog dialog(nullptr);
    dialog.setTrainingData(name, description, trainer, date, time, prix);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getName();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();

        if (formations->modifier(name, newName, newDescription, newTrainer, newDate, newTime, newPrix)) {
            QMessageBox::information(nullptr, "Success", "Training updated successfully!");
            if (notificationManager) {
                notificationManager->addNotification("Updated Training", "Training Section", QString("Training: %1 updated to %2").arg(name, newName), row);
            }
            refreshTrainingTable();
            trainingProxyModel->setFilterRegularExpression(""); // Reset filter
            trainingProxyModel->sort(-1); // Reset sorting
            ui->trainingTableView->viewport()->update(); // Force repaint
        } else {
            QMessageBox::critical(nullptr, "Error", "Failed to update training.");
        }
    }
}

bool TrainingManager::validateTrainingInputs()
{
    QString name = ui->trainingNameInput->text().trimmed();
    QString description = ui->trainingDescriptionInput->text().trimmed();
    QDate date = ui->trainingDateEdit->date();
    QString trainer = ui->trainingTrainerInput->text().trimmed();
    int time = ui->trainingTimeSpinBox->value();
    QString prixStr = ui->trainingPhoneNumberInput->text().trimmed();

    if (name.isEmpty() || description.isEmpty() || trainer.isEmpty() || prixStr.isEmpty()) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in all fields.");
        return false;
    }
    if (!isValidName(name) || !isValidName(trainer)) {
        QMessageBox::warning(nullptr, "Input Error", "Invalid name or trainer format.");
        return false;
    }
    if (!date.isValid()) {
        QMessageBox::warning(nullptr, "Input Error", "Invalid date.");
        return false;
    }
    if (time <= 0) {
        QMessageBox::warning(nullptr, "Input Error", "Training duration must be greater than 0.");
        return false;
    }
    bool ok;
    double prix = prixStr.toDouble(&ok);
    if (!ok || prix < 0) {
        QMessageBox::warning(nullptr, "Input Error", "Invalid price.");
        return false;
    }
    return true;
}

bool TrainingManager::isValidName(const QString &name)
{
    QRegularExpression re("^[A-Za-z0-9\\s]+$");
    return re.match(name).hasMatch();
}

void TrainingManager::clearTrainingInputs()
{
    ui->trainingNameInput->clear();
    ui->trainingDescriptionInput->clear();
    ui->trainingDateEdit->setDate(QDate::currentDate());
    ui->trainingTrainerInput->clear();
    ui->trainingTimeSpinBox->setValue(0);
    ui->trainingPhoneNumberInput->clear();
}

void TrainingManager::performTrainingSearch()
{
    QString searchText = ui->trainingSearchInput->text().trimmed();
    int criteriaIndex = ui->trainingSearchCriteriaComboBox->currentIndex();
    QString column;
    switch (criteriaIndex) {
    case 0: column = "FORMATION"; break;
    case 1: column = "DESCRIPTION"; break;
    case 2: column = "TRAINER"; break;
    case 3: column = "DATEF"; break;
    case 4: column = "TIME"; break;
    case 5: column = "PRIX"; break;
    default: column = "FORMATION"; break;
    }

    int columnIndex = trainingTableModel->record().indexOf(column);
    if (columnIndex != -1) {
        trainingProxyModel->setFilterKeyColumn(columnIndex);
        trainingProxyModel->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(searchText),
                                                                          QRegularExpression::CaseInsensitiveOption));
    } else {
        qDebug() << "Column not found:" << column;
        trainingProxyModel->setFilterKeyColumn(0);
        trainingProxyModel->setFilterRegularExpression("");
    }
}

void TrainingManager::refreshTrainingTable()
{
    trainingTableModel->setQuery("SELECT FORMATION, DESCRIPTION, TRAINER, DATEF, TIME, PRIX FROM AHMED.FORMATIONS", QSqlDatabase::database());
    if (trainingTableModel->lastError().isValid()) {
        qDebug() << "Training table query failed:" << trainingTableModel->lastError().text();
        return;
    }
    trainingTableModel->setHeaderData(0, Qt::Horizontal, "Name");
    trainingTableModel->setHeaderData(1, Qt::Horizontal, "Description");
    trainingTableModel->setHeaderData(2, Qt::Horizontal, "Trainer");
    trainingTableModel->setHeaderData(3, Qt::Horizontal, "Date");
    trainingTableModel->setHeaderData(4, Qt::Horizontal, "Duration");
    trainingTableModel->setHeaderData(5, Qt::Horizontal, "Price");
    ui->trainingTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TrainingManager::on_trainingSearchInput_textChanged(const QString &text)
{
    if (!m_dbConnected) {
        return;
    }
    if (text.isEmpty()) {
        trainingProxyModel->setFilterRegularExpression("");
        refreshTrainingTable();
    } else {
        performTrainingSearch();
    }
}

void TrainingManager::on_trainingSearchCriteriaComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (!m_dbConnected) {
        return;
    }
    if (!ui->trainingSearchInput->text().isEmpty()) {
        performTrainingSearch();
    }
}

void TrainingManager::on_trainingResetSearchButton_clicked()
{
    if (!m_dbConnected) {
        return;
    }
    ui->trainingSearchInput->clear();
    ui->trainingSearchCriteriaComboBox->setCurrentIndex(0); // Reset to first item
    trainingProxyModel->setFilterRegularExpression("");
    trainingProxyModel->sort(-1); // Reset sorting
    refreshTrainingTable();
    ui->trainingTableView->viewport()->update(); // Force repaint
}

void TrainingManager::on_trainingTableViewHeader_clicked(int logicalIndex)
{
    if (!m_dbConnected) {
        return;
    }
    if (currentSortColumn == logicalIndex) {
        currentSortOrder = (currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        currentSortColumn = logicalIndex;
        currentSortOrder = Qt::AscendingOrder;
    }
    trainingProxyModel->sort(logicalIndex, currentSortOrder);
}

void TrainingManager::on_trainingExportPdfButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(nullptr, "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }
    exportTrainingsToPdf();
}

void TrainingManager::exportTrainingsToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&pdfWriter);
    QFont font("Arial", 10);
    painter.setFont(font);

    QStringList headers = {"Name", "Description", "Trainer", "Date", "Duration", "Price"};
    int y = 50;
    int tableWidth = pdfWriter.width() - 40;
    QVector<qreal> columnWidths = {0.2, 0.2, 0.15, 0.15, 0.15, 0.15};

    // Draw title
    painter.drawText(20, 30, "Training List");

    // Draw headers
    int x = 20;
    for (int i = 0; i < headers.size(); ++i) {
        int columnWidth = tableWidth * columnWidths[i];
        painter.drawText(x, y, columnWidth, 20, Qt::AlignLeft, headers[i]);
        x += columnWidth;
    }
    y += 20;
    painter.drawLine(20, y, tableWidth + 20, y);
    y += 10;

    // Draw rows using the proxy model to reflect filtering and sorting
    int rowCount = trainingProxyModel->rowCount();
    if (rowCount == 0) {
        painter.drawText(20, y, "No trainings to display.");
        return;
    }

    for (int row = 0; row < rowCount; ++row) {
        x = 20;
        for (int col = 0; col < headers.size(); ++col) {
            int columnWidth = tableWidth * columnWidths[col];
            QString text = trainingProxyModel->data(trainingProxyModel->index(row, col)).toString();
            painter.drawText(x, y, columnWidth, 20, Qt::AlignLeft, text);
            x += columnWidth;
        }
        y += 20;
        if (y > pdfWriter.height() - 40) {
            pdfWriter.newPage();
            y = 50;
            x = 20;
            for (int i = 0; i < headers.size(); ++i) {
                int columnWidth = tableWidth * columnWidths[i];
                painter.drawText(x, y, columnWidth, 20, Qt::AlignLeft, headers[i]);
                x += columnWidth;
            }
            y += 20;
            painter.drawLine(20, y, tableWidth + 20, y);
            y += 10;
        }
    }
}

QMap<QString, int> TrainingManager::getStatisticsByCategory(const QString &category)
{
    QMap<QString, int> stats;
    if (!m_dbConnected) {
        return stats; // Return empty map if database is not connected
    }

    QString column;
    // Map the UI filter to the database column name
    if (category == "Trainer") {
        column = "TRAINER";
    } else if (category == "Price") {
        column = "PRIX";
    } else if (category == "Date") {
        column = "DATEF";
    } else {
        qDebug() << "Unsupported filter category:" << category;
        return stats;
    }

    // Construct SQL query to group by the specified category and count occurrences
    QString queryString = QString("SELECT %1, COUNT(*) as count FROM AHMED.FORMATIONS GROUP BY %1").arg(column);
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return stats;
    }

    // Populate the QMap with the results
    while (query.next()) {
        QString key;
        if (category == "Date") {
            // Format date for better readability in charts
            QDate date = query.value(0).toDate();
            key = date.toString("yyyy-MM-dd");
        } else {
            key = query.value(0).toString();
        }
        int count = query.value(1).toInt();
        stats[key] = count;
    }

    return stats;
}
