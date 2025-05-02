/*#include "mainwindow.h"
#include "qtimer.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "resource.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <Qstring>
#include <QDate>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QTableView>
#include <QInputDialog>
#include <QBuffer>
#include <QFileDialog>
#include <QPixmap>
#include <QFile>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDate>
#include <QDebug>
#include <QCameraViewfinder>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>*/

#include "mainwindow.h"
#include "qtimer.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "resource.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QDate>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableWidget>
#include <QTableView>
#include <QInputDialog>
#include <QBuffer>
#include <QFileDialog>
#include <QPixmap>
#include <QFile>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QDate>
#include <QDebug>
#include <QImage>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    /*, ui(new Ui::MainWindow)
    , selectedResourceId(-1)
    , isDarkTheme(false)
    , imageData(QByteArray())*/
    , ui(new Ui::MainWindow)          // Matches declaration order
    , selectedResourceId(-1)          // Matches declaration order
    , imageData(QByteArray())         // Matches declaration order
    , isDarkTheme(false)              // Matches declaration order
    , sidebarWidget(nullptr)
    , searchTimer(new QTimer(this))
    , employeeName("DefaultEmployee") // Default employee name, you might want to set this dynamically
    , networkManager(new QNetworkAccessManager(this)) // Initialize network manager
    , pieSeries(new QPieSeries(this))
    , barSeries(new QBarSeries(this))

{
    ui->setupUi(this);



//connect(ui->btnLookForResource, &QPushButton::clicked, this, &MainWindow::on_btnLookForResource_clicked);


    QWidget *statisticsTab = ui->tabWidget->widget(3); // Statistics tab

    // Initialize pie chart
    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Resource Quantity Distribution");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);


    // Initialize stacked bar chart
    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Quantity by Type (%)");
    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignRight);
    ui->stackedBarChartView->setChart(barChart);
    ui->stackedBarChartView->setRenderHint(QPainter::Antialiasing);

    applyLightTheme(); // Set default theme

    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButtonClick);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &MainWindow::on_cancelFormButton_clicked);

    // New connections for search functionality
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_searchTextChanged);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButton_clicked);

    connect(ui->downloadHistoryButton, &QPushButton::clicked, this, &MainWindow::on_downloadHistoryButton_clicked);
    connect(ui->clearHistoryButton, &QPushButton::clicked, this, &MainWindow::on_clearHistoryButton_clicked);

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::on_imageAnalysisFinished); // Connect API response slot


    searchTimer->setSingleShot(true); // Timer fires only once per start
    connect(searchTimer, &QTimer::timeout, this, &MainWindow::on_searchTimeout);

    ui->tableWidget->setSortingEnabled(false);  // We'll handle sorting manually
    connect(ui->tableWidget->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &MainWindow::on_tableHeaderClicked);



    ui->tableWidget->verticalHeader()->setDefaultSectionSize(60);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabWidget->setTabText(0, "Add Resources");
    ui->tabWidget->setTabText(1, "Manage Resources");
    ui->tabWidget->setTabText(2, "New Calendar Name");
    ui->tabWidget->setTabText(3, "Statistics");

    // Remove the "New Calendar Name" tab (index 2)
    ui->tabWidget->removeTab(2);
    //loadResourcesIntoTable();
        refreshTableWidget();
    updatePieChart();
        updateStackedBarChart();


}

MainWindow::~MainWindow()
{

    delete networkManager;
    delete searchTimer;
    delete ui;
}




/*void MainWindow::on_btnLookForResource_clicked()
{
    SearchDialog dialog(this);
    dialog.exec();
}*/
void MainWindow::on_btnLookForResource_clicked() {
    qDebug() << "Look for Resource button clicked";
    SearchDialog searchDialog(this);
    if (searchDialog.exec() == QDialog::Accepted) {
        refreshTableWidget(); // Refresh table after dialog closes
    }
}


