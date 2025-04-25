#include "chartwindow.h"
#include "ui_chartwindow.h"
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPieSeries>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsTextItem>

ChartWindow::ChartWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChartWindow),
    currentChart(new QChart()),
    tooltipLabel(new QLabel(this)),
    currentBarSet(nullptr)
{
    ui->setupUi(this);
    ui->filterComboBox->setEnabled(false);

    ui->statsChartView->setChart(currentChart);
    ui->statsChartView->setRenderHint(QPainter::Antialiasing);

    tooltipLabel->setStyleSheet("QLabel { background-color: #333333; color: white; padding: 5px; border-radius: 3px; }");
    tooltipLabel->setVisible(false);

    connect(ui->statsTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::updateFilterComboBox);
    connect(ui->filterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::updateChart);
    connect(ui->chartTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChartWindow::updateChart);
    connect(ui->refreshChartButton, &QPushButton::clicked, this, &ChartWindow::on_refreshChartButton_clicked);
    connect(ui->resetChartButton, &QPushButton::clicked, this, &ChartWindow::on_resetChartButton_clicked);

    populateStatsViews();
    updateChart();
}

ChartWindow::~ChartWindow()
{
    delete currentChart;
    delete ui;
}

void ChartWindow::populateStatsViews()
{
    ui->statsTypeComboBox->clear();
    ui->statsTypeComboBox->addItems({"Client Statistics", "Training Statistics"});
    updateFilterComboBox();
}

void ChartWindow::updateFilterComboBox()
{
    ui->filterComboBox->clear();
    QString statsType = ui->statsTypeComboBox->currentText();
    if (statsType == "Client Statistics") {
        ui->filterComboBox->addItems({
            "Total Consultations by Sector",
            "Consultation Trends Over Time"
        });
    } else if (statsType == "Training Statistics") {
        ui->filterComboBox->addItems({
            "Total Trainings by Formation Type",
            "Training Frequency Over Time"
        });
    }
    ui->filterComboBox->setEnabled(true);
    updateChart();
}

void ChartWindow::updateChart()
{
    QString statsType = ui->statsTypeComboBox->currentText();
    QString statsFilter = ui->filterComboBox->currentText();
    QString chartType = ui->chartTypeComboBox->currentText();

    // Clear the chart
    currentChart->removeAllSeries();
    for (QAbstractAxis *axis : currentChart->axes()) {
        currentChart->removeAxis(axis);
        delete axis;
    }

    // Clear percentage labels
    for (QGraphicsTextItem *item : percentageLabels) {
        currentChart->scene()->removeItem(item);
        delete item;
    }
    percentageLabels.clear();

    // Reset currentBarSet and description label
    currentBarSet = nullptr;
    ui->hoverDescriptionLabel->setText("Hover over a chart element to see details");

    // Check database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        db.close();
        if (!db.open()) {
            qDebug() << "Database error:" << db.lastError().text();
            QMessageBox::critical(this, "Database Error", "Cannot connect to database: " + db.lastError().text());
            return;
        }
    }

    QSqlQuery query;
    QStringList categories;
    bool querySuccess = false;

    if (statsType == "Client Statistics") {
        if (statsFilter == "Total Consultations by Sector") {
            query.prepare("SELECT SECTOR, COUNT(*) AS count FROM AHMED.CLIENTS GROUP BY SECTOR");
            querySuccess = query.exec();
            if (!querySuccess) {
                qDebug() << "Client Sector Query Error:" << query.lastError().text();
                QMessageBox::warning(this, "Query Error", "Failed to fetch client sector data: " + query.lastError().text());
                return;
            }
        } else if (statsFilter == "Consultation Trends Over Time") {
            query.prepare("SELECT TO_CHAR(TRUNC(CONSULTATION_DATE, 'MM'), 'YYYY-MM') AS month, "
                          "COUNT(*) AS count FROM AHMED.CLIENTS GROUP BY TRUNC(CONSULTATION_DATE, 'MM') "
                          "ORDER BY TRUNC(CONSULTATION_DATE, 'MM')");
            querySuccess = query.exec();
            if (!querySuccess) {
                qDebug() << "Client Trends Query Error:" << query.lastError().text();
                QMessageBox::warning(this, "Query Error", "Failed to fetch client trends data: " + query.lastError().text());
                return;
            }
        }
    } else if (statsType == "Training Statistics") {
        if (statsFilter == "Total Trainings by Formation Type") {
            query.prepare("SELECT FORMATION, COUNT(*) AS count FROM AHMED.FORMATIONS GROUP BY FORMATION");
            querySuccess = query.exec();
            if (!querySuccess) {
                qDebug() << "Training Formation Query Error:" << query.lastError().text();
                QMessageBox::warning(this, "Query Error", "Failed to fetch training formation data: " + query.lastError().text());
                return;
            }
        } else if (statsFilter == "Training Frequency Over Time") {
            query.prepare("SELECT TO_CHAR(TRUNC(DATEF, 'MM'), 'YYYY-MM') AS month, "
                          "COUNT(*) AS count FROM AHMED.FORMATIONS GROUP BY TRUNC(DATEF, 'MM') "
                          "ORDER BY TRUNC(DATEF, 'MM')");
            querySuccess = query.exec();
            if (!querySuccess) {
                qDebug() << "Training Trends Query Error:" << query.lastError().text();
                QMessageBox::warning(this, "Query Error", "Failed to fetch training trends data: " + query.lastError().text());
                return;
            }
        }
    }

    currentChart->setAnimationOptions(QChart::SeriesAnimations);
    currentChart->setTitle(statsFilter);

    if (chartType == "Pie Chart") {
        QPieSeries *series = new QPieSeries();
        qreal total = 0;
        QMap<QString, qreal> values;

        while (query.next()) {
            QString category = query.value(0).toString();
            qreal value = query.value(1).toReal();
            values[category] = value;
            total += value;
        }

        for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
            QString category = it.key();
            qreal value = it.value();
            qreal percentage = (total > 0) ? (value / total) * 100 : 0;
            QPieSlice *slice = series->append(category, value);
            slice->setLabel(QString("%1\n%2%").arg(category).arg(percentage, 0, 'f', 1));
            slice->setLabelVisible(true);
            slice->setLabelPosition(QPieSlice::LabelOutside);
        }

        currentChart->addSeries(series);
        currentChart->legend()->setVisible(true);
        currentChart->legend()->setAlignment(Qt::AlignRight);

        connect(series, &QPieSeries::hovered, this, &ChartWindow::handlePieSliceHovered);
    } else {
        QBarSeries *series = new QBarSeries();
        currentBarSet = new QBarSet(statsFilter);
        qreal total = 0;
        QList<qreal> counts;

        while (query.next()) {
            categories << query.value(0).toString();
            qreal value = query.value(1).toReal();
            *currentBarSet << value;
            counts << value;
            total += value;
        }

        series->append(currentBarSet);
        currentChart->addSeries(series);

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setTitleText("Category");
        currentChart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Count");
        axisY->setLabelFormat("%i");
        qreal maxValue = 0;
        for (int i = 0; i < currentBarSet->count(); ++i) {
            maxValue = qMax(maxValue, currentBarSet->at(i));
        }
        axisY->setRange(0, currentBarSet->count() ? qMax(5.0, maxValue + 1) : 5);
        currentChart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Add percentage labels below the category labels
        for (int i = 0; i < counts.size(); ++i) {
            qreal value = counts[i];
            qreal percentage = (total > 0) ? (value / total) * 100 : 0;
            QGraphicsTextItem *text = new QGraphicsTextItem(QString("%1%").arg(percentage, 0, 'f', 1));
            currentChart->scene()->addItem(text);
            // Position the label at the x-coordinate of the bar, but at y = 0 (bottom of the chart)
            QPointF position = currentChart->mapToPosition(QPointF(i + 0.5, 0));
            // Adjust y to be below the category labels (e.g., 20 pixels below the x-axis)
            // Adjust x to center the text under the category label
            text->setPos(position.x() - 15, position.y() + 20);
            text->setZValue(10);
            percentageLabels.append(text);
        }

        currentChart->legend()->setVisible(true);
        currentChart->legend()->setAlignment(Qt::AlignBottom);

        connect(currentBarSet, &QBarSet::hovered, this, &ChartWindow::handleBarHovered);
    }
}

