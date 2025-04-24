#include "formationwindow.h"
#include "formations.h"
#include "mainwindow.h"
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

FormationWindow::FormationWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::formationwindow),
    formation(),
    isDarkTheme(false),
    tableModel(nullptr),
    proxyModel(new QSortFilterProxyModel(this)),
    notificationCount(0),
    notificationLabel(ui->notificationLabel),
    sidebarWidget(nullptr),
    changeHistory(),
    m_dbConnected(dbConnected), // Moved up to match declaration order
    networkManager(nullptr), // Moved down to match declaration order
    totalFormations(0),
    totalCost(0.0),
    avgCost(0.0)
{
    ui->setupUi(this);
    applyLightTheme();

    ui->date->setDate(QDate::currentDate());

    ui->notificationLabel->setText("Notifications: 0");
    ui->notificationLabel->setStyleSheet("font-weight: bold; color: #3A5DAE;");

    // Manual connections
    connect(ui->add, &QPushButton::clicked, this, &FormationWindow::on_addButtonclicked);
    connect(ui->deletef, &QPushButton::clicked, this, &FormationWindow::on_deleteButtonClicked);
    connect(ui->updateButton, &QPushButton::clicked, this, &FormationWindow::on_updateButtonClicked);
    connect(ui->themeButton, &QPushButton::clicked, this, &FormationWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &FormationWindow::toggleSidebar);
    connect(ui->exportButton, &QPushButton::clicked, this, &FormationWindow::exportToPdf);
    connect(ui->notificationLabel, &QLabel::linkActivated, this, &FormationWindow::onNotificationLabelClicked);
    connect(ui->refreshStatsButton, &QPushButton::clicked, this, &FormationWindow::on_refreshStatsButton_clicked);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &FormationWindow::on_searchInput_textChanged);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &FormationWindow::on_resetSearchButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &FormationWindow::on_pushButton_2_clicked);

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

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &FormationWindow::onSmsRequestFinished);

    setAttribute(Qt::WA_DeleteOnClose);

    if (!m_dbConnected) {
        // Disable database-dependent UI elements
        ui->add->setEnabled(false);
        ui->deletef->setEnabled(false);
        ui->updateButton->setEnabled(false);
        ui->exportButton->setEnabled(false);
        ui->refreshStatsButton->setEnabled(false);
        ui->searchInput->setEnabled(false);
        ui->resetSearchButton->setEnabled(false);
        statusBar()->showMessage("Database not connected. Database features are disabled.");
    } else {
        refreshTableView();
        updateStatistics();
    }
}

FormationWindow::~FormationWindow()
{
    delete tableModel;
    delete ui;
}

void FormationWindow::refreshTableView()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot load formation data: Database is not connected.");
        return;
    }

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

void FormationWindow::updateStatistics()
{
    qDebug() << "Starting updateStatistics";

    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot update statistics: Database is not connected.");
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
        return;
    }

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
        totalCost = query.value("total_cost").toDouble();
        avgCost = (totalFormations > 0) ? totalCost / totalFormations : 0.0;

        ui->totalFormationsLabel->setText(QString("Total Formations: %1").arg(totalFormations));
        ui->totalCostLabel->setText(QString("Total Cost: $%1").arg(totalCost, 0, 'f', 2));
        ui->avgCostLabel->setText(QString("Average Cost: $%1").arg(avgCost, 0, 'f', 2));
    } else {
        qDebug() << "No statistics data retrieved";
        totalFormations = 0;
        totalCost = 0.0;
        avgCost = 0.0;
        ui->totalFormationsLabel->setText("Total Formations: 0");
        ui->totalCostLabel->setText("Total Cost: $0.00");
        ui->avgCostLabel->setText("Average Cost: $0.00");
    }

    qDebug() << "Updated statistics - Total Formations:" << totalFormations
             << "Total Cost:" << totalCost
             << "Average Cost:" << avgCost;
}

void FormationWindow::on_pushButton_2_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open Main Window: Database is not connected.");
        return;
    }
    MainWindow *mainWindow = new MainWindow(m_dbConnected, this);
    mainWindow->show();
    this->close(); // Close FormationWindow
}

