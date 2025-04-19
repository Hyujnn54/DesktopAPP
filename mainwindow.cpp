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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , employee(new Employee())
    , employeeModel(new QSqlQueryModel(this))
    , imageDelegate(new ImageDelegate(this))
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

    // Set up new connections
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &MainWindow::on_modifyBtnClicked);
    connect(ui->saveUpdateBtn, &QPushButton::clicked, this, &MainWindow::on_saveUpdateBtnClicked);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteBtnClicked);
    connect(ui->downloadBtn, &QPushButton::clicked, this, &MainWindow::on_downloadBtnClicked);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButtonclicked);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);
    connect(ui->logoutButton, &QPushButton::clicked, this, &::MainWindow::on_logoutButtonclicked);


}
void MainWindow::updateStatistics()
{
    qDebug() << "Starting updateStatistics";

    // Clear existing charts if they exist
    if (pieChartView) {
        qDebug() << "Removing pieChartView";
        if (ui->verticalLayoutStats) {
            ui->verticalLayoutStats->removeWidget(pieChartView);
        }
        delete pieChartView;
        pieChartView = nullptr;
    }
    if (barChartView) {
        qDebug() << "Removing barChartView";
        if (ui->verticalLayoutStats) {
            ui->verticalLayoutStats->removeWidget(barChartView);
        }
        delete barChartView;
        barChartView = nullptr;
    }

    if (!ui->verticalLayoutStats) {
        qDebug() << "verticalLayoutStats is null!";
        return;
    }

    // Pie Chart: Distribution of Employees by Role
    qDebug() << "Creating pie chart";
    QPieSeries *pieSeries = new QPieSeries();
    QSqlQuery roleQuery("SELECT ROLE, COUNT(*) as count FROM EMPLOYEE GROUP BY ROLE");
    int totalRoleCount = 0;
    QList<QPair<QString, int>> roleData;

    // Collect role data and calculate total count
    while (roleQuery.next()) {
        QString role = roleQuery.value("ROLE").toString();
        int count = roleQuery.value("count").toInt();
        if (!role.isEmpty()) {
            roleData.append(qMakePair(role, count));
            totalRoleCount += count;
            qDebug() << "Role:" << role << "Count:" << count;
        }
    }
    if (totalRoleCount == 0) {
        pieSeries->append("No Data", 1);
        qDebug() << "No role data, using placeholder";
    } else {
        // Add slices with percentage labels
        for (const auto &data : roleData) {
            QString role = data.first;
            int count = data.second;
            qreal percentage = (count * 100.0) / totalRoleCount;
            QString label = QString("%1 (%2%)").arg(role).arg(percentage, 0, 'f', 1);
            pieSeries->append(label, count);
            qDebug() << "Role:" << role << "Count:" << count << "Percentage:" << percentage;
        }
    }
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Employee Distribution by Role");
    pieChart->legend()->setAlignment(Qt::AlignRight);
    pieChartView = new QChartView(pieChart);
    qDebug() << "Pie chart view created";
    pieChartView->setRenderHint(QPainter::Antialiasing);

    // Bar Chart: Distribution of Employees by Specialty
    qDebug() << "Creating bar chart";
    QBarSeries *barSeries = new QBarSeries();
    QBarSet *barSet = new QBarSet("Employees");
    QSqlQuery specialtyQuery("SELECT SPECIALITY, COUNT(*) as count FROM EMPLOYEE GROUP BY SPECIALITY");
    QStringList categories;
    int totalSpecialtyCount = 0;
    QList<QPair<QString, int>> specialtyData;

    // Collect specialty data and calculate total count
    while (specialtyQuery.next()) {
        QString specialty = specialtyQuery.value("SPECIALITY").toString();
        int count = specialtyQuery.value("count").toInt();
        specialtyData.append(qMakePair(specialty, count));
        totalSpecialtyCount += count;
        qDebug() << "Specialty:" << specialty << "Count:" << count;
    }

    // Add bars with percentage tooltips
    if (totalSpecialtyCount == 0) {
        *barSet << 1;
        categories << "No Data";
        qDebug() << "No specialty data, using placeholder";
    } else {
        for (const auto &data : specialtyData) {
            QString specialty = data.first.isEmpty() ? "Unknown" : data.first;
            int count = data.second;
            qreal percentage = (count * 100.0) / totalSpecialtyCount;
            *barSet << count;
            categories << specialty;
            // Set tooltip with percentage
            barSet->setLabel(QString("%1 (%2%)").arg(specialty).arg(percentage, 0, 'f', 1));
            qDebug() << "Specialty:" << specialty << "Count:" << count << "Percentage:" << percentage;
        }
    }
    barSeries->append(barSet);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Employee Distribution by Specialty");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up the category axis
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsAngle(0);
    axisX->setLabelsVisible(true);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    // Set up the value axis (Y-axis) for the number of employees
    QValueAxis *axisY = new QValueAxis();
    qreal maxValue = 0;
    for (int i = 0; i < barSet->count(); ++i) {
        qreal value = barSet->at(i);
        if (value > maxValue) {
            maxValue = value;
        }
    }
    maxValue = qMax(5.0, maxValue + 1);
    axisY->setRange(0, maxValue);
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // Adjust bar chart layout
    qDebug() << "Adjusting bar chart layout";
    barChart->setTheme(QChart::ChartThemeQt);
    if (barChart->layout()) {
        barChart->layout()->setContentsMargins(0, 0, 0, 0);
    } else {
        qDebug() << "Bar chart layout is null!";
    }
    barChart->legend()->setAlignment(Qt::AlignBottom);
    barChartView = new QChartView(barChart);
    qDebug() << "Bar chart view created";
    barChartView->setMinimumSize(400, 250);
    barChartView->setRenderHint(QPainter::Antialiasing);

    // Verify bar count matches categories
    qDebug() << "Expected bars:" << categories.size() << "Actual bars in barSet:" << barSet->count();

    // Add charts to the layout
    qDebug() << "Adding charts to layout";
    ui->verticalLayoutStats->insertWidget(0, pieChartView);
    ui->verticalLayoutStats->insertWidget(1, barChartView);
    qDebug() << "Charts added to layout";

    qDebug() << "Specialty categories:" << categories;
    qDebug() << "Statistics updated with pie and bar charts";
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

    // Current date for validation
    QDate currentDate = QDate::currentDate(); // March 11, 2025

    // Validation checks
    QStringList errors;

    // CIN: Must be an 8-digit number
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (cin.isEmpty()) {
        errors << "CIN is required.";
    } else if (!cinRegex.match(cin).hasMatch()) {
        errors << "CIN must be an 8-digit number.";
    }

    // Last Name: Must be letters only
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");
    if (lastName.isEmpty()) {
        errors << "Last Name is required.";
    } else if (!nameRegex.match(lastName).hasMatch()) {
        errors << "Last Name must contain only letters, spaces, or hyphens.";
    }

    // First Name: Must be letters only
    if (firstName.isEmpty()) {
        errors << "First Name is required.";
    } else if (!nameRegex.match(firstName).hasMatch()) {
        errors << "First Name must contain only letters, spaces, or hyphens.";
    }

    // Date of Birth: Must not be in the future, default to 18 years ago if invalid
    if (dateOfBirth > currentDate) {
        errors << "Date of Birth cannot be in the future.";
    } else if (dateOfBirth.isNull() || !dateOfBirth.isValid()) {
        dateOfBirth = currentDate.addYears(-18); // Default to 18 years ago (March 11, 2007)
        qDebug() << "Invalid Date of Birth, defaulting to:" << dateOfBirth.toString("yyyy-MM-dd");
    }

    // Phone Number: Must be an 8-digit number
    QRegularExpression phoneRegex("^[0-9]{8}$");
    if (phoneNumber.isEmpty()) {
        errors << "Phone Number is required.";
    } else if (!phoneRegex.match(phoneNumber).hasMatch()) {
        errors << "Phone Number must be an 8-digit number.";
    }

    // Email: Must match email format
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (email.isEmpty()) {
        errors << "Email is required.";
    } else if (!emailRegex.match(email).hasMatch()) {
        errors << "Email must be a valid email address.";
    }

    // Gender: Must be selected
    if (gender.isEmpty()) {
        errors << "Please select a gender.";
    }

    // Salary: Must be a valid number >= 1000
    if (salaryText.isEmpty()) {
        errors << "Salary is required.";
    } else if (!salaryOk || salary < 1000) {
        errors << "Salary must be a number greater than or equal to 1000.";
    }

    // Date of Hire: Default to current date if invalid, must be after birth + 18 and not future
    if (dateOfHire.isNull() || !dateOfHire.isValid()) {
        dateOfHire = currentDate; // Default to March 11, 2025
        qDebug() << "Invalid Date of Hire, defaulting to:" << dateOfHire.toString("yyyy-MM-dd");
    }
    QDate minHireDate = dateOfBirth.addYears(18);
    if (dateOfHire < minHireDate) {
        errors << "Hire date must be at least 18 years after birth date.";
    } else if (dateOfHire > currentDate) {
        errors << "Hire date cannot be in the future.";
    }

    // Specialty: Must be selected
    if (field.isEmpty()) {
        errors << "Specialty is required.";
    }

    // Role: Must be selected
    if (role.isEmpty()) {
        errors << "Role is required.";
    }

    // Image Path: Optional, but must exist if provided
    if (!imagePath.isEmpty()) {
        QFile file(imagePath);
        if (!file.exists()) {
            errors << "The specified image path does not exist.";
        }
    }

    // Check for duplicate CIN
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            errors << "An employee with this CIN already exists.";
        }
    } else {
        qDebug() << "Error checking CIN uniqueness:" << checkQuery.lastError().text();
        errors << "Failed to verify CIN uniqueness.";
    }

    // If there are errors, display them and return
    if (!errors.isEmpty()) {
        QString errorMessage = errors.join("\n");
        QMessageBox::warning(this, "Input Error", errorMessage);
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
    if (query.exec("SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY,IMAGE,ROLE FROM EMPLOYEE")) {
        employeeModel->setQuery(std::move(query)); // Use move semantics
        ui->tableView->setModel(employeeModel);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->hideColumn(11);
    } else {
        QMessageBox::critical(this, "Error", "Failed to load employees!");
        qDebug() << "Load error:" << query.lastError().text();
    }
}

