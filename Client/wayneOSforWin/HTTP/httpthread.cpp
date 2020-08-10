#include "httpthread.h"

#include "Reqs/reqosdata.h"
#include "Reqs/reqosfile.h"
#include "Reqs/reqdevicevolume.h"

#include <QDir>

#include <QMessageBox>
#include <QApplication>



HttpThread::HttpThread()
{
    manager = new QNetworkAccessManager(this);
	// ~ Added by LEE Jeun jeun@wayne-inc.com
	manager2 = new QNetworkAccessManager(this);
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(ReplyFinished(QNetworkReply*)));
	QObject::connect(manager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotReply(QNetworkReply*)));
    manager2->get(QNetworkRequest(QUrl("http://ip-api.com/json")));
	updateFile = NULL;
	// Added by LEE Jeun jeun@wayne-inc.com ~
    slideIndex = 0;
    slideCount = 0;

}

HttpThread::~HttpThread()
{
	if (manager != NULL)
	{
		delete manager;
	}
	// ~ Added by LEE jeun jeun@wayne-inc.com
	if (manager2 != NULL)
	{
		delete manager2;
	}
	if (updateFile != NULL)
	{
		delete updateFile;
	}
	// Added by LEE jeun jeun@wayne-inc.com ~
}


void HttpThread::Wake()

{
    qDebug()<<"nowProtocol"<<nowProtocol;

    if(nowProtocol == "reqOSData")
    {
        qDebug()<<"reqOSData Call";

        QMetaObject::invokeMethod(this, "RequestOSData", Qt::DirectConnection);

    }
    else if(nowProtocol == "reqOSFile")
    {
        QMetaObject::invokeMethod(this, "RequestOSFile", Qt::DirectConnection);
    }
    else if(nowProtocol == "reqDeviceVolume")
    {
        QMetaObject::invokeMethod(this, "ReqeustDeviceVolume", Qt::DirectConnection);
    }

}
void HttpThread::run()
{
    qDebug()<<"run start";

	// ~ Modified by LEE jeun jeun@wayne-inc.com
    /*syncPause.lock();
    pauseCond.wait(&syncPause);
	syncPause.unlock();*/
	// Modified by LEE jeun jeun@wayne-inc.com ~
}


void HttpThread::RequestOSData()
{
     ReqOsData reqOsData(manager);
}

void HttpThread::ReqeustDeviceVolume()
{
    ReqDeviceVolume ReqDeviceVolume(manager);
}

void HttpThread::RequestOSFile()
{
    if(slideIndex ==0)
    {
        qDebug()<<"File Create";


        if(!QFile::exists("updStatus"))
        {
            QDir dir;
            dir.mkdir("updStatus");
        }
		updateFile = new QFile("updStatus/" + getFileName()); // ~ Modified by LEE Jeun jeun@wayne-inc.com
        updateFile->open(QIODevice::WriteOnly);
        emit DownloadStatus(0,slideCount);

    }
    ReqOsFile ReqOsFile(manager,updateFileName,slideIndex,slideCount);

}