void FormationWindow::on_addButtonclicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot add formation: Database is not connected.");
        return;
    }

    QString formationName = ui->format->text().trimmed();
    QString description = ui->des->text().trimmed();
    QString trainer = ui->tr->text().trimmed();
    QDate date = ui->date->date();
    int time = ui->timeb->value();
    double prix = ui->prixb->value();
    QString phoneNumber = ui->phoneNumberInput->text().trimmed();

    if (formationName.isEmpty() || description.isEmpty() || trainer.isEmpty() || phoneNumber.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    if (!date.isValid() || date < QDate::currentDate()) {
        QMessageBox::warning(this, "Input Error", "Please select a valid future date.");
        return;
    }

    QRegularExpression phoneRx("\\+\\d{10,15}");
    if (!phoneRx.match(phoneNumber).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Phone number must start with '+' followed by 10-15 digits.");
        return;
    }

    Formations f;
    auto result = f.ajouter(formationName, description, trainer, date, time, prix);
    if (result.first) {
        QMessageBox::information(this, "Success", "Formation added successfully!");
        sendSmsNotification(phoneNumber, formationName, date);
        refreshTableView();
        updateStatistics();
        addChangeToHistory("Added", formationName);

        ui->format->clear();
        ui->des->clear();
        ui->tr->clear();
        ui->date->setDate(QDate::currentDate());
        ui->timeb->setValue(1);
        ui->prixb->setValue(1);
        ui->phoneNumberInput->clear();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add formation: " + result.second);
    }
}

void FormationWindow::on_deleteButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot delete formation: Database is not connected.");
        return;
    }

    QModelIndexList selectedIndexes = ui->tabtr->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a formation to delete.");
        return;
    }

    int row = selectedIndexes.first().row();
    QModelIndex index = proxyModel->mapToSource(selectedIndexes.first());
    QString formationName = tableModel->data(tableModel->index(index.row(), 1)).toString();

    Formations f;
    if (f.supprimer(formationName)) {
        QMessageBox::information(this, "Success", "Formation deleted successfully!");
        refreshTableView();
        updateStatistics();
        addChangeToHistory("Deleted", formationName);
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete formation.");
    }
}

void FormationWindow::on_updateButtonClicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot update formation: Database is not connected.");
        return;
    }

    QModelIndexList selectedIndexes = ui->tabtr->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a formation to update.");
        return;
    }

    int row = selectedIndexes.first().row();
    QModelIndex index = proxyModel->mapToSource(selectedIndexes.first());

    QString idForm = tableModel->data(tableModel->index(index.row(), 0)).toString();
    QString formationName = tableModel->data(tableModel->index(index.row(), 1)).toString();
    QString description = tableModel->data(tableModel->index(index.row(), 2)).toString();
    QString trainer = tableModel->data(tableModel->index(index.row(), 3)).toString();
    QDate date = QDate::fromString(tableModel->data(tableModel->index(index.row(), 4)).toString(), "yyyy-MM-dd");
    int time = tableModel->data(tableModel->index(index.row(), 5)).toInt();
    double prix = tableModel->data(tableModel->index(index.row(), 6)).toDouble();

    UpdateTrainingDialog dialog(this);
    dialog.setTrainingData(idForm, formationName, description, trainer, date, time, prix);

    if (dialog.exec() == QDialog::Accepted) {
        QString newFormationName = dialog.getFormationName();
        QString newDescription = dialog.getDescription();
        QString newTrainer = dialog.getTrainer();
        QDate newDate = dialog.getDate();
        int newTime = dialog.getTime();
        double newPrix = dialog.getPrix();

        Formations f;
        bool success = f.modifier(idForm, newFormationName, newDescription, newTrainer, newDate, newTime, newPrix);
        if (success) {
            QMessageBox::information(this, "Success", "Formation updated successfully!");
            refreshTableView();
            updateStatistics();
            addChangeToHistory("Updated", newFormationName);
        } else {
            QMessageBox::critical(this, "Error", "Failed to update formation.");
        }
    }
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
        QLineEdit, QDateEdit, QSpinBox {
            background-color: #F5F7FA;
            border: 1px solid #3A5DAE;
            border-radius: 4px;
            padding: 4px;
            color: #333333;
        }
        QLineEdit:focus, QDateEdit:focus, QSpinBox:focus {
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
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        #notificationLabel {
            font-size: 10pt;
            font-weight: bold;
            color: #3A5DAE;
        }
        QFrame#sideMenu {
            border: 2px solid #3A5DAE;
            border-radius: 5px;
            background-color: #E6ECF5;
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
        }
        QPushButton:hover {
            background-color: #F5A38A;
        }
        QPushButton:pressed {
            background-color: #D96C53;
        }
        QLineEdit, QDateEdit, QSpinBox {
            background-color: #6A6A6A;
            border: 1px solid #F28C6F;
            border-radius: 4px;
            padding: 4px;
            color: #F0F0F0;
        }
        QLineEdit:focus, QDateEdit:focus, QSpinBox:focus {
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
        QLabel {
            font-size: 10pt;
            padding: 2px;
        }
        #notificationLabel {
            font-size: 10pt;
            font-weight: bold;
            color: #F28C6F;
        }
        QFrame#sideMenu {
            border: 2px solid #F28C6F;
            border-radius: 5px;
            background-color: #7A7A7A;
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void FormationWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}

