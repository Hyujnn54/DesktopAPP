// ui/mainwindow/mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "managers/meeting/meeting.h"
#include "dialog/updateemployee/updateemployeedialog.h"  // Updated include path
#include "dialog/updateresourcesdialog/updateresourcesdialog.h"  // Include for UpdateResourceDialog
#include "lib/qrcodegen/qrcodegen.hpp"  // Ajout de l'inclusion qrcodegen
#include <QMessageBox>
#include <QRegularExpression>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QHeaderView>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDoubleValidator>

MainWindow::MainWindow(bool dbConnected, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dbConnected(dbConnected),
    clientManager(new ClientManager(dbConnected, this)),
    trainingManager(new TrainingManager(dbConnected, this)),
    meetingManager(new MeetingManager(dbConnected, this)),
    employeeManager(new EmployeeManager(this)),
    resourceManager(new ResourceManager(this)),
    notificationManager(new NotificationManager(this)),
    networkManager(new QNetworkAccessManager(this))
{
    qDebug() << "Entering MainWindow constructor";

    // Block signals during UI setup
    this->blockSignals(true);

    ui->setupUi(this);
    applyLightTheme();
    setAttribute(Qt::WA_DeleteOnClose);

    clientManager->setNotificationManager(notificationManager);
    trainingManager->setNotificationManager(notificationManager);
    meetingManager->setNotificationManager(notificationManager);

    // Initialize resource-specific variables
    selectedResourceId = -1;
    imageData = QByteArray();
    currentSortColumn = 0;
    currentSortOrder = Qt::AscendingOrder;
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    connect(searchTimer, &QTimer::timeout, this, &MainWindow::on_searchTimeout);
    pieSeries = new QPieSeries(this);
    barSeries = new QBarSeries(this);

    setupUiConnections();
    setupChartConnections();

    // Connect networkManager and AI chat signals after UI setup
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAIResponseReceived);
    connect(ui->trainingNotificationLabel, &QPushButton::clicked, this, &MainWindow::handleNotificationLabelClicked);
    
    // Important: Connect notification count changes to update the label
    connect(notificationManager, &NotificationManager::notificationCountChanged, 
            this, &MainWindow::updateNotificationLabel);
    
    // Initialize notification label with 0 count
    updateNotificationLabel(0);

    // Move appendChatMessage after all setup
    appendChatMessage("Hello! I'm your Meeting Assistant. How can I help you today?", true);

    if (!m_dbConnected) {
        ui->clientSectionButton->setEnabled(false);
        ui->trainingSectionButton->setEnabled(false);
        ui->meetingSectionButton->setEnabled(false);
        ui->employeeSectionButton->setEnabled(false);
        ui->resourceSectionButton->setEnabled(false);
        statusBar()->showMessage("Database not connected. Some features are disabled.");
    } else {
        clientManager->initialize(ui);
        trainingManager->initialize(ui);
        meetingManager->initialize(ui);
        employeeManager->initialize(this); // Correction: passage de 'this' au lieu de 'ui'
        
        // Apply improved table styling to all tables right after initialization
        if (ui->clientTableView) improveTableDisplay(ui->clientTableView);
        if (ui->clientDateConsultationsView) improveTableDisplay(ui->clientDateConsultationsView);
        if (ui->trainingTableView) improveTableDisplay(ui->trainingTableView);
        if (ui->meetingTableWidget) improveTableWidgetDisplay(ui->meetingTableWidget);
        if (ui->tableView) improveTableDisplay(ui->tableView);
        
        // Initialize charts for each section
        setupClientChart();
        setupTrainingChart();
        setupMeetingChart();
        setupEmployeeChart();
        setupResourceChart();
        
        on_meetingSectionButton_clicked();
    }

    // Re-enable signals after all setup
    this->blockSignals(false);

    qDebug() << "Exiting MainWindow constructor";
}

MainWindow::~MainWindow()
{
    delete clientManager;
    delete trainingManager;
    delete meetingManager;
    delete employeeManager;
    delete resourceManager;
    delete notificationManager;
    delete networkManager;
    delete ui;
}

void MainWindow::setupUiConnections()
{
    qDebug() << "Setting up UI connections";
    connect(ui->clientSectionButton, &QPushButton::clicked, this, &MainWindow::on_clientSectionButton_clicked);
    connect(ui->trainingSectionButton, &QPushButton::clicked, this, &MainWindow::on_trainingSectionButton_clicked);
    connect(ui->meetingSectionButton, &QPushButton::clicked, this, &MainWindow::on_meetingSectionButton_clicked);
    connect(ui->employeeSectionButton, &QPushButton::clicked, this, &MainWindow::on_employeeSectionButton_clicked);
    connect(ui->resourceSectionButton, &QPushButton::clicked, this, &MainWindow::on_resourceSectionButton_clicked);
    connect(ui->menuButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);
    connect(ui->themeButton, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    connect(ui->meetingChatSendButton, &QPushButton::clicked, this, &MainWindow::on_chatSendButton_clicked);
    connect(ui->meetingChatClearButton, &QPushButton::clicked, this, &MainWindow::on_chatClearButton_clicked);
    
    // Configurer les validateurs d'entrée
    setupInputValidators();
}

void MainWindow::setupInputValidators()
{
    // Configuration des validateurs pour les champs d'entrée employé
    
    // CIN - Validation en temps réel
    connect(ui->lineEdit_CIN, &QLineEdit::textChanged, this, &MainWindow::validateCIN);
    
    // Nom et prénom - Validation en temps réel
    connect(ui->lineEdit_Nom, &QLineEdit::textChanged, this, &MainWindow::validateName);
    connect(ui->lineEdit_Prenom, &QLineEdit::textChanged, this, &MainWindow::validateName);
    
    // Téléphone - Validation en temps réel
    connect(ui->lineEdit_phone, &QLineEdit::textChanged, this, &MainWindow::validatePhone);
    
    // Email - Validation en temps réel
    connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &MainWindow::validateEmail);
    
    // Salaire - Validation en temps réel
    connect(ui->lineEdit_salaire, &QLineEdit::textChanged, this, &MainWindow::validateSalary);
    
    // Réinitialiser les styles des champs
    ui->lineEdit_CIN->setStyleSheet("");
    ui->lineEdit_Nom->setStyleSheet("");
    ui->lineEdit_Prenom->setStyleSheet("");
    ui->lineEdit_phone->setStyleSheet("");
    ui->lineEdit_email->setStyleSheet("");
    ui->lineEdit_salaire->setStyleSheet("");
}

void MainWindow::validateCIN(const QString &text)
{
    // Limiter à 8 caractères et seulement des chiffres
    QString filtered = text;
    for (int i = filtered.length() - 1; i >= 0; --i) {
        if (!filtered[i].isDigit()) {
            filtered.remove(i, 1);
        }
    }
    
    // Limiter à 8 chiffres maximum
    if (filtered.length() > 8) {
        filtered = filtered.left(8);
    }
    
    // Si le texte a été modifié, mettre à jour le champ
    if (filtered != text) {
        ui->lineEdit_CIN->setText(filtered);
    }
    
    // Mise à jour du style selon la validité
    if (filtered.length() == 8) {
        ui->lineEdit_CIN->setStyleSheet("border: 1px solid green;");
    } else {
        ui->lineEdit_CIN->setStyleSheet("border: 1px solid red;");
    }
}

void MainWindow::validateName(const QString &text)
{
    QLineEdit* senderLineEdit = qobject_cast<QLineEdit*>(sender());
    if (!senderLineEdit) return;
    
    QString filtered = text;
    for (int i = filtered.length() - 1; i >= 0; --i) {
        // Accepter seulement les lettres et les espaces
        if (!filtered[i].isLetter() && filtered[i] != ' ' && filtered[i] != '-') {
            filtered.remove(i, 1);
        }
    }
    
    // Si le texte a été modifié, mettre à jour le champ
    if (filtered != text) {
        senderLineEdit->setText(filtered);
    }
    
    // Mise à jour du style selon la validité
    if (!filtered.isEmpty()) {
        senderLineEdit->setStyleSheet("border: 1px solid green;");
    } else {
        senderLineEdit->setStyleSheet("border: 1px solid red;");
    }
}

void MainWindow::validatePhone(const QString &text)
{
    // Limiter à des chiffres seulement
    QString filtered = text;
    for (int i = filtered.length() - 1; i >= 0; --i) {
        if (!filtered[i].isDigit()) {
            filtered.remove(i, 1);
        }
    }
    
    // Si le texte a été modifié, mettre à jour le champ
    if (filtered != text) {
        ui->lineEdit_phone->setText(filtered);
    }
    
    // Mise à jour du style selon la validité
    if (filtered.length() >= 8) {
        ui->lineEdit_phone->setStyleSheet("border: 1px solid green;");
    } else {
        ui->lineEdit_phone->setStyleSheet("border: 1px solid red;");
    }
}

void MainWindow::validateEmail(const QString &text)
{
    // Vérification simple d'email avec une expression régulière
    QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b");
    bool isValid = emailRegex.match(text).hasMatch();
    
    // Mise à jour du style selon la validité
    if (isValid || text.isEmpty()) {
        ui->lineEdit_email->setStyleSheet("border: 1px solid green;");
    } else {
        ui->lineEdit_email->setStyleSheet("border: 1px solid red;");
    }
}

void MainWindow::validateSalary(const QString &text)
{
    // Vérifier que le texte peut être converti en nombre
    bool ok;
    text.toDouble(&ok);
    
    // Mise à jour du style selon la validité
    if (ok || text.isEmpty()) {
        ui->lineEdit_salaire->setStyleSheet("border: 1px solid green;");
    } else {
        ui->lineEdit_salaire->setStyleSheet("border: 1px solid red;");
    }
}

void MainWindow::on_clientSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->clientPage);
    clientManager->refresh();
    
    // Apply improved table styling for better readability
    improveTableDisplay(ui->clientTableView);
    improveTableDisplay(ui->clientDateConsultationsView);
}

void MainWindow::on_trainingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->trainingPage);
    trainingManager->refresh();
    
    // Apply improved table styling for better readability
    improveTableDisplay(ui->trainingTableView);
}

void MainWindow::on_meetingSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->meetingPage);
    meetingManager->refreshTableWidget();
    
    // Apply specific styling to the meeting table widget
    ui->meetingTableWidget->setStyleSheet("QTableWidget { gridline-color: #E5E5E5; }");
    
    // Configure column widths and resizing
    ui->meetingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->meetingTableWidget->horizontalHeader()->setStretchLastSection(true);
    
    // Improve meeting table display
    improveTableWidgetDisplay(ui->meetingTableWidget);
}

