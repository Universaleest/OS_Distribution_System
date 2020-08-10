#include "reqdevicevolume.h"

#include "../httpheaders.h"
#include "../urlstrings.h"

ReqDeviceVolume::ReqDeviceVolume(QNetworkAccessManager* manager)
{
    UrlStrings urlStrings;
    QUrl url(urlStrings.URL + urlStrings.URLDeviceVolume);

    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json;charset=UTF-8");
    manager->post(request,"");
}

ReqDeviceVolume::~ReqDeviceVolume()
{

}

