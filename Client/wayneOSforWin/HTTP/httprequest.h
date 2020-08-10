#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QString>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QTNetwork/QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QObject>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrlQuery>

#include "urlstrings.h"


class HttpRequest : QObject
{
     Q_OBJECT

public:

    UrlStrings urlStrings;

    HttpRequest();
    ~HttpRequest();

    void sendRequest(QString doURL);

signals:

public slots:
    void replyFinished (QNetworkReply *reply);

private:
   QNetworkAccessManager *manager;
};

#endif // HTTPREQUEST_H
