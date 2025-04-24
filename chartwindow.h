#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include "client.h"
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QSqlQuery> // Added for QSqlQuery

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
    void on_chartTypeComboBox_currentIndexChanged(int index);
    void on_resetChartButton_clicked();
    void on_pieSliceHovered(QPieSlice *slice, bool state);
    void on_barHovered(bool status, int index);
    void on_lineHovered(QPointF point, bool state);

private:
    Ui::ChartWindow *ui;
    Client Etmp;
    QString currentChartType;
    QMap<QString, int> currentDataMap;
    QBarSet *currentBarSet;
    int hoveredBarIndex;
    QMap<QDate, int> lineDataMap;

    void updateChart();
    void populateFilterValues();
    void updateChartOptions(); // Added declaration
};

#endif // CHARTWINDOW_H
