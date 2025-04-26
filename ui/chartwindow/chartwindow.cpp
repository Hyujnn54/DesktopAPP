#include "chartwindow.h"
#include "ui_ChartWindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QTimer>

ChartWindow::ChartWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentChart(nullptr),
    tooltipLabel(new QLabel(this)),
    currentBarSet(nullptr)
{
    ui->setupUi(this);

    tooltipLabel->setStyleSheet("QLabel { background-color: white; border: 1px solid black; padding: 5px; }");
    tooltipLabel->hide();

    // Block signals during initialization
    ui->statsTypeComboBox->blockSignals(true);

    connect(ui->refreshButton, &QPushButton::clicked, this, &ChartWindow::updateChart);
    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);
    connect(ui->chartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);
    connect(ui->toggleLegendCheckBox, &QCheckBox::toggled, this, &ChartWindow::updateChart);

    // Make sure we have the Meeting Statistics option
    if (ui->statsTypeComboBox->findText("Meeting Statistics") == -1) {
        ui->statsTypeComboBox->addItem("Meeting Statistics");
    }

    // Re-enable signals after initialization
    ui->statsTypeComboBox->blockSignals(false);
    
    // Initial chart refresh
    QTimer::singleShot(100, this, &ChartWindow::updateChart);
}

ChartWindow::~ChartWindow()
{
    if (tooltipLabel) {
        delete tooltipLabel;
        tooltipLabel = nullptr;
    }
    delete ui;
}

void ChartWindow::populateStatsViews()
{
    ui->statsTypeComboBox->clear();
    ui->statsTypeComboBox->addItems({"Client Statistics", "Training Statistics", "Meeting Statistics"});
    qDebug() << "Populated statsTypeComboBox with" << ui->statsTypeComboBox->count() << "items";
    updateChart();
}

void ChartWindow::updateFilterComboBox()
{
    ui->filterComboBox->clear();
    QString statsType = ui->statsTypeComboBox->currentText();

    if (statsType == "Client Statistics") {
        ui->filterComboBox->addItems({"All Clients", "By Sector", "By Consultation Year"});
    } else if (statsType == "Training Statistics") {
        ui->filterComboBox->addItems({"All Trainings", "By Formation", "By Year"});
    } else if (statsType == "Meeting Statistics") {
        ui->filterComboBox->addItems({"All Meetings", "By Agenda", "By Duration"});
    }
}

