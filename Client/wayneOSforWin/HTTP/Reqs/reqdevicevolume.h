#ifndef REQDEVICEVOLUME_H
#define REQDEVICEVOLUME_H

#include <QtNetwork/QNetworkAccessManager>


class ReqDeviceVolume
{
public:
    ReqDeviceVolume(QNetworkAccessManager* manager);
    ~ReqDeviceVolume();
};

#endif // REQDEVICEVOLUME_H