void MainWindow::on_employeeSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->employeePage);
    QSqlQueryModel* model = employeeManager->getAllEmployees();

    // Vérifier si le modèle est valide avant de l'assigner au tableView
    if (model) {
        ui->tableView->setModel(model);

        // Enable sorting
        ui->tableView->setSortingEnabled(true);
        
        // Enable horizontal scroll bar to see all columns
        ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        
        // Configure headers to be clickable for sorting
        ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
        ui->tableView->horizontalHeader()->setSectionsClickable(true);
        
        // Connect the header click to sorting function
        connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, [this](int logicalIndex) {
            Qt::SortOrder order = ui->tableView->horizontalHeader()->sortIndicatorOrder();
            QSqlQueryModel* sortedModel = employeeManager->sortEmployees(logicalIndex, order);
            if (sortedModel) {
                ui->tableView->setModel(sortedModel);
                
                // Restore visual settings after model change
                ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                ui->tableView->horizontalHeader()->setSortIndicatorShown(true);
                improveTableDisplay(ui->tableView);
            }
        });

        // Configuration pour afficher toutes les colonnes correctement
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);

        // Définir des largeurs optimales pour chaque colonne
        ui->tableView->setColumnWidth(0, 50);   // ID
        ui->tableView->setColumnWidth(1, 100);  // CIN
        ui->tableView->setColumnWidth(2, 120);  // Last Name
        ui->tableView->setColumnWidth(3, 120);  // First Name
        ui->tableView->setColumnWidth(4, 100);  // Date of Birth
        ui->tableView->setColumnWidth(5, 100);  // Phone
        ui->tableView->setColumnWidth(6, 180);  // Email
        ui->tableView->setColumnWidth(7, 80);   // Gender
        ui->tableView->setColumnWidth(8, 80);   // Salary
        ui->tableView->setColumnWidth(9, 100);  // Date of Hiring
        ui->tableView->setColumnWidth(10, 120); // Speciality
        ui->tableView->setColumnWidth(11, 80);  // Image
        ui->tableView->setColumnWidth(12, 80);  // Role
        ui->tableView->setColumnWidth(13, 80);  // RFID UID

        // Assurez-vous que toutes les colonnes sont visibles
        for (int i = 0; i < model->columnCount(); i++) {
            ui->tableView->setColumnHidden(i, false);
        }

        // Connect search functionality
        connect(ui->searchInput, &QLineEdit::textChanged, this, &MainWindow::on_employeeSearchChanged);

        // Apply improved table styling for better readability
        improveTableDisplay(ui->tableView);
    } else {
        // Afficher un message d'erreur à l'utilisateur
        QMessageBox::critical(this, "Erreur", "Impossible de charger les données des employés. Vérifiez la connexion à la base de données.");

        // Nettoyer le tableView pour éviter d'afficher d'anciennes données
        ui->tableView->setModel(nullptr);
    }
}

void MainWindow::on_statisticsButton_clicked()
{
    if (!m_dbConnected) {
        QMessageBox::warning(this, "Database Error", "Cannot open statistics: Database is not connected.");
        return;
    }
    
    // Navigate to the current section's statistics tab
    QWidget* currentPage = ui->mainStackedWidget->currentWidget();
    
    if (currentPage == ui->clientPage) {
        ui->clientTabWidget->setCurrentIndex(ui->clientTabWidget->indexOf(ui->clientStatisticsTab));
        updateClientChart();
    } 
    else if (currentPage == ui->trainingPage) {
        ui->trainingTabWidget->setCurrentIndex(ui->trainingTabWidget->indexOf(ui->trainingStatisticsTab));
        updateTrainingChart();
    } 
    else if (currentPage == ui->meetingPage) {
        ui->meetingTabWidget->setCurrentIndex(ui->meetingTabWidget->indexOf(ui->meetingStatisticsTab));
        updateMeetingChart();
    } 
    else if (currentPage == ui->employeePage) {
        ui->employeeTabWidget->setCurrentIndex(ui->employeeTabWidget->indexOf(ui->employeeStatsTab));
        updateEmployeeChart();
    }
    else {
        QMessageBox::information(this, "Statistics", "Please navigate to a section first (Clients, Training, Meetings, or Employees).");
    }
}

void MainWindow::on_clientChartRefreshButton_clicked()
{
    updateClientChart();
}

void MainWindow::on_trainingChartRefreshButton_clicked()
{
    updateTrainingChart();
}

void MainWindow::on_meetingChartRefreshButton_clicked()
{
    updateMeetingChart();
}

void MainWindow::on_clientChartTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateClientChart();
}

void MainWindow::on_trainingChartTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateTrainingChart();
}

void MainWindow::on_meetingChartTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateMeetingChart();
}

void MainWindow::on_clientChartFilterComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateClientChart();
}

void MainWindow::on_trainingChartFilterComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateTrainingChart();
}

void MainWindow::on_meetingChartFilterComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateMeetingChart();
}

void MainWindow::toggleSidebar()
{
    bool isVisible = ui->sideMenu->isVisible();
    ui->sideMenu->setVisible(!isVisible);
}

