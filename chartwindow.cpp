#include "ChartWindow.h"
#include "ui_ChartWindow.h"
#include <QDebug>

ChartWindow::ChartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChartWindow)
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

    QMap<QString, int> dataMap;

    if (filterType == "By Sector") {
        QSqlQueryModel *model = filterValue.isEmpty() ? Etmp.afficher() : Etmp.searchBySector(filterValue);
        qDebug() << "Sector Model Row Count:" << model->rowCount();
        for (int row = 0; row < model->rowCount(); ++row) {
            QString sector = model->data(model->index(row, 1)).toString();
            dataMap[sector] += 1;
        }
    } else if (filterType == "By Date") {
        QDate date = QDate::fromString(filterValue, "yyyy-MM-dd");
        if (date.isValid()) {
            QSqlQueryModel *model = Etmp.getConsultationsForDate(date);
            qDebug() << "Date Model Row Count:" << model->rowCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString name = model->data(model->index(row, 0)).toString();
                dataMap[name] += 1;
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
                dataMap[name] += 1;
            }
        } else {
            qDebug() << "Invalid consultant ID:" << filterValue;
        }
    }

    qDebug() << "DataMap Contents:" << dataMap;
    if (dataMap.isEmpty()) {
        qDebug() << "Error: DataMap is empty, no chart will be generated.";
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
        QBarSet *set = new QBarSet("Consultations");

        for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
            *set << it.value();
        }
        series->append(set);
        qDebug() << "BarSet Count:" << set->count();

        chart->addSeries(series);

        QStringList categories = dataMap.keys();
        qDebug() << "Categories:" << categories;

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        int maxValue = 0;
        for (const int &value : dataMap.values()) {
            if (value > maxValue) maxValue = value;
        }
        maxValue = qMax(1, maxValue + 1);
        qDebug() << "Max Value for Y-Axis:" << maxValue;
        axisY->setRange(0, maxValue);
        axisY->setLabelFormat("%d");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    } else if (currentChartType == "Pie Chart") {
        qDebug() << "Creating Pie Chart...";
        QPieSeries *series = new QPieSeries();

        // Calculate total for percentages
        int total = 0;
        for (const int &value : dataMap.values()) {
            total += value;
        }

        for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
            // Calculate percentage
            qreal percentage = (it.value() * 100.0) / total;
            // Abbreviate name (first letter of each word)
            QStringList words = it.key().split(" ");
            QString abbreviation;
            for (const QString &word : words) {
                if (!word.isEmpty()) {
                    abbreviation += word[0].toUpper();
                }
            }
            // Create label with percentage and abbreviation
            QString label = QString("%1% %2").arg(percentage, 0, 'f', 1).arg(abbreviation);
            QPieSlice *slice = series->append(label, it.value());
            slice->setLabelVisible(true); // Show labels on the chart
        }
        qDebug() << "Pie slices added:" << series->count();

        chart->addSeries(series);
    }

    ui->statsChartView->setChart(chart);
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);
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
