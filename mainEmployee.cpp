#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "employee.h"
#include <QComboBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <Qstring>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QTableView>
#include <QInputDialog>
#include "updateemployeedialog.h"
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <sign.h>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QImage>
#include <QBuffer>
#include "qrcodegen/qrcodegen.hpp"
#include <QHBoxLayout>
#include <QSerialPort>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , employee(new Employee())
    , employeeModel(new QSqlQueryModel(this))
    , imageDelegate(new ImageDelegate(this))
    , arduino(new Arduino())
    , serialBuffer("")
    , isDarkTheme(false)
    , pieChartView(nullptr)
    , barChartView(nullptr)
{
    static int instanceCount = 0;
    instanceCount++;
    qDebug() << "MainWindow constructor called, instance count:" << instanceCount;

    ui->setupUi(this);
    applyLightTheme();

    ui->tableView->setModel(employeeModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->setItemDelegateForColumn(11, imageDelegate);

    QRegularExpression cinRegex("^[0-9]{8}$");
    QRegularExpressionValidator* cinValidator = new QRegularExpressionValidator(cinRegex, this);
    ui->lineEdit_CIN->setValidator(cinValidator);

    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");
    QValidator* nameValidator = new QRegularExpressionValidator(nameRegex, this);
    ui->lineEdit_Nom->setValidator(nameValidator);
    ui->lineEdit_Prenom->setValidator(nameValidator);

    QRegularExpression phoneRegex("^[0-9]{8}$");
    QRegularExpressionValidator* phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    ui->lineEdit_phone->setValidator(phoneValidator);

    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    QRegularExpressionValidator* emailValidator = new QRegularExpressionValidator(emailRegex, this);
    ui->lineEdit_email->setValidator(emailValidator);

    QIntValidator* salaryValidator = new QIntValidator(0, 1000000, this);
    ui->lineEdit_salaire->setValidator(salaryValidator);
    QDate currentDate = QDate::currentDate();
    ui->dateEdit_hiring->setDate(currentDate);
    ui->dateEdit_hiring->setMinimumDate(currentDate);
    ui->dateEdit_hiring->setMaximumDate(currentDate);

    loadEmployees();

    ui->deleteBtn->setAutoRepeat(false);
    ui->modifyBtn->setAutoRepeat(false);
    ui->downloadBtn->setAutoRepeat(false);

    ui->deleteBtn->setEnabled(false);
    ui->modifyBtn->setEnabled(false);
    ui->downloadBtn->setEnabled(false);

    setupConnections();

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::handleHeaderClicked);
    connect(ui->selectImageButton, &QPushButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        if (!fileName.isEmpty()) {
            ui->imagePathLineEdit->setText(fileName);
        }
    });

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this]() {
                bool hasSelection = !ui->tableView->selectionModel()->selectedRows().isEmpty();
                ui->deleteBtn->setEnabled(hasSelection && loggedInRole == "Admin");
                ui->modifyBtn->setEnabled(hasSelection && (loggedInRole == "Admin" || loggedInRole == "Manager"));
                ui->downloadBtn->setEnabled(hasSelection);
                ui->generateQRCodeBtn->setEnabled(hasSelection); // Enable QR code button on selection
                qDebug() << "Selection changed, hasSelection:" << hasSelection;
            });
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->refreshStatsButton, &QPushButton::clicked, this, &MainWindow::on_refreshStatsButtonClicked);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        QWidget *tab = ui->tabWidget->widget(index);
        if (tab) {
            QPropertyAnimation *anim = new QPropertyAnimation(tab, "pos");
            anim->setDuration(400);
            anim->setStartValue(QPoint(tab->width(), tab->y()));
            anim->setEndValue(QPoint(0, tab->y()));
            anim->setEasingCurve(QEasingCurve::InOutQuad); // Smooth transition, no bounce
            anim->start(QAbstractAnimation::DeleteWhenStopped);

            if (index == 2) { // Statistics tab
                updateStatistics();
                if (pieChartView && barChartView) {
                    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);

                    QPropertyAnimation *pieOpacityAnim = new QPropertyAnimation(pieChartView, "opacity");
                    pieOpacityAnim->setDuration(600);
                    pieOpacityAnim->setStartValue(0.0);
                    pieOpacityAnim->setEndValue(1.0);
                    pieOpacityAnim->setEasingCurve(QEasingCurve::InOutQuad);

                    QPropertyAnimation *barOpacityAnim = new QPropertyAnimation(barChartView, "opacity");
                    barOpacityAnim->setDuration(600);
                    barOpacityAnim->setStartValue(0.0);
                    barOpacityAnim->setEndValue(1.0);
                    barOpacityAnim->setEasingCurve(QEasingCurve::InOutQuad);

                    group->addAnimation(pieOpacityAnim);
                    group->addAnimation(barOpacityAnim);
                    group->start(QAbstractAnimation::DeleteWhenStopped);
                }
            }
        }
    });

    // Button scale animation (no bounce)
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        connect(button, &QPushButton::clicked, this, [button]() {
            QPropertyAnimation *scaleAnim = new QPropertyAnimation(button, "geometry");
            scaleAnim->setDuration(200);
            QRect startRect = button->geometry();
            QRect endRect = startRect.adjusted(-2, -2, 2, 2); // Slight scale up
            scaleAnim->setStartValue(startRect);
            scaleAnim->setEndValue(endRect);
            scaleAnim->setEasingCurve(QEasingCurve::InOutQuad);

            QPropertyAnimation *reverseAnim = new QPropertyAnimation(button, "geometry");
            reverseAnim->setDuration(200);
            reverseAnim->setStartValue(endRect);
            reverseAnim->setEndValue(startRect);
            reverseAnim->setEasingCurve(QEasingCurve::InOutQuad);

            QSequentialAnimationGroup *group = new QSequentialAnimationGroup(button);
            group->addAnimation(scaleAnim);
            group->addAnimation(reverseAnim);
            group->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }


    updateUIBasedOnRole();
    // Initialize Arduino serial connection
    QString portName = "COM4"; // Adjust this to your Arduino's port
    if (arduino->connectArduino(portName)) {
        qDebug() << "Arduino connected successfully";
        connect(arduino, &Arduino::dataAvailable, this, &MainWindow::handleSerialData);
    } else {
        qDebug() << "Failed to connect to Arduino";
        QMessageBox::warning(this, "Arduino Error", "Failed to connect to Arduino on port " + portName);
    }
     statusBar()->setVisible(true);
    // ... (rest of the constructor remains unchanged)
}