void MainWindow::toggleTheme()
{
    if (ui->themeButton->text() == "Dark Theme") {
        applyDarkTheme();
        ui->themeButton->setText("Light Theme");
    } else {
        applyLightTheme();
        ui->themeButton->setText("Dark Theme");
    }
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #F0F8FF, stop:1 #C4D7ED); 
            color: #333333; 
            font-family: Arial, sans-serif; /* Modification: utilisation d'une police plus universelle */
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
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox, QDoubleSpinBox { 
            background-color: #F5F7FA; 
            border: 1px solid #3A5DAE; 
            border-radius: 4px; 
            padding: 4px; 
            color: #333333; 
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { 
            border: 2px solid #3A5DAE; 
        }
        /* Consistent table styles for all table views and widgets */
        QTableView, QTableWidget { 
            background-color: #FFFFFF; 
            border: 1px solid #D3DCE6; 
            border-radius: 4px; 
            selection-background-color: #A1B8E6; 
            selection-color: #333333;
            gridline-color: #E5E5E5;
            alternate-background-color: #F5F7FA;
        }
        QTableView::item, QTableWidget::item {
            padding: 4px;
            border: none;
        }
        QTableView::item:selected, QTableWidget::item:selected {
            background-color: #A1B8E6;
            color: #333333;
        }
        QHeaderView::section { 
            background-color: #3A5DAE; 
            color: white; 
            padding: 6px; 
            border: none; 
            font-weight: bold;
        }
        QHeaderView::section:horizontal {
            border-right: 1px solid #FFFFFF;
        }
        QHeaderView::section:vertical {
            border-bottom: 1px solid #FFFFFF;
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
        QCalendarWidget QAbstractItemView {
            background-color: #FFFFFF;
            selection-background-color: #3A5DAE;
            selection-color: white;
        }
        QCalendarWidget QAbstractItemView:enabled {
            color: #333333;
        }
        QCalendarWidget QAbstractItemView:disabled {
            color: #A0A0A0;
        }
        QCalendarWidget QAbstractItemView:item {
            color: #333333;
        }
        QCalendarWidget QWidget {
            alternate-background-color: #F0F8FF;
        }
        QCalendarWidget QMenu {
            background-color: #FFFFFF;
        }
        QCalendarWidget QSpinBox {
            background-color: #FFFFFF;
        }
        QCalendarWidget QTableView {
            border: none;
            gridline-color: #FFFFFF;
        }
        QTabWidget::pane { 
            border: 1px solid #3A5DAE; 
            border-radius: 4px; 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #F0F8FF, stop:1 #C4D7ED);
        }
        QTabBar::tab { 
            background-color: #D3DCE6; 
            color: #333333; 
            padding: 8px 12px; 
            border-top-left-radius: 4px; 
            border-top-right-radius: 4px;
            margin-right: 2px;
        }
        QTabBar::tab:selected { 
            background-color: #3A5DAE; 
            color: white; 
        }
        QTextEdit { 
            background-color: #F5F7FA; 
            border: 1px solid #3A5DAE; 
            border-radius: 4px; 
            color: #333333; 
        }
        QChartView { 
            background-color: #FFFFFF; 
            border: 1px solid #D3DCE6; 
            border-radius: 4px; 
        }
        QLabel { 
            font-size: 10pt; 
            padding: 2px;
            background: transparent;
        }
        QGroupBox {
            border: 1px solid #B0C4DE;
            border-radius: 5px;
            margin-top: 10px;
            font-weight: bold;
            background-color: rgba(240, 248, 255, 150);
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 5px;
            color: #3A5DAE;
        }
        QStatusBar {
            background-color: #E6EEF8;
            color: #333333;
            border-top: 1px solid #B0C4DE;
        }
        #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel,
        #meetingTitleLabel, #meetingOrganiserLabel, #meetingParticipantLabel, #meetingAgendaLabel, #meetingDurationLabel, #meetingDateLabel {
            font-size: 11pt; 
            font-weight: bold; 
            color: #3A5DAE; 
            background: transparent;
            padding: 2px; 
            qproperty-alignment: AlignRight; 
        }
        #headerLabel { 
            font-size: 18pt; 
            font-weight: bold; 
            color: #3A5DAE; 
            qproperty-alignment: AlignCenter;
            background: transparent;
        }
        #trainingNotificationLabel { 
            font-size: 10pt; 
            font-weight: bold; 
            color: #3A5DAE;
            background: transparent;
        }
        QFrame { 
            background: transparent;
        }
        QFrame#header { 
            border: 1px solid #B0C4DE; 
            border-radius: 5px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E6F0FF, stop:1 #D3E5FA);
        }
        QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { 
            border: 1px solid #B0C4DE; 
            border-radius: 5px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #E1EBFA, stop:1 #C4D7ED);
        }
        QScrollBar:vertical {
            border: none;
            background: #E6EEF8;
            width: 12px;
            margin: 12px 0 12px 0;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background: #3A5DAE;
            min-height: 20px;
            border-radius: 6px;
        }
        QScrollBar::add-line:vertical {
            border: none;
            background: #E6EEF8;
            height: 12px;
            subcontrol-position: bottom;
            subcontrol-origin: margin;
            border-bottom-left-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QScrollBar::sub-line:vertical {
            border: none;
            background: #E6EEF8;
            height: 12px;
            subcontrol-position: top;
            subcontrol-origin: margin;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
        }
        QScrollBar:horizontal {
            border: none;
            background: #E6EEF8;
            height: 12px;
            margin: 0 12px 0 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:horizontal {
            background: #3A5DAE;
            min-width: 20px;
            border-radius: 6px;
        }
        QScrollBar::add-line:horizontal {
            border: none;
            background: #E6EEF8;
            width: 12px;
            subcontrol-position: right;
            subcontrol-origin: margin;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QScrollBar::sub-line:horizontal {
            border: none;
            background: #E6EEF8;
            width: 12px;
            subcontrol-position: left;
            subcontrol-origin: margin;
            border-top-left-radius: 6px;
            border-bottom-left-radius: 6px;
        }
    )");
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(R"(
        QWidget { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3A3A3A, stop:1 #2A2A2A); 
            color: #E0E0E0; 
            font-family: 'Segoe UI', Arial, sans-serif; 
        }
        QPushButton { 
            background-color: #666666; 
            color: white; 
            border: 1px solid #555555; 
            border-radius: 5px; 
            padding: 6px; 
            font-weight: bold; 
        }
        QPushButton:hover { 
            background-color: #777777; 
        }
        QPushButton:pressed { 
            background-color: #555555; 
        }
        QLineEdit, QComboBox, QDateTimeEdit, QDateEdit, QSpinBox, QDoubleSpinBox { 
            background-color: #454545; 
            border: 1px solid #555555; 
            border-radius: 4px; 
            padding: 4px; 
            color: #E0E0E0; 
        }
        QLineEdit:focus, QComboBox:focus, QDateTimeEdit:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { 
            border: 2px solid #777777; 
        }
        /* Consistent table styles for all table views and widgets */
        QTableView, QTableWidget { 
            background-color: #3A3A3A; 
            border: 1px solid #555555; 
            border-radius: 4px; 
            selection-background-color: #666666; 
            selection-color: #FFFFFF;
            gridline-color: #4A4A4A;
            alternate-background-color: #333333;
        }
        QTableView::item, QTableWidget::item {
            padding: 4px;
            border: none;
        }
        QTableView::item:selected, QTableWidget::item:selected {
            background-color: #666666;
            color: #FFFFFF;
        }
        QHeaderView::section { 
            background-color: #555555; 
            color: white; 
            padding: 6px; 
            border: none; 
            font-weight: bold;
        }
        QHeaderView::section:horizontal {
            border-right: 1px solid #666666;
        }
        QHeaderView::section:vertical {
            border-bottom: 1px solid #666666;
        }
        QCalendarWidget { 
            background-color: #3A3A3A; 
            border: 1px solid #555555; 
            border-radius: 4px; 
        }
        QCalendarWidget QToolButton { 
            background-color: #555555; 
            color: white; 
            border-radius: 3px; 
        }
        QCalendarWidget QAbstractItemView {
            background-color: #3A3A3A;
            selection-background-color: #666666;
            selection-color: white;
        }
        QCalendarWidget QAbstractItemView:enabled {
            color: #E0E0E0;
        }
        QCalendarWidget QAbstractItemView:disabled {
            color: #707070;
        }
        QCalendarWidget QAbstractItemView:item {
            color: #E0E0E0;
        }
        QCalendarWidget QWidget {
            alternate-background-color: #333333;
        }
        QCalendarWidget QMenu {
            background-color: #3A3A3A;
        }
        QCalendarWidget QSpinBox {
            background-color: #454545;
        }
        QCalendarWidget QTableView {
            border: none;
            gridline-color: #4A4A4A;
        }
        QTabWidget::pane { 
            border: 1px solid #555555; 
            border-radius: 4px; 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3A3A3A, stop:1 #2A2A2A);
        }
        QTabBar::tab { 
            background-color: #454545; 
            color: #E0E0E0; 
            padding: 8px 12px; 
            border-top-left-radius: 4px; 
            border-top-right-radius: 4px;
            margin-right: 2px;
        }
        QTabBar::tab:selected { 
            background-color: #666666; 
            color: white; 
        }
        QTextEdit { 
            background-color: #454545; 
            border: 1px solid #555555; 
            border-radius: 4px; 
            color: #E0E0E0; 
        }
        QChartView { 
            background-color: #3A3A3A; 
            border: 1px solid #555555; 
            border-radius: 4px; 
        }
        QLabel { 
            font-size: 10pt; 
            padding: 2px;
            background: transparent;
            color: #E0E0E0;
        }
        QGroupBox {
            border: 1px solid #555555;
            border-radius: 5px;
            margin-top: 10px;
            font-weight: bold;
            background-color: rgba(65, 65, 65, 150);
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 5px;
            color: #AAAAAA;
        }
        QStatusBar {
            background-color: #333333;
            color: #E0E0E0;
            border-top: 1px solid #555555;
        }
        #clientNameLabel, #clientSectorLabel, #clientContactLabel, #clientEmailLabel, #clientConsultationDateLabel, #clientConsultantLabel,
        #trainingNameLabel, #trainingDescriptionLabel, #trainingTrainerLabel, #trainingDateLabel, #trainingTimeLabel, #trainingPriceLabel, #trainingPhoneLabel,
        #meetingTitleLabel, #meetingOrganiserLabel, #meetingParticipantLabel, #meetingAgendaLabel, #meetingDurationLabel, #meetingDateLabel {
            font-size: 11pt; 
            font-weight: bold; 
            color: #AAAAAA; 
            background: transparent;
            padding: 2px; 
            qproperty-alignment: AlignRight; 
        }
        #headerLabel { 
            font-size: 18pt; 
            font-weight: bold; 
            color: #AAAAAA; 
            qproperty-alignment: AlignCenter;
            background: transparent;
        }
        #trainingNotificationLabel { 
            font-size: 10pt; 
            font-weight: bold; 
            color: #AAAAAA;
            background: transparent;
        }
        QFrame { 
            background: transparent;
        }
        QFrame#header { 
            border: 1px solid #555555; 
            border-radius: 5px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #404040, stop:1 #333333);
        }
        QFrame#sideMenu, QFrame#frame_2, QFrame#frame_4 { 
            border: 1px solid #555555; 
            border-radius: 5px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3A3A3A, stop:1 #333333);
        }
        QScrollBar:vertical {
            border: none;
            background: #333333;
            width: 12px;
            margin: 12px 0 12px 0;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background: #666666;
            min-height: 20px;
            border-radius: 6px;
        }
        QScrollBar::add-line:vertical {
            border: none;
            background: #333333;
            height: 12px;
            subcontrol-position: bottom;
            subcontrol-origin: margin;
            border-bottom-left-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QScrollBar::sub-line:vertical {
            border: none;
            background: #333333;
            height: 12px;
            subcontrol-position: top;
            subcontrol-origin: margin;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
        }
        QScrollBar:horizontal {
            border: none;
            background: #333333;
            height: 12px;
            margin: 0 12px 0 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:horizontal {
            background: #666666;
            min-width: 20px;
            border-radius: 6px;
        }
        QScrollBar::add-line:horizontal {
            border: none;
            background: #333333;
            width: 12px;
            subcontrol-position: right;
            subcontrol-origin: margin;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QScrollBar::sub-line:horizontal {
            border: none;
            background: #333333;
            width: 12px;
            subcontrol-position: left;
            subcontrol-origin: margin;
            border-top-left-radius: 6px;
            border-bottom-left-radius: 6px;
        }
    )");
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

void MainWindow::handleNotificationLabelClicked()
{
    const QVector<NotificationManager::Notification>& notifications = notificationManager->getNotifications();
    if (notifications.isEmpty()) {
        QMessageBox::information(this, "Notifications", "No new notifications.");
        return;
    }

    // Create a dialog to display notifications
    QDialog dialog(this);
    dialog.setWindowTitle("Notifications");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Add a scrollable area for notifications
    QScrollArea *scrollArea = new QScrollArea(&dialog);
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    for (const NotificationManager::Notification &notif : notifications) {
        QString notificationText = QString("%1\n%2\n%3")
        .arg(notif.title)
            .arg(notif.description)
            .arg(notif.details);
        QLabel *label = new QLabel(notificationText, scrollWidget);
        label->setWordWrap(true);
        label->setStyleSheet("QLabel { border-bottom: 1px solid #d3d3d3; padding: 5px; }");
        scrollLayout->addWidget(label);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    // Add a "Clear Notifications" button
    QPushButton *clearButton = new QPushButton("Clear Notifications", &dialog);
    connect(clearButton, &QPushButton::clicked, this, [this, &dialog]() {
        notificationManager->clearNotifications();
        dialog.accept();
    });
    layout->addWidget(clearButton);

    // Add a "Close" button
    QPushButton *closeButton = new QPushButton("Close", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog.setLayout(layout);
    dialog.resize(400, 300);
    dialog.exec();

    // Update the notification label after the dialog is closed
    updateNotificationLabel(notificationManager->getNotificationCount());
}

void MainWindow::updateNotificationLabel(int count)
{
    // Create a clear and visible notification label
    QString styleSheet;
    
    if (count > 0) {
        // Make notification count more visible when there are notifications
        styleSheet = QString(
            "QPushButton {"
            "  background-color: #3A5DAE;"
            "  color: white;"
            "  border-radius: 5px;"
            "  padding: 5px 10px;"
            "  font-weight: bold;"
            "  min-width: 140px;"
            "  max-width: 180px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #4A70C2;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #2A4682;"
            "}"
        );
        
        // Use plain text instead of HTML formatting to avoid display issues
        ui->trainingNotificationLabel->setText(QString("Notifications (%1)").arg(count));
    } else {
        // Use more subtle styling when there are no notifications
        styleSheet = QString(
            "QPushButton {"
            "  background-color: #808080;"
            "  color: white;"
            "  border-radius: 5px;"
            "  padding: 5px 10px;"
            "  min-width: 140px;"
            "  max-width: 180px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #9A9A9A;"
            "}"
        );
        ui->trainingNotificationLabel->setText("No Notifications");
    }
    
    ui->trainingNotificationLabel->setStyleSheet(styleSheet);
    
    // Force an update to ensure the button is redrawn immediately
    ui->trainingNotificationLabel->update();
}

void MainWindow::on_chatSendButton_clicked()
{
    QString input = ui->meetingChatInputLineEdit->text().trimmed();
    if (input.isEmpty()) {
        return;
    }
    appendChatMessage(input);
    processUserInput(input);
    ui->meetingChatInputLineEdit->clear();
}

void MainWindow::on_chatClearButton_clicked()
{
    ui->meetingChatInputLineEdit->clear();
    ui->meetingChatTextEdit->clear();
}

void MainWindow::appendChatMessage(const QString &message, bool isBot)
{
    qDebug() << "Entering appendChatMessage, message:" << message << ", isBot:" << isBot;
    if (!ui || !ui->meetingChatTextEdit) {
        qDebug() << "Error: ui or meetingChatTextEdit is null";
        return;
    }

    QString formattedMessage = QString("[%1] %2: %3\n")
                                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss"),
                                        isBot ? "Assistant" : "User", message);
    ui->meetingChatTextEdit->append(formattedMessage);

    if (!isBot) {
        qDebug() << "Network request disabled for testing";
        ui->meetingChatTextEdit->append("[System] Network requests are disabled for testing.");
        // Uncomment and configure when API is ready
        /*
        QUrl url("https://api.example.com/chat");
        if (!url.isValid()) {
            qDebug() << "Error: Invalid URL in appendChatMessage:" << url.toString();
            ui->meetingChatTextEdit->append("[Error] Invalid API URL. Please check the configuration.");
            return;
        }

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["message"] = message;
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        QNetworkReply *reply = networkManager->post(request, data);
        if (!reply) {
            qDebug() << "Error: Failed to create network reply";
            ui->meetingChatTextEdit->append("[Error] Failed to send message to API.");
        } else {
            connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError code) {
                qDebug() << "Network error in appendChatMessage:" << reply->errorString();
                ui->meetingChatTextEdit->append("[Error] Network error: " + reply->errorString());
            });
        }
        */
    }
    qDebug() << "Exiting appendChatMessage";
}

void MainWindow::processUserInput(const QString &input)
{
    static const QRegularExpression addMeetingRegex(R"(add meeting\s+(.+?)\s+by\s+(.+?)\s+with\s+(.+?)\s+about\s+(.+?)\s+for\s+(\d+)\s+min\s+on\s+(.+))", QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression deleteMeetingRegex(R"(delete meeting\s+(\d+))", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch addMatch = addMeetingRegex.match(input);
    QRegularExpressionMatch deleteMatch = deleteMeetingRegex.match(input);

    if (addMatch.hasMatch()) {
        QStringList parts = addMatch.capturedTexts();
        QString errorMessage;
        if (!validateMeetingInput(parts, errorMessage)) {
            appendChatMessage("Error: " + errorMessage, true);
            return;
        }

        meeting m = createMeetingFromInput(input);
        if (m.add()) {
            meetingManager->refreshTableWidget();
            appendChatMessage("Meeting added successfully!", true);
            if (notificationManager) {
                notificationManager->addNotification("Meeting Added", "Meeting: " + m.getTitle(), "Added meeting via chat", -1);
            }
        } else {
            appendChatMessage("Failed to add meeting. Check database connection.", true);
        }
    } else if (deleteMatch.hasMatch()) {
        int id = deleteMatch.captured(1).toInt();
        meeting m;
        if (m.deleteMeeting(id)) {
            meetingManager->refreshTableWidget();
            appendChatMessage("Meeting deleted successfully!", true);
            if (notificationManager) {
                notificationManager->addNotification("Meeting Deleted", "Meeting ID: " + QString::number(id), "Deleted meeting via chat", -1);
            }
        } else {
            appendChatMessage("Failed to delete meeting. Check database connection.", true);
        }
    } else {
        QNetworkRequest request(QUrl("http://api.example.com/chatbot"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["message"] = input;
        QJsonDocument doc(json);
        networkManager->post(request, doc.toJson());
    }
}

meeting MainWindow::createMeetingFromInput(const QString &input)
{
    static const QRegularExpression addMeetingRegex(R"(add meeting\s+(.+?)\s+by\s+(.+?)\s+with\s+(.+?)\s+about\s+(.+?)\s+for\s+(\d+)\s+min\s+on\s+(.+))", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = addMeetingRegex.match(input);

    QString title = match.captured(1);
    QString organiser = match.captured(2);
    QString participant = match.captured(3);
    QString agenda = match.captured(4);
    int duration = match.captured(5).toInt();
    QDateTime dateTime = QDateTime::fromString(match.captured(6), "yyyy-MM-dd hh:mm");

    // Use -1 as temporary ID, using the constructor compatible with meeting.h
    return meeting(-1, title, organiser, participant, agenda, duration, dateTime);
}

bool MainWindow::validateMeetingInput(const QStringList &parts, QString &errorMessage)
{
    if (parts.size() != 7) {
        errorMessage = "Invalid input format. Use: add meeting <title> by <organiser> with <participant> about <agenda> for <duration> min on <date>";
        return false;
    }

    bool ok;
    int duration = parts[5].toInt(&ok);
    if (!ok || duration <= 0) {
        errorMessage = "Duration must be a positive number.";
        return false;
    }

    QDateTime dateTime = QDateTime::fromString(parts[6], "yyyy-MM-dd hh:mm");
    if (!dateTime.isValid() || dateTime < QDateTime::currentDateTime()) {
        errorMessage = "Date and time must be valid and in the future.";
        return false;
    }

    return true;
}

void MainWindow::onAIResponseReceived(QNetworkReply *reply)
{
    qDebug() << "Entering onAIResponseReceived";
    if (!reply) {
        qDebug() << "Error: Network reply is null";
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error:" << reply->errorString();
        appendChatMessage("Error: " + reply->errorString(), true);
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);
    if (doc.isNull()) {
        qDebug() << "Error: Failed to parse JSON response";
        appendChatMessage("Error: Failed to parse API response.", true);
        reply->deleteLater();
        return;
    }

    QJsonObject json = doc.object();
    QString botResponse = json.value("response").toString();
    if (botResponse.isEmpty()) {
        qDebug() << "Error: Empty response from API";
        appendChatMessage("Error: Empty response from API.", true);
    } else {
        appendChatMessage(botResponse, true);
    }

    reply->deleteLater();
    qDebug() << "Exiting onAIResponseReceived";
}

void MainWindow::setupChartConnections()
{
    if (!m_dbConnected) return;
    
    // Connect client chart controls
    connect(ui->clientChartRefreshButton, &QPushButton::clicked, this, &MainWindow::on_clientChartRefreshButton_clicked);
    connect(ui->clientChartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_clientChartTypeComboBox_currentIndexChanged);
    connect(ui->clientChartFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_clientChartFilterComboBox_currentIndexChanged);
    
    // Connect training chart controls
    connect(ui->trainingChartRefreshButton, &QPushButton::clicked, this, &MainWindow::on_trainingChartRefreshButton_clicked);
    connect(ui->trainingChartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_trainingChartTypeComboBox_currentIndexChanged);
    connect(ui->trainingChartFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_trainingChartFilterComboBox_currentIndexChanged);
    
    // Connect meeting chart controls
    connect(ui->meetingChartRefreshButton, &QPushButton::clicked, this, &MainWindow::on_meetingChartRefreshButton_clicked);
    connect(ui->meetingChartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_meetingChartTypeComboBox_currentIndexChanged);
    connect(ui->meetingChartFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_meetingChartFilterComboBox_currentIndexChanged);
    
    // Add these to setup chart connections
    connect(ui->employeeChartRefreshButton, &QPushButton::clicked, this, &MainWindow::on_employeeChartRefreshButton_clicked);
    connect(ui->employeeChartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_employeeChartTypeComboBox_currentIndexChanged);
    connect(ui->employeeChartFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_employeeChartFilterComboBox_currentIndexChanged);
    connect(ui->employeeToggleLegendCheckBox, &QCheckBox::toggled, this, &MainWindow::on_employeeToggleLegendCheckBox_toggled);
    // Resource statistics chart controls
    connect(ui->resourceRefreshStatsButton, &QPushButton::clicked, this, [this]() { updateResourceChart(); });
    connect(ui->resourceChartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { updateResourceChart(); });
    connect(ui->resourceChartFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { updateResourceChart(); });
    connect(ui->resourceToggleLegendCheckBox, &QCheckBox::toggled, this, [this](bool) { updateResourceChart(); });
}

void MainWindow::setupClientChart()
{
    if (!m_dbConnected) return;
    
    // Create a default chart 
    QChart *chart = new QChart();
    chart->setTitle("Client Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // Set the chart on the ChartView
    ui->clientChartView->setChart(chart);
    ui->clientChartView->setRenderHint(QPainter::Antialiasing);
    
    // Enable chart hover effects
    ui->clientChartView->setRubberBand(QChartView::RectangleRubberBand);
    
    // Update with initial data
    updateClientChart();
}

void MainWindow::setupTrainingChart()
{
    if (!m_dbConnected) return;
    
    // Create a default chart 
    QChart *chart = new QChart();
    chart->setTitle("Training Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // Set the chart on the ChartView
    ui->trainingChartView->setChart(chart);
    ui->trainingChartView->setRenderHint(QPainter::Antialiasing);
    
    // Enable chart hover effects
    ui->trainingChartView->setRubberBand(QChartView::RectangleRubberBand);
    
    // Update with initial data
    updateTrainingChart();
}

void MainWindow::setupMeetingChart()
{
    if (!m_dbConnected) return;
    
    // Create a default chart 
    QChart *chart = new QChart();
    chart->setTitle("Meeting Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // Set the chart on the ChartView
    ui->meetingChartView->setChart(chart);
    ui->meetingChartView->setRenderHint(QPainter::Antialiasing);
    
    // Enable chart hover effects
    ui->meetingChartView->setRubberBand(QChartView::RectangleRubberBand);
    
    // Update with initial data
    updateMeetingChart();
}

void MainWindow::setupEmployeeChart()
{
    if (!m_dbConnected) return;
    
    // Create a default chart directly using the employee chart view
    QChart* chart = new QChart();
    chart->setTitle("Employee Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // Set the chart on the ChartView
    if (ui->employeeChartView) {
        ui->employeeChartView->setChart(chart);
        ui->employeeChartView->setRenderHint(QPainter::Antialiasing);
        
        // Enable chart hover effects
        ui->employeeChartView->setRubberBand(QChartView::RectangleRubberBand);
        
        // Update with initial data
        updateEmployeeChart();
    }
}

void MainWindow::updateClientChart()
{
    if (!m_dbConnected) return;
    
    try {
        // Get chart type and filter
        QString chartType = ui->clientChartTypeComboBox->currentText();
        QString filter = ui->clientChartFilterComboBox->currentText();
        bool showLegend = ui->clientToggleLegendCheckBox->isChecked();
        
        // Get the chart from the view
        QChart *chart = ui->clientChartView->chart();
        if (!chart) return;
        
        // Clear existing series and disconnect old connections
        foreach(QAbstractSeries *series, chart->series()) {
            if (QBarSeries *barSeries = qobject_cast<QBarSeries*>(series)) {
                disconnect(barSeries, &QBarSeries::hovered, nullptr, nullptr);
            } else if (QPieSeries *pieSeries = qobject_cast<QPieSeries*>(series)) {
                for (QPieSlice *slice : pieSeries->slices()) {
                    disconnect(slice, &QPieSlice::hovered, nullptr, nullptr);
                }
            }
        }
        chart->removeAllSeries();
        
        // Also clear the axes to prevent duplication
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis *axis : axes) {
            chart->removeAxis(axis);
            delete axis;
        }
        
        // Update chart title based on filter
        chart->setTitle(QString("Client Statistics by %1").arg(filter));
        
        // Show/hide legend based on checkbox
        chart->legend()->setVisible(showLegend);
        
        // Create appropriate chart based on selection
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            
            // Get data from database through client manager
            QMap<QString, int> data = clientManager->getStatisticsByCategory(filter);
            
            // Add slices to the pie series
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                slice->setLabelVisible(true);
                slice->setLabel(QString("%1: %2%").arg(it.key()).arg(100 * slice->percentage(), 0, 'f', 1));
                
                // Connect slice signals for hover effects
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->clientHoverDescriptionLabel->setText(
                            QString("%1: %2 clients (%3%)").arg(
                                slice->label(), 
                                QString::number(slice->value()),
                                QString::number(100 * slice->percentage(), 'f', 1)
                            )
                        );
                    } else {
                        slice->setExploded(false);
                        ui->clientHoverDescriptionLabel->setText("Hover over a chart element to see details");
                    }
                });
            }
            
            chart->addSeries(series);
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            
            // Get data from database through client manager
            QMap<QString, int> data = clientManager->getStatisticsByCategory(filter);
            
            // Create a bar set
            QBarSet *set = new QBarSet("Clients");
            
            // Create categories list for axis
            QStringList categories;
            
            // Add values to the bar set
            for (auto it = data.begin(); it != data.end(); ++it) {
                *set << it.value();
                categories << it.key();
            }
            
            series->append(set);
            chart->addSeries(series);
            
            // Create axes
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            
            QValueAxis *axisY = new QValueAxis();
            // Ensure we don't crash with empty data
            if (set->count() > 0) {
                double maxValue = 0;
                for (int i = 0; i < set->count(); i++) {
                    maxValue = qMax(maxValue, set->at(i));
                }
                axisY->setRange(0, maxValue * 1.1); // Set range with some padding
            } else {
                axisY->setRange(0, 10); // Default range if no data
            }
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            
            // Connect hover signals
            connect(series, &QBarSeries::hovered, [this](bool status, int index, QBarSet *barset) {
                if (status) {
                    ui->clientHoverDescriptionLabel->setText(
                        QString("%1: %2 clients").arg(
                            barset->label(), 
                            QString::number(barset->at(index))
                        )
                    );
                } else {
                    ui->clientHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateClientChart:" << e.what();
        QMessageBox::warning(this, "Chart Error", "Failed to update client chart: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateClientChart";
        QMessageBox::warning(this, "Chart Error", "Unknown error updating client chart");
    }
}

void MainWindow::updateTrainingChart()
{
    if (!m_dbConnected) return;
    
    try {
        // Get chart type and filter
        QString chartType = ui->trainingChartTypeComboBox->currentText();
        QString filter = ui->trainingChartFilterComboBox->currentText();
        bool showLegend = ui->trainingToggleLegendCheckBox->isChecked();
        
        // Get the chart from the view
        QChart *chart = ui->trainingChartView->chart();
        if (!chart) return;
        
        // Clear existing series and disconnect old connections
        foreach(QAbstractSeries *series, chart->series()) {
            if (QBarSeries *barSeries = qobject_cast<QBarSeries*>(series)) {
                disconnect(barSeries, &QBarSeries::hovered, nullptr, nullptr);
            } else if (QPieSeries *pieSeries = qobject_cast<QPieSeries*>(series)) {
                for (QPieSlice *slice : pieSeries->slices()) {
                    disconnect(slice, &QPieSlice::hovered, nullptr, nullptr);
                }
            }
        }
        chart->removeAllSeries();
        
        // Also clear the axes to prevent duplication
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis *axis : axes) {
            chart->removeAxis(axis);
            delete axis;
        }
        
        // Update chart title based on filter
        chart->setTitle(QString("Training Statistics by %1").arg(filter));
        
        // Show/hide legend based on checkbox
        chart->legend()->setVisible(showLegend);
        
        // Create appropriate chart based on selection
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            
            // Get data from database through training manager
            QMap<QString, int> data = trainingManager->getStatisticsByCategory(filter);
            
            // Add slices to the pie series
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                slice->setLabelVisible(true);
                slice->setLabel(QString("%1: %2%").arg(it.key()).arg(100 * slice->percentage(), 0, 'f', 1));
                
                // Connect slice signals for hover effects
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->trainingHoverDescriptionLabel->setText(
                            QString("%1: %2 trainings (%3%)").arg(
                                slice->label(), 
                                QString::number(slice->value()),
                                QString::number(100 * slice->percentage(), 'f', 1)
                            )
                        );
                    } else {
                        slice->setExploded(false);
                        ui->trainingHoverDescriptionLabel->setText("Hover over a chart element to see details");
                    }
                });
            }
            
            chart->addSeries(series);
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            
            // Get data from database through training manager
            QMap<QString, int> data = trainingManager->getStatisticsByCategory(filter);
            
            // Create a bar set
            QBarSet *set = new QBarSet("Trainings");
            
            // Create categories list for axis
            QStringList categories;
            
            // Add values to the bar set
            for (auto it = data.begin(); it != data.end(); ++it) {
                *set << it.value();
                categories << it.key();
            }
            
            series->append(set);
            chart->addSeries(series);
            
            // Create axes
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            
            QValueAxis *axisY = new QValueAxis();
            // Ensure we don't crash with empty data
            if (set->count() > 0) {
                double maxValue = 0;
                for (int i = 0; i < set->count(); i++) {
                    maxValue = qMax(maxValue, set->at(i));
                }
                axisY->setRange(0, maxValue * 1.1); // Set range with some padding
            } else {
                axisY->setRange(0, 10); // Default range if no data
            }
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            
            // Connect hover signals
            connect(series, &QBarSeries::hovered, [this](bool status, int index, QBarSet *barset) {
                if (status) {
                    ui->trainingHoverDescriptionLabel->setText(
                        QString("%1: %2 trainings").arg(
                            barset->label(), 
                            QString::number(barset->at(index))
                        )
                    );
                } else {
                    ui->trainingHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateTrainingChart:" << e.what();
        QMessageBox::warning(this, "Chart Error", "Failed to update training chart: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateTrainingChart";
        QMessageBox::warning(this, "Chart Error", "Unknown error updating training chart");
    }
}

void MainWindow::updateMeetingChart()
{
    if (!m_dbConnected) return;

    try {
        // Get chart type and filter
        QString chartType = ui->meetingChartTypeComboBox->currentText();
        QString filter = ui->meetingChartFilterComboBox->currentText();
        bool showLegend = ui->meetingToggleLegendCheckBox->isChecked();

        // Get the chart from the view
        QChart *chart = ui->meetingChartView->chart();
        if (!chart) return;

        // Clear existing series and disconnect old connections
        foreach(QAbstractSeries *series, chart->series()) {
            if (QBarSeries *barSeries = qobject_cast<QBarSeries*>(series)) {
                disconnect(barSeries, &QBarSeries::hovered, nullptr, nullptr);
            } else if (QPieSeries *pieSeries = qobject_cast<QPieSeries*>(series)) {
                for (QPieSlice *slice : pieSeries->slices()) {
                    disconnect(slice, &QPieSlice::hovered, nullptr, nullptr);
                }
            }
        }
        chart->removeAllSeries();

        // Also clear the axes to prevent duplication
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis *axis : axes) {
            chart->removeAxis(axis);
            delete axis;
        }

        // Update chart title based on filter
        chart->setTitle(QString("Meeting Statistics by %1").arg(filter));

        // Show/hide legend based on checkbox
        chart->legend()->setVisible(showLegend);

        // Create appropriate chart based on selection
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();

            // Get data from database through meeting manager
            QMap<QString, int> data = meetingManager->getStatisticsByCategory(filter);

            // Add slices to the pie series
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                slice->setLabelVisible(true);
                slice->setLabel(QString("%1: %2%").arg(it.key()).arg(100 * slice->percentage(), 0, 'f', 1));

                // Connect slice signals for hover effects
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->meetingHoverDescriptionLabel->setText(
                            QString("%1: %2 meetings (%3%)").arg(
                                slice->label(),
                                QString::number(slice->value()),
                                QString::number(100 * slice->percentage(), 'f', 1)
                                )
                            );
                    } else {
                        slice->setExploded(false);
                        ui->meetingHoverDescriptionLabel->setText("Hover over a chart element to see details");
                    }
                });
            }

            chart->addSeries(series);
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();

            // Get data from database through meeting manager
            QMap<QString, int> data = meetingManager->getStatisticsByCategory(filter);

            // Create a bar set
            QBarSet *set = new QBarSet("Meetings");

            // Create categories list for axis
            QStringList categories;

            // Add values to the bar set
            for (auto it = data.begin(); it != data.end(); ++it) {
                *set << it.value();
                categories << it.key();
            }

            series->append(set);
            chart->addSeries(series);

            // Create axes
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);

            QValueAxis *axisY = new QValueAxis();
            // Ensure we don't crash with empty data
            if (set->count() > 0) {
                double maxValue = 0;
                for (int i = 0; i < set->count(); i++) {
                    maxValue = qMax(maxValue, set->at(i));
                }
                axisY->setRange(0, maxValue * 1.1); // Set range with some padding
            } else {
                axisY->setRange(0, 10); // Default range if no data
            }
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);

            // Connect hover signals, capturing categories
            connect(series, &QBarSeries::hovered, [this, categories](bool status, int index, QBarSet *barset) {
                if (status) {
                    ui->meetingHoverDescriptionLabel->setText(
                        QString("%1: %2 meetings").arg(
                            categories.at(index),
                            QString::number(barset->at(index))
                            )
                        );
                } else {
                    ui->meetingHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateMeetingChart:" << e.what();
        QMessageBox::warning(this, "Chart Error", "Failed to update meeting chart: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateMeetingChart";
        QMessageBox::warning(this, "Chart Error", "Unknown error updating meeting chart");
    }
}

void MainWindow::updateEmployeeChart()
{
    if (!m_dbConnected) return;
    try {
        QComboBox* employeeChartTypeComboBox = ui->employeeChartTypeComboBox;
        QComboBox* employeeChartFilterComboBox = ui->employeeChartFilterComboBox;
        QCheckBox* employeeToggleLegendCheckBox = ui->employeeToggleLegendCheckBox;
        QString chartType = employeeChartTypeComboBox ? employeeChartTypeComboBox->currentText() : "Pie Chart";
        QString filter = employeeChartFilterComboBox ? employeeChartFilterComboBox->currentText() : "Role";
        bool showLegend = employeeToggleLegendCheckBox ? employeeToggleLegendCheckBox->isChecked() : true;
        QChartView* chartView = ui->employeeChartView;
        if (!chartView) return;
        QChart* chart = chartView->chart();
        if (!chart) return;
        chart->removeAllSeries();
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis* axis : axes) { chart->removeAxis(axis); delete axis; }
        chart->setTitle(QString("Employee Statistics by %1").arg(filter));
        chart->setAnimationOptions(QChart::SeriesAnimations); // Enable animation
        chart->legend()->setVisible(showLegend);
        QMap<QString, int> data;
        // Query EMPLOYEE table directly for statistics
        if (filter == "Role") {
            QSqlQuery query("SELECT ROLE, COUNT(*) as COUNT FROM EMPLOYEE GROUP BY ROLE");
            while (query.next()) {
                QString role = query.value(0).toString();
                int count = query.value(1).toInt();
                data[role] = count;
            }
        } else if (filter == "Specialty") {
            QSqlQuery query("SELECT SPECIALITY, COUNT(*) as COUNT FROM EMPLOYEE GROUP BY SPECIALITY");
            while (query.next()) {
                QString specialty = query.value(0).toString();
                int count = query.value(1).toInt();
                data[specialty] = count;
            }
        }
        if (data.isEmpty()) {
            chart->setTitle("No employee data available");
            if (ui->employeeHoverDescriptionLabel) ui->employeeHoverDescriptionLabel->setText("No data");
            return;
        }
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            int total = 0;
            for (auto it = data.begin(); it != data.end(); ++it) total += it.value();
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                slice->setLabelVisible(true);
                slice->setLabel(QString("%1: %2 (%3%)").arg(it.key()).arg(it.value()).arg(100.0 * it.value() / total, 0, 'f', 1));
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
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Employees");
            QStringList categories;
            for (auto it = data.begin(); it != data.end(); ++it) {
                *set << it.value();
                categories << it.key();
            }
            series->append(set);
            chart->addSeries(series);
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            QValueAxis *axisY = new QValueAxis();
            int maxValue = 0;
            for (auto it = data.begin(); it != data.end(); ++it) maxValue = qMax(maxValue, it.value());
            axisY->setRange(0, maxValue * 1.1);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            connect(series, &QBarSeries::hovered, [this, categories](bool status, int index, QBarSet *barset) {
                if (status && index >= 0 && index < categories.size()) {
                    ui->employeeHoverDescriptionLabel->setText(QString("%1: %2 employees").arg(categories.at(index)).arg(barset->at(index)));
                } else {
                    ui->employeeHoverDescriptionLabel->setText("Hover over a chart element to see details");
                }
            });
        }
        if (ui->employeeTotalCountLabel) {
            QSqlQuery totalQuery("SELECT COUNT(*) FROM EMPLOYEE");
            int total = 0;
            if (totalQuery.next()) total = totalQuery.value(0).toInt();
            ui->employeeTotalCountLabel->setText(QString("Total Employees: %1").arg(total));
        }
        if (ui->employeeAverageSalaryLabel) {
            QSqlQuery avgQuery("SELECT AVG(SALAIRE) FROM EMPLOYEE");
            double avg = 0;
            if (avgQuery.next()) avg = avgQuery.value(0).toDouble();
            ui->employeeAverageSalaryLabel->setText(QString("Average Salary: $%1").arg(avg, 0, 'f', 2));
        }
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::RectangleRubberBand);
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateEmployeeChart:" << e.what();
        QMessageBox::warning(this, "Chart Error", "Failed to update employee chart: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateEmployeeChart";
        QMessageBox::warning(this, "Chart Error", "Unknown error updating employee chart");
    }
}

void MainWindow::updateResourceChart()
{
    if (!m_dbConnected) return;
    try {
        QChartView* chartView = ui->chartView;
        if (!chartView) return;
        QComboBox* chartTypeCombo = ui->resourceChartTypeComboBox;
        QComboBox* filterCombo = ui->resourceChartFilterComboBox;
        QCheckBox* legendCheck = ui->resourceToggleLegendCheckBox;
        QString chartType = chartTypeCombo ? chartTypeCombo->currentText() : "Pie Chart";
        QString filter = filterCombo ? filterCombo->currentText() : "Type";
        bool showLegend = legendCheck ? legendCheck->isChecked() : true;
        QChart* chart = chartView->chart();
        if (!chart) return;
        chart->removeAllSeries();
        QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis* axis : axes) { chart->removeAxis(axis); delete axis; }
        chart->setTitle(QString("Resource Statistics by %1").arg(filter));
        chart->setAnimationOptions(QChart::SeriesAnimations); // Enable animation
        chart->legend()->setVisible(showLegend);
        QMap<QString, int> data;
        // Fix: Always query the actual RESOURCES table columns
        if (filter == "Type") {
            QSqlQuery query("SELECT TYPE, COUNT(*) as COUNT FROM RESOURCES GROUP BY TYPE");
            while (query.next()) {
                QString type = query.value(0).toString();
                int count = query.value(1).toInt();
                data[type] = count;
            }
        } else if (filter == "Purchase Date") {
            QSqlQuery query("SELECT TO_CHAR(PURCHASE_DATE, 'YYYY-MM') as MONTH, COUNT(*) as COUNT FROM RESOURCES GROUP BY TO_CHAR(PURCHASE_DATE, 'YYYY-MM') ORDER BY MONTH");
            while (query.next()) {
                QString month = query.value(0).toString();
                int count = query.value(1).toInt();
                data[month] = count;
            }
        }
        if (data.isEmpty()) {
            chart->setTitle("No resource data available");
            if (ui->resourceHoverLabel) ui->resourceHoverLabel->setText("No data");
            return;
        }
        if (chartType == "Pie Chart") {
            QPieSeries *series = new QPieSeries();
            int total = 0;
            for (auto it = data.begin(); it != data.end(); ++it) total += it.value();
            for (auto it = data.begin(); it != data.end(); ++it) {
                QPieSlice *slice = series->append(it.key(), it.value());
                slice->setLabelVisible(true);
                slice->setLabel(QString("%1: %2 (%3%)").arg(it.key()).arg(it.value()).arg(100.0 * it.value() / total, 0, 'f', 1));
                connect(slice, &QPieSlice::hovered, [this, slice](bool hovered) {
                    if (hovered) {
                        slice->setExploded(true);
                        ui->resourceHoverLabel->setText(slice->label());
                    } else {
                        slice->setExploded(false);
                        ui->resourceHoverLabel->setText("Hover over a chart element to see details");
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
        } else if (chartType == "Bar Chart") {
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Resources");
            QStringList categories;
            for (auto it = data.begin(); it != data.end(); ++it) {
                *set << it.value();
                categories << it.key();
            }
            series->append(set);
            chart->addSeries(series);
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            chart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);
            QValueAxis *axisY = new QValueAxis();
            int maxValue = 0;
            for (auto it = data.begin(); it != data.end(); ++it) maxValue = qMax(maxValue, it.value());
            axisY->setRange(0, maxValue * 1.1);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
            connect(series, &QBarSeries::hovered, [this, categories](bool status, int index, QBarSet *barset) {
                if (status && index >= 0 && index < categories.size()) {
                    ui->resourceHoverLabel->setText(QString("%1: %2 resources").arg(categories.at(index)).arg(barset->at(index)));
                } else {
                    ui->resourceHoverLabel->setText("Hover over a chart element to see details");
                }
            });
        }
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setRubberBand(QChartView::RectangleRubberBand);
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateResourceChart:" << e.what();
        QMessageBox::warning(this, "Chart Error", "Failed to update resource chart: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateResourceChart";
        QMessageBox::warning(this, "Chart Error", "Unknown error updating resource chart");
    }
}

// Add this new helper method to improve all table displays
void MainWindow::improveTableDisplay(QTableView* tableView)
{
    if (!tableView) return;
    
    // Set row height to be more spacious
    tableView->verticalHeader()->setDefaultSectionSize(40);
    
    // Set better font for readability
    QFont tableFont = tableView->font();
    tableFont.setPointSize(10);
    tableView->setFont(tableFont);
    
    // Make sure headers are visible and readable
    tableView->horizontalHeader()->setVisible(true);
    tableView->horizontalHeader()->setFont(tableFont);
    
    // Set better cell padding
    tableView->setStyleSheet(
        "QTableView {"
        "    gridline-color: #E5E5E5;"
        "    selection-background-color: #A1B8E6;"
        "    selection-color: #333333;"
        "}"
        "QTableView::item {"
        "    padding: 5px 8px;"
        "    border: none;"
        "}"
        "QTableView::item:selected {"
        "    background-color: #A1B8E6;"
        "    color: #333333;"
        "}"
        "QHeaderView::section {"
        "    background-color: #3A5DAE;"
        "    color: white;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "    border: none;"
        "}"
    );
    
    // Optimiser la largeur des colonnes spécifiquement pour la table des employés
    if (tableView->model() && tableView->model()->columnCount() >= 14) {
        if (tableView->model()->headerData(1, Qt::Horizontal).toString() == "CIN" &&
            tableView->model()->headerData(2, Qt::Horizontal).toString() == "Last Name") {

            // Configurer la largeur des colonnes pour qu'elles s'adaptent au contenu
            tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
            tableView->horizontalHeader()->setStretchLastSection(true);

            // Définir des largeurs optimales pour chaque colonne
            tableView->setColumnWidth(0, 40);   // ID
            tableView->setColumnWidth(1, 80);   // CIN
            tableView->setColumnWidth(2, 120);  // Last Name
            tableView->setColumnWidth(3, 120);  // First Name
            tableView->setColumnWidth(4, 100);  // Date of Birth
            tableView->setColumnWidth(5, 100);  // Phone
            tableView->setColumnWidth(6, 150);  // Email
            tableView->setColumnWidth(7, 70);   // Gender
            tableView->setColumnWidth(8, 70);   // Salary
            tableView->setColumnWidth(9, 100);  // Date of Hiring
            tableView->setColumnWidth(10, 120); // Speciality
            tableView->setColumnWidth(11, 80);  // Image
            tableView->setColumnWidth(12, 80);  // Role
            tableView->setColumnWidth(13, 80);  // RFID UID

            // Hide the IMAGE column
            tableView->setColumnHidden(11, true);
        } else {
            // Configuration par défaut si ce n'est pas la table des employés
            tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
            tableView->horizontalHeader()->setStretchLastSection(true);
            tableView->horizontalHeader()->setDefaultSectionSize(150);
            tableView->horizontalHeader()->setMinimumSectionSize(100);
        }
    } else {
        // Configuration par défaut pour les autres tableaux
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->horizontalHeader()->setDefaultSectionSize(150);
        tableView->horizontalHeader()->setMinimumSectionSize(100);
    }
    
    // Donner au tableau une largeur minimale pour éviter qu'il ne soit trop à l'étroit
    int columnCount = tableView->model() ? tableView->model()->columnCount() : 6;
    tableView->setMinimumWidth(columnCount * 120); // Largeur minimale basée sur les colonnes
}

// Similar method for QTableWidget
void MainWindow::improveTableWidgetDisplay(QTableWidget* tableWidget)
{
    if (!tableWidget) return;
    
    // Set row height to be more spacious
    tableWidget->verticalHeader()->setDefaultSectionSize(40);
    
    // Set better font for readability
    QFont tableFont = tableWidget->font();
    tableFont.setPointSize(10);
    tableWidget->setFont(tableFont);
    
    // Make sure headers are visible and readable
    tableWidget->horizontalHeader()->setVisible(true);
    tableWidget->horizontalHeader()->setFont(tableFont);
    tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    tableWidget->horizontalHeader()->setMinimumSectionSize(100);
    
    // Set better cell padding
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #E5E5E5;"
        "    selection-background-color: #A1B8E6;"
        "    selection-color: #333333;"
        "}"
        "QTableWidget::item {"
        "    padding: 5px 8px;"
        "    border: none;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #A1B8E6;"
        "    color: #333333;"
        "}"
        "QHeaderView::section {"
        "    background-color: #3A5DAE;"
        "    color: white;"
        "    padding: 8px;"
        "    font-weight: bold;"
        "    border: none;"
        "}"
    );
    
    // Ensure reasonable column sizing
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    
    // Give the table some minimum width to prevent it from being too cramped
    tableWidget->setMinimumWidth(tableWidget->columnCount() * 150); // Minimum width based on columns
}

void MainWindow::on_employeeSearchChanged(const QString &text)
{
    QString criteria = ui->searchCriteriaComboBox->currentText();
    QSqlQueryModel* model = employeeManager->searchEmployees(criteria, text);
    ui->tableView->setModel(model);
    improveTableDisplay(ui->tableView);
}

void MainWindow::on_resetSearchButton_clicked()
{
    ui->searchInput->clear();
    QSqlQueryModel* model = employeeManager->getAllEmployees();
    ui->tableView->setModel(model);
    improveTableDisplay(ui->tableView);
}

void MainWindow::on_addButton_clicked()
{
    // Validation des champs de saisie
    if (!validateEmployeeInput()) {
        return;
    }
    
    QString cin = ui->lineEdit_CIN->text();
    QString lastName = ui->lineEdit_Nom->text();
    QString firstName = ui->lineEdit_Prenom->text();
    QDate dateOfBirth = ui->dateEdit_birth->date();
    QString phoneNumber = ui->lineEdit_phone->text();
    QString email = ui->lineEdit_email->text();
    QString gender = ui->radioButton_H->isChecked() ? "Homme" : "Femme";
    int salary = ui->lineEdit_salaire->text().toInt();
    QDate dateOfHire = ui->dateEdit_hiring->date();
    QString field = ui->comboBox->currentText();
    QString imagePath = ui->imagePathLineEdit->text();
    QString role = ui->roleComboBox->currentText();

    bool success = employeeManager->addEmployee(cin, lastName, firstName, dateOfBirth, phoneNumber, 
                                               email, gender, salary, dateOfHire, field, imagePath, role);
    
    if (success) {
        QMessageBox::information(this, "Success", "Employee added successfully!");
        
        // Clear fields after adding
        ui->lineEdit_CIN->clear();
        ui->lineEdit_Nom->clear();
        ui->lineEdit_Prenom->clear();
        ui->dateEdit_birth->setDate(QDate::currentDate());
        ui->lineEdit_phone->clear();
        ui->lineEdit_email->clear();
        ui->radioButton_H->setChecked(true);
        ui->lineEdit_salaire->clear();
        ui->dateEdit_hiring->setDate(QDate::currentDate());
        ui->imagePathLineEdit->clear();
        
        // Refresh the table
        QSqlQueryModel* model = employeeManager->getAllEmployees();
        ui->tableView->setModel(model);
        improveTableDisplay(ui->tableView);
        
        // Add notification
        notificationManager->addNotification("Employee Added", "New employee: " + firstName + " " + lastName, 
                                           "Added by " + QString::number(QDateTime::currentDateTime().toSecsSinceEpoch()), -1);
    } else {
        QMessageBox::critical(this, "Error", "Failed to add employee. Check the fields and try again.");
    }
}

void MainWindow::on_deleteBtn_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Error", "Please select an employee to delete.");
        return;
    }
    
    int row = currentIndex.row();
    int employeeId = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
    QString employeeName = ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString() + " " +
                          ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toString();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete", 
        "Are you sure you want to delete employee " + employeeName + "?", 
        QMessageBox::Yes | QMessageBox::No);
        
    if (reply == QMessageBox::Yes) {
        bool success = employeeManager->deleteEmployee(employeeId);
        if (success) {
            QMessageBox::information(this, "Success", "Employee deleted successfully!");
            
            // Refresh the table
            QSqlQueryModel* model = employeeManager->getAllEmployees();
            ui->tableView->setModel(model);
            improveTableDisplay(ui->tableView);
            
            // Add notification
            notificationManager->addNotification("Employee Deleted", "Deleted: " + employeeName, 
                                             "Deleted at " + QDateTime::currentDateTime().toString(), -1);
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete employee. Please try again.");
        }
    }
}

void MainWindow::on_modifyBtn_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Error", "Please select an employee to update.");
        return;
    }
    
    int row = currentIndex.row();
    int employeeId = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();
    
    // Open update dialog with selected employee ID
    UpdateEmployeeDialog updateDialog(employeeId, this);
    if (updateDialog.exec() == QDialog::Accepted) {
        // Refresh the table after update
        QSqlQueryModel* model = employeeManager->getAllEmployees();
        ui->tableView->setModel(model);
        improveTableDisplay(ui->tableView);
        
        QString employeeName = ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString() + " " +
                              ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toString();
                              
        // Add notification
        notificationManager->addNotification("Employee Updated", "Updated: " + employeeName, 
                                         "Updated at " + QDateTime::currentDateTime().toString(), -1);
    }
}

void MainWindow::on_downloadBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Employee List", "", "CSV Files (*.csv);;Text Files (*.txt)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return;
    }
    
    QTextStream out(&file);
    
    // Write header
    QStringList headers;
    QAbstractItemModel* model = ui->tableView->model();
    for (int column = 0; column < model->columnCount(); ++column) {
        headers << model->headerData(column, Qt::Horizontal).toString();
    }
    out << headers.join(",") << "\n";
    
    // Write data
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int column = 0; column < model->columnCount(); ++column) {
            QString data = model->data(model->index(row, column)).toString();
            // Escape commas by enclosing in quotes
            if (data.contains(',')) {
                data = "\"" + data + "\"";
            }
            rowData << data;
        }
        out << rowData.join(",") << "\n";
    }
    
    file.close();
    QMessageBox::information(this, "Export Successful", "Employee data exported successfully!");
}

