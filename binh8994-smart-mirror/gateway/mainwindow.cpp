#include <QDebug>
#include <QTimeZone>
#include <QDesktopWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "openweathermap.h"
#include "config.h"
#include "rss.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	initUi();
    qDebug() << "Smart Mirror Espitek version:" << APP_FIRMWARE_VERSION;
    qDebug() << "supportsSsl():" << QSslSocket::supportsSsl();

	Config.loadLocal(APP_LOCAL_CONFIG_PATH);
    refreshConfig();
    connect(&updateConfigTimer, SIGNAL(timeout()), this, SLOT(refreshConfig()));
    updateConfigTimer.setInterval(24*60*60000);
    updateConfigTimer.start();

	connect(&OpenWeatherMap, SIGNAL(refreshDataDone()), this, SLOT(refreshWeatherUi()));
	OpenWeatherMap.refreshData();
	connect(&refreshDataTimer, SIGNAL(timeout()), &OpenWeatherMap, SLOT(refreshData()));
	refreshDataTimer.setInterval(60000);
	refreshDataTimer.start();

	refreshClockUi();
	connect(&refreshClockTimer, SIGNAL(timeout()), this, SLOT(refreshClockUi()));
	refreshClockTimer.setInterval(1000);
	refreshClockTimer.start();

	refreshDateUi();
	connect(&refreshDateTimer, SIGNAL(timeout()), this, SLOT(refreshDateUi()));
	refreshDateTimer.setInterval(60000);
    refreshDateTimer.start();

    Rss.setParams("https://vnexpress.net/rss/tin-moi-nhat.rss");
    Rss.refreshNews();
    connect(&refreshNewsTimer, SIGNAL(timeout()), &Rss, SLOT(refreshNews()));
    refreshNewsTimer.setInterval(60000);
    refreshNewsTimer.start();

    changeNewsUi();
    connect(&changeNewsTimer, SIGNAL(timeout()), this, SLOT(changeNewsUi()));
    changeNewsTimer.setInterval(6000);
    changeNewsTimer.start();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi() {
    QMainWindow::showFullScreen();

    ui->date_lb->setGeometry(0, 0,
                             QApplication::desktop()->width()/3, QApplication::desktop()->height() * 4/30);
    ui->time_lb->setGeometry(ui->date_lb->x(), ui->date_lb->y() + ui->date_lb->height(),
                             QApplication::desktop()->width()/4, QApplication::desktop()->height()/3);
    ui->sec_lb->setGeometry(ui->time_lb->x() + ui->time_lb->width(), ui->time_lb->y(),
                            ui->time_lb->width()/2, ui->time_lb->height());
    ui->news_lb->setGeometry(ui->time_lb->x(), ui->time_lb->y() + ui->time_lb->height(),
                             QApplication::desktop()->width()/3, QApplication::desktop()->height() * 1/3);
    ui->news_lb->setWordWrap(true);

    ui->city_lb->setGeometry(QApplication::desktop()->width() * 2/3 , 0,
                             QApplication::desktop()->width()/3, QApplication::desktop()->height()/10);
    ui->whicon_lb->setGeometry(QApplication::desktop()->width() * 10/16, ui->city_lb->height(),
                               QApplication::desktop()->width()/16, QApplication::desktop()->height()/30);
    ui->whum_lb->setGeometry(ui->whicon_lb->x() + ui->whicon_lb->width(), ui->whicon_lb->y(),
                             ui->whicon_lb->width(), ui->whicon_lb->height());

    ui->wwicon_lb->setGeometry(QApplication::desktop()->width() * 12/16, ui->whicon_lb->y(),
                               ui->whicon_lb->width(), ui->whicon_lb->height());
    ui->wwind_lb->setGeometry(ui->wwicon_lb->x() + ui->wwicon_lb->width(), ui->whicon_lb->y(),
                              ui->whicon_lb->width(), ui->whicon_lb->height());

    ui->wvicon_lb->setGeometry(ui->wwind_lb->x() + ui->wwind_lb->width(), ui->whicon_lb->y(),
                               ui->whicon_lb->width(), ui->whicon_lb->height());
    ui->wvisibility_lb->setGeometry(ui->wvicon_lb->x() + ui->wvicon_lb->width(), ui->whicon_lb->y(),
                                    ui->whicon_lb->width(), ui->whicon_lb->height());

    ui->wtemp_lb->setGeometry(QApplication::desktop()->width() * 12/16, ui->whicon_lb->y() + ui->whicon_lb->height(),
                              QApplication::desktop()->width()/4, QApplication::desktop()->height()/3);
    ui->wticon_lb->setGeometry(QApplication::desktop()->width() * 2/4, ui->wtemp_lb->y(),
                               QApplication::desktop()->width()/4, QApplication::desktop()->height()/3);

    ui->fday_lb1->setGeometry(QApplication::desktop()->width() * 10/16, ui->wtemp_lb->y() + ui->wtemp_lb->height(),
                              ui->wtemp_lb->width()/2, ui->wtemp_lb->height()/5);
    ui->ficon_lb1->setGeometry(ui->fday_lb1->x() + ui->fday_lb1->width(), ui->fday_lb1->y(),
                               ui->fday_lb1->width()/2, ui->fday_lb1->height());
    ui->ftemp_lb1->setGeometry(ui->ficon_lb1->x() + ui->ficon_lb1->width(), ui->ficon_lb1->y(),
                               ui->ficon_lb1->width(), ui->ficon_lb1->height());
    ui->fhum_lb1->setGeometry(ui->ftemp_lb1->x() + ui->ftemp_lb1->width(), ui->ftemp_lb1->y(),
                              ui->ftemp_lb1->width(), ui->ftemp_lb1->height());

    ui->fday_lb2->setGeometry(ui->fday_lb1->x(), ui->fday_lb1->y() + ui->fday_lb1->height(),
                              ui->fday_lb1->width(), ui->fday_lb1->height());
    ui->ficon_lb2->setGeometry(ui->fday_lb2->x() + ui->fday_lb2->width(), ui->fday_lb2->y(),
                               ui->fday_lb2->width()/2, ui->fday_lb2->height());
    ui->ftemp_lb2->setGeometry(ui->ficon_lb2->x() + ui->ficon_lb2->width(), ui->ficon_lb2->y(),
                               ui->ficon_lb2->width(), ui->ficon_lb2->height());
    ui->fhum_lb2->setGeometry(ui->ftemp_lb2->x() + ui->ftemp_lb2->width(), ui->ftemp_lb2->y(),
                              ui->ftemp_lb2->width(), ui->ftemp_lb2->height());

    ui->fday_lb3->setGeometry(ui->fday_lb2->x(), ui->fday_lb2->y() + ui->fday_lb2->height(),
                              ui->fday_lb2->width(), ui->fday_lb2->height());
    ui->ficon_lb3->setGeometry(ui->fday_lb3->x() + ui->fday_lb3->width(), ui->fday_lb3->y(),
                               ui->fday_lb3->width()/2, ui->fday_lb3->height());
    ui->ftemp_lb3->setGeometry(ui->ficon_lb3->x() + ui->ficon_lb3->width(), ui->ficon_lb3->y(),
                               ui->ficon_lb3->width(), ui->ficon_lb3->height());
    ui->fhum_lb3->setGeometry(ui->ftemp_lb3->x() + ui->ftemp_lb3->width(), ui->ftemp_lb3->y(),
                              ui->ftemp_lb3->width(), ui->ftemp_lb3->height());

    ui->fday_lb4->setGeometry(ui->fday_lb3->x(), ui->fday_lb3->y() + ui->fday_lb3->height(),
                              ui->fday_lb3->width(), ui->fday_lb3->height());
    ui->ficon_lb4->setGeometry(ui->fday_lb4->x() + ui->fday_lb4->width(), ui->fday_lb4->y(),
                               ui->fday_lb4->width()/2, ui->fday_lb4->height());
    ui->ftemp_lb4->setGeometry(ui->ficon_lb4->x() + ui->ficon_lb4->width(), ui->ficon_lb4->y(),
                               ui->ficon_lb4->width(), ui->ficon_lb4->height());
    ui->fhum_lb4->setGeometry(ui->ftemp_lb4->x() + ui->ftemp_lb4->width(), ui->ftemp_lb4->y(),
                              ui->ftemp_lb4->width(), ui->ftemp_lb4->height());

    ui->fday_lb5->setGeometry(ui->fday_lb4->x(), ui->fday_lb4->y() + ui->fday_lb4->height(),
                              ui->fday_lb4->width(), ui->fday_lb4->height());
    ui->ficon_lb5->setGeometry(ui->fday_lb5->x() + ui->fday_lb5->width(), ui->fday_lb5->y(),
                               ui->fday_lb5->width()/2, ui->fday_lb5->height());
    ui->ftemp_lb5->setGeometry(ui->ficon_lb5->x() + ui->ficon_lb5->width(), ui->ficon_lb5->y(),
                               ui->ficon_lb5->width(), ui->ficon_lb5->height());
    ui->fhum_lb5->setGeometry(ui->ftemp_lb5->x() + ui->ftemp_lb5->width(), ui->ftemp_lb5->y(),
                              ui->ftemp_lb5->width(), ui->ftemp_lb5->height());

    QString hum = APP_IMAGE32_PATH "/humidity.png";
    QImage ihum(hum);
    ui->whicon_lb->setPixmap(QPixmap::fromImage(ihum));

    QString wind = APP_IMAGE32_PATH "/wind.png";
    QImage iwind(wind);
    ui->wwicon_lb->setPixmap(QPixmap::fromImage(iwind));

    QString visibility = APP_IMAGE32_PATH "/visibility.png";
    QImage ivisibility(visibility);
    ui->wvicon_lb->setPixmap(QPixmap::fromImage(ivisibility));

    QString wea = APP_IMAGE256_PATH "/02d.png";
    QImage iwea(wea);
    ui->wticon_lb->setPixmap(QPixmap::fromImage(iwea));

    QString forc = APP_IMAGE32_PATH "/02d.png";
    QImage iforc(forc);
    ui->ficon_lb1->setPixmap(QPixmap::fromImage(iforc));
    ui->ficon_lb2->setPixmap(QPixmap::fromImage(iforc));
    ui->ficon_lb3->setPixmap(QPixmap::fromImage(iforc));
    ui->ficon_lb4->setPixmap(QPixmap::fromImage(iforc));
    ui->ficon_lb5->setPixmap(QPixmap::fromImage(iforc));

}

