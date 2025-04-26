#include "chartwindow.h"
#include "ui_chartwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QGraphicsTextItem>
#include <QDebug>

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
    ui->filterComboBox->blockSignals(true);

    connect(ui->refreshChartButton, &QPushButton::clicked, this, &ChartWindow::on_refreshChartButton_clicked);
    connect(ui->resetChartButton, &QPushButton::clicked, this, &ChartWindow::on_resetChartButton_clicked);
    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);
    connect(ui->filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateChart);
    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartWindow::updateFilterComboBox);

    updateFilterComboBox();
    populateStatsViews();

    // Re-enable signals after initialization
    ui->statsTypeComboBox->blockSignals(false);
    ui->filterComboBox->blockSignals(false);
}

ChartWindow::~ChartWindow()
{
    // No need to delete currentChart manually; QChartView will handle it
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
    QString filter = ui->filterComboBox->currentText();

    qDebug() << "Starting updateChart() with statsType:" << statsType << "and filter:" << filter;

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
        // Clear the existing chart instead of deleting it
        currentChart->removeAllSeries();
        while (!currentChart->axes().isEmpty()) {
            currentChart->removeAxis(currentChart->axes().first());
        }
    }
    qDebug() << "Prepared QChart";
    currentChart->setTitle(statsType);
    currentChart->setAnimationOptions(QChart::SeriesAnimations);

    QSqlQuery query;
    bool dataAvailable = false;

    if (statsType == "Client Statistics") {
        if (filter == "All Clients") {
            qDebug() << "Processing All Clients filter";
            if (!query.exec("SELECT COUNT(*) FROM CLIENTS")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve client count: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            if (query.next()) {
                int count = query.value(0).toInt();
                qDebug() << "Client count:" << count;
                QBarSeries *series = new QBarSeries();
                qDebug() << "Created QBarSeries";
                QBarSet *set = new QBarSet("Clients");
                qDebug() << "Created QBarSet";
                *set << count;
                qDebug() << "Added count to QBarSet:" << count;
                series->append(set);
                qDebug() << "Appended QBarSet to QBarSeries";
                currentChart->addSeries(series);
                qDebug() << "Added QBarSeries to QChart";
                QStringList categories = {"Total"};
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                qDebug() << "Created QBarCategoryAxis";
                axisX->append(categories);
                currentChart->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                qDebug() << "Attached axisX to series";
                QValueAxis *axisY = new QValueAxis();
                qDebug() << "Created QValueAxis";
                axisY->setRange(0, count > 0 ? count + 1 : 1);
                currentChart->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                qDebug() << "Attached axisY to series";
                dataAvailable = (count > 0);
            }
        } else if (filter == "By Sector") {
            qDebug() << "Processing By Sector filter";
            if (!query.exec("SELECT SECTOR, COUNT(*) FROM CLIENTS GROUP BY SECTOR")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve clients by sector: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            QPieSeries *series = new QPieSeries();
            int total = 0;
            while (query.next()) {
                QString sector = query.value(0).toString();
                int count = query.value(1).toInt();
                qDebug() << "Sector:" << sector << "Count:" << count;
                series->append(sector, count);
                total += count;
            }
            if (total > 0) {
                currentChart->addSeries(series);
                dataAvailable = true;
            }
        } else if (filter == "By Consultation Year") {
            qDebug() << "Processing By Consultation Year filter";
            if (!query.exec("SELECT TO_CHAR(CONSULTATION_DATE, 'YYYY') AS YEAR, COUNT(*) FROM CLIENTS GROUP BY TO_CHAR(CONSULTATION_DATE, 'YYYY')")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve clients by year: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Clients");
            QStringList categories;
            int maxValue = 0;
            while (query.next()) {
                QString year = query.value(0).toString();
                int count = query.value(1).toInt();
                qDebug() << "Year:" << year << "Count:" << count;
                *set << count;
                categories << year;
                maxValue = qMax(maxValue, count);
                dataAvailable = true;
            }
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
    } else if (statsType == "Training Statistics") {
        if (filter == "All Trainings") {
            qDebug() << "Processing All Trainings filter";
            if (!query.exec("SELECT COUNT(*) FROM FORMATIONS")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve training count: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            if (query.next()) {
                int count = query.value(0).toInt();
                qDebug() << "Training count:" << count;
                QBarSeries *series = new QBarSeries();
                QBarSet *set = new QBarSet("Trainings");
                *set << count;
                series->append(set);
                currentChart->addSeries(series);
                QStringList categories = {"Total"};
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                axisX->append(categories);
                currentChart->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                QValueAxis *axisY = new QValueAxis();
                axisY->setRange(0, count > 0 ? count + 1 : 1);
                currentChart->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                dataAvailable = (count > 0);
            }
        } else if (filter == "By Formation") {
            qDebug() << "Processing By Formation filter";
            if (!query.exec("SELECT FORMATION, COUNT(*) FROM FORMATIONS GROUP BY FORMATION")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve trainings by formation: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            QPieSeries *series = new QPieSeries();
            int total = 0;
            while (query.next()) {
                QString formation = query.value(0).toString();
                int count = query.value(1).toInt();
                qDebug() << "Formation:" << formation << "Count:" << count;
                series->append(formation, count);
                total += count;
            }
            if (total > 0) {
                currentChart->addSeries(series);
                dataAvailable = true;
            }
        } else if (filter == "By Year") {
            qDebug() << "Processing By Year filter for Training Statistics";
            if (!query.exec("SELECT TO_CHAR(DATEF, 'YYYY') AS YEAR, COUNT(*) FROM FORMATIONS GROUP BY TO_CHAR(DATEF, 'YYYY')")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve trainings by year: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet("Trainings");
            QStringList categories;
            int maxValue = 0;
            while (query.next()) {
                QString year = query.value(0).toString();
                int count = query.value(1).toInt();
                qDebug() << "Year:" << year << "Count:" << count;
                *set << count;
                categories << year;
                maxValue = qMax(maxValue, count);
                dataAvailable = true;
            }
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
    } else if (statsType == "Meeting Statistics") {
        if (filter == "All Meetings") {
            qDebug() << "Processing All Meetings filter";
            if (!query.exec("SELECT COUNT(*) FROM MEETING")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve meeting count: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            if (query.next()) {
                int count = query.value(0).toInt();
                qDebug() << "Meeting count:" << count;
                QBarSeries *series = new QBarSeries();
                QBarSet *set = new QBarSet("Meetings");
                *set << count;
                series->append(set);
                currentChart->addSeries(series);
                QStringList categories = {"Total"};
                QBarCategoryAxis *axisX = new QBarCategoryAxis();
                axisX->append(categories);
                currentChart->addAxis(axisX, Qt::AlignBottom);
                series->attachAxis(axisX);
                QValueAxis *axisY = new QValueAxis();
                axisY->setRange(0, count > 0 ? count + 1 : 1);
                currentChart->addAxis(axisY, Qt::AlignLeft);
                series->attachAxis(axisY);
                dataAvailable = (count > 0);
            }
        } else if (filter == "By Agenda") {
            qDebug() << "Processing By Agenda filter";
            if (!query.exec("SELECT AGENDA, COUNT(*) FROM MEETING GROUP BY AGENDA")) {
                qDebug() << "Query failed:" << query.lastError().text();
                QMessageBox::warning(this, "Database Error", "Failed to retrieve meetings by agenda: " + query.lastError().text());
                QGraphicsTextItem *textItem = new QGraphicsTextItem("Error retrieving data", currentChart);
                textItem->setPos(50, 50);
                if (currentChart->scene()) {
                    currentChart->scene()->addItem(textItem);
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            QPieSeries *series = new QPieSeries();
            int total = 0;
            while (query.next()) {
                QString agenda = query.value(0).toString();
                int count = query.value(1).toInt();
                qDebug() << "Agenda:" << agenda << "Count:" << count;
                series->append(agenda, count);
                total += count;
            }
            if (total > 0) {
                currentChart->addSeries(series);
                dataAvailable = true;
            }
        } else if (filter == "By Duration") {
            qDebug() << "Processing By Duration filter";
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
                    qDebug() << "Added QGraphicsTextItem to scene";
                } else {
                    qDebug() << "Error: Chart scene is null";
                }
                goto finalizeChart;
            }
            QPieSeries *series = new QPieSeries();
            int total = 0;
            while (query.next()) {
                QString range = query.value(0).toString();
                int count = query.value(1).toInt();
                qDebug() << "Duration Range:" << range << "Count:" << count;
                series->append(range, count);
                total += count;
            }
            if (total > 0) {
                currentChart->addSeries(series);
                dataAvailable = true;
            }
        }
    }

    if (!dataAvailable) {
        qDebug() << "No data available, adding placeholder text";
        QGraphicsTextItem *textItem = new QGraphicsTextItem("No data available", currentChart);
        textItem->setPos(50, 50);
        if (currentChart->scene()) {
            currentChart->scene()->addItem(textItem);
            qDebug() << "Added QGraphicsTextItem to scene";
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