void MainWindow::on_selectImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Employee Image", "", 
                                                  "Image Files (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        ui->imagePathLineEdit->setText(fileName);
    }
}

void MainWindow::on_generateQRCodeBtn_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Error", "Please select an employee.");
        return;
    }
    
    int row = currentIndex.row();
    QString employeeId = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();
    QString employeeName = ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString() + " " +
                          ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toString();
    QString employeeCIN = ui->tableView->model()->data(ui->tableView->model()->index(row, 1)).toString();
    
    // Create QR Code content
    QString qrContent = "ID: " + employeeId + "\n"
                       + "Name: " + employeeName + "\n"
                       + "CIN: " + employeeCIN;
    
    // Generate QR code using the qrcodegen library
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(qrContent.toUtf8().constData(), qrcodegen::QrCode::Ecc::MEDIUM);
    
    // Convert to QImage
    const int qrSize = qr.getSize();
    const int scale = 10;  // Scale factor for better visibility
    QImage image(qrSize * scale, qrSize * scale, QImage::Format_RGB32);
    image.fill(Qt::white);
    
    QPainter painter(&image);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    
    for (int y = 0; y < qrSize; ++y) {
        for (int x = 0; x < qrSize; ++x) {
            if (qr.getModule(x, y)) {
                painter.fillRect(x * scale, y * scale, scale, scale, Qt::black);
            }
        }
    }
    
    // Save QR code image
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save QR Code", 
                                                     "employee_" + employeeId + "_qr.png", 
                                                     "PNG Files (*.png)");
    if (!saveFileName.isEmpty()) {
        if (image.save(saveFileName)) {
            QMessageBox::information(this, "Success", "QR Code saved successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Failed to save QR Code image.");
        }
    }
}

