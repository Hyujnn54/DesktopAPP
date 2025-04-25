#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "meeting.h"
#include "updatemeeting.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPdfWriter>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refreshTableWidget();

private slots:
    void handleAddButtonClick();
    void handleDeleteButtonClick();
    void handleUpdateButtonClick();
    void updateInputFields();
    void handleSearchButtonClick();
    void handleSearchTextChanged(const QString& text);
    void handleSortCriteriaChanged(int index);
    void handleStatisticsButtonClick();
    void toggleTheme();
    void toggleSidebar();
    void handleGenerateQRCodeButtonClick();
    void on_chatSendButton_clicked();
    void on_chatClearButton_clicked();
    void processUserInput(const QString &input);
    void handleRefreshStatsButtonClick();
    void on_toggleLegendCheckBox_stateChanged(int state);
    void onAIResponseReceived(QNetworkReply *reply); // New slot for API response
    void handleExportPdfButtonClick();
    void handleTabChanged(int index); // New slot
private:
    Ui::MainWindow *ui;
    bool isDarkTheme;
    QChart *agendaChart;
    QChart *durationChart;
    QNetworkAccessManager *networkManager; // New: For API requests
    void applyDarkTheme();
    void applyLightTheme();
    void appendChatMessage(const QString &message, bool isBot = false);
    meeting createMeetingFromInput(const QString &input);
    bool validateMeetingInput(const QStringList &parts, QString &errorMessage);
};

#endif // MAINWINDOW_H
