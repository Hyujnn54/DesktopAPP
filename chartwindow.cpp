#include "ChartWindow.h"
#include "ui_ChartWindow.h"
#include <QDebug>

ChartWindow::ChartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentBarSet(nullptr),
    hoveredBarIndex(-1)
{
    ui->setupUi(this);

    QSqlQueryModel *testModel = Etmp.afficher();
    qDebug() << "Initial Client Data Row Count:" << testModel->rowCount();

    connect(ui->refreshChartButton, &QPushButton::clicked, this, &ChartWindow::on_refreshChartButton_clicked);
    connect(ui->statsFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::on_statsFilterComboBox_currentIndexChanged);
    connect(ui->chartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::on_chartTypeComboBox_currentIndexChanged);
    connect(ui->resetChartButton, &QPushButton::clicked, this, &ChartWindow::on_resetChartButton_clicked);

    currentChartType = ui->chartTypeComboBox->currentText();
    populateFilterValues();
    updateChart();
}

ChartWindow::~ChartWindow()
{
    delete ui;
}

void ChartWindow::updateChart()
{
    QString filterType = ui->statsFilterComboBox->currentText();
    QString filterValue = ui->filterValueComboBox->currentText();
    qDebug() << "Filter Type:" << filterType << "Filter Value:" << filterValue << "Chart Type:" << currentChartType;

    currentDataMap.clear();
    if (filterType == "By Sector") {
        QSqlQueryModel *model = filterValue.isEmpty() ? Etmp.afficher() : Etmp.searchBySector(filterValue);
        qDebug() << "Sector Model Row Count:" << model->rowCount();
        for (int row = 0; row < model->rowCount(); ++row) {
            QString sector = model->data(model->index(row, 1)).toString();
            currentDataMap[sector] += 1;
        }
    } else if (filterType == "By Date") {
        QDate date = QDate::fromString(filterValue, "yyyy-MM-dd");
        if (date.isValid()) {
            QSqlQueryModel *model = Etmp.getConsultationsForDate(date);
            qDebug() << "Date Model Row Count:" << model->rowCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString name = model->data(model->index(row, 0)).toString();
                currentDataMap[name] += 1;
            }
        } else {
            qDebug() << "Invalid date:" << filterValue;
        }
    } else if (filterType == "By Consultant") {
        bool ok;
        int consultantId = filterValue.toInt(&ok);
        if (ok) {
            QSqlQueryModel *model = Etmp.searchByConsultant(consultantId);
            qDebug() << "Consultant Model Row Count:" << model->rowCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString name = model->data(model->index(row, 0)).toString();
                currentDataMap[name] += 1;
            }
        } else {
            qDebug() << "Invalid consultant ID:" << filterValue;
        }
    }

    qDebug() << "DataMap Contents:" << currentDataMap;
    if (currentDataMap.isEmpty()) {
        qDebug() << "Error: DataMap is empty, no chart will be generated.";
        ui->chartDetailsLabel->setText("No data available");
        return;
    }

    QChart *chart = new QChart();
    chart->setTitle("Number of Consultations per " + filterType.split(" ").last().toLower());
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor(0, 128, 0, 100)));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor(0, 128, 0, 100)));
    chart->setPlotAreaBackgroundVisible(true);

    if (currentChartType == "Bar Chart") {
        qDebug() << "Creating Bar Chart...";
        QBarSeries *series = new QBarSeries();
        currentBarSet = new QBarSet("Consultations");

        for (auto it = currentDataMap.begin(); it != currentDataMap.end(); ++it) {
            *currentBarSet << it.value();
        }
        currentBarSet->setBrush(QBrush(Qt::blue));
        series->append(currentBarSet);
        qDebug() << "BarSet Count:" << currentBarSet->count();

        chart->addSeries(series);

        QStringList categories = currentDataMap.keys();
        qDebug() << "Categories:" << categories;

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (const int &value : currentDataMap.values()) {
            if (value > maxValue) maxValue = value;
        }
        maxValue = qMax(1, maxValue + 1);
        qDebug() << "Max Value for Y-Axis:" << maxValue;
        axisY->setRange(0, maxValue);
        axisY->setLabelFormat("%d");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Connect hover signal
        connect(series, &QBarSeries::hovered, this, &ChartWindow::on_barHovered);
    } else if (currentChartType == "Pie Chart") {
        qDebug() << "Creating Pie Chart...";
        QPieSeries *series = new QPieSeries();

        int total = 0;
        for (const int &value : currentDataMap.values()) {
            total += value;
        }

        for (auto it = currentDataMap.begin(); it != currentDataMap.end(); ++it) {
            qreal percentage = (it.value() * 100.0) / total;
            QStringList words = it.key().split(" ");
            QString abbreviation;
            for (const QString &word : words) {
                if (!word.isEmpty()) abbreviation += word[0].toUpper();
            }
            QString label = QString("%1% %2").arg(percentage, 0, 'f', 1).arg(abbreviation);
            QPieSlice *slice = series->append(label, it.value());
            slice->setLabelVisible(true);
        }
        qDebug() << "Pie slices added:" << series->count();

        chart->addSeries(series);

        // Connect hover signal
        connect(series, &QPieSeries::hovered, this, &ChartWindow::on_pieSliceHovered);
    }

    ui->statsChartView->setChart(chart);
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);
    ui->chartDetailsLabel->setText("Hover over a chart element for details");
    qDebug() << "Chart updated and set to view";
}