bool MainWindow::validateEmployeeInput()
{
    // CIN validation: Should be 8 digits
    QString cin = ui->lineEdit_CIN->text();
    QRegularExpression cinRegex("^\\d{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "CIN must be exactly 8 digits.");
        return false;
    }
    
    // Name validation: Should not be empty and contain only letters
    QString lastName = ui->lineEdit_Nom->text();
    QString firstName = ui->lineEdit_Prenom->text();
    QRegularExpression nameRegex("^[A-Za-z\\s]+$");
    if (lastName.isEmpty() || !nameRegex.match(lastName).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Last name should contain only letters.");
        return false;
    }
    if (firstName.isEmpty() || !nameRegex.match(firstName).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "First name should contain only letters.");
        return false;
    }
    
    // Phone validation: Should be a valid phone number
    QString phone = ui->lineEdit_phone->text();
    QRegularExpression phoneRegex("^\\d{8}$");
    if (!phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Phone number must be 8 digits.");
        return false;
    }
    
    // Email validation
    QString email = ui->lineEdit_email->text();
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid email address.");
        return false;
    }
    
    // Salary validation: Should be a positive number
    QString salaryText = ui->lineEdit_salaire->text();
    bool ok;
    int salary = salaryText.toInt(&ok);
    if (!ok || salary <= 0) {
        QMessageBox::warning(this, "Input Error", "Salary must be a positive number.");
        return false;
    }
    
    // Date validations
    QDate birthDate = ui->dateEdit_birth->date();
    QDate hiringDate = ui->dateEdit_hiring->date();
    QDate currentDate = QDate::currentDate();
    
    // Birth date should be in the past and reasonable (at least 18 years old)
    if (birthDate > currentDate.addYears(-18)) {
        QMessageBox::warning(this, "Input Error", "Employee must be at least 18 years old.");
        return false;
    }
    
    // Hiring date should not be before birth date + 18 years
    QDate minHiringDate = birthDate.addYears(18);
    if (hiringDate < minHiringDate) {
        QMessageBox::warning(this, "Input Error", "Hiring date cannot be before employee turned 18.");
        return false;
    }
    
    // Hiring date should not be in the future
    if (hiringDate > currentDate) {
        QMessageBox::warning(this, "Input Error", "Hiring date cannot be in the future.");
        return false;
    }
    
    return true;
}