void MainWindow::handleSerialData()
{
    // Read all available data and append to the buffer
    QByteArray data = arduino->readData();
    serialBuffer += QString(data);

    // Process complete lines (ending with \n)
    int newlineIndex;
    while ((newlineIndex = serialBuffer.indexOf('\n')) != -1) {
        // Extract the complete line (excluding \n)
        QString uid = serialBuffer.left(newlineIndex).trimmed();
        serialBuffer.remove(0, newlineIndex + 1);

        if (uid.isEmpty()) {
            continue;
        }

        qDebug() << "Received RFID UID from Arduino:" << uid;

        // Look up the employee with the matching RFID_UID
        QSqlQuery query;
        query.prepare("SELECT ID, FIRST_NAME, LAST_NAME FROM EMPLOYEE WHERE RFID_UID = :uid");
        query.bindValue(":uid", uid);
        if (!query.exec()) {
            qDebug() << "Query Error:" << query.lastError().text();
            QMessageBox::warning(this, "Database Error", "Failed to execute query: " + query.lastError().text());
            statusBar()->showMessage("Database Error", 5000);
            continue;
        }

        if (query.next()) {
            int employeeId = query.value("ID").toInt();
            QString firstName = query.value("FIRST_NAME").toString();
            QString lastName = query.value("LAST_NAME").toString();
            qDebug() << "Employee found - ID:" << employeeId << "Name:" << firstName << lastName;

            // Query the number of consultations for today
            QSqlQuery countQuery;
            countQuery.prepare("SELECT COUNT(*) FROM CLIENTS WHERE CONSULTANT_ID = :employeeId AND TRUNC(CONSULTATION_DATE) = TO_DATE(:today, 'YYYY-MM-DD')");
            countQuery.bindValue(":employeeId", employeeId);
            countQuery.bindValue(":today", QDate::currentDate().toString("yyyy-MM-dd")); // Use current date dynamically
            if (!countQuery.exec()) {
                qDebug() << "Count Query Error:" << countQuery.lastError().text();
                statusBar()->showMessage("Error retrieving consultation count", 5000);
                // Send "0" to Arduino in case of query error
                arduino->sendData("0");
            } else if (countQuery.next()) {
                int consultationCount = countQuery.value(0).toInt();
                statusBar()->showMessage("Consultations Today for " + firstName + " " + lastName + ": " + QString::number(consultationCount), 5000);
                qDebug() << "Consultations today for Employee ID" << employeeId << ":" << consultationCount;
                // Send the consultation count to the Arduino
                arduino->sendData(QString::number(consultationCount));
            } else {
                statusBar()->showMessage("Consultations Today for " + firstName + " " + lastName + ": 0", 5000);
                qDebug() << "No consultations found for Employee ID" << employeeId;
                // Send "0" to Arduino if no consultations
                arduino->sendData("0");
            }

            // Display employee info in a message box (without consultation count)
            QMessageBox::information(this, "Employee Found", "RFID card belongs to Employee ID: " + QString::number(employeeId) + "\nName: " + firstName + " " + lastName);
        } else {
            qDebug() << "No employee found with RFID UID:" << uid;
            QMessageBox::warning(this, "No Match", "No employee found with RFID UID " + uid + ".");
            statusBar()->showMessage("No employee found for RFID UID: " + uid, 5000);
            // Send "0" to Arduino if no employee found
            arduino->sendData("0");
        }
    }
}
MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor called, instance count decremented";
    delete ui;
    delete employee;
    delete employeeModel;
    delete imageDelegate;
    delete pieChartView;
    delete barChartView;
}
void MainWindow::setLoggedInRole(const QString &role)
{
    loggedInRole = role;
    updateUIBasedOnRole();
}

void MainWindow::updateUIBasedOnRole()
{
    if (loggedInRole == "Admin") {
        ui->addButton->setEnabled(true);
        ui->modifyBtn->setEnabled(true);
        ui->deleteBtn->setEnabled(true);
        ui->downloadBtn->setEnabled(ui->tableView->selectionModel()->hasSelection());
        ui->tabWidget->setTabEnabled(0, true);
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
    } else if (loggedInRole == "Manager") {
        ui->addButton->setEnabled(false);
        ui->modifyBtn->setEnabled(true);
        ui->deleteBtn->setEnabled(false);
        ui->downloadBtn->setEnabled(ui->tableView->selectionModel()->hasSelection());
        ui->tabWidget->setTabEnabled(0, false);
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
    } else if (loggedInRole == "Employee") {
        ui->addButton->setEnabled(false);
        ui->modifyBtn->setEnabled(false);
        ui->deleteBtn->setEnabled(false);
        ui->downloadBtn->setEnabled(ui->tableView->selectionModel()->hasSelection());
        ui->tabWidget->setTabEnabled(0, false);
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
    } else {
        qDebug() << "Unknown role:" << loggedInRole;
        ui->addButton->setEnabled(false);
        ui->modifyBtn->setEnabled(false);
        ui->deleteBtn->setEnabled(false);
        ui->tabWidget->setTabEnabled(0, false);
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
    }
    qDebug() << "Role set to:" << loggedInRole << "UI updated";
}

void MainWindow::on_refreshStatsButtonClicked()
{
    qDebug() << "Refresh Statistics button clicked";
    updateStatistics();
}