void MainWindow::updateStackedBarChart()
{
    // Clear existing data
    barSeries->clear();

    // Define the resource types
    QStringList resourceTypes = {"Electronics", "Cleaning Supplies", "Kitchen Supplies", "Stationery"};
    QMap<QString, double> typeQuantities;

    // Initialize quantities to zero
    for (const QString &type : resourceTypes) {
        typeQuantities[type] = 0.0;
    }

    // Query the database for quantities by type
    QSqlQuery query;
    query.prepare("SELECT TYPE, SUM(QUANTITY) as TOTAL_QUANTITY FROM RESOURCES GROUP BY TYPE");
    if (!query.exec()) {
        qDebug() << "Error fetching resource types for stacked bar chart:" << query.lastError().text();
        QBarSet *errorSet = new QBarSet("Error");
        *errorSet << 1.0;
        barSeries->append(errorSet);
        return;
    }

    // Calculate total quantity and quantities per type
    double totalQuantity = 0.0;
    while (query.next()) {
        QString type = query.value("TYPE").toString();
        double quantity = query.value("TOTAL_QUANTITY").toDouble();
        if (resourceTypes.contains(type)) {
            typeQuantities[type] = quantity;
            totalQuantity += quantity;
        }
    }

    // Create bar sets for each type
    if (totalQuantity > 0) {
        for (const QString &type : resourceTypes) {
            double quantity = typeQuantities[type];
            double percentage = (quantity / totalQuantity) * 100.0;
            QBarSet *barSet = new QBarSet(type + QString(" (%1%)").arg(percentage, 0, 'f', 1));
            *barSet << percentage; // Use percentage for the bar height
            barSeries->append(barSet);
        }

        // Set colors for each bar set
        static const QList<QColor> colors = {
            QColor("#FF6B6B"), // Electronics
            QColor("#4ECDC4"), // Cleaning Supplies
            QColor("#45B7D1"), // Kitchen Supplies
            QColor("#96CEB4")  // Stationery
        };
        int colorIndex = 0;
        for (QBarSet *set : barSeries->barSets()) {
            set->setColor(colors[colorIndex % colors.size()]);
            colorIndex++;
        }
    } else {
        QBarSet *noDataSet = new QBarSet("No Data");
        *noDataSet << 100.0;
        noDataSet->setColor(Qt::gray);
        barSeries->append(noDataSet);
    }

    // Configure axes
    QChart *chart = dynamic_cast<QChart *>(ui->stackedBarChartView->chart());
    if (chart) {
        // Remove existing axes to avoid duplicates
        chart->removeAxis(chart->axisX());
        chart->removeAxis(chart->axisY());

        // X-axis: Categories (just one bar)
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append("Resource Types");
        chart->addAxis(axisX, Qt::AlignBottom);
        barSeries->attachAxis(axisX);

        // Y-axis: Percentage (0 to 100%)
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, 100);
        axisY->setTitleText("Percentage (%)");
        chart->addAxis(axisY, Qt::AlignLeft);
        barSeries->attachAxis(axisY);
    }
}

void MainWindow::updatePieChart()
{
    pieSeries->clear();

    QSqlQuery query;
    query.prepare("SELECT NAME, QUANTITY FROM RESOURCES");
    if (!query.exec()) {
        qDebug() << "Error fetching resources for pie chart:" << query.lastError().text();
        pieSeries->append("Error", 1.0);
        return;
    }

    double totalQuantity = 0.0;
    QMap<QString, double> resourceQuantities;
    while (query.next()) {
        QString name = query.value("NAME").toString();
        double quantity = query.value("QUANTITY").toDouble();
        resourceQuantities[name] += quantity;
        totalQuantity += quantity;
    }

    if (totalQuantity > 0) {
        for (auto it = resourceQuantities.constBegin(); it != resourceQuantities.constEnd(); ++it) {
            QString name = it.key();
            double quantity = it.value();
            double percentage = (quantity / totalQuantity) * 100.0;
            QPieSlice *slice = new QPieSlice(name + QString(" (%1%)").arg(percentage, 0, 'f', 1), quantity);
            pieSeries->append(slice);
        }
    } else {
        pieSeries->append("No Data", 1.0);
    }
}

void MainWindow::on_tableHeaderClicked(int column)
{
    // If clicking the same column, toggle order; otherwise, default to ascending
    if (column == currentSortColumn) {
        currentSortOrder = (currentSortOrder == Qt::AscendingOrder) ?
                               Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        currentSortOrder = Qt::AscendingOrder;
    }

    currentSortColumn = column;
    sortTableByColumn(column, currentSortOrder);
}


void MainWindow::sortTableByColumn(int column, Qt::SortOrder order)
{
    // Store current selection
    int selectedRow = ui->tableWidget->currentRow();

    // Create a list to store all rows
    QList<QList<QTableWidgetItem*>> rows;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QList<QTableWidgetItem*> rowItems;
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            rowItems.append(ui->tableWidget->takeItem(i, j));
        }
        rows.append(rowItems);
    }

    // Sort based on column type
    std::sort(rows.begin(), rows.end(),
              [column, order](const QList<QTableWidgetItem*>& row1, const QList<QTableWidgetItem*>& row2) {
                  QTableWidgetItem* item1 = row1[column];
                  QTableWidgetItem* item2 = row2[column];

                  bool ascending = (order == Qt::AscendingOrder);

                  switch(column) {
                  case 0:  // ID (numeric)
                  case 4:  // Quantity (numeric)
                  {
                      int val1 = item1->text().toInt();
                      int val2 = item2->text().toInt();
                      return ascending ? (val1 < val2) : (val1 > val2);
                  }
                  case 5:  // Purchase Date (date)
                  {
                      QDate date1 = QDate::fromString(item1->text(), "yyyy-MM-dd");
                      QDate date2 = QDate::fromString(item2->text(), "yyyy-MM-dd");
                      return ascending ? (date1 < date2) : (date1 > date2);
                  }
                  case 6:  // Image (no sorting, return false)
                      return false;
                  default: // Name, Type, Brand (alphabetic)
                  {
                      QString text1 = item1->text();
                      QString text2 = item2->text();
                      return ascending ?
                                 (text1.compare(text2, Qt::CaseInsensitive) < 0) :
                                 (text1.compare(text2, Qt::CaseInsensitive) > 0);
                  }
                  }
              });

    // Clear and repopulate the table
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    for (int i = 0; i < rows.count(); i++) {
        ui->tableWidget->insertRow(i);
        for (int j = 0; j < rows[i].count(); j++) {
            ui->tableWidget->setItem(i, j, rows[i][j]);
        }
    }

    // Restore selection if valid
    if (selectedRow >= 0 && selectedRow < ui->tableWidget->rowCount()) {
        ui->tableWidget->selectRow(selectedRow);
    }
}