void ChartWindow::updateChart()
{
    QString statsType = ui->statsTypeComboBox->currentText();
    QString chartType = ui->chartTypeComboBox->currentText();

    qDebug() << "Starting updateChart() with statsType:" << statsType << "and chartType:" << chartType;

    // Handle empty statsType
    if (statsType.isEmpty()) {
        qDebug() << "Error: statsType is empty, defaulting to 'Client Statistics'";
        statsType = "Client Statistics";
        ui->statsTypeComboBox->setCurrentIndex(0);
    }

    // Create a new chart or reuse the existing one
    if (!currentChart) {
        currentChart = new QChart();
    } else {
        // Clear the existing chart completely
        currentChart->removeAllSeries();
        while (!currentChart->axes().isEmpty()) {
            currentChart->removeAxis(currentChart->axes().first());
        }
        // Remove all items (like "No data available" text)
        if (currentChart->scene()) {
            for (QGraphicsItem *item : currentChart->scene()->items()) {
                if (item->parentItem() == nullptr) { // Only remove top-level items
                    currentChart->scene()->removeItem(item);
                    delete item;
                }
            }
        }
    }

    currentChart->setTitle(statsType);
    currentChart->setAnimationOptions(QChart::SeriesAnimations);

    QSqlQuery query;
    bool dataAvailable = false;

    try {
        if (statsType == "Client Statistics") {
            // Show clients by sector
            qDebug() << "Processing Client Statistics";
            if (!query.exec("SELECT SECTOR, COUNT(*) FROM CLIENTS GROUP BY SECTOR")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve clients by sector: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }

            if (chartType == "Bar Chart") {
                QBarSeries *series = new QBarSeries();
                QBarSet *set = new QBarSet("Clients");
                QStringList categories;
                int maxValue = 0;
                while (query.next()) {
                    QString sector = query.value(0).toString();
                    int count = query.value(1).toInt();
                    qDebug() << "Sector:" << sector << "Count:" << count;
                    *set << count;
                    categories << sector;
                    maxValue = qMax(maxValue, count);
                    dataAvailable = true;
                }
                
                if (dataAvailable) {
                    series->append(set);
                    currentChart->addSeries(series);
                    QBarCategoryAxis *axisX = new QBarCategoryAxis();
                    axisX->append(categories);
                    currentChart->addAxis(axisX, Qt::AlignBottom);
                    series->attachAxis(axisX);
                    QValueAxis *axisY = new QValueAxis();
                    axisY->setRange(0, maxValue > 0 ? maxValue + 1 : 1);
                    currentChart->addAxis(axisY, Qt::AlignLeft);
                    series->attachAxis(axisY);
                }
            } else { // Pie Chart
                QPieSeries *series = new QPieSeries();
                int total = 0;
                query.seek(-1); // Reset query cursor
                while (query.next()) {
                    QString sector = query.value(0).toString();
                    int count = query.value(1).toInt();
                    qDebug() << "Sector:" << sector << "Count:" << count;
                    series->append(sector, count);
                    total += count;
                    dataAvailable = true;
                }
                
                if (dataAvailable) {
                    currentChart->addSeries(series);
                }
            }
        } else if (statsType == "Training Statistics") {
            // Show trainings by formation
            qDebug() << "Processing Training Statistics";
            if (!query.exec("SELECT FORMATION, COUNT(*) FROM FORMATIONS GROUP BY FORMATION")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve trainings by formation: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }

            if (chartType == "Bar Chart") {
                QBarSeries *series = new QBarSeries();
                QBarSet *set = new QBarSet("Trainings");
                QStringList categories;
                int maxValue = 0;
                while (query.next()) {
                    QString formation = query.value(0).toString();
                    int count = query.value(1).toInt();
                    qDebug() << "Formation:" << formation << "Count:" << count;
                    *set << count;
                    categories << formation;
                    maxValue = qMax(maxValue, count);
                    dataAvailable = true;
                }
                
                if (dataAvailable) {
                    series->append(set);
                    currentChart->addSeries(series);
                    QBarCategoryAxis *axisX = new QBarCategoryAxis();
                    axisX->append(categories);
                    currentChart->addAxis(axisX, Qt::AlignBottom);
                    series->attachAxis(axisX);
                    QValueAxis *axisY = new QValueAxis();
                    axisY->setRange(0, maxValue > 0 ? maxValue + 1 : 1);
                    currentChart->addAxis(axisY, Qt::AlignLeft);
                    series->attachAxis(axisY);
                }
            } else { // Pie Chart
                QPieSeries *series = new QPieSeries();
                int total = 0;
                query.seek(-1); // Reset query cursor
                while (query.next()) {
                    QString formation = query.value(0).toString();
                    int count = query.value(1).toInt();
                    qDebug() << "Formation:" << formation << "Count:" << count;
                    series->append(formation, count);
                    total += count;
                    dataAvailable = true;
                }
                
                if (dataAvailable) {
                    currentChart->addSeries(series);
                }
            }
        } else if (statsType == "Meeting Statistics") {
            // Show meetings by duration
            qDebug() << "Processing Meeting Statistics";
            if (!query.exec("SELECT CASE "
                            "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                            "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                            "ELSE 'Long (>60 mins)' END AS RANGE, COUNT(*) "
                            "FROM MEETING GROUP BY CASE "
                            "WHEN DURATION <= 30 THEN 'Short (0-30 mins)' "
                            "WHEN DURATION <= 60 THEN 'Medium (31-60 mins)' "
                            "ELSE 'Long (>60 mins)' END")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve meetings by duration: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }

            if (chartType == "Bar Chart") {
                QBarSeries *series = new QBarSeries();
                QBarSet *set = new QBarSet("Meetings");
                QStringList categories;
                int maxValue = 0;
                while (query.next()) {
                    QString range = query.value(0).toString();
                    int count = query.value(1).toInt();
                    qDebug() << "Duration Range:" << range << "Count:" << count;
                    *set << count;
                    categories << range;
                    maxValue = qMax(maxValue, count);
                    dataAvailable = true;
                }
                
                if (dataAvailable) {
                    series->append(set);
                    currentChart->addSeries(series);
                    QBarCategoryAxis *axisX = new QBarCategoryAxis();
                    axisX->append(categories);
                    currentChart->addAxis(axisX, Qt::AlignBottom);
                    series->attachAxis(axisX);
                    QValueAxis *axisY = new QValueAxis();
                    axisY->setRange(0, maxValue > 0 ? maxValue + 1 : 1);
                    currentChart->addAxis(axisY, Qt::AlignLeft);
                    series->attachAxis(axisY);
                }
            } else { // Pie Chart
                QPieSeries *series = new QPieSeries();
                int total = 0;
                query.seek(-1); // Reset query cursor
                while (query.next()) {
                    QString range = query.value(0).toString();
                    int count = query.value(1).toInt();
                    qDebug() << "Duration Range:" << range << "Count:" << count;
                    series->append(range, count);
                    total += count;
                    dataAvailable = true;
                }
                
                if (dataAvailable) {
                    currentChart->addSeries(series);
                }
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateChart:" << e.what();
        QMessageBox::critical(this, "Error", "An unexpected error occurred: " + QString(e.what()));
    } catch (...) {
        qDebug() << "Unknown exception in updateChart";
        QMessageBox::critical(this, "Error", "An unknown error occurred");
    }

    if (!dataAvailable) {
        qDebug() << "No data available, adding placeholder text";
        QGraphicsTextItem *textItem = new QGraphicsTextItem("No data available", currentChart);
        textItem->setPos(50, 50);
        if (currentChart->scene()) {
            currentChart->scene()->addItem(textItem);
        } else {
            qDebug() << "Error: Chart scene is null";
        }
    }

    for (QAbstractSeries *series : currentChart->series()) {
        if (QPieSeries *pieSeries = dynamic_cast<QPieSeries*>(series)) {
            for (QPieSlice *slice : pieSeries->slices()) {
                slice->setLabel(QString("%1: %2 (%3%)")
                                   .arg(slice->label())
                                   .arg(static_cast<int>(slice->value()))
                                   .arg(slice->percentage() * 100, 0, 'f', 1));
                slice->setLabelVisible();
                connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
                    handlePieSliceHovered(slice, state);
                });
            }
        } else if (QBarSeries *barSeries = dynamic_cast<QBarSeries*>(series)) {
            for (QBarSet *set : barSeries->barSets()) {
                currentBarSet = set;
                connect(set, &QBarSet::hovered, this, &ChartWindow::handleBarHovered);
            }
        }
    }

finalizeChart:
    currentChart->legend()->setVisible(ui->toggleLegendCheckBox->isChecked());
    currentChart->legend()->setAlignment(Qt::AlignBottom);
    if (ui->statsChartView) {
        ui->statsChartView->setChart(currentChart);
        ui->statsChartView->setRenderHint(QPainter::Antialiasing);
        qDebug() << "Set chart to QChartView successfully";
    } else {
        qDebug() << "Error: QChartView is null";
    }
    qDebug() << "Finished updateChart()";
}

