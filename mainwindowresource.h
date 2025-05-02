#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "resource.h"
#include "searchdialog.h"
#include <QFileDialog>
#include <QPixmap>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts>
#include <QNetworkAccessManager> // Added for API calls
#include <QNetworkReply>         // For reply handling
#include <QJsonDocument>         // For JSON parsing
#include <QJsonObject>
#include <QJsonArray>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts>
#include <QtCore>
#include <QtGui>
#include <QSerialPort>




QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

//QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSelectImage_clicked();
    void on_confirmFormButton_clicked();
    //void refreshTableWidget();
    void refreshTableWidget(const QString &filter = "");
    void handleDeleteButtonClick();
    void on_updateButton_clicked();
    void toggleSidebar();
    void toggleTheme();
    void on_cancelFormButton_clicked();
    void on_searchTextChanged(const QString &text); // New slot for dynamic search
    void on_resetSearchButton_clicked(); // New slot for reset search
    void on_searchTimeout();
    void on_exportPdfButton_clicked();
    void on_tableHeaderClicked(int column);  // New slot for header clicks
    void on_downloadHistoryButton_clicked(); // New slot
    void on_clearHistoryButton_clicked();
    void on_imageAnalysisFinished(QNetworkReply *reply);
    void on_btnLookForResource_clicked();




private:
    Ui::MainWindow *ui;

    int selectedResourceId;
    QByteArray imageData;
    void applyDarkTheme();
    void applyLightTheme();
    bool isDarkTheme;
    QWidget *sidebarWidget;
    QTimer *searchTimer;
    QString pendingSearchText;
    int currentSortColumn = -1;
    Qt::SortOrder currentSortOrder = Qt::AscendingOrder;
    QString employeeName;

    void sortTableByColumn(int column, Qt::SortOrder order);

    QNetworkAccessManager *networkManager;

      QMap<QString, QStringList> objectSynonyms;
    void initializeSynonyms();

     QPieSeries *pieSeries;
      void updatePieChart();


     QBarSeries *barSeries;
     void updateStackedBarChart();





};
#endif // MAINWINDOW_H