void MainWindow::on_exportPdfButton_clicked() {
    // Refresh the table with all data (no filter)
    refreshTableWidget("");
    qDebug() << "Table row count before export:" << ui->tableWidget->rowCount();

    // Prompt the user to choose a save location for the PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", QDir::homePath() + "/resources.pdf", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) {
        qDebug() << "PDF export canceled by user.";
        return;
    }

    // Ensure the file has a .pdf extension
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    // Check if file is writable to avoid issues later
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "PDF Export Error", "Cannot write to the selected file location.");
        qDebug() << "Failed to open file for writing:" << filePath;
        return;
    }
    file.close(); // Close it since QPrinter will handle writing

    // Set up the QPrinter for PDF output
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 50, 15, 15), QPageLayout::Millimeter);

    // Set up the QPainter to draw on the PDF
    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, "PDF Export Error", "Failed to initialize PDF painter.");
        qDebug() << "Failed to initialize QPainter for PDF export.";
        return;
    }

    // Get the page dimensions in pixels
    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);
    int pageWidth = pageRect.width();
    int pageHeight = pageRect.height();
    qDebug() << "Page dimensions:" << pageWidth << "x" << pageHeight;

    // Define layout constants
    const int tableMargin = 100;
    const int rowHeight = 700; // Increased to 500 for taller rows (was 400)
    const int fontSize = 10;   // Increased font size for better readability (was 9)
    const int headerFontSize = 12; // Increased header font size (was 11)
    const int imageSize = 600; // Increased to 300 for larger images (was 200)
    const int textPadding = 30; // Increased padding for better spacing (was 25)

    // Calculate column widths dynamically based on page width
    const int numColumns = ui->tableWidget->columnCount();
    int totalTableWidth = pageWidth - 2 * tableMargin;
    int colWidths[] = {40, 100, 100, 100, 100, 100, 200}; // Adjusted widths: Quantity (index 4) to 100, Image (index 6) to 350
    int totalDefinedWidth = 0;
    for (int colWidth : colWidths) {
        totalDefinedWidth += colWidth;
    }
    double scaleFactor = static_cast<double>(totalTableWidth) / totalDefinedWidth;
    for (int &colWidth : colWidths) {
        colWidth = static_cast<int>(colWidth * scaleFactor);
    }

    // Center the table on the page
    totalTableWidth = 0;
    for (int colWidth : colWidths) {
        totalTableWidth += colWidth;
    }
    int tableMarginAdjusted = (pageWidth - totalTableWidth) / 2;

    // Set up fonts
    QFont font("Arial", fontSize);
    QFont headerFont("Arial", headerFontSize, QFont::Bold);
    painter.setFont(font);

    // Draw the title and timestamp
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(tableMarginAdjusted, 150, "Resource Management System - Exported Data"); // Uncommented for better presentation
    painter.setFont(QFont("Arial", 10));
    painter.drawText(tableMarginAdjusted, 350, "Exported on: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    int y = 250;

    // Draw the table headers
    int x = tableMarginAdjusted;
    painter.setFont(headerFont);
    for (int col = 0; col < numColumns; ++col) {
        QString headerText = ui->tableWidget->horizontalHeaderItem(col)->text();
        QRect headerRect(x, y, colWidths[col], rowHeight);
        QRect textRect(x, y + textPadding, colWidths[col], rowHeight - 2 * textPadding);
        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, headerText);
        painter.drawRect(headerRect);
        x += colWidths[col];
    }
    y += rowHeight;

    // Set the font back to normal for the table data
    painter.setFont(font);

    // Draw the table data
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        x = tableMarginAdjusted;

        // Add alternating row colors
        if (row % 2 == 0) {
            painter.fillRect(QRect(tableMarginAdjusted, y, totalTableWidth, rowHeight), QBrush(QColor(240, 240, 240)));
        }

        for (int col = 0; col < numColumns; ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QRect cellRect(x, y, colWidths[col], rowHeight);
            QRect textRect(x, y + textPadding, colWidths[col], rowHeight - 2 * textPadding);

            if (col == 6) { // Image column
                if (item) {
                    QVariant imageData = item->data(Qt::DecorationRole);
                    if (imageData.canConvert<QPixmap>()) {
                        QPixmap pixmap = imageData.value<QPixmap>();
                        if (!pixmap.isNull()) {
                            QPixmap scaledPixmap = pixmap.scaled(imageSize, imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            int imageX = x + (colWidths[col] - imageSize) / 2; // Center horizontally
                            int imageY = y + (rowHeight - imageSize) / 2;      // Center vertically
                            painter.drawPixmap(imageX, imageY, scaledPixmap);
                            qDebug() << "Image rendered successfully for row" << row;
                        } else {
                            painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, "Image Load Failed");
                            qDebug() << "Image is null for row" << row;
                        }
                    } else {
                        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, "No Image Data");
                        qDebug() << "No image data for row" << row;
                    }
                } else {
                    painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, "No Item");
                    qDebug() << "No item in image column for row" << row;
                }
            } else { // Text columns
                QString text = item ? item->text() : "";
                painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
            }

            painter.drawRect(cellRect);
            x += colWidths[col];
        }

        y += rowHeight;

        // Check if we need a new page
        if (y > pageHeight - tableMargin) {
            printer.newPage();
            y = tableMargin;

            // Redraw the headers on the new page
            x = tableMarginAdjusted;
            painter.setFont(headerFont);
            for (int col = 0; col < numColumns; ++col) {
                QString headerText = ui->tableWidget->horizontalHeaderItem(col)->text();
                QRect headerRect(x, y, colWidths[col], rowHeight);
                QRect textRect(x, y + textPadding, colWidths[col], rowHeight - 2 * textPadding);
                painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, headerText);
                painter.drawRect(headerRect);
                x += colWidths[col];
            }
            y += rowHeight;
            painter.setFont(font);
        }
    }

    // Finish painting and save the PDF
    painter.end();

    // Verify the file was written
    if (QFile::exists(filePath)) {
        QMessageBox::information(this, "PDF Export", "Table data has been successfully exported to " + filePath);
        qDebug() << "PDF exported to:" << filePath;
    } else {
        QMessageBox::warning(this, "PDF Export Error", "Failed to save the PDF file.");
        qDebug() << "PDF export failed: File not found at" << filePath;
    }
}


