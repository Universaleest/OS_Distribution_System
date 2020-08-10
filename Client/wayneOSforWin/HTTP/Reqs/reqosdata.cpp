#include "reqosdata.h"

#include "../httpheaders.h"
#include "../urlstrings.h"

#include "../../Info/infomanager.h"
#include "../../Disk/diskmanager.h"

#include "HTTP/httpmanager.h"
#include "HTTP/httpthread.h"

#include <QMessageBox>
#include <QCoreApplication>
#include <qnetworkinterface>
#include <map>
#include <utility>
#include <algorithm>


ReqOsData::ReqOsData(QNetworkAccessManager* manager)
{
    UrlStrings urlStrings;
    QUrl url(urlStrings.URL + urlStrings.URLOSData);

    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json;charset=UTF-8");
    manager->post(request,GetInstallInfoData().toJson());
}

ReqOsData::~ReqOsData()
{

}

QJsonDocument ReqOsData::GetInstallInfoData()
{
    InfoManager* infoManager = InfoManager::GetInstance();

    QJsonObject jsonObject;
    QJsonDocument jsonDoc;

    if(infoManager->mVersionType == InfoManager::FREE) jsonObject.insert("versionType","FREE");
    else if(infoManager->mVersionType == InfoManager::PAID) jsonObject.insert("versionType","PAID");

    //Device Type Check
	jsonObject.insert("deviceType", "USB");
    //if(infoManager->mDeviceType == InfoManager::USB) jsonObject.insert("deviceType","USB");
    //else if(infoManager->mDeviceType == InfoManager::SSD) jsonObject.insert("deviceType","SSD");

   
	jsonObject.insert("deviceVolume",QString::number(infoManager->mSize));
	//USB or SSD Info(ex. serial Number...)
	QString serialNumber = DiskManager::GetInstance()->mSerialNumber.trimmed();
	QString productID = DiskManager::GetInstance()->mProductId.trimmed();
	QString vendorID = DiskManager::GetInstance()->mVendorId.trimmed();
	QString productRevision = DiskManager::GetInstance()->mProductRevision.trimmed();

	// ~ Modified by LEE Jeun jeun@wayne-inc.com
	//if (infoManager->mVersionType == InfoManager::PAID) jsonObject.insert("productID", "");
	//else jsonObject.insert("productID", productID);
	jsonObject.insert("productID", productID);
	// Modified by LEE Jeun jeun@wayne-inc.com

	jsonObject.insert("vendorID", vendorID);
	jsonObject.insert("productRevision", productRevision);
	jsonObject.insert("serialNumber", serialNumber);
	// ~ Added by LEE jeun jeun@wayne-inc.com
	ULONGLONG realCapacity = GetSelectedDiskCapacity();
	qDebug() << "real capacity: " << realCapacity << "bytes";
	jsonObject.insert("realCapacity", QJsonValue::fromVariant(realCapacity));

	qDebug() << HttpManager::GetInstance()->httpThread.IPAddr;
	jsonObject.insert("externalIP", HttpManager::GetInstance()->httpThread.IPAddr);
	
	RAM = getRamInfo();
	qDebug() << RAM.byteToGB() << "GB";
	jsonObject.insert("ramSize", RAM.byteToGB());

	qDebug() << RAM.ClockSpeed << "MHz";
	jsonObject.insert("ramClockSpeed", RAM.ClockSpeed);

	OS = getOSInfo();
	QString LocalOSver = OS.OSver.c_str(), LocalOSbit = QString::fromWCharArray(OS.bit);
	qDebug() << LocalOSver << " " << LocalOSbit << " " << OS.lang;
	jsonObject.insert("LocalOSinfo", LocalOSver + " " + LocalOSbit + " " + OS.lang);

	QString CPU = GetCpuName();
	//jsonObject.insert("customerInfo0", "CPU: " + CPU);
	qDebug() << CPU;
	jsonObject.insert("CPU", CPU);

	QString GPU = GetGpuName();
	//jsonObject.insert("customerInfo1", "GPU: " + GPU);
	qDebug() << GPU;
	jsonObject.insert("GPU", GPU);
	// Added by LEE jeun jeun@wayne-inc.com ~
	
	// ~ Modified by LEE jeun jeun@wayne-inc.com
	QList<QHostAddress> list = QNetworkInterface::allAddresses();

	for (int nIter = 0; nIter<list.count(); nIter++)

	{
		if (!list[nIter].isLoopback())
		{

			if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
			{
				qDebug() << list[nIter].toString();
				jsonObject.insert("internalIP", list[nIter].toString());
				break;
			}
		}

	} 
	// Modified by LEE jeun jeun@wayne-inc.com ~
    jsonDoc.setObject(jsonObject);
    qDebug()<<"send Data : "<<jsonDoc.toJson()<<endl;
    return jsonDoc;

}