void FormationWindow::exportToPdf()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot export to PDF: Database is not connected.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(20, 20, 20, 20));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    const qreal scaleFactor = pdfWriter.logicalDpiX() / 72.0;
    const qreal mmToPoints = 2.83465;
    const qreal margin = 20 * mmToPoints * scaleFactor;
    const qreal pageWidth = pdfWriter.width() - 2 * margin;
    const qreal pageHeight = pdfWriter.height() - 2 * margin;

    QFont titleFont("Arial", 14, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont bodyFont("Arial", 9);
    QFont summaryFont("Arial", 11, QFont::Bold);

    painter.setFont(titleFont);
    painter.setPen(Qt::darkBlue);
    QString title = "Formation Management Report";
    QString date = QDate::currentDate().toString("MMMM d, yyyy");
    painter.drawText(QRectF(margin, margin, pageWidth, 50 * scaleFactor), Qt::AlignCenter, title + "\nGenerated on: " + date);

    qreal yPos = margin + 60 * scaleFactor;

    QStringList headers = {"ID", "Formation", "Description", "Trainer", "Date", "Time", "Price"};
    qreal columnWidths[] = {0.5, 1.0, 1.5, 1.0, 1.0, 0.5, 0.5};
    qreal totalWidthUnits = 0;
    for (qreal width : columnWidths) totalWidthUnits += width;
    qreal tableWidth = pageWidth;
    qreal rowHeight = 30 * scaleFactor;

    QVector<qreal> actualWidths(headers.size());
    qreal accumulatedWidth = margin;
    for (int i = 0; i < headers.size(); ++i) {
        actualWidths[i] = (columnWidths[i] / totalWidthUnits) * tableWidth;
    }

    painter.setFont(headerFont);
    painter.setBrush(QBrush(QColor(200, 220, 255)));
    painter.setPen(Qt::black);
    painter.drawRect(QRectF(margin, yPos, tableWidth, rowHeight));
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(QRectF(accumulatedWidth, yPos, actualWidths[i], rowHeight),
                         Qt::AlignCenter | Qt::TextWordWrap, headers[i]);
        accumulatedWidth += actualWidths[i];
    }
    yPos += rowHeight;

    int rowCount = tableModel->rowCount();
    int colCount = tableModel->columnCount();
    int pageNumber = 1;

    painter.setFont(bodyFont);
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < rowCount; ++row) {
        if (yPos + rowHeight > pageHeight - margin) {
            painter.setFont(bodyFont);
            painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                             Qt::AlignRight, QString("Page %1").arg(pageNumber++));
            pdfWriter.newPage();
            yPos = margin;
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
            QString text = tableModel->data(tableModel->index(row, col)).toString();
            QRectF textRect(accumulatedWidth + 2 * scaleFactor, yPos, actualWidths[col] - 4 * scaleFactor, rowHeight);
            painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
            accumulatedWidth += actualWidths[col];
        }
        yPos += rowHeight;
    }

    painter.setFont(bodyFont);
    painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                     Qt::AlignRight, QString("Page %1").arg(pageNumber));

    yPos += 20 * scaleFactor;
    if (yPos + 60 * scaleFactor < pageHeight - margin) {
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QString summary = QString("Summary:\nTotal Formations: %1\nTotal Cost: $%2\nAverage Cost: $%3")
                              .arg(totalFormations)
                              .arg(totalCost, 0, 'f', 2)
                              .arg(avgCost, 0, 'f', 2);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
    } else {
        pdfWriter.newPage();
        yPos = margin;
        painter.setFont(summaryFont);
        painter.setPen(Qt::darkGreen);
        QString summary = QString("Summary:\nTotal Formations: %1\nTotal Cost: $%2\nAverage Cost: $%3")
                              .arg(totalFormations)
                              .arg(totalCost, 0, 'f', 2)
                              .arg(avgCost, 0, 'f', 2);
        painter.drawText(QRectF(margin, yPos, pageWidth, 60 * scaleFactor), Qt::AlignLeft, summary);
        painter.setFont(bodyFont);
        painter.setPen(Qt::black);
        painter.drawText(QRectF(margin, pageHeight + margin - 10 * scaleFactor, pageWidth, 10 * scaleFactor),
                         Qt::AlignRight, QString("Page %1").arg(pageNumber + 1));
    }

    painter.end();
    QMessageBox::information(this, "Success", "PDF exported successfully to " + fileName);
}

