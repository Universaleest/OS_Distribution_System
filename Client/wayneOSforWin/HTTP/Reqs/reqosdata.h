#ifndef REQOSDATA_H
#define REQOSDATA_H


#include <QtNetwork/QNetworkAccessManager>
#include <QSysInfo>
#include <intrin.h>
#include <Windows.h>
#include <cstdio>
#include <cstring>
#include <QString>
#include <string>
#include <fstream>
#include <atlstr.h>
#include <vector>

#define ID 0x80000000
// ~ Modified and Added by LEE Jeun jeun@wayne-inc.com
#define BUFSIZE 4096
#define DIV 1024
//#define CPU_INFO_QUERY_STRING "wmic cpu get name"
const CString CPU_INFO_QUERY_STRING = "wmic cpu get name";
//#define GPU_INFO_QUERY_STRING "wmic path win32_videocontroller get name"
const CString GPU_INFO_QUERY_STRING = "wmic path win32_videocontroller get name";
//#define RAM_INFO_QUERY_STRING "wmic memorychip get capacity, speed"
const CString RAM_INFO_QUERY_STRING = "wmic memorychip get capacity, speed";
//#define LOCAL_OS_INFO_QUERY_STRING "wmic os get caption, osarchitecture, muilanguages"
const CString LOCAL_OS_INFO_QUERY_STRING = "wmic os get caption, osarchitecture, muilanguages";

typedef struct _RAMINFO
{
	long long capacity;
	int ClockSpeed;
	int byteToGB() { return capacity / (DIV*DIV*DIV); }
}RamInfo;

typedef struct _OSINFO
{
	std::string OSver;
	TCHAR bit[16];
	QString lang;
}OSInfo;

class ReqOsData
{
public:
    ReqOsData(QNetworkAccessManager* manager);
    ~ReqOsData();
	QJsonDocument GetInstallInfoData();
	// ~ Added by LEE jeun jeun@wayne-inc.com
	QString GetCpuName(); 
	QString GetGpuName(); 
	RamInfo RAM;
	OSInfo OS;
	std::string getHwInfo(CString command);
	RamInfo getRamInfo();
	OSInfo getOSInfo();
	ULONGLONG GetSelectedDiskCapacity();
	// Added by LEE Jeun jeun@wayne-inc.com ~
};
// ~ Modified and Added by LEE Jeun jeun@wayne-inc.com

#endif // REQOSDATA_H
