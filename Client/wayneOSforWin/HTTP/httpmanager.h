#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H


#include "httpheaders.h"
#include "httpthread.h"

#include <QFile>


class HttpManager : public QObject
{
    Q_OBJECT

public:
    static HttpManager* GetInstance()
    {
        if(mThis == NULL)
        {
            mThis = new HttpManager();
        }
        return mThis;

    }
    ~HttpManager();

    HttpThread httpThread;

    void RequestOSData();
    void RequestOSFile();
    void RequestDeviceVolume();

    void DoQuit();

private:
    HttpManager();
protected:
   static HttpManager* mThis;


signals:
public slots:
     void DoWork();

};

#endif // HTTPMANAGER_H