// ~ Added by LEE jeun jeun@wayne-inc.com
/*QString ReqOsData::GetCpuName()
{
	QString ProccessorName;
	int CPUInfo[4] = { -1 };
	char CPUBrandString[0x40];
	__cpuid(CPUInfo, ID);
	unsigned int nExIDs = CPUInfo[0];
	memset(CPUBrandString, 0, sizeof(CPUBrandString));
	for (int i = ID; i <= nExIDs; ++i)
	{
		__cpuid(CPUInfo, i);
		if (i == ID + 2) memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == ID + 3) memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == ID + 4) memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	int size = strlen(CPUBrandString);
	for (int i = 0; i < size - 2; ++i)
	{
		if (CPUBrandString[i] == ' ' && CPUBrandString[i + 1] == ' ' && CPUBrandString[i + 2] == ' ') CPUBrandString[i] = 0;
	}
	ProccessorName = CPUBrandString;
	return ProccessorName;
}*/

QString ReqOsData::GetCpuName()
{
	std::string Data = getHwInfo(CPU_INFO_QUERY_STRING);
	int start = 0, end = 0;
	start = Data.find("\n") + 1;
	end = Data.find("  ", start);
	QString ProcessorName = Data.substr(start, end - start).c_str();
	return ProcessorName;
}