void MainWindow::on_resourceSectionButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget(ui->resourcePage);
    
    // Check if the table widget exists before setting up
    if (ui->tableWidget) {
        // Setup the resource table
        resourceManager->setupTable(ui->tableWidget);
        
        // Apply improved table styling for better readability
        improveTableWidgetDisplay(ui->tableWidget);
        
        // Connect table selection to update form fields
        connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::on_btnLookForResource_clicked);
        
        // Setup search functionality
        connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_searchTextChanged);
        connect(ui->resourceSearchColumnComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_resourceSearchColumnChanged);
        
        // Refresh statistics if needed
        updateResourceChart();
    } else {
        QMessageBox::warning(this, "UI Error", "Resource table widget not found. Please check the UI setup.");
    }
}

void MainWindow::setupResourceChart()
{
    if (!m_dbConnected) return;
    
    // Create a default chart for resources
    QChart *chart = new QChart();
    chart->setTitle("Resource Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    // Check if resourceChartView exists in the UI before using it
    // This will prevent the "no member named 'resourceChartView'" error
    QChartView* resourceChartView = ui->mainStackedWidget->findChild<QChartView*>("resourceChartView");
    if (resourceChartView) {
        resourceChartView->setChart(chart);
        resourceChartView->setRenderHint(QPainter::Antialiasing);
        resourceChartView->setRubberBand(QChartView::RectangleRubberBand);
        
        // Update with initial data
        updateResourceChart();
    } else {
        qDebug() << "Resource chart view not found in UI";
    }
}

void MainWindow::on_confirmFormButton_clicked()
{
    // Validate inputs
    QString name = ui->nameLineEdit->text().trimmed();
    QString type = ui->typeComboBox->currentText().trimmed();
    QString brand = ui->brandLineEdit->text().trimmed();
    QString quantityText = ui->quantityLineEdit->text().trimmed();
    QDate purchaseDate = ui->purchaseDateEdit->date();
    
    // Check required fields
    if (name.isEmpty() || type.isEmpty() || brand.isEmpty() || quantityText.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please fill in all required fields.");
        return;
    }
    
    // Convert quantity to int
    bool ok;
    int quantity = quantityText.toInt(&ok);
    if (!ok || quantity <= 0) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid quantity (positive number).");
        return;
    }
    
    // Check purchase date is not in the future
    if (purchaseDate > QDate::currentDate()) {
        QMessageBox::warning(this, "Validation Error", "Purchase date cannot be in the future.");
        return;
    }
    
    // Add resource
    bool success = resourceManager->addResource(name, type, brand, quantity, purchaseDate, imageData);
    
    if (success) {
        QMessageBox::information(this, "Success", "Resource added successfully");
        
        // Clear form
        ui->nameLineEdit->clear();
        ui->typeComboBox->setCurrentIndex(0);
        ui->brandLineEdit->clear();
        ui->quantityLineEdit->setText("1");
        ui->purchaseDateEdit->setDate(QDate::currentDate());
        ui->lblImagePreview->clear();
        imageData.clear();
        
        // Refresh table
        resourceManager->updateTable(ui->tableWidget);
        
        // Update statistics
        updateResourceChart();
        
        // Add notification
        notificationManager->addNotification(
            "Resource Added", 
            "New resource: " + name, 
            "Added at " + QDateTime::currentDateTime().toString(),
            -1
        );
    } else {
        QMessageBox::critical(this, "Error", "Failed to add resource. Check the database connection.");
    }
}