void MainWindow::searchEmployees()
{
    qDebug() << "searchEmployees called with input:" << ui->searchInput->text();

    QString criteria = ui->searchCriteriaComboBox->currentText().trimmed();
    QString input = ui->searchInput->text().trimmed();

    QString queryStr = "SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY FROM EMPLOYEE";
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
    qDebug() << "sortEmployees called for column:" << column;

    // Map column index to column name based on your table structure
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
    default: columnName = "ID"; break; // Default to ID if invalid column
    }

    Qt::SortOrder order = sortOrders.value(column, Qt::AscendingOrder);

    // Get the current search criteria and input
    QString criteria = ui->searchCriteriaComboBox->currentText();
    QString input = ui->searchInput->text();
    QString queryStr = "SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY FROM EMPLOYEE";

    // Apply search filter if present
    if (!input.isEmpty()) {
        if (criteria == "Full Name") {
            queryStr += " WHERE UPPER(LAST_NAME || ' ' || FIRST_NAME) LIKE UPPER(:input)";
        } else if (criteria == "CIN") {
            queryStr += " WHERE CIN LIKE :input";
        } else if (criteria == "Specialty") {
            queryStr += " WHERE SPECIALITY LIKE :input";
        }
    }

    // Add ORDER BY clause
    // Handle date columns with Oracle's TO_CHAR for sorting
    if (column == 4 || column == 9) { // DATE_BIRTH or DATE_HIRING
        queryStr += " ORDER BY TO_CHAR(" + columnName + ", 'MM/DD/YYYY HH24:MI') " + (order == Qt::AscendingOrder ? "ASC" : "DESC");
    } else {
        queryStr += " ORDER BY " + columnName + " " + (order == Qt::AscendingOrder ? "ASC" : "DESC");
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!input.isEmpty()) {
        query.bindValue(":input", "%" + input + "%");
    }

    qDebug() << "Executing query:" << query.lastQuery(); // Log the query for debugging
    if (query.exec()) {
        employeeModel->setQuery(std::move(query)); // Use move semantics
        ui->tableView->setModel(employeeModel);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        if (employeeModel->rowCount() > 0) {
            ui->tableView->selectRow(0);
        }
    } else {
        qDebug() << "Sort error:" << query.lastError().text();
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
