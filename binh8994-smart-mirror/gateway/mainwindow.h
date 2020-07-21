#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void initUi();

private:
	Ui::MainWindow *ui;
    QTimer quitTimer;
    QTimer refreshDataTimer;
	QTimer refreshClockTimer;
	QTimer refreshDateTimer;
    QTimer updateConfigTimer;
    QTimer changeNewsTimer;
    QTimer refreshNewsTimer;

	QString convertDaytoCountry(QString day);
	QString convertDatetoCountry(QString datetime);

	void checkForUpdate();
	bool downloadFile(QString url, QString local);

private slots:
	void refreshWeatherUi();
	void refreshClockUi();
	void refreshDateUi();
    void changeNewsUi();
    void refreshConfig();
};
#endif // MAINWINDOW_H