void MainWindow::refreshWeatherUi() {
	qDebug() << "refreshWeatherUi()";

    ui->city_lb->setText(OpenWeatherMap.getWName());

	ui->whum_lb->setText(QString::number(OpenWeatherMap.getWHumidity()) + "%");
	ui->wwind_lb->setText(QString::number(OpenWeatherMap.getWWindSpeed()) + "m/s");
    ui->wvisibility_lb->setText(QString::number(OpenWeatherMap.getWVisibility() / 1000) + "km");
	ui->wtemp_lb->setText(QString::number(OpenWeatherMap.getWTemperature()) + "°");

	QString path = APP_IMAGE256_PATH "/" + OpenWeatherMap.getWIcon() + ".png";
	QImage image(path);
	ui->wticon_lb->setPixmap(QPixmap::fromImage(image));

    ui->fday_lb1->setText(convertDaytoCountry(OpenWeatherMap.getFDay1()));
    ui->fday_lb2->setText(convertDaytoCountry(OpenWeatherMap.getFDay2()));
    ui->fday_lb3->setText(convertDaytoCountry(OpenWeatherMap.getFDay3()));
    ui->fday_lb4->setText(convertDaytoCountry(OpenWeatherMap.getFDay4()));
    ui->fday_lb5->setText(convertDaytoCountry(OpenWeatherMap.getFDay5()));

    ui->ftemp_lb1->setText(QString::number(static_cast<int>(OpenWeatherMap.getFTemperature1())) + "°");
    ui->ftemp_lb2->setText(QString::number(static_cast<int>(OpenWeatherMap.getFTemperature2())) + "°");
    ui->ftemp_lb3->setText(QString::number(static_cast<int>(OpenWeatherMap.getFTemperature3())) + "°");
    ui->ftemp_lb4->setText(QString::number(static_cast<int>(OpenWeatherMap.getFTemperature4())) + "°");
    ui->ftemp_lb5->setText(QString::number(static_cast<int>(OpenWeatherMap.getFTemperature5())) + "°");

    ui->fhum_lb1->setText(QString::number(OpenWeatherMap.getFHumidity1()) + "%");
    ui->fhum_lb2->setText(QString::number(OpenWeatherMap.getFHumidity2()) + "%");
    ui->fhum_lb3->setText(QString::number(OpenWeatherMap.getFHumidity3()) + "%");
    ui->fhum_lb4->setText(QString::number(OpenWeatherMap.getFHumidity4()) + "%");
    ui->fhum_lb5->setText(QString::number(OpenWeatherMap.getFHumidity5()) + "%");

    QString path1 = APP_IMAGE32_PATH "/" + OpenWeatherMap.getFIcon1() + ".png";
    QImage image1(path1);
    ui->ficon_lb1->setPixmap(QPixmap::fromImage(image1));

    QString path2 = APP_IMAGE32_PATH "/" + OpenWeatherMap.getFIcon2() + ".png";
    QImage image2(path2);
    ui->ficon_lb2->setPixmap(QPixmap::fromImage(image2));

    QString path3 = APP_IMAGE32_PATH "/" + OpenWeatherMap.getFIcon3() + ".png";
    QImage image3(path3);
    ui->ficon_lb3->setPixmap(QPixmap::fromImage(image3));

    QString path4 = APP_IMAGE32_PATH "/" + OpenWeatherMap.getFIcon4() + ".png";
    QImage image4(path4);
    ui->ficon_lb4->setPixmap(QPixmap::fromImage(image4));

    QString path5 = APP_IMAGE32_PATH "/" + OpenWeatherMap.getFIcon5() + ".png";
    QImage image5(path5);
    ui->ficon_lb5->setPixmap(QPixmap::fromImage(image5));

    qDebug() << "getWName:" << OpenWeatherMap.getWName();
//	qDebug() << "getWDescription:" << OpenWeatherMap.getWDescription();
//	qDebug() << "getWIcon:" << OpenWeatherMap.getWIcon();
//	qDebug() << "getWTemperature:" << OpenWeatherMap.getWTemperature();
//	qDebug() << "getWTemperatureMax:" << OpenWeatherMap.getWTemperatureMax();
//	qDebug() << "getWTemperatureMin:" << OpenWeatherMap.getWTemperatureMin();
//	qDebug() << "getWHumidity:" << OpenWeatherMap.getWHumidity();
//	qDebug() << "getWPressure:" << OpenWeatherMap.getWPressure();
//	qDebug() << "getWVisibility:" << OpenWeatherMap.getWVisibility();
//	qDebug() << "getWWindSpeed:" << OpenWeatherMap.getWWindSpeed();
	qDebug() << "getWTimestamp:" << OpenWeatherMap.getWTimestamp();

    qDebug() << "getFDay1:" << OpenWeatherMap.getFDay1();
//	qDebug() << "getFIcon1:" << OpenWeatherMap.getFIcon1();
//	qDebug() << "getFTemperature1:" << OpenWeatherMap.getFTemperature1();
//	qDebug() << "getFHumidity1:" << OpenWeatherMap.getFHumidity1();

	qDebug() << "getFDay2:" << OpenWeatherMap.getFDay2();
//	qDebug() << "getFIcon2:" << OpenWeatherMap.getFIcon2();
//	qDebug() << "getFTemperature2:" << OpenWeatherMap.getFTemperature2();
//	qDebug() << "getFHumidity2:" << OpenWeatherMap.getFHumidity2();

    qDebug() << "getFDay3:" << OpenWeatherMap.getFDay3();
//	qDebug() << "getFIcon3:" << OpenWeatherMap.getFIcon3();
//	qDebug() << "getFTemperature3:" << OpenWeatherMap.getFTemperature3();
//	qDebug() << "getFHumidity3:" << OpenWeatherMap.getFHumidity3();

	qDebug() << "getFDay4:" << OpenWeatherMap.getFDay4();
//	qDebug() << "getFIcon4:" << OpenWeatherMap.getFIcon4();
//	qDebug() << "getFTemperature4:" << OpenWeatherMap.getFTemperature4();
//	qDebug() << "getFHumidity4:" << OpenWeatherMap.getFHumidity4();

    qDebug() << "getFDay5:" << OpenWeatherMap.getFDay5();
//    qDebug() << "getFIcon5:" << OpenWeatherMap.getFIcon5();
//    qDebug() << "getFTemperature5:" << OpenWeatherMap.getFTemperature5();
//    qDebug() << "getFHumidity5:" << OpenWeatherMap.getFHumidity5();
}

