#include "httprequest.h"

#include <QDebug>

HttpRequest::HttpRequest()
{
    manager = new QNetworkAccessManager();

}

HttpRequest::~HttpRequest()
{
    qDebug()<<"httpmanager Delete";
    if(manager != NULL) delete manager;

}

void HttpRequest::sendRequest(QString doURL)
{
    QUrl url(urlStrings.URL + doURL);
    //QUrl url("" + doURL);
    QNetworkRequest request(url);



    connect(manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    QJsonObject jsonObject;
    jsonObject.insert("test1","test1");
    jsonObject.insert("test2","test2");

    QJsonDocument doc;
    doc.setObject(jsonObject);

    request.setRawHeader("Content-Type", "application/json;charset=UTF-8");


    qDebug()<<"request"<<endl<<request.rawHeader("content-Type");
    manager->post(request,doc.toJson());
}

void HttpRequest::replyFinished (QNetworkReply *reply)
{
    qDebug()<<"reponse"<<endl<<reply->readAll();
}