void MainWindow::setupConnections()
{
    qDebug() << "setupConnections called";
    // Disconnect any existing connections to avoid duplicates
    disconnect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    disconnect(ui->modifyBtn, &QPushButton::clicked, this, &MainWindow::on_modifyBtnClicked);
    disconnect(ui->saveUpdateBtn, &QPushButton::clicked, this, &MainWindow::on_saveUpdateBtnClicked);
    disconnect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteBtnClicked);
    disconnect(ui->downloadBtn, &QPushButton::clicked, this, &MainWindow::on_downloadBtnClicked);
    disconnect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButtonclicked);
    disconnect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);
    disconnect(ui->logoutButton, &QPushButton::clicked, this, &::MainWindow::on_logoutButtonclicked);
    disconnect(ui->generateQRCodeBtn, &QPushButton::clicked, this, &MainWindow::on_generateQRCodeBtnClicked);

    // Set up new connections
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &MainWindow::on_modifyBtnClicked);
    connect(ui->saveUpdateBtn, &QPushButton::clicked, this, &MainWindow::on_saveUpdateBtnClicked);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteBtnClicked);
    connect(ui->downloadBtn, &QPushButton::clicked, this, &MainWindow::on_downloadBtnClicked);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButtonclicked);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);
    connect(ui->logoutButton, &QPushButton::clicked, this, &::MainWindow::on_logoutButtonclicked);
    connect(ui->generateQRCodeBtn, &QPushButton::clicked, this, &MainWindow::on_generateQRCodeBtnClicked);

    // Employee statistics chart connections
    connect(ui->employeeChartRefreshButton, &QPushButton::clicked, this, &MainWindow::updateStatistics);
    connect(ui->employeeChartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateStatistics);
    connect(ui->employeeChartFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateStatistics);
    connect(ui->employeeToggleLegendCheckBox, &QCheckBox::toggled, this, &MainWindow::updateStatistics);
}
void MainWindow::updateStatistics()
{
    qDebug() << "Starting updateStatistics with new UI";

    // Get the chart from the chart view
    QChart *chart = new QChart();
    chart->setTitle("Employee Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(ui->employeeToggleLegendCheckBox->isChecked());
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // Get chart type and filter
    QString chartType = ui->employeeChartTypeComboBox->currentText();
    QString filter = ui->employeeChartFilterComboBox->currentText();
    
    // Clear any existing chart
    if (ui->employeeChartView) {
        ui->employeeChartView->setChart(chart);
    }
    
    // Update chart with employee data based on selected filter
    if (filter == "Specialty") {
        // Create a query for specialty distribution
        QSqlQuery query("SELECT SPECIALITY, COUNT(*) as count FROM EMPLOYEE GROUP BY SPECIALITY");
        QMap<QString, int> data;
        
        while (query.next()) {
            QString specialty = query.value("SPECIALITY").toString();
            if (specialty.isEmpty()) specialty = "Unspecified";
            int count = query.value("count").toInt();
            data[specialty] = count;
        }
        
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            
            // Add slices to the pie series
            int totalCount = 0;
            for (auto it = data.begin(); it != data.end(); ++it) {
                totalCount += it.value();
            }
            
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                float percentage = (float)it.value() / totalCount * 100.0;
                slice->setLabel(QString("%1: %2 (%3%)").arg(it.key()).arg(it.value()).arg(percentage, 0, 'f', 1));
                slice->setLabelVisible(true);
                
                // Connect hover signal for interactive feedback
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->employeeHoverDescriptionLabel->setText(slice->label());
                    } else {
                        slice->setExploded(false);
                        ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                    }
                });
                
                // Animate each slice appearing
                QPropertyAnimation *animation = new QPropertyAnimation(slice, "startAngle");
                animation->setDuration(500);
                animation->setStartValue(0);
                animation->setEndValue(slice->startAngle());
                animation->setEasingCurve(QEasingCurve::OutBack);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                
                QPropertyAnimation *angleAnimation = new QPropertyAnimation(slice, "angleSpan");
                angleAnimation->setDuration(500);
                angleAnimation->setStartValue(0);
                angleAnimation->setEndValue(slice->angleSpan());
                angleAnimation->setEasingCurve(QEasingCurve::OutBack);
                angleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }
            
            chart->addSeries(series);
            chart->setTitle("Employee Distribution by Specialty");
            
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *barSet = new QBarSet("Employees");
            
            QStringList categories;
            for (auto it = data.begin(); it != data.end(); ++it) {
                *barSet << it.value();
                categories << it.key();
            }
            
            series->append(barSet);
            chart->addSeries(series);
            
            // Set up the category axis
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            
            // Set up the value axis
            QValueAxis *axisY = new QValueAxis();
            int maxValue = 0;
            for (auto it = data.begin(); it != data.end(); ++it) {
                maxValue = qMax(maxValue, it.value());
            }
            axisY->setRange(0, maxValue * 1.1); // Add 10% padding
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            
            // Connect hover signals for interactive feedback
            connect(series, &QBarSeries::hovered, [this, categories](bool status, int index, QBarSet *barset) {
                if (status && index >= 0 && index < categories.size()) {
                    ui->employeeHoverDescriptionLabel->setText(
                        QString("%1: %2 employees").arg(categories.at(index)).arg(barset->at(index))
                    );
                } else {
                    ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
            
            chart->setTitle("Employee Count by Specialty");
        }
    } else if (filter == "Role") {
        // Create a query for role distribution
        QSqlQuery query("SELECT ROLE, COUNT(*) as count FROM EMPLOYEE GROUP BY ROLE");
        QMap<QString, int> data;
        
        while (query.next()) {
            QString role = query.value("ROLE").toString();
            if (role.isEmpty()) role = "Unspecified";
            int count = query.value("count").toInt();
            data[role] = count;
        }
        
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            
            // Add slices to the pie series
            int totalCount = 0;
            for (auto it = data.begin(); it != data.end(); ++it) {
                totalCount += it.value();
            }
            
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                float percentage = (float)it.value() / totalCount * 100.0;
                slice->setLabel(QString("%1: %2 (%3%)").arg(it.key()).arg(it.value()).arg(percentage, 0, 'f', 1));
                slice->setLabelVisible(true);
                
                // Connect hover signal for interactive feedback
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->employeeHoverDescriptionLabel->setText(slice->label());
                    } else {
                        slice->setExploded(false);
                        ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                    }
                });
                
                // Animate each slice appearing
                QPropertyAnimation *animation = new QPropertyAnimation(slice, "startAngle");
                animation->setDuration(500);
                animation->setStartValue(0);
                animation->setEndValue(slice->startAngle());
                animation->setEasingCurve(QEasingCurve::OutBack);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                
                QPropertyAnimation *angleAnimation = new QPropertyAnimation(slice, "angleSpan");
                angleAnimation->setDuration(500);
                angleAnimation->setStartValue(0);
                angleAnimation->setEndValue(slice->angleSpan());
                angleAnimation->setEasingCurve(QEasingCurve::OutBack);
                angleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }
            
            chart->addSeries(series);
            chart->setTitle("Employee Distribution by Role");
            
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *barSet = new QBarSet("Employees");
            
            QStringList categories;
            for (auto it = data.begin(); it != data.end(); ++it) {
                *barSet << it.value();
                categories << it.key();
            }
            
            series->append(barSet);
            chart->addSeries(series);
            
            // Set up the category axis
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            
            // Set up the value axis
            QValueAxis *axisY = new QValueAxis();
            int maxValue = 0;
            for (auto it = data.begin(); it != data.end(); ++it) {
                maxValue = qMax(maxValue, it.value());
            }
            axisY->setRange(0, maxValue * 1.1);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            
            // Connect hover signals for interactive feedback
            connect(series, &QBarSeries::hovered, [this, categories](bool status, int index, QBarSet *barset) {
                if (status && index >= 0 && index < categories.size()) {
                    ui->employeeHoverDescriptionLabel->setText(
                        QString("%1: %2 employees").arg(categories.at(index)).arg(barset->at(index))
                    );
                } else {
                    ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
            
            chart->setTitle("Employee Count by Role");
        }
    } else if (filter == "Hire Date") {
        // Create a query for hire date distribution by year
        QSqlQuery query("SELECT TO_CHAR(DATE_HIRING, 'YYYY') as year, COUNT(*) as count FROM EMPLOYEE GROUP BY TO_CHAR(DATE_HIRING, 'YYYY') ORDER BY year");
        QMap<QString, int> data;
        
        while (query.next()) {
            QString year = query.value("year").toString();
            int count = query.value("count").toInt();
            data[year] = count;
        }
        
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            
            // Add slices to the pie series
            int totalCount = 0;
            for (auto it = data.begin(); it != data.end(); ++it) {
                totalCount += it.value();
            }
            
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                float percentage = (float)it.value() / totalCount * 100.0;
                slice->setLabel(QString("%1: %2 (%3%)").arg(it.key()).arg(it.value()).arg(percentage, 0, 'f', 1));
                slice->setLabelVisible(true);
                
                // Connect hover signal for interactive feedback
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->employeeHoverDescriptionLabel->setText(slice->label());
                    } else {
                        slice->setExploded(false);
                        ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                    }
                });
                
                // Animate each slice appearing
                QPropertyAnimation *animation = new QPropertyAnimation(slice, "startAngle");
                animation->setDuration(500);
                animation->setStartValue(0);
                animation->setEndValue(slice->startAngle());
                animation->setEasingCurve(QEasingCurve::OutBack);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                
                QPropertyAnimation *angleAnimation = new QPropertyAnimation(slice, "angleSpan");
                angleAnimation->setDuration(500);
                angleAnimation->setStartValue(0);
                angleAnimation->setEndValue(slice->angleSpan());
                angleAnimation->setEasingCurve(QEasingCurve::OutBack);
                angleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }
            
            chart->addSeries(series);
            chart->setTitle("Employee Distribution by Hire Year");
            
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *barSet = new QBarSet("Employees");
            
            QStringList categories;
            for (auto it = data.begin(); it != data.end(); ++it) {
                *barSet << it.value();
                categories << it.key();
            }
            
            series->append(barSet);
            chart->addSeries(series);
            
            // Set up the category axis
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            
            // Set up the value axis
            QValueAxis *axisY = new QValueAxis();
            int maxValue = 0;
            for (auto it = data.begin(); it != data.end(); ++it) {
                maxValue = qMax(maxValue, it.value());
            }
            axisY->setRange(0, maxValue * 1.1);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            
            // Connect hover signals for interactive feedback
            connect(series, &QBarSeries::hovered, [this, categories](bool status, int index, QBarSet *barset) {
                if (status && index >= 0 && index < categories.size()) {
                    ui->employeeHoverDescriptionLabel->setText(
                        QString("%1: %2 employees").arg(categories.at(index)).arg(barset->at(index))
                    );
                } else {
                    ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
            
            chart->setTitle("Employee Count by Hire Year");
        }
    }
    
    // Set the chart in the chart view
    ui->employeeChartView->setChart(chart);
    ui->employeeChartView->setRenderHint(QPainter::Antialiasing);
    
    // Update summary statistics
    QSqlQuery countQuery("SELECT COUNT(*) as total FROM EMPLOYEE");
    if (countQuery.next()) {
        int totalEmployees = countQuery.value("total").toInt();
        ui->employeeTotalCountLabel->setText(QString("Total Employees: %1").arg(totalEmployees));
    }
    
    QSqlQuery salaryQuery("SELECT AVG(SALARY) as avg_salary FROM EMPLOYEE");
    if (salaryQuery.next()) {
        double avgSalary = salaryQuery.value("avg_salary").toDouble();
        ui->employeeAverageSalaryLabel->setText(QString("Average Salary: $%1").arg(avgSalary, 0, 'f', 2));
    }
    
    qDebug() << "Statistics updated with animations";
}
void MainWindow::on_addButtonclicked()
{
    qDebug() << "on_addButton_clicked called";
    QString cin = ui->lineEdit_CIN->text().trimmed();
    QString lastName = ui->lineEdit_Nom->text().trimmed();
    QString firstName = ui->lineEdit_Prenom->text().trimmed();
    QDate dateOfBirth = ui->dateEdit_birth->date();
    QString phoneNumber = ui->lineEdit_phone->text().trimmed();
    QString email = ui->lineEdit_email->text().trimmed();
    QString gender;
    if (ui->radioButton_H->isChecked()) {
        gender = "Homme";
    } else if (ui->radioButton_F->isChecked()) {
        gender = "Femme";
    }
    QString salaryText = ui->lineEdit_salaire->text().trimmed();
    bool salaryOk;
    int salary = salaryText.toInt(&salaryOk);
    QDate dateOfHire = ui->dateEdit_hiring->date();
    QString field = ui->comboBox->currentText().trimmed();
    QString role = ui->roleComboBox->currentText().trimmed();
    QString imagePath = ui->imagePathLineEdit->text().trimmed();
    
    // Vérifier si tous les champs sont vides
    if (cin.isEmpty() && lastName.isEmpty() && firstName.isEmpty() && phoneNumber.isEmpty() && 
        email.isEmpty() && gender.isEmpty() && salaryText.isEmpty() && field.isEmpty() && 
        role.isEmpty() && imagePath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont vides. Veuillez remplir au moins un champ.");
        return;
    }

    // Current date for validation
    QDate currentDate = QDate::currentDate(); // March 11, 2025

    // Validation checks
    QStringList errors;

    // CIN: Must be an 8-digit number
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (cin.isEmpty()) {
        errors << "CIN est obligatoire.";
    } else if (!cinRegex.match(cin).hasMatch()) {
        errors << "CIN doit être un nombre à 8 chiffres.";
    }

    // Last Name: Must be letters only
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");
    if (lastName.isEmpty()) {
        errors << "Le nom est obligatoire.";
    } else if (!nameRegex.match(lastName).hasMatch()) {
        errors << "Le nom ne doit contenir que des lettres, des espaces ou des traits d'union.";
    }

    // First Name: Must be letters only
    if (firstName.isEmpty()) {
        errors << "Le prénom est obligatoire.";
    } else if (!nameRegex.match(firstName).hasMatch()) {
        errors << "Le prénom ne doit contenir que des lettres, des espaces ou des traits d'union.";
    }

    // Date of Birth: Must not be in the future, default to 18 years ago if invalid
    if (dateOfBirth > currentDate) {
        errors << "La date de naissance ne peut pas être dans le futur.";
    } else if (dateOfBirth.isNull() || !dateOfBirth.isValid()) {
        dateOfBirth = currentDate.addYears(-18); // Default to 18 years ago
        qDebug() << "Date de naissance invalide, par défaut :" << dateOfBirth.toString("yyyy-MM-dd");
    }

    // Phone Number: Must be an 8-digit number
    QRegularExpression phoneRegex("^[0-9]{8}$");
    if (phoneNumber.isEmpty()) {
        errors << "Le numéro de téléphone est obligatoire.";
    } else if (!phoneRegex.match(phoneNumber).hasMatch()) {
        errors << "Le numéro de téléphone doit être un nombre à 8 chiffres.";
    }

    // Email: Must match email format
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (email.isEmpty()) {
        errors << "L'email est obligatoire.";
    } else if (!emailRegex.match(email).hasMatch()) {
        errors << "L'email doit être une adresse email valide.";
    }

    // Gender: Must be selected
    if (gender.isEmpty()) {
        errors << "Veuillez sélectionner un sexe.";
    }

    // Salary: Must be a valid number >= 1000
    if (salaryText.isEmpty()) {
        errors << "Le salaire est obligatoire.";
    } else if (!salaryOk || salary < 1000) {
        errors << "Le salaire doit être un nombre supérieur ou égal à 1000.";
    }

    // Date of Hire: Default to current date if invalid, must be after birth + 18 and not future
    if (dateOfHire.isNull() || !dateOfHire.isValid()) {
        dateOfHire = currentDate;
        qDebug() << "Date d'embauche invalide, par défaut :" << dateOfHire.toString("yyyy-MM-dd");
    }
    QDate minHireDate = dateOfBirth.addYears(18);
    if (dateOfHire < minHireDate) {
        errors << "La date d'embauche doit être au moins 18 ans après la date de naissance.";
    } else if (dateOfHire > currentDate) {
        errors << "La date d'embauche ne peut pas être dans le futur.";
    }

    // Specialty: Must be selected
    if (field.isEmpty()) {
        errors << "La spécialité est obligatoire.";
    }

    // Role: Must be selected
    if (role.isEmpty()) {
        errors << "Le rôle est obligatoire.";
    }

    // Image Path: Optional, but must exist if provided
    if (!imagePath.isEmpty()) {
        QFile file(imagePath);
        if (!file.exists()) {
            errors << "Le chemin d'image spécifié n'existe pas.";
        }
    }

    // Check for duplicate CIN
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            errors << "Un employé avec ce CIN existe déjà.";
        }
    } else {
        qDebug() << "Erreur lors de la vérification de l'unicité du CIN :" << checkQuery.lastError().text();
        errors << "Échec de la vérification de l'unicité du CIN.";
    }

    // If there are errors, display them and return
    if (!errors.isEmpty()) {
        QString errorMessage = errors.join("\n");
        QMessageBox::warning(this, "Erreur de saisie", errorMessage);
        return;
    }

    Employee e(cin, lastName, firstName, dateOfBirth, phoneNumber, email, gender, salary, dateOfHire, field, imagePath, role);
    if (e.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        loadEmployees();
        ui->lineEdit_CIN->clear();
        ui->lineEdit_Nom->clear();
        ui->lineEdit_Prenom->clear();
        ui->lineEdit_phone->clear();
        ui->lineEdit_email->clear();
        ui->radioButton_H->setChecked(false);
        ui->radioButton_F->setChecked(false);
        ui->lineEdit_salaire->clear();
        ui->comboBox->setCurrentIndex(0);
        ui->imagePathLineEdit->clear();
        ui->roleComboBox->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'employé.");
    }
}

