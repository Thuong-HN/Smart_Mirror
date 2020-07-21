#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>

#define APP_FIRMWARE_VERSION		"v0.0.6"
#define APP_ROOT_DISK				"/home/pi/gateway"

#define APP_LOCAL_CONFIG_PATH		APP_ROOT_DISK "/config.json"
#define APP_CLOUD_CONFIG_URL		"https://3rkhgbmv68.execute-api.ap-southeast-1.amazonaws.com/prod/config"

#define APP_LOCAL_FIRMWARE_PATH		APP_ROOT_DISK "/smartmirror.tar"
#define APP_CLOUD_FIRMWARE_PATH		"https://smartmirror-firmware.s3-ap-southeast-1.amazonaws.com/smartmirror.tar"

#define APP_IMAGE32_PATH			APP_ROOT_DISK "/32"
#define APP_IMAGE256_PATH			APP_ROOT_DISK "/256"


class config : public QObject
{
	Q_OBJECT
public:
	explicit config(QObject *parent = nullptr);
	bool getCloud(QString path);
	bool loadLocal(QString path);
	void printDetail();

	QString host;
	QString city_id;
    QString ip;
	QString app_id;
	QString lang;
	QString units;
    double lat;
    double lon;

	QString latest_firmware_version;

signals:

public slots:

};

extern config Config;

#endif // CONFIG_H