void ChartWindow::on_refreshChartButton_clicked()
{
    updateChart();
}

void ChartWindow::on_resetChartButton_clicked()
{
    ui->statsTypeComboBox->setCurrentIndex(0);
    updateFilterComboBox();
    updateChart();
}

void ChartWindow::handlePieSliceHovered(QPieSlice *slice, bool state)
{
    if (state) {
        tooltipLabel->setText(QString("%1: %2 (%3%)")
                                  .arg(slice->label())
                                  .arg(static_cast<int>(slice->value()))
                                  .arg(slice->percentage() * 100, 0, 'f', 1));
        QPoint pos = QCursor::pos();
        pos.setY(pos.y() + 20);
        tooltipLabel->move(pos);
        tooltipLabel->show();
    } else {
        tooltipLabel->hide();
    }
}

void ChartWindow::handleBarHovered(bool status, int index)
{
    if (status && currentBarSet) {
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(currentChart->axes(Qt::Horizontal).first());
        if (axisX && index < axisX->categories().size()) {
            QString category = axisX->categories().at(index);
            qreal value = currentBarSet->at(index);
            tooltipLabel->setText(QString("%1: %2").arg(category).arg(value));
            QPoint pos = QCursor::pos();
            pos.setY(pos.y() + 20);
            tooltipLabel->move(pos);
            tooltipLabel->show();
        }
    } else {
        tooltipLabel->hide();
    }
}
