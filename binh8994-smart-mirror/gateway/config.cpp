#include <QDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QtNetwork>
#include <QUrl>
#include <QTimer>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QEventLoop>

#include "config.h"

config::config(QObject *parent) : QObject(parent)
{
	(void) parent;
	host = "api.openweathermap.org";
	city_id = "1566083";
    ip = "14.241.251.180";
	app_id = "0eb7febd90911f4db52773062f732ce3";
	lang = "vi";
	units = "metric";
    lat = 10.83333;
    lon = 106.666672;

	latest_firmware_version = "v0.0.0";
}

bool config::getCloud(QString url) {
	qDebug() << "getCloud()";
	bool rcode = false;
	QNetworkAccessManager manager;
	QNetworkReply* reply;
	QUrl serviceURL(url);
	QNetworkRequest request(serviceURL);
	QEventLoop connection_loop;

	request.setRawHeader("Content-Type", "application/json");
	connect(&manager, SIGNAL( finished( QNetworkReply* ) ), &connection_loop, SLOT( quit() ) );
	reply = manager.get(request);
	connection_loop.exec();

	if(reply->error()) {
		qDebug() << reply->errorString();
	}
	else if (reply->bytesAvailable() > 0){
		QByteArray ba = reply->readAll();
//		qDebug() << ba;
		QJsonDocument jdoc = QJsonDocument::fromJson(ba);
		bool success = jdoc.object().value("success").toBool(false);
		if (success) {
			rcode = true;
			host = jdoc.object().value("host").toString(host);
            city_id = jdoc.object().value("city_id").toString(city_id);
			ip = jdoc.object().value("ip").toString(ip);
			app_id = jdoc.object().value("app_id").toString(app_id);
			lang = jdoc.object().value("lang").toString(lang);
			units = jdoc.object().value("units").toString(units);
            lat = jdoc.object().value("lat").toDouble(lat);
            lon = jdoc.object().value("lon").toDouble(lon);
			latest_firmware_version = jdoc.object().value("latest_firmware_version").toString(latest_firmware_version);
		}
	}
	else {
		qDebug() << "something went wrong!";
	}
	reply->deleteLater();
	return rcode;
}

bool config::loadLocal(QString path) {
	qDebug() << "loadLocal()";
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QJsonParseError error;
	QJsonDocument config = QJsonDocument::fromJson(file.readAll(), &error);
	if (error.error != QJsonParseError::NoError) {
		qDebug() << "QJsonParseError config";
		qDebug() << error.errorString();
		return false;
	}

	host =		config.object().value("host").toString(host);
    city_id =	config.object().value("city_id").toString(city_id);
	ip =		config.object().value("ip").toString(ip);
	app_id =	config.object().value("app_id").toString(app_id);
	lang =		config.object().value("lang").toString(lang);
	units =		config.object().value("units").toString(units);
    lat =       config.object().value("lat").toDouble(lat);
    lon =       config.object().value("lon").toDouble(lon);
	return true;
}

void config::printDetail() {
	qDebug() << "host: " << Config.host;
	qDebug() << "app_id: " << Config.app_id;
    qDebug() << "city_id: " << Config.city_id;
	qDebug() << "ip: " << Config.ip;
	qDebug() << "lang: " << Config.lang;
	qDebug() << "units: " << Config.units;
    qDebug() << "lat: " << lat;
    qDebug() << "lon: " << lon;
	qDebug() << "latest_firmware_version: " << latest_firmware_version;
}

config Config;
