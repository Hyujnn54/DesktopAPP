#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QSqlQueryModel>
#include <QtCharts>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWindow; }
QT_END_NAMESPACE

class ChartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr);
    ~ChartWindow();

private slots:
    void populateStatsViews();
    void updateFilterComboBox();
    void updateChart();
    void on_refreshChartButton_clicked();
    void on_resetChartButton_clicked();
    void handlePieSliceHovered(QPieSlice *slice, bool state);
    void handleBarHovered(bool status, int index);

private:
    Ui::ChartWindow *ui;
    QChart *currentChart;
    QSqlQueryModel *chartModel;
    QLabel *tooltipLabel;
    QBarSet *currentBarSet;
    QList<QGraphicsTextItem*> percentageLabels; // Track percentage labels
};

#endif // CHARTWINDOW_H