void MainWindow::on_modifyBtnClicked()
{
    qDebug() << "on_modifyBtn_clicked called";
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "No employee selected!");
        return;
    }

    int row = selected.at(0).row();
    int id = employeeModel->data(employeeModel->index(row, 0)).toInt();

    UpdateEmployeeDialog dialog(id, this);
    if (dialog.exec() == QDialog::Accepted) {
        loadEmployees();
        if (employeeModel->rowCount() > 0) {
            ui->tableView->selectRow(0);
        }
    }
}

void MainWindow::on_saveUpdateBtnClicked()
{
    qDebug() << "on_saveUpdateBtn_clicked called";
    QMessageBox::information(this, "Info", "Please use the 'Update' button to modify an employee.");
}

void MainWindow::on_deleteBtnClicked()
{
    qDebug() << "on_deleteBtn_clicked called";
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "No employee selected!");
        return;
    }

    int row = selected.at(0).row();
    int id = employeeModel->data(employeeModel->index(row, 0)).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete", "Are you sure you want to delete this employee?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (reply == QMessageBox::Yes) {
        if (employee->deleteEmployee(id)) {
            QMessageBox::information(this, "Success", "Employee deleted successfully!");
            loadEmployees();
            int newRow = row < employeeModel->rowCount() ? row : employeeModel->rowCount() - 1;
            if (newRow >= 0) {
                ui->tableView->selectRow(newRow);
            }
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete employee!");
        }
    }
}