void MainWindow::on_btnSelectImage_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!filePath.isEmpty()) {
        // Load the image into a QPixmap for preview
        QPixmap image;
        if (!image.load(filePath)) {
            QMessageBox::warning(this, "Image Load Error", "Failed to load the image. Please select a valid image file.");
            qDebug() << "Failed to load image into QPixmap:" << filePath;
            return;
        }

        // Scale and set the preview
        ui->lblImagePreview->setPixmap(image.scaled(ui->lblImagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Convert the image to QByteArray for database storage
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            imageData = file.readAll(); // Store the raw image data
            file.close();

            if (imageData.isEmpty()) {
                QMessageBox::warning(this, "Image Error", "The selected image file is empty or could not be read.");
                qDebug() << "Image data is empty after reading:" << filePath;
                ui->lblImagePreview->clear();
                imageData.clear();
                return;
            }

            qDebug() << "Image loaded successfully, size:" << imageData.size() << "bytes";
        } else {
            QMessageBox::warning(this, "File Error", "Failed to open the image file: " + file.errorString());
            qDebug() << "Failed to open image file:" << file.errorString();
            ui->lblImagePreview->clear();
            return;
        }
    } else {
        qDebug() << "No file selected by user.";
    }
}


void MainWindow::on_cancelFormButton_clicked()
{
    // Clear all form fields
    ui->nameLineEdit->clear();
    ui->typeComboBox->setCurrentIndex(0); // Reset to first item (assumed default)
    ui->brandLineEdit->clear();
    ui->quantityLineEdit->clear();
    ui->purchaseDateEdit->setDate(QDate::currentDate()); // Reset to today
    imageData.clear(); // Clear image data
    ui->lblImagePreview->clear(); // Clear image preview

    qDebug() << "Form cleared by Cancel button.";
}



#include <QMessageBox>
#include <QDate>
#include <QDebug>



