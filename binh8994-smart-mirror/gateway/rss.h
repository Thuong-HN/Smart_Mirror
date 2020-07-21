#ifndef RSS_H
#define RSS_H

#include <QObject>
#include <QStringList>

class rss : public QObject
{
    Q_OBJECT
public:
    explicit rss(QObject *parent = 0);
    void setParams(QString url);
    QString takeNextNews();

signals:

public slots:
    void refreshNews();

private:
    QString _url;
    QStringList _newsList;
    int _counter;
};

extern rss Rss;

#endif // RSS_H
