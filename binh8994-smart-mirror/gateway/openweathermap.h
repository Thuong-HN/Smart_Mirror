#ifndef OPENWEATHERMAP_H
#define OPENWEATHERMAP_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QtNetwork/QNetworkReply>

class openweathermap : public QObject
{
	Q_OBJECT
public:
	openweathermap(QObject *parent = nullptr);
	~openweathermap();
    void setParams(QString host, QString app_id, QString lang, QString units, double lat, double lon);
//	void getLatLon();
	void getWeather();
	void getForecast();

	QString getWName();
	QString getWDescription();
	QString getWIcon();
	double getWTemperature();
	double getWTemperatureMax();
	double getWTemperatureMin();
	double getWHumidity();
	int getWPressure();
	int getWVisibility();
	double getWWindSpeed();
	int getWTimestamp();
	int getWTimezoneOffset();

	int getFTimestamp1();
	QString getFDay1();
	QString getFIcon1();
	double getFTemperature1();
	double getFHumidity1();

	int getFTimestamp2();
	QString getFDay2();
	QString getFIcon2();
	double getFTemperature2();
	double getFHumidity2();

	int getFTimestamp3();
	QString getFDay3();
	QString getFIcon3();
	double getFTemperature3();
	double getFHumidity3();

	int getFTimestamp4();
	QString getFDay4();
	QString getFIcon4();
	double getFTemperature4();
	double getFHumidity4();

	int getFTimestamp5();
	QString getFDay5();
	QString getFIcon5();
	double getFTemperature5();
	double getFHumidity5();

private:
	QJsonDocument _weather;
	QJsonDocument _forecast;
	QJsonDocument _location;

	QString _host;
	QString _app_id;
	QString _lang;
	QString _units;
	double _lat;
	double _lon;

Q_SIGNALS:
	void refreshDataDone();

public slots:
	void refreshData();

private slots:
//	void getLatLonFinished(QNetworkReply *reply);
	void getWeatherFinished(QNetworkReply *reply);
	void getForecastFinished(QNetworkReply *reply);
};

extern openweathermap OpenWeatherMap;

#endif // OPENWEATHERMAP_H

//http://api.openweathermap.org/data/2.5/weather?id=1566083&appid=0eb7febd90911f4db52773062f732ce3&lang=vi&units=metric
//http://api.openweathermap.org/data/2.5/forecast?id=1566083&appid=0eb7febd90911f4db52773062f732ce3&lang=vi&units=metric&cnt=3

/*
{
  "coord": {
    "lon": 106.67,
    "lat": 10.75
  },
  "weather": [
    {
      "id": 802,
      "main": "Clouds",
      "description": "mây rải rác",
      "icon": "03n"
    }
  ],
  "base": "stations",
  "main": {
    "temp": 30,
    "pressure": 1008,
    "humidity": 74,
    "temp_min": 30,
    "temp_max": 30
  },
  "visibility": 8000,
  "wind": {
    "speed": 2.1,
    "deg": 250
  },
  "clouds": {
    "all": 40
  },
  "dt": 1574591298,
  "sys": {
    "type": 1,
    "id": 9314,
    "country": "VN",
    "sunrise": 1574549556,
    "sunset": 1574591244
  },
  "timezone": 25200,
  "id": 1566083,
  "name": "Thanh pho Ho Chi Minh",
  "cod": 200
}
*/

/*
{
  "cod": "200",
  "message": 0,
  "cnt": 3,
  "list": [
    {
      "dt": 1574596800,
      "main": {
        "temp": 25.14,
        "temp_min": 24.61,
        "temp_max": 25.14,
        "pressure": 1011,
        "sea_level": 1011,
        "grnd_level": 1011,
        "humidity": 91,
        "temp_kf": 0.53
      },
      "weather": [
        {
          "id": 500,
          "main": "Rain",
          "description": "mưa nhẹ",
          "icon": "10n"
        }
      ],
      "clouds": {
        "all": 26
      },
      "wind": {
        "speed": 2.66,
        "deg": 132
      },
      "rain": {
        "3h": 2.75
      },
      "sys": {
        "pod": "n"
      },
      "dt_txt": "2019-11-24 12:00:00"
    },
    {
      "dt": 1574607600,
      "main": {
        "temp": 24.46,
        "temp_min": 24.06,
        "temp_max": 24.46,
        "pressure": 1012,
        "sea_level": 1012,
        "grnd_level": 1012,
        "humidity": 91,
        "temp_kf": 0.4
      },
      "weather": [
        {
          "id": 500,
          "main": "Rain",
          "description": "mưa nhẹ",
          "icon": "10n"
        }
      ],
      "clouds": {
        "all": 20
      },
      "wind": {
        "speed": 2.12,
        "deg": 126
      },
      "rain": {
        "3h": 2.81
      },
      "sys": {
        "pod": "n"
      },
      "dt_txt": "2019-11-24 15:00:00"
    },
    {
      "dt": 1574618400,
      "main": {
        "temp": 24.05,
        "temp_min": 23.79,
        "temp_max": 24.05,
        "pressure": 1011,
        "sea_level": 1011,
        "grnd_level": 1010,
        "humidity": 92,
        "temp_kf": 0.26
      },
      "weather": [
        {
          "id": 800,
          "main": "Clear",
          "description": "bầu trời quang đãng",
          "icon": "01n"
        }
      ],
      "clouds": {
        "all": 10
      },
      "wind": {
        "speed": 1.83,
        "deg": 134
      },
      "sys": {
        "pod": "n"
      },
      "dt_txt": "2019-11-24 18:00:00"
    }
  ],
  "city": {
    "id": 1566083,
    "name": "Thanh pho Ho Chi Minh",
    "coord": {
      "lat": 10.75,
      "lon": 106.6667
    },
    "country": "VN",
    "timezone": 25200,
    "sunrise": 1574549557,
    "sunset": 1574591244
  }
}
*/