void ChartWindow::populateFilterValues()
{
    ui->filterValueComboBox->clear();
    QString filterType = ui->statsFilterComboBox->currentText();

    if (filterType == "By Sector") {
        QSqlQuery query("SELECT DISTINCT SECTOR FROM Clients");
        while (query.next()) {
            ui->filterValueComboBox->addItem(query.value(0).toString());
        }
    } else if (filterType == "By Date") {
        QSqlQuery query("SELECT DISTINCT CONSULTATION_DATE FROM Clients");
        while (query.next()) {
            ui->filterValueComboBox->addItem(query.value(0).toDateTime().toString("yyyy-MM-dd"));
        }
    } else if (filterType == "By Consultant") {
        QSqlQuery query("SELECT DISTINCT CONSULTANT FROM Clients");
        while (query.next()) {
            ui->filterValueComboBox->addItem(query.value(0).toString());
        }
    }
    qDebug() << "Filter Values Populated for" << filterType << ":" << ui->filterValueComboBox->count() << "items";
}

void ChartWindow::on_refreshChartButton_clicked()
{
    updateChart();
}

void ChartWindow::on_statsFilterComboBox_currentIndexChanged(int index)
{
    populateFilterValues();
    updateChart();
}

void ChartWindow::on_chartTypeComboBox_currentIndexChanged(int index)
{
    currentChartType = ui->chartTypeComboBox->currentText();
    updateChart();
}

void ChartWindow::on_resetChartButton_clicked()
{
    qDebug() << "Resetting chart to default...";
    ui->statsFilterComboBox->setCurrentIndex(0); // "By Sector"
    ui->filterValueComboBox->setCurrentIndex(-1); // Clear selection
    ui->chartTypeComboBox->setCurrentIndex(0); // "Bar Chart"
    currentChartType = "Bar Chart";
    populateFilterValues();
    updateChart();
}

void ChartWindow::on_pieSliceHovered(QPieSlice *slice, bool state)
{
    if (!slice) return;

    slice->setExploded(state);
    if (state) {
        QString fullName;
        int value = slice->value();
        for (auto it = currentDataMap.begin(); it != currentDataMap.end(); ++it) {
            if (it.value() == value && slice->label().contains(it.key().split(" ")[0][0])) {
                fullName = it.key();
                break;
            }
        }
        ui->chartDetailsLabel->setText(QString("Category: %1\nConsultations: %2").arg(fullName).arg(value));
    } else {
        ui->chartDetailsLabel->setText("Hover over a chart element for details");
    }
}

void ChartWindow::on_barHovered(bool status, int index)
{
    if (!currentBarSet || index < 0 || index >= currentBarSet->count()) return;

    QChart *chart = ui->statsChartView->chart();
    if (status) {
        // Disable animations to prevent reset
        chart->setAnimationOptions(QChart::NoAnimation);

        // Store the original value and increase the height slightly to "pop out"
        qreal originalValue = currentBarSet->at(index);
        currentBarSet->replace(index, originalValue + 0.2); // Add a small increase

        // Highlight by changing the pen color (outline) instead of brush
        QPen pen = currentBarSet->pen();
        pen.setColor(Qt::cyan);
        pen.setWidth(2); // Thicker outline
        currentBarSet->setPen(pen);

        hoveredBarIndex = index;

        QString category = currentDataMap.keys().at(index);
        int value = qRound(originalValue); // Use original value for display
        ui->chartDetailsLabel->setText(QString("Category: %1\nConsultations: %2").arg(category).arg(value));
    } else {
        // Revert the height
        if (hoveredBarIndex != -1) {
            qreal originalValue = currentBarSet->at(hoveredBarIndex) - 0.2;
            currentBarSet->replace(hoveredBarIndex, originalValue);

            // Revert the pen
            QPen pen = currentBarSet->pen();
            pen.setColor(Qt::black); // Default outline color
            pen.setWidth(0); // Default width
            currentBarSet->setPen(pen);

            hoveredBarIndex = -1;
        }

        // Re-enable animations
        chart->setAnimationOptions(QChart::SeriesAnimations);

        ui->chartDetailsLabel->setText("Hover over a chart element for details");
    }
}