void MainWindow::on_resetSearchButtonclicked()
{
    qDebug() << "on_resetSearchButton_clicked called";
    ui->searchInput->clear();
    ui->searchCriteriaComboBox->setCurrentIndex(0);
    loadEmployees();
    if (employeeModel->rowCount() > 0) {
        ui->tableView->selectRow(0);
    }
}


void MainWindow::loadEmployees()
{
    QSqlQuery query;
    if (query.exec("SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE, RFID_UID FROM EMPLOYEE")) {
        employeeModel->setQuery(std::move(query)); // Use move semantics
        
        // Définir les titres des colonnes
        employeeModel->setHeaderData(0, Qt::Horizontal, "ID");
        employeeModel->setHeaderData(1, Qt::Horizontal, "CIN");
        employeeModel->setHeaderData(2, Qt::Horizontal, "Nom");
        employeeModel->setHeaderData(3, Qt::Horizontal, "Prénom");
        employeeModel->setHeaderData(4, Qt::Horizontal, "Date de naissance");
        employeeModel->setHeaderData(5, Qt::Horizontal, "Téléphone");
        employeeModel->setHeaderData(6, Qt::Horizontal, "Email");
        employeeModel->setHeaderData(7, Qt::Horizontal, "Sexe");
        employeeModel->setHeaderData(8, Qt::Horizontal, "Salaire");
        employeeModel->setHeaderData(9, Qt::Horizontal, "Date d'embauche");
        employeeModel->setHeaderData(10, Qt::Horizontal, "Spécialité");
        employeeModel->setHeaderData(12, Qt::Horizontal, "Rôle");
        employeeModel->setHeaderData(13, Qt::Horizontal, "RFID_UID");
        
        ui->tableView->setModel(employeeModel);
        
        // Masquer la colonne de l'image
        ui->tableView->hideColumn(11);
        
        // Enable horizontal scroll bar
        ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        // Setup columns to better fit their content
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        
        // Make the last column stretch to fill available space
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        
        // Sélectionner la première ligne si disponible
        if (employeeModel->rowCount() > 0) {
            ui->tableView->selectRow(0);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de charger les employés !");
        qDebug() << "Erreur de chargement :" << query.lastError().text();
    }
}

void MainWindow::searchEmployees()
{
    qDebug() << "searchEmployees called with input:" << ui->searchInput->text();

    QString criteria = ui->searchCriteriaComboBox->currentText().trimmed();
    QString input = ui->searchInput->text().trimmed();

    QString queryStr = "SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY,ROLE,RFID_UID FROM EMPLOYEE";
    bool hasFilter = !input.isEmpty();

    if (hasFilter) {
        if (criteria == "Full name") {
            // Use CONCAT for SQLite (|| might not work in all databases)
            queryStr += " WHERE UPPER(TRIM(LAST_NAME) || ' ' || TRIM(FIRST_NAME)) LIKE UPPER(:input)";
        } else if (criteria == "CIN") {
            queryStr += " WHERE UPPER(CIN) LIKE UPPER(:input)";
        } else if (criteria == "Specialty") {
            queryStr += " WHERE UPPER(SPECIALITY) LIKE UPPER(:input)";
        } else {
            qDebug() << "Unknown criteria:" << criteria;
            return; // Exit if criteria is invalid
        }
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (hasFilter) {
        query.bindValue(":input", "%" + input + "%");
    }

    if (query.exec()) {
        employeeModel->setQuery(std::move(query)); // Use move semantics
        ui->tableView->setModel(employeeModel);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        if (employeeModel->rowCount() > 0) {
            ui->tableView->selectRow(0); // Select the first row if results exist
        } else {
            qDebug() << "No results found for input:" << input;
        }
    } else {
        qDebug() << "Search error:" << query.lastError().text();
    }
}

void MainWindow::handleHeaderClicked(int column)
{
    Qt::SortOrder order = sortOrders.value(column, Qt::AscendingOrder);
    if (order == Qt::AscendingOrder) {
        sortOrders[column] = Qt::DescendingOrder;
    } else {
        sortOrders[column] = Qt::AscendingOrder;
    }
    sortEmployees(column);
}

void MainWindow::sortEmployees(int column)
{
    qDebug() << "sortEmployees appelé pour la colonne:" << column;

    // Correspondance entre l'index de colonne et le nom de colonne dans la base de données
    QString columnName;
    switch (column) {
    case 0: columnName = "ID"; break;
    case 1: columnName = "CIN"; break;
    case 2: columnName = "LAST_NAME"; break;
    case 3: columnName = "FIRST_NAME"; break;
    case 4: columnName = "DATE_BIRTH"; break;
    case 5: columnName = "PHONE"; break;
    case 6: columnName = "EMAIL"; break;
    case 7: columnName = "GENDER"; break;
    case 8: columnName = "SALARY"; break;
    case 9: columnName = "DATE_HIRING"; break;
    case 10: columnName = "SPECIALITY"; break;
    case 12: columnName = "ROLE"; break;      // Ajout du tri sur la colonne 'ROLE'
    case 13: columnName = "RFID_UID"; break;  // Ajout du tri sur la colonne 'RFID_UID'
    default: columnName = "ID"; break;        // Par défaut, tri sur l'ID
    }

    Qt::SortOrder order = sortOrders.value(column, Qt::AscendingOrder);

    // Récupération des critères de recherche actuels et de l'entrée
    QString criteria = ui->searchCriteriaComboBox->currentText();
    QString input = ui->searchInput->text();
    QString queryStr = "SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY, IMAGE, ROLE, RFID_UID FROM EMPLOYEE";

    // Application du filtre de recherche si présent
    if (!input.isEmpty()) {
        if (criteria == "Full Name" || criteria == "Nom complet") {
            queryStr += " WHERE UPPER(TRIM(LAST_NAME) || ' ' || TRIM(FIRST_NAME)) LIKE UPPER(:input)";
        } else if (criteria == "CIN") {
            queryStr += " WHERE CIN LIKE :input";
        } else if (criteria == "Specialty" || criteria == "Spécialité") {
            queryStr += " WHERE UPPER(SPECIALITY) LIKE UPPER(:input)";
        } else if (criteria == "Role" || criteria == "Rôle") {
            queryStr += " WHERE UPPER(ROLE) LIKE UPPER(:input)";
        }
    }

    // Ajout de la clause ORDER BY
    // Gestion spéciale pour les colonnes de date
    if (column == 4 || column == 9) { // DATE_BIRTH ou DATE_HIRING
        queryStr += " ORDER BY TO_CHAR(" + columnName + ", 'YYYY-MM-DD') " + (order == Qt::AscendingOrder ? "ASC" : "DESC");
    } 
    // Gestion spéciale pour le salaire (tri numérique)
    else if (column == 8) { // SALARY
        queryStr += " ORDER BY TO_NUMBER(SALARY) " + (order == Qt::AscendingOrder ? "ASC" : "DESC");
    }
    // Tri par défaut (alphabétique)
    else {
        queryStr += " ORDER BY " + columnName + " " + (order == Qt::AscendingOrder ? "ASC" : "DESC");
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!input.isEmpty()) {
        query.bindValue(":input", "%" + input + "%");
    }

    qDebug() << "Exécution de la requête:" << query.lastQuery();
    if (query.exec()) {
        employeeModel->setQuery(std::move(query));
        ui->tableView->setModel(employeeModel);
        
        // Définir à nouveau les en-têtes des colonnes après la mise à jour du modèle
        employeeModel->setHeaderData(0, Qt::Horizontal, "ID");
        employeeModel->setHeaderData(1, Qt::Horizontal, "CIN");
        employeeModel->setHeaderData(2, Qt::Horizontal, "Nom");
        employeeModel->setHeaderData(3, Qt::Horizontal, "Prénom");
        employeeModel->setHeaderData(4, Qt::Horizontal, "Date de naissance");
        employeeModel->setHeaderData(5, Qt::Horizontal, "Téléphone");
        employeeModel->setHeaderData(6, Qt::Horizontal, "Email");
        employeeModel->setHeaderData(7, Qt::Horizontal, "Sexe");
        employeeModel->setHeaderData(8, Qt::Horizontal, "Salaire");
        employeeModel->setHeaderData(9, Qt::Horizontal, "Date d'embauche");
        employeeModel->setHeaderData(10, Qt::Horizontal, "Spécialité");
        employeeModel->setHeaderData(12, Qt::Horizontal, "Rôle");
        employeeModel->setHeaderData(13, Qt::Horizontal, "RFID_UID");
        
        // Masquer la colonne de l'image
        ui->tableView->hideColumn(11);
        
        // Ajuster la taille des colonnes
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        
        if (employeeModel->rowCount() > 0) {
            ui->tableView->selectRow(0);
        }
    } else {
        qDebug() << "Erreur de tri:" << query.lastError().text();
    }
}
void MainWindow::on_downloadBtnClicked()
{
    qDebug() << "on_downloadBtn_clicked called";
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "No employee selected!");
        return;
    }

    int row = selected.at(0).row();
    QSqlRecord record = employeeModel->record(row);

    // Get employee info
    QString id = record.value("ID").toString();
    QString cin = record.value("CIN").toString();
    QString lastName = record.value("LAST_NAME").toString();
    QString firstName = record.value("FIRST_NAME").toString();
    QDate dateOfBirth = record.value("DATE_BIRTH").toDate();
    QString phone = record.value("PHONE").toString();
    QString email = record.value("EMAIL").toString();
    QString gender = record.value("GENDER").toString();
    QString salary = record.value("SALARY").toString();
    QDate dateOfHire = record.value("DATE_HIRING").toDate();
    QString specialty = record.value("SPECIALITY").toString();
    QString role = record.value("ROLE").toString();
    QByteArray imageData = record.value("IMAGE").toByteArray();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Employee PDF"),
                                                    QString("%1_%2.pdf").arg(lastName, firstName),
                                                    tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty())
        return;

    QTextDocument doc;
    QString imageHtml;
    if (!imageData.isEmpty()) {
        QString base64 = QString::fromLatin1(imageData.toBase64());
        imageHtml = QString(R"(
            <div style="text-align: center; margin-bottom: 20px;">
                <img src='data:image/png;base64,%1' width='120' height='160' style='border:2px solid #4a90e2; border-radius: 5px;'/>
            </div>
        )").arg(base64);
    } else {
        imageHtml = "<p><em>No Image Available</em></p>";
    }

    QString html = R"(
        <html>
        <head>
            <style>
                body {
                    font-family: 'Segoe UI', sans-serif;
                    background-color: #f7f7f7;
                    padding: 20px;
                    color: #333;
                }
                .cv-container {
                    background-color: #ffffff;
                    padding: 20px;
                    border-radius: 10px;
                    box-shadow: 0 0 10px rgba(0,0,0,0.1);
                    width: 100%;
                }
                .section {
                    margin-top: 20px;
                }
                .section h2 {
                    color: #2c5282;
                    border-bottom: 1px solid #ccc;
                    padding-bottom: 5px;
                }
                .info-item {
                    margin: 6px 0;
                }
                .info-item strong {
                    display: inline-block;
                    width: 140px;
                }
            </style>
        </head>
        <body>
            <div class='cv-container'>
                %1
                <div class='section'>
                    <h2>Personal Information</h2>
                    <div class='info-item'><strong>ID:</strong> %2</div>
                    <div class='info-item'><strong>CIN:</strong> %3</div>
                    <div class='info-item'><strong>Last Name:</strong> %4</div>
                    <div class='info-item'><strong>First Name:</strong> %5</div>
                    <div class='info-item'><strong>Date of Birth:</strong> %6</div>
                    <div class='info-item'><strong>Phone:</strong> %7</div>
                    <div class='info-item'><strong>Email:</strong> %8</div>
                    <div class='info-item'><strong>Gender:</strong> %9</div>
                </div>
                <div class='section'>
                    <h2>Professional Information</h2>
                    <div class='info-item'><strong>Salary:</strong> %10</div>
                    <div class='info-item'><strong>Date of Hire:</strong> %11</div>
                    <div class='info-item'><strong>Specialty:</strong> %12</div>
                    <div class='info-item'><strong>Role:</strong> %13</div>
                </div>
            </div>
        </body>
        </html>
    )";

    html = html.arg(imageHtml, id, cin, lastName, firstName,
                    dateOfBirth.toString("dd/MM/yyyy"),
                    phone, email, gender, salary,
                    dateOfHire.toString("dd/MM/yyyy"), specialty, role);

    doc.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    doc.print(&printer);
    QMessageBox::information(this, "Success", "Employee information exported as PDF!");
}
void MainWindow::on_logoutButtonclicked()
{
    qDebug() << "Logout button clicked";
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Logout", "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No
        );
    if (reply == QMessageBox::Yes) {
        // Reset the global stylesheet to let sign.ui use its own style
        qApp->setStyleSheet(""); // Clear the application-wide stylesheet
        sign *loginWindow = new sign();
        loginWindow->show();
        this->close(); // Close the MainWindow
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
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #F0F4F8, stop:1 #D9E2EC);
            color: #2D3748;
            font-family: 'Roboto', 'Segoe UI', sans-serif;
        }
        QPushButton {
            background-color: #38B2AC;
            color: #FFFFFF;
            border: none;
            border-radius: 8px;
            padding: 8px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #4FD1C5;
        }
        QPushButton:pressed {
            background-color: #319795;
        }
        QLineEdit, QComboBox, QDateTimeEdit {
            background-color: #FFFFFF;
            border: 1px solid #CBD5E0;
            border-radius: 6px;
            padding: 6px;
            color: #2D3748;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus {
            border: 2px solid #F56565;
            background-color: #F7FAFC;
        }
        QTableView {
            background-color: #FFFFFF;
            border: 1px solid #E2E8F0;
            border-radius: 6px;
            selection-background-color: #B2F5EA;
            selection-color: #2D3748;
        }
        QHeaderView::section {
            background-color: #4A5568;
            color: #FFFFFF;
            padding: 6px;
            border: none;
            border-radius: 4px;
        }
        QCalendarWidget {
            background-color: #FFFFFF;
            border: 1px solid #CBD5E0;
            border-radius: 6px;
        }
        QCalendarWidget QToolButton {
            background-color: #38B2AC;
            color: #FFFFFF;
            border-radius: 4px;
        }
        QToolTip {
            color: #2D3748;
            background-color: #EDF2F7;
            border: 1px solid #CBD5E0;
            border-radius: 4px;
        }
        QTabWidget::pane {
            border: 1px solid #CBD5E0;
            border-radius: 6px;
            background-color: #FFFFFF;
        }
        QTabBar::tab {
            background-color: #E2E8F0;
            color: #4A5568;
            padding: 8px 16px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
        }
        QTabBar::tab:selected {
            background-color: #F56565;
            color: #FFFFFF;
        }
        QLabel {
            font-size: 11pt;
            padding: 2px;
            color: #2D3748;
        }
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #38B2AC;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        #label {
            font-size: 20pt;
            font-weight: bold;
            color: #F56565;
            qproperty-alignment: AlignCenter;
        }
        QFrame#header {
            border: 1px solid #CBD5E0;
            border-radius: 8px;
            background-color: #F7FAFC;
        }
        QFrame#sideMenu {
            border: 1px solid #CBD5E0;
            border-radius: 8px;
            background-color: #EDF2F7;
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #E2E8F0;
            border-radius: 8px;
            background-color: #FFFFFF;
        }
    )";
    qApp->setStyleSheet(styleSheet);
}
QString MainWindow::generateQRCodeForEmployee(int id)
{
    // Encode the employee ID as a string
    return QString::number(id);
}

