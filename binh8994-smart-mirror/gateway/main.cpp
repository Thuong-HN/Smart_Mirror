#include <stdlib.h>
#include "mainwindow.h"
#include "config.h"

#include <QApplication>
#include <QProcess>
#include <QProcessEnvironment>
#include <QFile>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if (QFile::exists(APP_LOCAL_FIRMWARE_PATH)) {
		QFile::remove(APP_LOCAL_FIRMWARE_PATH);
	}

	MainWindow w;
	w.show();

	return a.exec();
}