void MainWindow::on_confirmFormButton_clicked()
{
    QString name = ui->nameLineEdit->text().trimmed();
    QString type = ui->typeComboBox->currentText().trimmed();
    QString brand = ui->brandLineEdit->text().trimmed();
    bool ok;
    int quantity = ui->quantityLineEdit->text().toInt(&ok);
    QDate purchase_date = ui->purchaseDateEdit->date();
    QDate currentDate = QDate::currentDate();

    if (name.isEmpty() || type.isEmpty() || brand.isEmpty() || !ok) {
        QMessageBox::warning(this, "Input Error", "All fields (Name, Type, Brand, Quantity) must be filled correctly.");
        return;
    }

    auto startsWithUpper = [](const QString& str) {
        return !str.isEmpty() && str[0].isUpper();
    };
    if (!startsWithUpper(name) || !startsWithUpper(type) || !startsWithUpper(brand)) {
        QMessageBox::warning(this, "Input Error", "Name, Type, and Brand must start with an uppercase letter.");
        return;
    }

    if (!ok || quantity <= 0) {
        QMessageBox::warning(this, "Input Error", "Quantity must be a positive number greater than zero.");
        return;
    }

    if (purchase_date > currentDate) {
        QMessageBox::warning(this, "Input Error", "Purchase date cannot be in the future.");
        return;
    }

    if (imageData.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select an image.");
        return;
    }

    // Prepare Clarifai API request
    QNetworkRequest request(QUrl("https://api.clarifai.com/v2/models/aaa03c23b3724a16a56b629203edc62c/outputs")); // General model ID
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Key 5deb057118f94dc8ac2dcd74b1047592"); // Replace with your Clarifai API key

    // Build the JSON payload
    QJsonObject imageObject;
    imageObject["base64"] = QString(imageData.toBase64()); // Encode image as base64

    QJsonObject dataObject;
    dataObject["image"] = imageObject;

    QJsonObject inputObject;
    inputObject["data"] = dataObject;

    QJsonArray inputsArray;
    inputsArray.append(inputObject);

    QJsonObject requestObject;
    requestObject["inputs"] = inputsArray;

    QJsonDocument doc(requestObject);
    QByteArray jsonData = doc.toJson();

    // Debug the JSON payload
    qDebug() << "JSON Payload:" << QString(jsonData);

    QNetworkReply *reply = networkManager->post(request, jsonData);
    reply->setProperty("resourceName", name);
    reply->setProperty("type", type);
    reply->setProperty("brand", brand);
    reply->setProperty("quantity", quantity);
    reply->setProperty("purchaseDate", purchase_date.toString("yyyy-MM-dd"));

    qDebug() << "Sent image analysis request to Clarifai API";
}



void MainWindow::on_imageAnalysisFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QString responseString = QString::fromUtf8(responseData);
        QMessageBox::warning(this, "API Error",
                             "Failed to analyze image: " + reply->errorString() + "\nServer response: " + responseString);
        qDebug() << "API request failed:" << reply->errorString();
        qDebug() << "Server response:" << responseString;
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = doc.object();

    QString resourceName = reply->property("resourceName").toString().toLower();
    bool matchFound = false;

    // Parse Clarifai response
    QJsonArray outputs = jsonObj["outputs"].toArray();
    if (!outputs.isEmpty()) {
        QJsonObject output = outputs[0].toObject();
        QJsonObject data = output["data"].toObject();
        QJsonArray concepts = data["concepts"].toArray();

        qDebug() << "Resource name to match:" << resourceName;
        qDebug() << "Detected concepts:";
        for (const QJsonValue &value : concepts) {
            QJsonObject concept = value.toObject();
            QString detectedName = concept["name"].toString().toLower();
            double confidence = concept["value"].toDouble();
            qDebug() << "- Name:" << detectedName << "Confidence:" << confidence;

            if (detectedName == resourceName) {
                matchFound = true;
                break;
            }
        }
    } else {
        qDebug() << "No concepts detected in the image.";
    }

    if (!matchFound) {
        QMessageBox::warning(this, "Mismatch Error", "The picture you added does not match the resource name '" + resourceName + "'.");
        reply->deleteLater();
        return;
    }

    Resource resource;
    resource.setName(reply->property("resourceName").toString());
    resource.setType(reply->property("type").toString());
    resource.setBrand(reply->property("brand").toString());
    resource.setQuantity(reply->property("quantity").toInt());
    resource.setPurchaseDate(QDate::fromString(reply->property("purchaseDate").toString(), "yyyy-MM-dd"));
    resource.setImage(imageData);

    if (resource.addResource(employeeName)) {
        qDebug() << "Resource added successfully by " << employeeName;
        QMessageBox::information(this, "Success", "Resource added successfully!");
        ui->nameLineEdit->clear();
        ui->typeComboBox->setCurrentIndex(0);
        ui->brandLineEdit->clear();
        ui->quantityLineEdit->clear();
        ui->purchaseDateEdit->setDate(QDate::currentDate());
        imageData.clear();
        ui->lblImagePreview->clear();
        refreshTableWidget();
        updatePieChart(); // Added
        updateStackedBarChart();
    } else {
        qDebug() << "Failed to add resource:" << QSqlQuery().lastError().text();
        QMessageBox::warning(this, "Error", "Failed to add resource: " + QSqlQuery().lastError().text());
    }

    reply->deleteLater();
}



void MainWindow::initializeSynonyms()
{
    objectSynonyms["pen"] = {"pen", "ballpoint pen", "ink pen", "writing instrument", "gel pen", "pencil", "ink", "writing"};
    objectSynonyms["paper"] = {"paper", "sheet", "notebook paper", "printer paper"};
    objectSynonyms["book"] = {"book", "textbook", "novel", "hardcover book"};
    objectSynonyms["laptop"] = {"laptop", "notebook computer", "computer", "laptop computer"};
    objectSynonyms["phone"] = {"phone", "smartphone", "mobile phone", "cell phone"};
    objectSynonyms["chair"] = {"chair", "office chair", "seat"};
    objectSynonyms["desk"] = {"desk", "table", "writing desk"};
    // Add more as needed
}