void FormationWindow::addChangeToHistory(const QString &action, const QString &formationName)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString change = QString("%1: %2 formation '%3'").arg(timestamp, action, formationName);
    changeHistory.append(change);

    notificationCount++;
    ui->notificationLabel->setText(QString("<a href='view_notifications' style='color: %1; text-decoration: none;'>Notifications: %2</a>")
                                       .arg(isDarkTheme ? "#F28C6F" : "#3A5DAE")
                                       .arg(notificationCount));
}

void FormationWindow::onNotificationLabelClicked(const QString &link)
{
    if (link == "view_notifications") {
        QString historyText = changeHistory.isEmpty() ? "No changes recorded." : changeHistory.join("\n");
        QMessageBox::information(this, "Change History", historyText);
        notificationCount = 0;
        ui->notificationLabel->setText(QString("<a href='view_notifications' style='color: %1; text-decoration: none;'>Notifications: 0</a>")
                                           .arg(isDarkTheme ? "#F28C6F" : "#3A5DAE"));
    }
}

void FormationWindow::on_refreshStatsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot refresh statistics: Database is not connected.");
        return;
    }
    updateStatistics();
}

void FormationWindow::on_searchInput_textChanged(const QString &text)
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot search: Database is not connected.");
        return;
    }
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    proxyModel->setFilterWildcard(text);
}

void FormationWindow::on_resetSearchButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot reset search: Database is not connected.");
        return;
    }
    ui->searchInput->clear();
    proxyModel->setFilterWildcard("");
}

void FormationWindow::sendSmsNotification(const QString &phoneNumber, const QString &formationName, const QDate &date)
{
    QSettings settings("MyApp", "FormationApp");
    QString apiKey = settings.value("twilioApiKey", "").toString();
    QString senderNumber = settings.value("twilioSenderNumber", "").toString();

    if (apiKey.isEmpty() || senderNumber.isEmpty()) {
        QMessageBox::warning(this, "SMS Error", "Twilio API key or sender number not configured.");
        return;
    }

    QString message = QString("Reminder: You have a formation '%1' scheduled on %2.").arg(formationName, date.toString("yyyy-MM-dd"));

    QUrl url("https://api.twilio.com/2010-04-01/Accounts/YOUR_ACCOUNT_SID/Messages.json");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QByteArray(("YOUR_ACCOUNT_SID:" + apiKey).toUtf8()).toBase64());

    QUrlQuery params;
    params.addQueryItem("To", phoneNumber);
    params.addQueryItem("From", senderNumber);
    params.addQueryItem("Body", message);

    networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void FormationWindow::onSmsRequestFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();
        QString status = obj["status"].toString();
        if (status == "queued" || status == "sent") {
            QMessageBox::information(this, "SMS Success", "SMS notification sent successfully!");
        } else {
            QMessageBox::warning(this, "SMS Error", "Failed to send SMS: " + obj["message"].toString());
        }
    } else {
        QMessageBox::warning(this, "SMS Error", "Failed to send SMS: " + reply->errorString());
    }
    reply->deleteLater();
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
            updateStatistics();
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

void FormationWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}
