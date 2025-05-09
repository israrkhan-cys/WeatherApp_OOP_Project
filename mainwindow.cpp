#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QUrl>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new QNetworkAccessManager(this))
    , locationManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::handleWeatherData);

    connect(locationManager, &QNetworkAccessManager::finished,
            this, &MainWindow::onLocationDetected);

    locationManager->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString city = ui->lineEdit->text().trimmed();

    if (city.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }


    fetchWeather(city);
}

void MainWindow::fetchWeather(const QString& city)
{
    const QString apiKey = "my api key";//api key remoed due to privacy
    QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                         .arg(city)
                         .arg(apiKey);

    QUrl url(urlStr);
    QNetworkRequest request(url);


    networkManager->get(request);
}


void MainWindow::handleWeatherData(QNetworkReply *reply)
{

    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Network Error", reply->errorString());
        reply->deleteLater();
        return;
    }

    // Parse the JSON response
    QByteArray resp = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(resp);
    QJsonObject obj = doc.object();


    ui->label_city->setText("📍City : " + obj.value("name").toString());


    double temp = obj["main"].toObject().value("temp").toDouble();
    ui->label_temp->setText(QString("🌡️ Temperature: %1 °C").arg(temp));


    QString desc;
    auto weatherArr = obj["weather"].toArray();
    if (!weatherArr.isEmpty()) {
        desc = weatherArr.first().toObject().value("description").toString();
    }
    ui->label_weather->setText("⛅ Weather: " + desc);


    double humidity = obj["main"].toObject().value("humidity").toDouble();
    ui->label_humidity->setText(QString("💧 Humidity: %1%").arg(humidity));


    double wind = obj["wind"].toObject().value("speed").toDouble();
    ui->label_wind->setText(QString("💨 Wind Speed: %1 m/s").arg(wind));

     reply->deleteLater();
}


void MainWindow::onLocationDetected(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {

        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);

        if (jsonDoc.isObject()) {
            // Extract city name
            QString city = jsonDoc.object().value("city").toString();

            ui->lineEdit->setText(city);

            fetchWeather(city);
        }
    } else {
        qDebug() << "Location fetch error:" << reply->errorString();
    }

    reply->deleteLater();
}
