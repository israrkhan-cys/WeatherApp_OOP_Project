#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"


#include <QMainWindow>
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QNetworkReply;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor for initializing the application Mainwindow
    explicit MainWindow(QWidget *parent = nullptr);


    ~MainWindow();

private slots:

    void on_pushButton_clicked();


    void handleWeatherData(QNetworkReply *reply);


    void onLocationDetected(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *networkManager;
    QNetworkAccessManager *locationManager;


    void fetchWeather(const QString& city);
};

#endif // MAINWINDOW_H