void MainWindow::refreshTableWidget(const QString &filter) {
    Resource R;
    QSqlQueryModel* model = new QSqlQueryModel();

    // Create a QSqlQuery object for executing the query
    QSqlQuery query;

    // Construct the SQL query with a filter if provided
    QString queryString = "SELECT RESOURCE_ID, NAME, TYPE, BRAND, QUANTITY, PURCHASE_DATE, IMAGE FROM RESOURCES";

    // Add filter if provided
    if (!filter.isEmpty()) {
        queryString += " WHERE UPPER(NAME) LIKE :nameFilter"
                       " OR UPPER(TYPE) LIKE :typeFilter"
                       " OR UPPER(BRAND) LIKE :brandFilter"
                       " OR CAST(RESOURCE_ID AS VARCHAR(10)) LIKE :idFilter"
                       " OR CAST(QUANTITY AS VARCHAR(10)) LIKE :quantityFilter"
                       " OR TO_CHAR(PURCHASE_DATE, 'YYYY-MM-DD') LIKE :dateFilter";
    }

    // Prepare the query
    query.prepare(queryString);

    // Bind the filter value if a filter is provided
    if (!filter.isEmpty()) {
        QString upperLikeFilter = "%" + filter.toUpper() + "%";
        QString likeFilter = "%" + filter + "%";
        query.bindValue(":nameFilter", upperLikeFilter);
        query.bindValue(":typeFilter", upperLikeFilter);
        query.bindValue(":brandFilter", upperLikeFilter);
        query.bindValue(":idFilter", likeFilter);
        query.bindValue(":quantityFilter", likeFilter);
        query.bindValue(":dateFilter", likeFilter);
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    // Set the query result to the model
    model->setQuery(query);

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error when displaying resources:" << model->lastError().text();
    }

    // Set up the tableWidget with data from the model
    ui->tableWidget->setRowCount(0); // Clear existing rows
    ui->tableWidget->setColumnCount(7); // Match the number of columns
    QStringList headers;
    headers << "ID" << "Name" << "Type" << "Brand" << "Quantity" << "Purchase Date" << "Image";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Copy data from model to tableWidget
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem();
            if (col == 5) { // Purchase Date column (index 5)
                QDate date = model->data(model->index(row, col)).toDate();
                if (date.isValid()) {
                    item->setText(date.toString("yyyy-MM-dd"));
                } else {
                    item->setText("Invalid Date");
                }
            } else if (col == 6) { // Image column (index 6)
                QByteArray imageData = model->data(model->index(row, col)).toByteArray();
                if (!imageData.isEmpty()) {
                    QPixmap pixmap;
                    pixmap.loadFromData(imageData);
                    if (!pixmap.isNull()) {
                        item->setData(Qt::DecorationRole, pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    } else {
                        item->setText("Image Load Failed");
                    }
                } else {
                    item->setText("No Image");
                }
            } else {
                item->setText(model->data(model->index(row, col)).toString());
            }
            // Store the original data type information for sorting
            item->setData(Qt::UserRole, model->data(model->index(row, col)));
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->setColumnWidth(6, 120); // Adjust width for the image column

    // Apply sorting if a column was previously sorted
    if (currentSortColumn != -1) {
        sortTableByColumn(currentSortColumn, currentSortOrder);
    }

    delete model;
}







void MainWindow::on_searchTextChanged(const QString &text) {
    // Store the text to search
    pendingSearchText = text;
    // Start or restart the timer (300ms delay)
    searchTimer->start(300);
}

void MainWindow::on_resetSearchButton_clicked() {
    // Clear the search bar
    ui->searchLineEdit->clear();


    // Refresh the table with no filter and no sorting
    refreshTableWidget();

}

void MainWindow::on_searchTimeout() {
    // Refresh the table with the pending search text
    refreshTableWidget(pendingSearchText);

    ui->searchLineEdit->setFocus();
}



void MainWindow::handleDeleteButtonClick() {
    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a resource to delete.");
        return;
    }

    // Get the ID of the selected resource
    int resource_id = ui->tableWidget->item(row, 0)->text().toInt();

    // Delete the resource from the database
    Resource R;
    if (R.delet(resource_id, employeeName)) { // Pass employeeName
        QMessageBox::information(this, "Success", "Resource deleted successfully.");

        // Refresh the table to remove the deleted resource
        refreshTableWidget();
        updatePieChart(); // Added
        updateStackedBarChart();
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete Resource. Check the database connection.");
    }
}





