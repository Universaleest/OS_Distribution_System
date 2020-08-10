#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMetaObject>
#include <QFile>
#include <QMutex>

#include "httpheaders.h"

class HttpThread : public QThread
{
     Q_OBJECT

public:
    HttpThread();
    ~HttpThread();

     QNetworkAccessManager* manager;
	 QNetworkAccessManager* manager2; //Added by LEE jeun jeun@wayne-inc.com
     QString nowProtocol;

     void Wake();
     void run();

     QFile* updateFile;
	 QString IPAddr; //Added by LEE jeun jeun@wayne-inc.com
	 QString getFileName();


private:
     int slideCount =0;
     int slideIndex = 0; //now index


	 QString updateFileName;
     //thread Var
	 // ~ Modified by LEE jeun jeun@wayne-inc.com
	 /*QMutex syncPause;
     QWaitCondition pauseCond;*/
	 // Modified by LEE jeun jeun@wayne-inc.com ~

signals:
    void DownloadStatus(int index,int count);
    void DownloadDeviceVolume(QStringList list );

public slots:
    void ReplyFinished (QNetworkReply *reply);
    void RequestOSData();
    void RequestOSFile();
    void ReqeustDeviceVolume();
	void gotReply(QNetworkReply* networkReply); //Added by LEE jeun jeun@wayne-inc.com
};

#endif // HTTPTHREAD_H
