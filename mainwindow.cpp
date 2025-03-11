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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , employee(new Employee())
    , employeeModel(new QSqlQueryModel(this))
    , isDarkTheme(false)
{
    static int instanceCount = 0;
    instanceCount++;
    qDebug() << "MainWindow constructor called, instance count:" << instanceCount;

    ui->setupUi(this);
    applyLightTheme();
    // Set up table view
    ui->tableView->setModel(employeeModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Validators
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

    // Load initial employee data
    loadEmployees();

    // Ensure buttons don't auto-repeat
    ui->deleteBtn->setAutoRepeat(false);
    ui->modifyBtn->setAutoRepeat(false);

    // Initially disable buttons if no selection
    ui->deleteBtn->setEnabled(false);
    ui->modifyBtn->setEnabled(false);

    // Set up connections
    setupConnections();

    // Connect header click to sorting
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::handleHeaderClicked);
    connect(ui->selectImageButton, &QPushButton::clicked, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        if (!fileName.isEmpty()) {
            ui->imagePathLineEdit->setText(fileName);
        }
    });

    // Connect selection changes to enable/disable buttons
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this]() {
                bool hasSelection = !ui->tableView->selectionModel()->selectedRows().isEmpty();
                ui->deleteBtn->setEnabled(hasSelection);
                ui->modifyBtn->setEnabled(hasSelection);
                qDebug() << "Selection changed, hasSelection:" << hasSelection;
            });
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);

}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destructor called, instance count decremented";
    delete ui;
    delete employee;
    delete employeeModel;
}

void MainWindow::setupConnections()
{
    qDebug() << "setupConnections called";
    // Disconnect any existing connections to avoid duplicates
    disconnect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    disconnect(ui->modifyBtn, &QPushButton::clicked, this, &MainWindow::on_modifyBtnClicked);
    disconnect(ui->saveUpdateBtn, &QPushButton::clicked, this, &MainWindow::on_saveUpdateBtnClicked);
    disconnect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteBtnClicked);
    disconnect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButtonclicked);
    disconnect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);

    // Set up new connections
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButtonclicked);
    connect(ui->modifyBtn, &QPushButton::clicked, this, &MainWindow::on_modifyBtnClicked);
    connect(ui->saveUpdateBtn, &QPushButton::clicked, this, &MainWindow::on_saveUpdateBtnClicked);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::on_deleteBtnClicked);
    connect(ui->resetSearchButton, &QPushButton::clicked, this, &MainWindow::on_resetSearchButtonclicked);
    connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::searchEmployees);
}

void MainWindow::on_addButtonclicked()
{
    qDebug() << "on_addButton_clicked called";
    QString cin = ui->lineEdit_CIN->text();
    QString lastName = ui->lineEdit_Nom->text();
    QString firstName = ui->lineEdit_Prenom->text();
    QDate dateOfBirth = ui->dateEdit_birth->date();
    QString phoneNumber = ui->lineEdit_phone->text();
    QString email = ui->lineEdit_email->text();
    QString gender;
    if (ui->radioButton_H->isChecked()) {
        gender = "Homme";
    } else if (ui->radioButton_F->isChecked()) {
        gender = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre.");
        return;
    }
    int salary = ui->lineEdit_salaire->text().toInt();
    QDate dateOfHire = ui->dateEdit_hiring->date();
    QString field = ui->comboBox->currentText();
    QString imagePath = ui->imagePathLineEdit->text();
    QString role = ui->roleComboBox->currentText();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYEE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "Erreur", "Un employé avec ce CIN existe déjà !");
            return;
        }
    } else {
        qDebug() << "Error checking CIN uniqueness:" << checkQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification du CIN.");
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
        ui->lineEdit_salaire->clear();
        ui->radioButton_H->setChecked(false);
        ui->radioButton_F->setChecked(false);
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
    if (query.exec("SELECT ID, CIN, LAST_NAME, FIRST_NAME, DATE_BIRTH, PHONE, EMAIL, GENDER, SALARY, DATE_HIRING, SPECIALITY,ROLE FROM EMPLOYEE")) {
        employeeModel->setQuery(std::move(query)); // Use move semantics
        ui->tableView->setModel(employeeModel);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