void MainWindow::on_updateButton_clicked()
{
    qDebug() << "on_updateButton_clicked called. Current row:" << ui->tableWidget->currentRow();

    // Get the selected row
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a row to update.");
        qDebug() << "No row selected, exiting.";
        return;
    }

    qDebug() << "Selected row:" << row << "with ID:" << ui->tableWidget->item(row, 0)->text();

    // Disconnect the button to prevent multiple clicks during the process
    disconnect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);

    // Create a dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Update Resource");

    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Create form fields
    QLabel *nameLabel = new QLabel("Name:");
    QLineEdit *nameEdit = new QLineEdit();
    QLabel *typeLabel = new QLabel("Type:");
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems(QStringList() << "Electronics" << "Cleaning Supplies" << "Kitchen Supplies" << "Stationery");
    QLabel *brandLabel = new QLabel("Brand:");
    QLineEdit *brandEdit = new QLineEdit();
    QLabel *quantityLabel = new QLabel("Quantity:");
    QLineEdit *quantityEdit = new QLineEdit();
    QLabel *purchaseDateLabel = new QLabel("Purchase Date:");
    QDateEdit *purchaseDateEdit = new QDateEdit();
    purchaseDateEdit->setDisplayFormat("yyyy-MM-dd");
    purchaseDateEdit->setCalendarPopup(true);

    // Add image selection fields
    QLabel *imageLabel = new QLabel("Image:");
    QPushButton *selectImageButton = new QPushButton("Select Image");
    QLabel *imagePreview = new QLabel();
    imagePreview->setFixedSize(100, 100); // Adjust size as needed
    imagePreview->setScaledContents(true);

    // Local variable to store the selected image data
    QByteArray updatedImageData;

    // Pre-fill with selected row data
    selectedResourceId = ui->tableWidget->item(row, 0)->text().toInt();
    nameEdit->setText(ui->tableWidget->item(row, 1)->text());
    typeCombo->setCurrentText(ui->tableWidget->item(row, 2)->text());
    brandEdit->setText(ui->tableWidget->item(row, 3)->text());
    quantityEdit->setText(ui->tableWidget->item(row, 4)->text());
    purchaseDateEdit->setDate(QDate::fromString(ui->tableWidget->item(row, 5)->text(), "yyyy-MM-dd"));

    // Connect the image selection button
    connect(selectImageButton, &QPushButton::clicked, &dialog, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&dialog, "Select an Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!filePath.isEmpty()) {
            QPixmap image;
            if (!image.load(filePath)) {
                QMessageBox::warning(&dialog, "Image Load Error", "Failed to load the image. Please select a valid image file.");
                qDebug() << "Failed to load image into QPixmap:" << filePath;
                return;
            }

            imagePreview->setPixmap(image.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                updatedImageData = file.readAll();
                file.close();

                if (updatedImageData.isEmpty()) {
                    QMessageBox::warning(&dialog, "Image Error", "The selected image file is empty or could not be read.");
                    qDebug() << "Image data is empty after reading:" << filePath;
                    imagePreview->clear();
                    updatedImageData.clear();
                    return;
                }

                qDebug() << "Updated image loaded successfully, size:" << updatedImageData.size() << "bytes";
            } else {
                QMessageBox::warning(&dialog, "File Error", "Failed to open the image file: " + file.errorString());
                qDebug() << "Failed to open image file:" << file.errorString();
                imagePreview->clear();
                return;
            }
        } else {
            qDebug() << "No image selected for update.";
        }
    });

    // Add fields to layout
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(typeLabel);
    layout->addWidget(typeCombo);
    layout->addWidget(brandLabel);
    layout->addWidget(brandEdit);
    layout->addWidget(quantityLabel);
    layout->addWidget(quantityEdit);
    layout->addWidget(purchaseDateLabel);
    layout->addWidget(purchaseDateEdit);
    layout->addWidget(imageLabel);
    layout->addWidget(selectImageButton);
    layout->addWidget(imagePreview);

    // Add OK and Cancel buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    // Connect buttons with validation
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, [&]() {
        // Retrieve values from the form
        QString name = nameEdit->text().trimmed();
        QString type = typeCombo->currentText().trimmed();
        QString brand = brandEdit->text().trimmed();
        bool ok;
        int quantity = quantityEdit->text().toInt(&ok);
        QDate purchase_date = purchaseDateEdit->date();
        QDate currentDate = QDate::currentDate(); // Current date: April 09, 2025

        // Validation checks
        // 1. Check if any field is empty
        if (name.isEmpty() || type.isEmpty() || brand.isEmpty() || !ok) {
            QMessageBox::warning(&dialog, "Input Error", "All fields (Name, Type, Brand, Quantity) must be filled correctly.");
            return;
        }

        // 2. Check if fields start with uppercase
        auto startsWithUpper = [](const QString& str) {
            return !str.isEmpty() && str[0].isUpper();
        };
        if (!startsWithUpper(name) || !startsWithUpper(type) || !startsWithUpper(brand)) {
            QMessageBox::warning(&dialog, "Input Error", "Name, Type, and Brand must start with an uppercase letter.");
            return;
        }

        // 3. Check quantity: must be > 0
        if (!ok || quantity <= 0) {
            QMessageBox::warning(&dialog, "Input Error", "Quantity must be a positive number greater than zero.");
            return;
        }

        // 4. Check purchase date: must not be in the future
        if (purchase_date > currentDate) {
            QMessageBox::warning(&dialog, "Input Error", "Purchase date cannot be in the future.");
            return;
        }

        // 5. Check if an image is selected
        if (updatedImageData.isEmpty()) {
            QMessageBox::warning(&dialog, "Input Error", "Please select an image.");
            return;
        }

        // If all validations pass, update the resource
        Resource resource;
        resource.setResourceId(selectedResourceId);
        resource.setName(name);
        resource.setType(type);
        resource.setBrand(brand);
        resource.setQuantity(quantity);
        resource.setPurchaseDate(purchase_date);
        resource.setImage(updatedImageData); // Set the updated image data

        if (resource.updateResource(employeeName)) { // Pass employeeName
            qDebug() << "Resource updated successfully by " << employeeName;
            refreshTableWidget();
            updatePieChart(); // Added
            updateStackedBarChart();
            QMessageBox::information(this, "Success", "Resource updated successfully.");
            dialog.accept();
        } else {
            qDebug() << "Failed to update resource:" << QSqlQuery().lastError().text();
            QMessageBox::warning(this, "Update Error", "Failed to update resource: " + QSqlQuery().lastError().text());
        }
    });
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    qDebug() << "Showing dialog...";
    dialog.exec();
    qDebug() << "Dialog closed. Current row after exec:" << ui->tableWidget->currentRow();
    qDebug() << "Selected Resource ID after exec:" << selectedResourceId;

    // Reconnect the button after the dialog closes
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    selectedResourceId = -1;
    qDebug() << "Reset selectedResourceId to -1. Current row after reset:" << ui->tableWidget->currentRow();
}

