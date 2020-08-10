#include "urlstrings.h"

UrlStrings::UrlStrings()
{
	URL = "http://localhost:8080";

    URLMainInfo = URL + "/resources/Info/mainInfo-Eng.html";
    URLLicenseInfo = URL + "/resources/Info/licenseInfo-Eng.html";

    URLOSData = "/OSData.do";
    URLOSFile = "/OSFileDownload.do";
    URLDeviceVolume="/devicevolume/volumeAllToClient.do";
}

UrlStrings::~UrlStrings()
{

}