QImage MainWindow::generateQRCodeImage(const QString &text, int scale)
{
    using namespace qrcodegen;

    // Generate the QR Code
    QrCode qr = QrCode::encodeText(text.toStdString().c_str(), QrCode::Ecc::MEDIUM);

    // Get the size of the QR code (it's a square)
    int size = qr.getSize();
    QImage image(size * scale, size * scale, QImage::Format_RGB32);
    image.fill(Qt::white); // Background color

    QPainter painter(&image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black); // QR code color

    // Draw the QR code
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qr.getModule(x, y)) {
                painter.drawRect(x * scale, y * scale, scale, scale);
            }
        }
    }

    painter.end();
    return image;
}

void MainWindow::showQRCodeDialog(const QImage &qrImage, int id)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Employee QR Code");
    dialog->setFixedSize(400, 450);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Création d'un label pour afficher les informations de l'employé
    QSqlQuery query;
    query.prepare("SELECT CIN, LAST_NAME, FIRST_NAME FROM EMPLOYEE WHERE ID = :id");
    query.bindValue(":id", id);
    
    QString employeeInfo = QString("Employee ID: %1").arg(id);
    
    if (query.exec() && query.next()) {
        QString cin = query.value("CIN").toString();
        QString lastName = query.value("LAST_NAME").toString();
        QString firstName = query.value("FIRST_NAME").toString();
        
        employeeInfo += QString("\nCIN: %1\nName: %2 %3")
                        .arg(cin, lastName, firstName);
    }
    
    QLabel *infoLabel = new QLabel(employeeInfo, dialog);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin-bottom: 10px;");
    layout->addWidget(infoLabel);
    
    // Affichage du code QR
    QLabel *qrLabel = new QLabel(dialog);
    qrLabel->setAlignment(Qt::AlignCenter);
    QPixmap pixmap = QPixmap::fromImage(qrImage.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    qrLabel->setPixmap(pixmap);
    layout->addWidget(qrLabel);
    
    // Message d'instruction
    QLabel *instructionLabel = new QLabel("Scannez ce code QR pour identifier l'employé", dialog);
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet("font-style: italic; color: gray;");
    layout->addWidget(instructionLabel);
    
    // Bouton pour fermer le dialogue
    QPushButton *closeButton = new QPushButton("Fermer", dialog);
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);
    
    // Option facultative pour sauvegarder
    QPushButton *saveButton = new QPushButton("Enregistrer le code QR...", dialog);
    connect(saveButton, &QPushButton::clicked, this, [this, qrImage, id, dialog]() {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save QR Code"),
                                                        QString("employee_qr_%1.png").arg(id),
                                                        tr("PNG Files (*.png)"));
        if (!fileName.isEmpty()) {
            qrImage.save(fileName);
            QMessageBox::information(this, "Succès", "Code QR enregistré avec succès !");
        }
    });
    layout->addWidget(saveButton);
    
    dialog->setLayout(layout);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_generateQRCodeBtnClicked()
{
    qDebug() << "on_generateQRCodeBtnClicked called";
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Error", "No employee selected!");
        return;
    }

    int row = selected.at(0).row();
    int employeeId = employeeModel->data(employeeModel->index(row, 0)).toInt();
    QString qrData = generateQRCodeForEmployee(employeeId);
    QImage qrImage = generateQRCodeImage(qrData);
    showQRCodeDialog(qrImage, employeeId);
}
void MainWindow::applyDarkTheme() {
    QString styleSheet = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #1A202C, stop:1 #2D3748);
            color: #E2E8F0;
            font-family: 'Roboto', 'Segoe UI', sans-serif;
        }
        QPushButton {
            background-color: #00B5D8;
            color: #FFFFFF;
            border: none;
            border-radius: 8px;
            padding: 8px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #00D4FF;
        }
        QPushButton:pressed {
            background-color: #0097B2;
        }
        QLineEdit, QComboBox, QDateTimeEdit {
            background-color: #2D3748;
            border: 1px solid #4A5568;
            border-radius: 6px;
            padding: 6px;
            color: #E2E8F0;
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus {
            border: 2px solid #ED64A6;
            background-color: #3C4B64;
        }
        QTableView {
            background-color: #2D3748;
            border: 1px solid #4A5568;
            border-radius: 6px;
            selection-background-color: #00B5D8;
            selection-color: #FFFFFF;
        }
        QHeaderView::section {
            background-color: #4A5568;
            color: #E2E8F0;
            padding: 6px;
            border: none;
            border-radius: 4px;
        }
        QCalendarWidget {
            background-color: #2D3748;
            border: 1px solid #4A5568;
            border-radius: 6px;
        }
        QCalendarWidget QToolButton {
            background-color: #00B5D8;
            color: #FFFFFF;
            border-radius: 4px;
        }
        QToolTip {
            color: #E2E8F0;
            background-color: #4A5568;
            border: 1px solid #00B5D8;
            border-radius: 4px;
        }
        QTabWidget::pane {
            border: 1px solid #4A5568;
            border-radius: 6px;
            background-color: #2D3748;
        }
        QTabBar::tab {
            background-color: #4A5568;
            color: #A0AEC0;
            padding: 8px 16px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
        }
        QTabBar::tab:selected {
            background-color: #ED64A6;
            color: #FFFFFF;
        }
        QLabel {
            font-size: 11pt;
            padding: 2px;
            color: #E2E8F0;
        }
        QLabel[formLabel="true"], #label_name, #label_sector, #label_contact, #label_email, #label_date, #label_consultant {
            font-size: 12pt;
            font-weight: bold;
            color: #00B5D8;
            padding: 2px;
            qproperty-alignment: AlignRight;
        }
        #label {
            font-size: 20pt;
            font-weight: bold;
            color: #ED64A6;
            qproperty-alignment: AlignCenter;
        }
        QFrame#header {
            border: 1px solid #4A5568;
            border-radius: 8px;
            background-color: #2D3748;
        }
        QFrame#sideMenu {
            border: 1px solid #4A5568;
            border-radius: 8px;
            background-color: #3C4B64;
        }
        QFrame#frame_2, QFrame#frame_4 {
            border: 1px solid #4A5568;
            border-radius: 8px;
            background-color: #2D3748;
        }
    )";
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::toggleSidebar() {
    bool isVisible = ui->sideMenu->isVisible();
    QPropertyAnimation *slideAnim = new QPropertyAnimation(ui->sideMenu, "geometry");
    slideAnim->setDuration(300);
    QRect startRect = ui->sideMenu->geometry();
    QRect endRect;

    if (isVisible) {
        endRect = QRect(-ui->sideMenu->width(), startRect.y(), startRect.width(), startRect.height());
    } else {
        endRect = QRect(0, startRect.y(), startRect.width(), startRect.height());
        ui->sideMenu->show();
    }

    slideAnim->setStartValue(startRect);
    slideAnim->setEndValue(endRect);
    slideAnim->setEasingCurve(QEasingCurve::InOutQuad);

    // Pulse animation after sliding in
    if (!isVisible) {
        QPropertyAnimation *pulseAnim = new QPropertyAnimation(ui->sideMenu, "geometry");
        pulseAnim->setDuration(200);
        pulseAnim->setStartValue(endRect);
        pulseAnim->setEndValue(endRect.adjusted(-5, -5, 5, 5));
        pulseAnim->setEasingCurve(QEasingCurve::OutBounce);

        QPropertyAnimation *pulseReverseAnim = new QPropertyAnimation(ui->sideMenu, "geometry");
        pulseReverseAnim->setDuration(200);
        pulseReverseAnim->setStartValue(endRect.adjusted(-5, -5, 5, 5));
        pulseReverseAnim->setEndValue(endRect);
        pulseReverseAnim->setEasingCurve(QEasingCurve::InQuad);

        QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
        group->addAnimation(slideAnim);
        group->addAnimation(pulseAnim);
        group->addAnimation(pulseReverseAnim);
        group->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        connect(slideAnim, &QPropertyAnimation::finished, this, [this]() {
            ui->sideMenu->hide();
        });
        slideAnim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
