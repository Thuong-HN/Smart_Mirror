#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QtNetwork>
#include <QUrl>

#include "openweathermap.h"

openweathermap::openweathermap(QObject *parent) {
    (void) parent;
}

openweathermap::~openweathermap() {
}

void openweathermap::setParams(QString host, QString app_id, QString lang, QString units, double lat, double lon) {
	_host = host;
	_app_id = app_id;
	_lang = lang;
	_units = units;
    _lat = lat;
    _lon = lon;

}

//void openweathermap::getLatLon() {
//	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//	QNetworkRequest request(QUrl("http://ip-api.com/json"));
//	request.setRawHeader("Content-Type", "application/json");

//	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getLatLonFinished(QNetworkReply*)));
//	manager->get(request);
//}

void openweathermap::getWeather() {
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QString url = "http://" + _host + "/data/2.5/weather?"
			+ "appid=" + _app_id
			+ "&lat=" + QString::number(_lat)
			+ "&lon=" + QString::number(_lon)
			+ "&lang=" + _lang
			+ "&units=" + _units;
	QUrl serviceURL(url);
	QNetworkRequest request(serviceURL);
	request.setRawHeader("Content-Type", "application/json");

	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getWeatherFinished(QNetworkReply*)));
	manager->get(request);
}

void openweathermap::getForecast() {
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QString url = "http://" + _host + "/data/2.5/forecast?"
			+ "appid=" + _app_id
			+ "&lat=" + QString::number(_lat)
			+ "&lon=" + QString::number(_lon)
			+ "&lang=" + _lang
			+ "&units=" + _units;
	QUrl serviceURL(url);
	QNetworkRequest request(serviceURL);
	request.setRawHeader("Content-Type", "application/json");

	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getForecastFinished(QNetworkReply*)));
	manager->get(request);
}

QString openweathermap::getWName() {
	return _weather.object().value("name").toString("Hồ Chí Minh");
}

QString openweathermap::getWDescription() {
	return _weather.object().value("weather").toArray().at(0).toObject().value("description").toString("mây rải rác");
}

QString openweathermap::getWIcon() {
	return _weather.object().value("weather").toArray().at(0).toObject().value("icon").toString("01d");
}

double openweathermap::getWTemperature() {
	QJsonValue jv = _weather.object().value("main").toObject().value("temp");
	return (jv.isDouble() ? jv.toDouble(27.0) : jv.toInt(27));
}

double openweathermap::getWTemperatureMax() {
	QJsonValue jv = _weather.object().value("main").toObject().value("temp_max");
	return (jv.isDouble() ? jv.toDouble(30.0) : jv.toInt(30));
}

double openweathermap::getWTemperatureMin() {
	QJsonValue jv = _weather.object().value("main").toObject().value("temp_min");
	return (jv.isDouble() ? jv.toDouble(20.0) : jv.toInt(20));
}

double openweathermap::getWHumidity() {
	QJsonValue jv = _weather.object().value("main").toObject().value("humidity");
	return (jv.isDouble() ? jv.toDouble(70.0) : jv.toInt(70));
}

int openweathermap::getWPressure() {
	return _weather.object().value("main").toObject().value("pressure").toInt(1008); //"hPa";
}

int openweathermap::getWVisibility() {
	return _weather.object().value("visibility").toInt(8000); //m
}

double openweathermap::getWWindSpeed() {
	QJsonValue jv = _weather.object().value("wind").toObject().value("speed"); //m/s
	return (jv.isDouble() ? jv.toDouble(2.0) : jv.toInt(2));
}

int openweathermap::getWTimezoneOffset() {
	return _weather.object().value("timezone").toInt(7*60*60);
}

int openweathermap::getWTimestamp() {
	return _weather.object().value("dt").toInt();
}

int openweathermap::getFTimestamp1() {
	return _forecast.object().value("list").toArray().at(0).toObject().value("dt").toInt();
}

QString openweathermap::getFDay1() {
	unsigned int dt = static_cast<unsigned int>(getFTimestamp1());
    return (QDateTime::fromTime_t(dt).toString("dddd"));
}

QString openweathermap::getFIcon1() {
	return _forecast.object().value("list").toArray().at(0).toObject().value("weather").toArray().at(0).toObject().value("icon").toString("01d");
}

double openweathermap::getFTemperature1() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(0).toObject().value("main").toObject().value("temp");
	return (jv.isDouble() ? jv.toDouble(27.0) : jv.toInt(27));
}

double openweathermap::getFHumidity1() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(0).toObject().value("main").toObject().value("humidity");
	return (jv.isDouble() ? jv.toDouble(70.0) : jv.toInt(70));
}

int openweathermap::getFTimestamp2() {
	return _forecast.object().value("list").toArray().at(8).toObject().value("dt").toInt();
}

QString openweathermap::getFDay2() {
	unsigned int dt = static_cast<unsigned int>(getFTimestamp2());
    return (QDateTime::fromTime_t(dt).toString("dddd"));
}

QString openweathermap::getFIcon2() {
	return _forecast.object().value("list").toArray().at(8).toObject().value("weather").toArray().at(0).toObject().value("icon").toString("01d");
}

