#include "httpmanager.h"

#include "Reqs/reqosdata.h"
#include "Reqs/reqosfile.h"



#include <QDir>

HttpManager* HttpManager::mThis;


HttpManager::HttpManager()
{

}

HttpManager::~HttpManager()
{
	
}

void HttpManager::RequestOSData()
{
    httpThread.nowProtocol ="reqOSData";
    httpThread.Wake();
}

void HttpManager::RequestOSFile()
{
    httpThread.nowProtocol ="reqOSFile";
    httpThread.Wake();

}

void HttpManager::RequestDeviceVolume()
{
    httpThread.nowProtocol ="reqDeviceVolume";
    httpThread.Wake();
}

void HttpManager::DoWork()
{
    this->moveToThread(&httpThread);
    httpThread.start();
}