void ChartWindow::on_refreshChartButton_clicked()
{
    updateChart();
}

void ChartWindow::on_resetChartButton_clicked()
{
    ui->statsTypeComboBox->setCurrentIndex(0);
    updateFilterComboBox();
}

void ChartWindow::handlePieSliceHovered(QPieSlice *slice, bool state)
{
    if (!slice || !tooltipLabel || !isVisible()) return;

    slice->setExploded(state);

    if (state) {
        qreal percentage = slice->percentage() * 100;
        QString details = QString("Category: %1, Count: %2, Percentage: %3%")
                              .arg(slice->label().split("\n").first())
                              .arg(slice->value())
                              .arg(percentage, 0, 'f', 1);
        tooltipLabel->setText(details);
        tooltipLabel->move(QCursor::pos() + QPoint(10, 10));
        tooltipLabel->show();
        ui->hoverDescriptionLabel->setText(details);
    } else {
        tooltipLabel->hide();
        ui->hoverDescriptionLabel->setText("Hover over a chart element to see details");
    }
}

void ChartWindow::handleBarHovered(bool status, int index)
{
    if (!currentBarSet || !tooltipLabel || !isVisible()) return;

    if (status) {
        currentBarSet->setBrush(QBrush(Qt::darkCyan));

        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(currentChart->axes(Qt::Horizontal).first());
        if (!axisX || index < 0 || index >= axisX->categories().count()) {
            tooltipLabel->hide();
            ui->hoverDescriptionLabel->setText("Hover over a chart element to see details");
            return;
        }
        QString category = axisX->categories().at(index);
        qreal value = currentBarSet->at(index);
        qreal total = currentBarSet->sum();
        qreal percentage = (total > 0) ? (value / total) * 100 : 0;
        QString details = QString("Category: %1, Count: %2, Percentage: %3%")
                              .arg(category)
                              .arg(value)
                              .arg(percentage, 0, 'f', 1);
        tooltipLabel->setText(details);
        tooltipLabel->move(QCursor::pos() + QPoint(10, 10));
        tooltipLabel->show();
        ui->hoverDescriptionLabel->setText(details);
    } else {
        tooltipLabel->hide();
        currentBarSet->setBrush(QBrush(Qt::cyan));
        ui->hoverDescriptionLabel->setText("Hover over a chart element to see details");
    }
}
