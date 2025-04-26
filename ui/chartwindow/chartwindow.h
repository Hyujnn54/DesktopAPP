// ui/chartwindow/chartwindow.h
#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QChartView>
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
    void updateChart();
    void handlePieSliceHovered(QPieSlice *slice, bool state);
    void handleBarHovered(bool status, int index);

private:
    Ui::ChartWindow *ui;
    QChart *currentChart;
    QLabel *tooltipLabel;
    QBarSet *currentBarSet;
};

#endif // CHARTWINDOW_H