void MainWindow::on_downloadHistoryButton_clicked() {
    qDebug() << "on_downloadHistoryButton_clicked triggered";

    // Disconnect the button to prevent multiple triggers during execution
    disconnect(ui->downloadHistoryButton, &QPushButton::clicked, this, &MainWindow::on_downloadHistoryButton_clicked);

    // Open the save file dialog
    QString filePath = QFileDialog::getSaveFileName(this, "Save History File", QDir::homePath() + "/resource_history.txt", "Text Files (*.txt)");
    if (filePath.isEmpty()) {
        qDebug() << "Download canceled by user.";
        connect(ui->downloadHistoryButton, &QPushButton::clicked, this, &MainWindow::on_downloadHistoryButton_clicked);
        return;
    }

    // Check if the source history file exists
    QFile sourceFile("resource_history.txt");
    if (!sourceFile.exists()) {
        // If it doesn't exist, create an empty file
        if (sourceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            sourceFile.close();
            qDebug() << "Created an empty resource_history.txt file.";
        } else {
            QMessageBox::warning(this, "Error", "Failed to create history file: " + sourceFile.errorString());
            qDebug() << "Failed to create history file: " + sourceFile.errorString();
            connect(ui->downloadHistoryButton, &QPushButton::clicked, this, &MainWindow::on_downloadHistoryButton_clicked);
            return;
        }
    }

    // Check if the destination file already exists and remove it if it does
    if (QFile::exists(filePath)) {
        if (!QFile::remove(filePath)) {
            QMessageBox::warning(this, "Error", "Failed to overwrite existing file at " + filePath + ": " + QFile(filePath).errorString());
            qDebug() << "Failed to remove existing file: " << QFile(filePath).errorString();
            connect(ui->downloadHistoryButton, &QPushButton::clicked, this, &MainWindow::on_downloadHistoryButton_clicked);
            return;
        }
        qDebug() << "Removed existing file at " << filePath << " to allow overwrite.";
    }

    // Attempt to copy the file to the user-specified location
    if (QFile::copy("resource_history.txt", filePath)) {
        QMessageBox::information(this, "Success", "History file downloaded successfully to " + filePath);
        qDebug() << "History file downloaded to: " << filePath;
    } else {
        QMessageBox::warning(this, "Error", "Failed to download history file: " + sourceFile.errorString());
        qDebug() << "Failed to download history file: " + sourceFile.errorString();
    }

    // Reconnect the button after execution
    connect(ui->downloadHistoryButton, &QPushButton::clicked, this, &MainWindow::on_downloadHistoryButton_clicked);
    qDebug() << "on_downloadHistoryButton_clicked completed";
}

void MainWindow::on_clearHistoryButton_clicked() {
    qDebug() << "on_clearHistoryButton_clicked triggered";

    // Disconnect the button to prevent multiple triggers during execution
    disconnect(ui->clearHistoryButton, &QPushButton::clicked, this, &MainWindow::on_clearHistoryButton_clicked);

    QFile file("resource_history.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.close();
        QMessageBox::information(this, "Success", "History file cleared successfully.");
        qDebug() << "History file cleared successfully.";
    } else {
        QMessageBox::warning(this, "Error", "Failed to clear history file: " + file.errorString());
        qDebug() << "Failed to clear history file: " + file.errorString();
    }

    // Reconnect the button after execution
    connect(ui->clearHistoryButton, &QPushButton::clicked, this, &MainWindow::on_clearHistoryButton_clicked);
    qDebug() << "on_clearHistoryButton_clicked completed";
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

void MainWindow::toggleSidebar() {
    // Check if the sidebar is currently visible
    bool isVisible = ui->sideMenu->isVisible();

    // Toggle visibility
    ui->sideMenu->setVisible(!isVisible);
}