void MainWindow::refreshClockUi() {
//    qDebug() << "refreshClockUi()";
	QString hhmm = QTime::currentTime().toString("HH:mm");
	QString ss = QTime::currentTime().toString("ss");
	ui->time_lb->setText(hhmm);
	ui->sec_lb->setText(ss);
	//	qDebug() << hhmm << ":" << ss;
}

void MainWindow::refreshDateUi() {
	qDebug() << "refreshDateUi()";
	QDateTime datetime = QDateTime::currentDateTime();
    QString datetime_str = convertDatetoCountry(datetime.toString("dddd, dd MMMM yyyy"));
    ui->date_lb->setText(datetime_str);
	qDebug() << datetime_str;
}

void MainWindow::changeNewsUi() {
//    qDebug() << "changeNewsUi()";
    ui->news_lb->setText(Rss.takeNextNews());
}

void MainWindow::refreshConfig() {
    qDebug() << "refreshConfig()";
    Config.getCloud(APP_CLOUD_CONFIG_URL);
    Config.printDetail();
    checkForUpdate();
    OpenWeatherMap.setParams(Config.host, Config.app_id, Config.lang, Config.units, Config.lat, Config.lon);
}

QString MainWindow::convertDatetoCountry(QString datetime) {
	if (Config.lang != "vi") return datetime;
	return datetime
            .replace("Monday", "Thứ hai")
            .replace("Tuesday", "Thứ ba")
            .replace("Wednesday", "Thứ tư")
            .replace("Thursday", "Thứ năm")
            .replace("Friday", "Thứ sáu")
            .replace("Saturday", "Thứ bảy")
            .replace("Sunday", "Chủ nhật")
            .replace("January", "Tháng một")
            .replace("February", "Tháng hai")
            .replace("March", "Tháng ba")
            .replace("April", "Tháng bốn")
            .replace("May", "Tháng năm")
            .replace("June", "Tháng sáu")
            .replace("July", "Tháng bảy")
            .replace("August", "Tháng tám")
            .replace("September", "Tháng chín")
            .replace("October", "Tháng mười")
            .replace("November", "Tháng mười một")
            .replace("December", "Tháng mười hai");
}