void MainWindow::on_btnSelectImage_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select Resource Image",
        "",
        "Image Files (*.png *.jpg *.jpeg *.bmp)"
    );
    
    if (!filePath.isEmpty()) {
        // Load image data
        imageData = ResourceManager::loadImageData(filePath);
        
        if (!imageData.isEmpty()) {
            // Display image preview
            QPixmap pixmap = ResourceManager::byteArrayToPixmap(imageData);
            if (!pixmap.isNull()) {
                ui->lblImagePreview->setPixmap(pixmap.scaled(200, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                ui->lblImagePreview->setText("Invalid image format");
                imageData.clear();
            }
        } else {
            ui->lblImagePreview->setText("Failed to load image");
        }
    }
}

void MainWindow::on_cancelFormButton_clicked()
{
    // Clear form
    ui->nameLineEdit->clear();
    ui->typeComboBox->setCurrentIndex(0);
    ui->brandLineEdit->clear();
    ui->quantityLineEdit->setText("1");
    ui->purchaseDateEdit->setDate(QDate::currentDate());
    ui->lblImagePreview->clear();
    imageData.clear();
    
    // Deselect any selected row
    if (ui->tableWidget) {
        ui->tableWidget->clearSelection();
    }
    
    // Reset the selected resource id
    selectedResourceId = -1;
}

void MainWindow::on_updateButton_clicked()
{
    // Check if a resource is selected
    if (selectedResourceId <= 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a resource to update.");
        return;
    }
    
    // Open update dialog with the correct class name
    UpdateResourceDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Refresh table
        resourceManager->updateTable(ui->tableWidget);
        
        // Update statistics
        updateResourceChart();
        
        // Add notification
        notificationManager->addNotification(
            "Resource Updated",
            "Updated resource ID: " + QString::number(selectedResourceId),
            "Updated at " + QDateTime::currentDateTime().toString(),
            -1
        );
    }
}