double openweathermap::getFTemperature2() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(8).toObject().value("main").toObject().value("temp");
	return (jv.isDouble() ? jv.toDouble(27.0) : jv.toInt(27));
}

double openweathermap::getFHumidity2() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(8).toObject().value("main").toObject().value("humidity");
	return (jv.isDouble() ? jv.toDouble(70.0) : jv.toInt(70));
}

int openweathermap::getFTimestamp3() {
	return _forecast.object().value("list").toArray().at(16).toObject().value("dt").toInt();
}

QString openweathermap::getFDay3() {
	unsigned int dt = static_cast<unsigned int>(getFTimestamp3());
    return (QDateTime::fromTime_t(dt).toString("dddd"));
}

QString openweathermap::getFIcon3() {
	return _forecast.object().value("list").toArray().at(16).toObject().value("weather").toArray().at(0).toObject().value("icon").toString("01d");
}

double openweathermap::getFTemperature3() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(16).toObject().value("main").toObject().value("temp");
	return (jv.isDouble() ? jv.toDouble(27.0) : jv.toInt(27));
}

double openweathermap::getFHumidity3() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(16).toObject().value("main").toObject().value("humidity");
	return (jv.isDouble() ? jv.toDouble(70.0) : jv.toInt(70));
}

int openweathermap::getFTimestamp4() {
	return _forecast.object().value("list").toArray().at(24).toObject().value("dt").toInt();
}

QString openweathermap::getFDay4() {
	unsigned int dt = static_cast<unsigned int>(getFTimestamp4());
    return (QDateTime::fromTime_t(dt).toString("dddd"));
}

QString openweathermap::getFIcon4() {
	return _forecast.object().value("list").toArray().at(24).toObject().value("weather").toArray().at(0).toObject().value("icon").toString("01d");
}

double openweathermap::getFTemperature4() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(24).toObject().value("main").toObject().value("temp");
	return (jv.isDouble() ? jv.toDouble(27.0) : jv.toInt(27));
}

double openweathermap::getFHumidity4() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(24).toObject().value("main").toObject().value("humidity");
	return (jv.isDouble() ? jv.toDouble(70.0) : jv.toInt(70));
}

int openweathermap::getFTimestamp5() {
	return _forecast.object().value("list").toArray().at(32).toObject().value("dt").toInt();
}

QString openweathermap::getFDay5() {
	unsigned int dt = static_cast<unsigned int>(getFTimestamp5());
    return (QDateTime::fromTime_t(dt).toString("dddd"));
}

QString openweathermap::getFIcon5() {
	return _forecast.object().value("list").toArray().at(32).toObject().value("weather").toArray().at(0).toObject().value("icon").toString("01d");
}

double openweathermap::getFTemperature5() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(32).toObject().value("main").toObject().value("temp");
	return (jv.isDouble() ? jv.toDouble(27.0) : jv.toInt(27));
}

double openweathermap::getFHumidity5() {
	QJsonValue jv = _forecast.object().value("list").toArray().at(32).toObject().value("main").toObject().value("humidity");
	return (jv.isDouble() ? jv.toDouble(70.0) : jv.toInt(70));
}

void openweathermap::refreshData() {
//	this->getLatLon();
    this->getWeather();
}

//void openweathermap::getLatLonFinished(QNetworkReply *reply) {
//    qDebug() << "getLatLonFinished()";
//    if(reply->error()) {
//        qDebug() << reply->errorString();
//    }
//    else {
//        QByteArray ba = reply->readAll();
//        //		qDebug() << ba;
//        _location = QJsonDocument::fromJson(ba);
//    }
//    reply->deleteLater();
//    reply->manager()->deleteLater();

//    QString status = _location.object().value("status").toString("error");
//    if (status == "success") {
//        QJsonValue lat_jv = _location.object().value("lat");
//        _lat = (lat_jv.isDouble() ? lat_jv.toDouble(_lat) : lat_jv.toInt(11));

//        QJsonValue lon_jv = _location.object().value("lon");
//        _lon = (lon_jv.isDouble() ? lon_jv.toDouble(_lon) : lon_jv.toInt(107));

//        qDebug() << "_lat: " << _lat;
//        qDebug() << "_lon: " << _lon;
//    }

//	this->getWeather();
//}

void openweathermap::getWeatherFinished (QNetworkReply *reply) {
	qDebug() << "getWeatherFinished()";
	if(reply->error()) {
		qDebug() << reply->errorString();
	}
	else {
		QByteArray ba = reply->readAll();
		//		qDebug() << ba;
		_weather = QJsonDocument::fromJson(ba);
	}
	reply->deleteLater();
	reply->manager()->deleteLater();

	this->getForecast();
}

void openweathermap::getForecastFinished (QNetworkReply *reply) {
	qDebug() << "getForecastFinished()";
	if(reply->error()) {
		qDebug() << reply->errorString();
	}
	else {
		QByteArray ba = reply->readAll();
		//		qDebug() << ba;
		_forecast = QJsonDocument::fromJson(ba);
	}
	reply->deleteLater();
	reply->manager()->deleteLater();

	emit refreshDataDone();
}

openweathermap OpenWeatherMap;
