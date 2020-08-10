#ifndef URLSTRINGS_H
#define URLSTRINGS_H


#include <QString>

class UrlStrings
{
public:
    UrlStrings();
    ~UrlStrings();

    QString URL;
    QString URLMainInfo;
    QString URLLicenseInfo;

    QString URLOSData;
    QString URLOSFile;
    QString URLDeviceVolume;

};

#endif // URLSTRINGS_H