std::string ReqOsData::getHwInfo(CString command)
{
	//TCHAR cmd[BUFSIZE];
	CString cmd = command;
	//lstrcpy(cmd, command);
	HANDLE hChildStdOut_Rd = NULL, hChildStdOut_Wr = NULL;
	CHAR Buf[BUFSIZE];
	memset(Buf, 0, sizeof(Buf));
	std::string Data;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	CreatePipe(&hChildStdOut_Rd, &hChildStdOut_Wr, &sa, 0);
	SetHandleInformation(hChildStdOut_Rd, HANDLE_FLAG_INHERIT, 0);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdOutput = hChildStdOut_Wr;
	si.hStdError = hChildStdOut_Wr;
	si.wShowWindow = SW_HIDE;
	BOOL bSuccess = CreateProcess(NULL, cmd.GetBuffer(cmd.GetLength()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	cmd.ReleaseBuffer();
	if (!bSuccess)
	{
		DWORD errnum = GetLastError();
		qDebug() << errnum;
		QMessageBox::critical(NULL, QString("Error"), QString::number(errnum) + QString(" error occured!"));
		QCoreApplication::exit(-1);
	}
	DWORD dwRead = 0, dwOut = 0;
	DWORD dwExitCode = 0, dwResult = 0;
	/*while (PeekNamedPipe(hChildStdOut_Rd, NULL, 0, NULL, &dwOut, NULL))
	{
		if (dwOut <= 0 && WaitForSingleObject(pi.hProcess, 0) != WAIT_TIMEOUT) break;
		while (PeekNamedPipe(hChildStdOut_Rd, NULL, 0, NULL, &dwOut, NULL) && dwOut > 0)
		{
			ReadFile(hChildStdOut_Rd, Buf, sizeof(Buf), &dwRead, NULL);
			Buf[dwRead] = 0;
			Data += std::string(Buf);
		}
	}*/
	while (dwExitCode == 0)
	{
		dwResult = WaitForSingleObject(pi.hProcess, 1000);
		if (PeekNamedPipe(hChildStdOut_Rd, NULL, 0, NULL, &dwOut, NULL) && dwOut > 0)
		{
			ReadFile(hChildStdOut_Rd, Buf, sizeof(Buf), &dwRead, NULL);
			Data += std::string(Buf);
		}

		if (dwResult != WAIT_TIMEOUT)
		{
			dwExitCode = 1;
		}
	}

	CloseHandle(hChildStdOut_Rd);
	CloseHandle(hChildStdOut_Wr);

	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	return Data;
}

RamInfo ReqOsData::getRamInfo()
{
	RamInfo ri;
	memset(&ri, 0, sizeof(ri));
	std::vector<std::string> infoArray;
	std::string tmp;
	bool toggle = false;
	int cnt = 0;
	memset(&ri, 0, sizeof(ri));
	std::string Data = getHwInfo(RAM_INFO_QUERY_STRING);
	for (auto it = Data.begin(); it != Data.end(); ++it)
	{
		if (*it != ' ' && *it != '\r' && *it != '\n') toggle = true;
		else
		{
			cnt = *it == ' ' ? cnt + 1 : cnt;
			toggle = false;
		}
		if (toggle) tmp += *it;
		else
		{
			if (cnt > 1)
			{
				if (!tmp.empty() && tmp.find("  ", 0) == std::string::npos)
				{
					infoArray.push_back(tmp);
					tmp.clear();
					cnt = 0;
				}
			}
		}
	}
	for (int i = 2; i < infoArray.size(); ++i)
	{
		if (i % 2 == 0)
		{
			ri.capacity += atoll(infoArray[i].c_str());
		}
	}
	ri.ClockSpeed = atoi(infoArray[infoArray.size() - 1].c_str());
	return ri;
}

/*OSInfo ReqOsData::getOSInfo()
{
	OSInfo oi;
	std::vector<std::string> infoArray;
	std::string tmp;
	bool toggle = false;
	int cnt = 0;
	memset(&oi, 0, sizeof(oi));
	std::string Data = getHwInfo(_T(LOCAL_OS_INFO_QUERY_STRING));
	for (auto it = Data.begin(); it != Data.end(); ++it)
	{
		if (*it != ' ' && *it != '\r' && *it != '\n') toggle = true;
		else
		{
			cnt = *it == ' ' ? cnt + 1 : cnt;
			toggle = false;
		}
		if (toggle) tmp += *it;
		else
		{
			if (cnt > 1)
			{
				if (!tmp.empty() && tmp.find("  ", 0) == std::string::npos)
				{
					infoArray.push_back(tmp);
					tmp.clear();
					cnt = 0;
				}
			}
		}
		cnt = toggle || cnt > 1 ? 0 : cnt;
	}
	oi.OSver = infoArray[2];
	char tmpBuf[8];
	strcpy(tmpBuf, infoArray[3].c_str());
	MultiByteToWideChar(CP_ACP, 0, tmpBuf, sizeof(tmpBuf), oi.bit, sizeof(oi.bit));
	return oi;
}*/

OSInfo ReqOsData::getOSInfo()
{
	const std::string caption = "Caption";
	const std::string osarchitecture = "OSArchitecture";
	const std::string muilanguages = "MUILanguages";

	OSInfo oi;

	std::string Data = getHwInfo(LOCAL_OS_INFO_QUERY_STRING);

	std::string value;

	std::map<int, std::string> idxMap;
	std::map<int, std::string>::iterator it;

	std::map<std::string, std::string> OsInfo;

	std::pair<int, std::string> largest;

	int idx = 0, subIdx1 = Data.find(caption), subIdx2 = Data.find(osarchitecture), subIdx3 = Data.find(muilanguages);

	idxMap[subIdx1] = caption;
	idxMap[subIdx2] = osarchitecture;
	idxMap[subIdx3] = muilanguages;
	it = idxMap.begin();

	largest = *(--idxMap.end());

	idx = largest.first + largest.second.size();

	while (idx < Data.size())
	{
		if ((Data[idx] != '\n') && (Data[idx] != '\r') && (Data[idx] != ' '))
		{
			value += Data[idx];
		}

		else if (Data[idx] == ' ')
		{
			if (idx < Data.size() - 1 && Data[idx + 1] == ' ' && value.size() != 0)
			{
				OsInfo[it->second] = value;
				value.clear();
				++it;
			}

			else if (idx < Data.size() - 1 && Data[idx + 1] != '\n' && Data[idx + 1] != '\r' 
				&& Data[idx + 1] != ' ' && Data[idx - 1] != ' ')
			{
				value += Data[idx];
			}
		}
		++idx;
	}

	oi.OSver = OsInfo[caption];

	char tmpBuf[BUFSIZE];
	strcpy(tmpBuf, OsInfo[osarchitecture].c_str());
	memset(oi.bit, 0, sizeof(oi.bit));
	MultiByteToWideChar(CP_ACP, 0, tmpBuf, strlen(tmpBuf), oi.bit, sizeof(oi.bit));

	memset(tmpBuf, 0, sizeof(tmpBuf));
	strcpy(tmpBuf, OsInfo[muilanguages].c_str());
	TCHAR tmpWBuf[BUFSIZE];
	MultiByteToWideChar(CP_ACP, 0, tmpBuf, sizeof(tmpBuf), tmpWBuf, sizeof(tmpWBuf));
	oi.lang = QString::fromWCharArray(tmpWBuf);

	return oi;
}

QString ReqOsData::GetGpuName()
{
	const std::string name = "Name";

	std::string Data = getHwInfo(GPU_INFO_QUERY_STRING);

	std::string value;

	std::vector<std::string> gpuArr;

	int idx = 0;
	idx = Data.find(name) + name.size();

	while (idx < Data.size())
	{
		if ((Data[idx] != '\n') && (Data[idx] != '\r') && (Data[idx] != ' '))
		{
			value += Data[idx];
		}
		
		else if (Data[idx] == ' ')
		{
			if (idx < Data.size() - 1 && Data[idx + 1] == ' ' && value.size() != 0)
			{
				gpuArr.push_back(value);
				value.clear();
			}

			else if (idx < Data.size() - 1 && Data[idx + 1] != '\n' && Data[idx + 1] != '\r' && Data[idx + 1] != ' ' && Data[idx - 1] != ' ')
			{
				value += Data[idx];
			}
		}
		++idx;
	}

	QString ProcessorName;

	ProcessorName += "[";

	for (int i = 0; i < gpuArr.size(); ++i)
	{
		qDebug() << "GPU" << i + 1 << ": " << gpuArr[i].c_str();
		ProcessorName += "{";
		ProcessorName += gpuArr[i].c_str();
		ProcessorName += "}";
		ProcessorName += i < gpuArr.size() - 1 ? ", " : "]";
	}

	return ProcessorName;
}

ULONGLONG ReqOsData::GetSelectedDiskCapacity()
{
	/*QString path = InfoManager::GetInstance()->mDriveInstallPath.section("", 2, 3);
	TCHAR DrivePath[8];
	memset(DrivePath, 0, sizeof(DrivePath));
	path.toWCharArray(DrivePath);
	ULARGE_INTEGER lpFreeByteAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
	//UINT DiskType = GetDriveType(DrivePath);
	BOOL bResult = GetDiskFreeSpaceEx(DrivePath, &lpFreeByteAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes);
	if (!bResult) return 0;
	LONGLONG selectedDiskCapacity = lpTotalNumberOfBytes.QuadPart;
	return selectedDiskCapacity;*/
	int device = InfoManager::GetInstance()->mDeviceId;
	std::string selected = "\\\\?\\PhysicalDrive" + std::to_string(device);

	BYTE geometry[256];
	ZeroMemory(geometry, sizeof(geometry));
	PDISK_GEOMETRY_EX diskGeometry = (PDISK_GEOMETRY_EX)(void*)geometry;

	HANDLE hd = INVALID_HANDLE_VALUE;
	DWORD size = 0;
	BOOL bResult = FALSE;

	hd = CreateFileA(selected.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);
	if (hd == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	bResult = DeviceIoControl(hd, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, 
		NULL, 0, geometry, sizeof(geometry), &size, NULL);
	if (!bResult)
	{
		return 0;
	}
	
	return diskGeometry->DiskSize.QuadPart;
}
// Added by LEE jeun jeun@wayne-inc.com ~