void HttpThread::ReplyFinished(QNetworkReply *reply)
{
    qDebug()<<"response call";

    if(reply->error() == QNetworkReply::NoError)
    {

       QJsonParseError err;
       QByteArray resData = reply->readAll();

       qDebug()<<"resData Size : "<<resData.size()<<endl;


       QJsonDocument jsonReturn =  QJsonDocument::fromJson(resData,&err);

       //protocol analyze..
       if(jsonReturn.isObject())
       {

           QJsonObject jsonObj =jsonReturn.object();
           QString responseType = jsonObj.find("responseType").value().toString();
		   updateFileName = jsonObj.find("updateFileName").value().toString();

		   qDebug() << "responseType :  " << responseType << endl;
		   

           if(responseType == "OSData")
           {
			   if (!updateFileName.isEmpty())  
			   {

				   this->slideCount = jsonObj.find("slideCount").value().toInt();
				   /*if (slideCount == 0)
				   {
					   QMessageBox::critical(NULL, QString::fromLocal8Bit("Error"), QString::fromLocal8Bit("An error is occured during downloading."));
					   QApplication::exit();
				   }*/ // Modified by LEE Jeun jeun@wayne-inc.com
				   RequestOSFile();
			   }
			   else
			   {
				   QString versionType = jsonObj.find("versionType").value().toString();
				   if (versionType == "PAID")
				   {
					   QMessageBox::critical(NULL, QString::fromLocal8Bit("Wrong information"), QString::fromLocal8Bit("This device is not for Registered version (Paid)."));
					   QApplication::exit();
				   }
				   else if (versionType == "FREE")
				   {
					   QMessageBox::critical(NULL, QString::fromLocal8Bit("Wrong information"), QString::fromLocal8Bit("This device is not supporting Free version."));
					   QApplication::exit();
				   }
				   
			   }

           }
       }

       else if(jsonReturn.isArray())
       {

           qDebug()<<"json Return"<<jsonReturn<<endl;

           QJsonArray jsonArr =  jsonReturn.array();

           QString responseType ="";
           for(int i=0; i<jsonArr.size(); i++)
           {
               QJsonObject jsonObj =  jsonArr.at(i).toObject();

               responseType =  jsonObj.find("responseType").value().toString();
               if(responseType =="deviceVolume")
               {
                    qDebug()<<"response Type: "<<responseType<<endl;
                    break;
               }

           }

           if(responseType == "deviceVolume")
           {

              QStringList list;

                qDebug()<<"jsonArr Size : "<<jsonArr.size();
                for(int i=0; i<jsonArr.size(); i++)
                {

                    QJsonObject jsonObj =  jsonArr.at(i).toObject();   
                    if(jsonObj.find("responseType").value().toString() == "deviceVolume")
                    {
                        break;
                    }
                    int volumeSize = jsonObj.find("volumeSize").value().toInt();
                    list.append(QString::number(volumeSize));


                }
                emit DownloadDeviceVolume(list);
           }




       }


       else
       {
           QString slideFile = QString::fromStdString(reply->rawHeader("slideFile").toStdString());

           if(slideFile == "true")
           {

               qDebug()<<"slideFile:"<<reply->rawHeader("slideFile")<<endl;
               qDebug()<<"slideIndex:"<<reply->rawHeader("slideIndex")<<endl;
               qDebug()<<"slideCount:"<<reply->rawHeader("slideCount")<<endl;

               slideIndex =  reply->rawHeader("slideIndex").toInt();
               slideCount =  reply->rawHeader("slideCount").toInt();

               if(slideIndex < slideCount)
               {
                   emit DownloadStatus(slideIndex,slideCount);

                   qDebug()<<"File write"<<slideIndex;
                   updateFile->write(resData);

                   RequestOSFile();
               }
               else if(slideIndex == slideCount){

                   updateFile->write(resData);
                   updateFile->close();
				   
				   emit DownloadStatus(slideIndex, slideCount);
                  

               }
               else
               {
                   qDebug()<<"FileDown Error";
                   QMessageBox::critical(NULL, "Connect Error", QString("can not connect to Server.."));
                   QApplication::exit();

               }

           }
       }

   }

   else
   {
		if (updateFileName != "dummy.img") // Modified by LEE Jeun jeun@wayne-inc.com
		{
			qDebug() << "error :" << reply->errorString() << endl;
			int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
			qDebug() << "statusCode" << statusCode << endl;
			qDebug() << "FileDown Error";
			QMessageBox::critical(NULL, "Connect Error", QString("can not connect to Server.."));
			QApplication::exit();
		}
   }
}

// ~ Added by LEE jeun jeun@wayne-inc.com
void HttpThread::gotReply(QNetworkReply* networkReply)
{
    QByteArray response = networkReply->readAll();
	QJsonObject Jobj = QJsonDocument::fromJson(response).object();
    IPAddr = Jobj.find("query").value().toString();
}

QString HttpThread::getFileName()
{
	return updateFileName;
}
// Added by LEE jeun jeun@wayne-inc.com ~