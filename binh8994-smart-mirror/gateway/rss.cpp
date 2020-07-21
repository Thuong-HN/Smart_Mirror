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
#include <QSsl>
#include <QEventLoop>
#include <QtXml>

#include "rss.h"

rss::rss(QObject *parent) : QObject(parent) {
    (void) parent;
    _counter = 0;
    _url = "https://vnexpress.net/rss/tin-moi-nhat.rss";
}

void rss::setParams(QString url) {
    _url = url;
}

void rss::refreshNews() {
    qDebug() << "refreshNews()";
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QUrl serviceURL(_url);
    QNetworkRequest request(serviceURL);
    QEventLoop loop;

    connect(&manager, SIGNAL( finished( QNetworkReply* ) ), &loop, SLOT( quit() ) );
    reply = manager.get(request);
    loop.exec();

    if(reply->error()) {
        qDebug() << reply->errorString();
    }
    else if (reply->bytesAvailable() > 0){
        QByteArray ba = reply->readAll();
        //        qDebug() << ba;

        _newsList.clear();
        QDomDocument xmlBOM;
        xmlBOM.setContent(ba);
        QDomElement rssElem = xmlBOM.documentElement();
        QDomElement channelElem = rssElem.firstChildElement("channel");
        QDomElement itemElem = channelElem.firstChildElement("item");
        for (; !itemElem.isNull(); itemElem = itemElem.nextSiblingElement("item")) {
            QDomElement titleElem = itemElem.firstChildElement("title");
            //            qDebug() << titleElem.text();
            _newsList.append(titleElem.text());
        }
        qDebug() << "_newsList.size():" << _newsList.size();
    }
    else {
        qDebug() << "something went wrong!";
    }
    reply->deleteLater();
}

QString rss::takeNextNews() {
    return _newsList[++_counter % _newsList.size()];
}

rss Rss;