QString MainWindow::convertDaytoCountry(QString day) {
	if (Config.lang != "vi") return day;
	return day
            .replace("Monday", "Thứ hai")
            .replace("Tuesday", "Thứ ba")
            .replace("Wednesday", "Thứ tư")
            .replace("Thursday", "Thứ năm")
            .replace("Friday", "Thứ sáu")
            .replace("Saturday", "Thứ bảy")
            .replace("Sunday", "Ch nhật");
}

void MainWindow::checkForUpdate() {
	if (Config.latest_firmware_version > APP_FIRMWARE_VERSION) {
        if (downloadFile(APP_CLOUD_FIRMWARE_PATH, APP_LOCAL_FIRMWARE_PATH)) {
            connect(&quitTimer, SIGNAL(timeout()), qApp, SLOT(quit()));
            quitTimer.start(1000);
            qDebug() << "connecting to qApp::quit()";
        }
	}
}

bool MainWindow::downloadFile(QString url, QString local) {
	qDebug() << "downloadFile()";
	bool rcode = false;
	QNetworkAccessManager manager;
	QNetworkReply* reply;
	QByteArray ba;
	QEventLoop connection_loop;
	QUrl serviceURL(url);
	QNetworkRequest request(serviceURL);
	QFile file(local);

	if (!file.open(QIODevice::WriteOnly)) {
		rcode = false;
	}
	else {
		connect(&manager, SIGNAL( finished( QNetworkReply* ) ), &connection_loop, SLOT( quit() ) );
		reply = manager.get(request);
		connection_loop.exec();
		if(reply->error()) {
			qDebug() << reply->errorString();
			rcode = false;
		}
		else {
			ba = reply->readAll();
			qDebug() <<  "length: " << ba.length();
			rcode = true;

			file.write(ba);
			file.close();
		}
		reply->deleteLater();
	}

	return rcode;
}
