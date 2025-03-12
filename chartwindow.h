#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include "client.h"
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

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
    void on_refreshChartButton_clicked();
    void on_statsFilterComboBox_currentIndexChanged(int index);

private:
    Ui::ChartWindow *ui;
    Client Etmp;

    void updateChart();
    void populateFilterValues();
};

#endif // CHARTWINDOW_H