void MainWindow::on_deleteButton_clicked()
{
    // Check if a resource is selected
    if (selectedResourceId <= 0) {
        QMessageBox::warning(this, "Selection Error", "Please select a resource to delete.");
        return;
    }
    
    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Delete",
        "Are you sure you want to delete this resource?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Delete the resource
        if (resourceManager->deleteResource(selectedResourceId)) {
            QMessageBox::information(this, "Success", "Resource deleted successfully");
            
            // Refresh table
            resourceManager->updateTable(ui->tableWidget);
            
            // Clear selection
            selectedResourceId = -1;
            
            // Update statistics
            updateResourceChart();
            
            // Clear form
            on_cancelFormButton_clicked();
            
            // Add notification
            notificationManager->addNotification(
                "Resource Deleted",
                "Deleted resource ID: " + QString::number(selectedResourceId),
                "Deleted at " + QDateTime::currentDateTime().toString(),
                -1
            );
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete resource. Check the database connection.");
        }
    }
}

void MainWindow::on_exportPdfButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export Resources to PDF",
        "",
        "PDF Files (*.pdf)"
    );
    
    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
            filePath += ".pdf";
        }
        
        if (resourceManager->exportToPdf(filePath)) {
            QMessageBox::information(this, "Success", "Resources exported to PDF successfully");
        } else {
            QMessageBox::critical(this, "Error", "Failed to export resources to PDF");
        }
    }
}

void MainWindow::on_searchTextChanged(const QString &text)
{
    if (searchTimer) {
        searchTimer->stop();
        
        if (!text.isEmpty()) {
            searchTimer->start(300);  // Delay search by 300ms to avoid frequent queries
        } else {
            resourceManager->updateTable(ui->tableWidget);
        }
    }
}

void MainWindow::on_searchTimeout()
{
    QString searchText = ui->searchLineEdit->text().trimmed();
    QString column = ui->resourceSearchColumnComboBox->currentText();
    resourceManager->updateTable(ui->tableWidget, searchText, column);
}

void MainWindow::on_resetResourceSearchButton_clicked()
{
    ui->searchLineEdit->clear();
    resourceManager->updateTable(ui->tableWidget);
}

void MainWindow::on_downloadHistoryButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save History Log",
        "",
        "Text Files (*.txt)"
    );
    
    if (!filePath.isEmpty()) {
        if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
            filePath += ".txt";
        }
        
        if (resourceManager->exportHistoryToFile(filePath)) {
            QMessageBox::information(this, "Success", "Resource history exported successfully");
        } else {
            QMessageBox::critical(this, "Error", "Failed to export resource history. The history file may not exist.");
        }
    }
}

void MainWindow::on_clearHistoryButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Clear History",
        "Are you sure you want to clear the resource history? This action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (resourceManager->clearHistory()) {
            QMessageBox::information(this, "Success", "Resource history cleared successfully");
        } else {
            QMessageBox::critical(this, "Error", "Failed to clear resource history");
        }
    }
}

void MainWindow::on_btnLookForResource_clicked()
{
    QTableWidget *tableWidget = ui->tableWidget;
    if (!tableWidget) return;
    
    // Get selected row
    QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        selectedResourceId = -1;
        return;
    }
    
    // Get resource ID from first column of selected row
    int row = selectedItems.first()->row();
    QTableWidgetItem *idItem = tableWidget->item(row, 0);
    
    if (!idItem) return;
    
    // Get the resource ID
    selectedResourceId = idItem->text().toInt();
    
    // Get resource data to populate the form
    QString name = tableWidget->item(row, 1)->text();
    QString type = tableWidget->item(row, 2)->text();
    QString brand = tableWidget->item(row, 3)->text();
    int quantity = tableWidget->item(row, 4)->text().toInt();
    QDate purchaseDate = QDate::fromString(tableWidget->item(row, 5)->text(), "yyyy-MM-dd");
    
    // Update form with selected resource
    ui->nameLineEdit->setText(name);
    
    // Find and set the type in comboBox
    int typeIndex = ui->typeComboBox->findText(type);
    if (typeIndex >= 0) {
        ui->typeComboBox->setCurrentIndex(typeIndex);
    } else {
        ui->typeComboBox->setCurrentText(type);
    }
    
    ui->brandLineEdit->setText(brand);
    ui->quantityLineEdit->setText(QString::number(quantity));
    ui->purchaseDateEdit->setDate(purchaseDate);
    
    // Show image if available
    QLabel *imageLabel = qobject_cast<QLabel*>(tableWidget->cellWidget(row, 6));
    if (imageLabel && !imageLabel->pixmap().isNull()) {
        ui->lblImagePreview->setPixmap(imageLabel->pixmap());
    } else {
        ui->lblImagePreview->clear();
    }
}

void MainWindow::setLoggedInRole(const QString &role)
{
    m_loggedInRole = role;
    
    // Update UI based on role (e.g., disable certain sections for specific roles)
    bool isAdmin = (role == "Admin");
    
    // Example: Allow only admins to access the employee section
    if (ui->employeeSectionButton) {
        ui->employeeSectionButton->setEnabled(isAdmin);
    }
    
    // Display the role in the status bar
    statusBar()->showMessage(QString("Logged in as: %1").arg(role));
}

void MainWindow::on_employeeChartRefreshButton_clicked()
{
    updateEmployeeChart();
}

void MainWindow::on_employeeChartTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateEmployeeChart();
}

void MainWindow::on_employeeChartFilterComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    updateEmployeeChart();
}

void MainWindow::on_employeeToggleLegendCheckBox_toggled(bool checked)
{
    Q_UNUSED(checked);
    updateEmployeeChart();
}

void MainWindow::on_resourceSearchColumnChanged(int index)
{
    Q_UNUSED(index);
    on_searchTimeout(); // Re-filter when column changes
}
