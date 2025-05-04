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
#include <QCheckBox>
#include <QSpinBox>

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

    // Populate trainer combo box with employee names and IDs
    ui->trainingTrainerComboBox->clear();
    QSqlQuery empQuery("SELECT ID, FIRST_NAME, LAST_NAME FROM EMPLOYEE");
    while (empQuery.next()) {
        int id = empQuery.value(0).toInt();
        QString name = empQuery.value(1).toString() + " " + empQuery.value(2).toString();
        ui->trainingTrainerComboBox->addItem(name, id);
    }
    // Populate client combo box with client names and IDs
    ui->trainingClientComboBox->clear();
    QSqlQuery clientQuery("SELECT CLIENT_ID, NAME FROM CLIENTS");
    while (clientQuery.next()) {
        int id = clientQuery.value(0).toInt();
        QString name = clientQuery.value(1).toString();
        ui->trainingClientComboBox->addItem(name, id);
    }
    // Populate resource scroll area with checkboxes and spinboxes for each resource
    QLayout* resourceLayout = ui->trainingResourceVBoxLayout;
    QSqlQuery resQuery("SELECT RESOURCE_ID, NAME FROM RESOURCES");
    while (resQuery.next()) {
        int id = resQuery.value(0).toInt();
        QString name = resQuery.value(1).toString();
        QWidget* rowWidget = new QWidget();
        QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
        QCheckBox* checkBox = new QCheckBox(name);
        checkBox->setObjectName(QString("resourceCheckBox_%1").arg(id));
        QSpinBox* spinBox = new QSpinBox();
        spinBox->setMinimum(1);
        spinBox->setMaximum(1000);
        spinBox->setValue(1);
        spinBox->setObjectName(QString("resourceSpinBox_%1").arg(id));
        spinBox->setEnabled(false);
        rowLayout->addWidget(checkBox);
        rowLayout->addWidget(spinBox);
        rowWidget->setLayout(rowLayout);
        resourceLayout->addWidget(rowWidget);
        QObject::connect(checkBox, &QCheckBox::toggled, spinBox, &QSpinBox::setEnabled);
    }

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
    int trainerId = ui->trainingTrainerComboBox->currentData().toInt();
    QString trainerName = ui->trainingTrainerComboBox->currentText();
    int clientId = ui->trainingClientComboBox->currentData().toInt();
    int time = ui->trainingTimeSpinBox->value();
    double prix = ui->trainingPriceSpinBox->value();

    // Collect selected resources and their quantities
    QList<QPair<int, int>> selectedResources; // (resourceId, quantity)
    QLayout* resourceLayout = ui->trainingResourceVBoxLayout;
    for (int i = 0; i < resourceLayout->count(); ++i) {
        QWidget* rowWidget = resourceLayout->itemAt(i)->widget();
        if (!rowWidget) continue;
        QCheckBox* checkBox = rowWidget->findChild<QCheckBox*>();
        QSpinBox* spinBox = rowWidget->findChild<QSpinBox*>();
        if (checkBox && spinBox && checkBox->isChecked()) {
            // Extract resourceId from objectName
            QString objName = checkBox->objectName();
            int resourceId = objName.section('_', 1, 1).toInt();
            int quantity = spinBox->value();
            selectedResources.append(qMakePair(resourceId, quantity));
        }
    }

    // Get the next value from the sequence for the new training ID
    QSqlQuery seqQuery("SELECT FORMATIONS_SEQ.NEXTVAL FROM DUAL");
    int newTrainingId = -1;
    if (seqQuery.next()) {
        newTrainingId = seqQuery.value(0).toInt();
    } else {
        QMessageBox::critical(nullptr, "Error", "Failed to get new training ID from sequence.");
        return;
    }

    // Insert the training using the newTrainingId
    auto result = formations->ajouterWithId(newTrainingId, name, description, trainerName, trainerId, clientId, date, time, prix);
    if (result.first) {
        // Insert into TRAINING_RESOURCES table for each selected resource
        for (const auto& pair : selectedResources) {
            int resourceId = pair.first;
            int quantity = pair.second;
            QSqlQuery linkQuery;
            linkQuery.prepare("INSERT INTO AHMED.TRAINING_RESOURCES (TRAINING_ID, RESOURCE_ID, QUANTITY) VALUES (:tid, :rid, :qty)");
            linkQuery.bindValue(":tid", newTrainingId);
            linkQuery.bindValue(":rid", resourceId);
            linkQuery.bindValue(":qty", quantity);
            linkQuery.exec(); // Error handling can be added
        }
        QMessageBox::information(nullptr, "Success", "Training added successfully!");
        refreshTrainingTable();
        trainingProxyModel->setFilterRegularExpression("");
        trainingProxyModel->sort(-1);
        ui->trainingTableView->viewport()->update();
        int newRow = trainingTableModel->rowCount() - 1;
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
    // Find the training ID (assume FORMATION is unique, otherwise adjust logic)
    QSqlQuery idQuery;
    idQuery.prepare("SELECT IDFORM FROM AHMED.FORMATIONS WHERE FORMATION = :formation");
    idQuery.bindValue(":formation", name);
    int trainingId = -1;
    if (idQuery.exec() && idQuery.next()) {
        trainingId = idQuery.value(0).toInt();
    }
    if (trainingId != -1) {
        dialog.setResourceData(trainingId);
    }
    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getName();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();
        // --- Update resources in DB ---
        if (trainingId != -1) {
            QSqlQuery delQuery;
            delQuery.prepare("DELETE FROM AHMED.TRAINING_RESOURCES WHERE TRAINING_ID = :tid");
            delQuery.bindValue(":tid", trainingId);
            delQuery.exec();
            for (const auto& pair : dialog.getSelectedResources()) {
                int resourceId = pair.first;
                int quantity = pair.second;
                QSqlQuery linkQuery;
                linkQuery.prepare("INSERT INTO AHMED.TRAINING_RESOURCES (TRAINING_ID, RESOURCE_ID, QUANTITY) VALUES (:tid, :rid, :qty)");
                linkQuery.bindValue(":tid", trainingId);
                linkQuery.bindValue(":rid", resourceId);
                linkQuery.bindValue(":qty", quantity);
                linkQuery.exec();
            }
        }
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
    int trainerIndex = ui->trainingTrainerComboBox->currentIndex();
    int clientIndex = ui->trainingClientComboBox->currentIndex();
    int time = ui->trainingTimeSpinBox->value();
    QString prixStr = ui->trainingPhoneNumberInput->text().trimmed();

    // Validate at least one resource is selected
    bool resourceSelected = false;
    QLayout* resourceLayout = ui->trainingResourceVBoxLayout;
    for (int i = 0; i < resourceLayout->count(); ++i) {
        QWidget* rowWidget = resourceLayout->itemAt(i)->widget();
        if (!rowWidget) continue;
        QCheckBox* checkBox = rowWidget->findChild<QCheckBox*>();
        if (checkBox && checkBox->isChecked()) {
            resourceSelected = true;
            break;
        }
    }

    if (name.isEmpty() || description.isEmpty() || trainerIndex < 0 || clientIndex < 0 || !resourceSelected || prixStr.isEmpty()) {
        QMessageBox::warning(nullptr, "Input Error", "Please fill in all fields and select at least one resource.");
        return false;
    }
    if (!isValidName(name)) {
        QMessageBox::warning(nullptr, "Input Error", "Invalid name format.");
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
    ui->trainingTrainerComboBox->setCurrentIndex(-1);
    ui->trainingClientComboBox->setCurrentIndex(-1);
    ui->trainingTimeSpinBox->setValue(0);
    ui->trainingPhoneNumberInput->clear();
    // Uncheck all resource checkboxes and reset spinboxes
    QLayout* resourceLayout = ui->trainingResourceVBoxLayout;
    for (int i = 0; i < resourceLayout->count(); ++i) {
        QWidget* rowWidget = resourceLayout->itemAt(i)->widget();
        if (!rowWidget) continue;
        QCheckBox* checkBox = rowWidget->findChild<QCheckBox*>();
        QSpinBox* spinBox = rowWidget->findChild<QSpinBox*>();
        if (checkBox) checkBox->setChecked(false);
        if (spinBox) {
            spinBox->setValue(1);
            spinBox->setEnabled(false);
        }
    }
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
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save PDF", "Trainings.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(50, 50, 50, 50)); // Increased margins
    QPainter painter(&pdfWriter);
    QFont regularFont("Arial", 16); // Larger font
    QFont headerFont("Arial", 20, QFont::Bold); // Larger header font
    QFont titleFont("Arial", 32, QFont::Bold); // Larger title font
    int pageWidth = pdfWriter.width();
    int tableWidth = pageWidth - 100; // Adjusted for larger margins
    int rowHeight = 80; // Increased row height
    int cellPadding = 24; // Increased padding
    painter.setFont(titleFont);
    painter.drawText(50, 100, "Training List"); // Adjusted Y position
    painter.setFont(headerFont);
    painter.drawText(50, 160, QString("Generated on %1").arg(QDate::currentDate().toString("yyyy-MM-dd"))); // Adjusted Y position
    QStringList headers = {"Name", "Description", "Trainer", "Date", "Duration", "Price"};
    QVector<qreal> columnWidths = {0.2, 0.2, 0.15, 0.15, 0.15, 0.15};
    int y = 220; // Adjusted starting Y position
    painter.setFont(headerFont);
    int x = 50; // Adjusted X position
    QRect headerRect(50, y, tableWidth, rowHeight);
    painter.fillRect(headerRect, QColor(230, 230, 230));
    painter.setPen(QPen(Qt::black, 2)); // Slightly thicker border
    painter.drawRect(headerRect);
    for (int i = 0; i < headers.size(); ++i) {
        int colWidth = tableWidth * columnWidths[i];
        QRect cellRect(x, y, colWidth, rowHeight);
        painter.drawRect(cellRect);
        painter.drawText(cellRect.adjusted(cellPadding, 0, -cellPadding, 0), Qt::AlignVCenter | Qt::AlignLeft, headers[i]); // Left-aligned headers
        x += colWidth;
    }
    y += rowHeight;
    painter.setFont(regularFont);
    int rowCount = trainingProxyModel->rowCount();
    if (rowCount == 0) {
        QRect noDataRect(50, y, tableWidth, rowHeight);
        painter.drawRect(noDataRect);
        painter.drawText(noDataRect, Qt::AlignCenter, "No trainings to display.");
        painter.end();
        return;
    }
    QColor altRowColor(245, 245, 245);
    for (int row = 0; row < rowCount; ++row) {
        if (row % 2 == 1) {
            QRect rowRect(50, y, tableWidth, rowHeight);
            painter.fillRect(rowRect, altRowColor);
        }
        x = 50;
        for (int col = 0; col < headers.size(); ++col) {
            int colWidth = tableWidth * columnWidths[col];
            QRect cellRect(x, y, colWidth, rowHeight);
            painter.drawRect(cellRect);
            QString text = trainingProxyModel->data(trainingProxyModel->index(row, col)).toString();
            if (col == 3 && !text.isEmpty()) {
                QDate date = trainingProxyModel->data(trainingProxyModel->index(row, col)).toDate();
                if (date.isValid()) {
                    text = date.toString("yyyy-MM-dd");
                }
            }
            if (col == 4) {
                text += " hours";
            }
            if (col == 5) {
                text = QString("$%1").arg(text);
            }
            painter.drawText(cellRect.adjusted(cellPadding, 0, -cellPadding, 0), Qt::AlignVCenter | Qt::AlignLeft, text); // Increased padding
            x += colWidth;
        }
        y += rowHeight;
        if (y > pdfWriter.height() - 120) { // Adjusted for larger margins
            pdfWriter.newPage();
            y = 100; // Reset Y position
            painter.setFont(headerFont);
            x = 50;
            QRect headerRect(50, y, tableWidth, rowHeight);
            painter.fillRect(headerRect, QColor(230, 230, 230));
            painter.drawRect(headerRect);
            for (int i = 0; i < headers.size(); ++i) {
                int colWidth = tableWidth * columnWidths[i];
                QRect cellRect(x, y, colWidth, rowHeight);
                painter.drawRect(cellRect);
                painter.drawText(cellRect.adjusted(cellPadding, 0, -cellPadding, 0), Qt::AlignVCenter | Qt::AlignLeft, headers[i]); // Left-aligned headers
                x += colWidth;
            }
            y += rowHeight;
            painter.setFont(regularFont);
        }
    }
    painter.end();
    QMessageBox::information(nullptr, "Success", "Training list exported to PDF successfully!");
    if (notificationManager) {
        notificationManager->addNotification("PDF Exported", "Training Section", "Training list exported to " + fileName, -1);
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
