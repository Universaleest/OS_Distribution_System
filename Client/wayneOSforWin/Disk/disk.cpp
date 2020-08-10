/**********************************************************************
*  This program is free software; you can redistribute it and/or     *
*  modify it under the terms of the GNU General Public License       *
*  as published by the Free Software Foundation; either version 2    *
*  of the License, or (at your option) any later version.            *
*                                                                    *
*  This program is distributed in the hope that it will be useful,   *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
*  GNU General Public License for more details.                      *
*                                                                    *
*  You should have received a copy of the GNU General Public License *
*  along with this program; if not, write to the Free Software       *
*  Foundation, Inc., 51 Franklin Street, Fifth Floor,                *
*  Boston, MA  02110-1301, USA.                                      *
*                                                                    *
*  ---                                                               *
*  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
*  Copyright (C) 2009, 2012 ImageWriter developers                   *
*                           https://launchpad.net/~image-writer-devs *
**********************************************************************/

#ifndef WINVER
#define WINVER 0x0500
#endif

#include <QtGui>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define NOMINMAX
#include <windows.h>
#include <winioctl.h>
#include "disk.h"
#include <QString>

//serial number
#include <WinIOCtl.h>
//#include <usbioctl.h>
#include <Setupapi.h>

DEFINE_GUID(GUID_DEVINTERFACE_USB_DISK,
	0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2,
	0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

#include "diskmanager.h"
HANDLE getHandleOnFile(char *filelocation, DWORD access)
{
	HANDLE hFile;
	char hdr[] = "\\\\.\\";
	// 'char *hdr = "foo"' gives a warning about string conversion
	int locLen = strlen(hdr) + strlen(filelocation) + 1;
	char *location = new char[locLen];
	memset(location, 0, locLen);
	sprintf(location, "%s%s", hdr, filelocation);
	//hFile = CreateFile(location, access, 0, NULL, (access == GENERIC_READ) ? OPEN_EXISTING:CREATE_ALWAYS, 0, NULL);
	hFile = CreateFileA(location, access, 0, NULL, (access == GENERIC_READ) ? OPEN_EXISTING : CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "File Error", QString::fromLocal8Bit("An error occurred when attempting to get a handle on the file.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	delete location;
	location = NULL;
	return hFile;
}

HANDLE getHandleOnDevice(int device, DWORD access)
{
	HANDLE hDevice;
	QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(device);
	//hDevice = CreateFile(devicename.toLatin1(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	hDevice = CreateFileA(devicename.toLatin1(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	//hDevice = CreateFile(devicename.toAscii().data(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Device Error", QString::fromLocal8Bit("An error occurred when attempting to get a handle on the device.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	return hDevice;
}

HANDLE getHandleOnVolume(int volume, DWORD access)
{
	HANDLE hVolume;
	char volumename[] = "\\\\.\\A:";
	volumename[4] += volume;
	//hVolume = CreateFile(volumename, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	hVolume = CreateFileA(volumename, access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hVolume == INVALID_HANDLE_VALUE)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Volume Error", QString::fromLocal8Bit("An error occurred when attempting to get a handle on the volume.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	return hVolume;
}

bool getLockOnVolume(HANDLE handle)
{
	DWORD bytesreturned;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesreturned, NULL);
	if (!bResult)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Lock Error", QString::fromLocal8Bit("An error occurred when attempting to lock the volume.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	return (bResult);
}

bool removeLockOnVolume(HANDLE handle)
{
	DWORD junk;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
	if (!bResult)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Unlock Error", QString::fromLocal8Bit("An error occurred when attempting to unlock the volume.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	return (bResult);
}

bool unmountVolume(HANDLE handle)
{
	DWORD junk;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
	if (!bResult)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Dismount Error", QString::fromLocal8Bit("An error occurred when attempting to dismount the volume.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	return (bResult);
}

bool isVolumeUnmounted(HANDLE handle)
{
	DWORD junk;
	BOOL bResult;
	bResult = DeviceIoControl(handle, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &junk, NULL);
	return (!bResult);
}

char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
	unsigned long bytesread;
	char *data = new char[sectorsize * numsectors];
	LARGE_INTEGER li;
	li.QuadPart = startsector * sectorsize;
	SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN);
	if (!ReadFile(handle, data, sectorsize * numsectors, &bytesread, NULL))
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Read Error", QString::fromLocal8Bit("An error occurred when attempting to read data from handle.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
		delete data;
		data = NULL;
	}
	return data;
}

bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
	unsigned long byteswritten;
	BOOL bResult;
	LARGE_INTEGER li;
	li.QuadPart = startsector * sectorsize;
	SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN);
	bResult = WriteFile(handle, data, sectorsize * numsectors, &byteswritten, NULL);
	if (!bResult)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		
		std::wstring errmsg = getWerrmsg(errormessage);
		
		QMessageBox::critical(NULL, "Write Error", QString::fromLocal8Bit("An error occurred when attempting to write data from handle.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}
	return (bResult);
}

unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize)
{
	DWORD junk;
	DISK_GEOMETRY_EX diskgeometry;
	BOOL bResult;
	bResult = DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &diskgeometry, sizeof(diskgeometry), &junk, NULL);
	if (!bResult)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Device Error", QString::fromLocal8Bit("An error occurred when attempting to get the device's geometry.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
		return 0;
	}
	if (sectorsize != NULL)
	{
		*sectorsize = (unsigned long long)diskgeometry.Geometry.BytesPerSector;
	}
	return (unsigned long long)diskgeometry.DiskSize.QuadPart / (unsigned long long)diskgeometry.Geometry.BytesPerSector;
}

unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize)
{
	unsigned long long retVal = 0;
	LARGE_INTEGER filesize;
	if (GetFileSizeEx(handle, &filesize) == 0)
	{
		// error
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "File Error", QString::fromLocal8Bit("An error occurred while getting the file size.\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
		retVal = 0;
	}
	else
	{
		retVal = ((unsigned long long)filesize.QuadPart / sectorsize);
	}
	return(retVal);
}

bool spaceAvailable(char *location, unsigned long long spaceneeded)
{
	ULARGE_INTEGER freespace;
	BOOL bResult;
	//bResult = GetDiskFreeSpaceEx(location, NULL, NULL, &freespace);
	bResult = GetDiskFreeSpaceExA(location, NULL, NULL, &freespace);
	if (!bResult)
	{
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "Free Space Error", QString::fromLocal8Bit("Failed to get the free space on drive %1.\nError %2: ").arg(location).arg(GetLastError()) + QString::fromStdWString(errmsg) + QString("\nChecking of free space will be skipped."));
		return true;
	}
	return (spaceneeded <= freespace.QuadPart);
}

BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc,
	DEVICE_NUMBER *devInfo)
{
	STORAGE_PROPERTY_QUERY Query; // input param for query
	DWORD dwOutBytes; // IOCTL output length
	BOOL bResult; // IOCTL return val
	BOOL retVal = true;

	// specify the query type
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	// Query using IOCTL_STORAGE_QUERY_PROPERTY 
	bResult = ::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&Query, sizeof(STORAGE_PROPERTY_QUERY), pDevDesc,
		pDevDesc->Size, &dwOutBytes, (LPOVERLAPPED)NULL);
	//QMessageBox::information(NULL, "Test ", QString((char*)pDevDesc+pDevDesc->SerialNumberOffset));
	//QMessageBox::information(NULL, "Test ", QString("size : %1").arg(sizeof(*pDevDesc)));
	if (bResult)
	{
		bResult = ::DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER,
			NULL, 0, devInfo, sizeof(DEVICE_NUMBER), &dwOutBytes,
			(LPOVERLAPPED)NULL);
		if (!bResult)
		{
			retVal = false;
			char *errormessage = NULL;
			//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

			std::wstring errmsg = getWerrmsg(errormessage);

			QMessageBox::critical(NULL, "File Error", QString::fromLocal8Bit("An error occurred while getting the device number.\nThis usually means something is currently accessing the device; please close all applications and try again.\n\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
			LocalFree(errormessage);
		}
	}
	else
	{
		retVal = false;
		char *errormessage = NULL;
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
		//After Add**********************************************************/

		std::wstring errmsg = getWerrmsg(errormessage);

		QMessageBox::critical(NULL, "File Error", QString("An error occurred while querying the properties.\nThis usually means something is currently accessing the device; please close all applications and try again.\n\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
		LocalFree(errormessage);
	}

	return(retVal);
}

// some routines fail if there's no trailing slash in a name,
// 		others fail if there is.  So this routine takes a name (trailing
// 		slash or no), and creates 2 versions - one with the slash, and one w/o
//
// 		CALLER MUST FREE THE 2 RETURNED STRINGS
bool slashify(char *str, char **slash, char **noSlash)
{
	bool retVal = false;

	int strLen = strlen(str);
	if (strLen > 0)
	{
		if (*(str + strLen - 1) == '\\')
		{
			// trailing slash exists
			if (((*slash = (char *)calloc((strLen + 1), sizeof(char))) != NULL) &&
				((*noSlash = (char *)calloc(strLen, sizeof(char))) != NULL))
			{
				strncpy(*slash, str, strLen);
				strncpy(*noSlash, *slash, (strLen - 1));
				retVal = true;
			}
		}
		else
		{
			// no trailing slash exists
			if (((*slash = (char *)calloc((strLen + 2), sizeof(char))) != NULL) &&
				((*noSlash = (char *)calloc((strLen + 1), sizeof(char))) != NULL))
			{
				strncpy(*noSlash, str, strLen);
				sprintf(*slash, "%s\\", *noSlash);
				retVal = true;
			}
		}
	}

	return(retVal);
}

bool checkDriveType(char *name, ULONG *pid)
{
	HANDLE                  hDevice;
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
	DEVICE_NUMBER deviceInfo;
	bool retVal = false;
	char *nameWithSlash;
	char *nameNoSlash;
	int driveType;

	// some calls require no tailing slash, some require a trailing slash...
	//   wheeeeeeeeeee.............
	if (!(slashify(name, &nameWithSlash, &nameNoSlash)))
	{
		return(retVal);
	}

	//driveType = GetDriveType(nameWithSlash);
	driveType = GetDriveTypeA(nameWithSlash);
	switch (driveType)
	{

	case DRIVE_REMOVABLE: // The media can be removed from the drive.
	case DRIVE_FIXED:     // The media cannot be removed from the drive.
		//hDevice = CreateFile(nameNoSlash, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		hDevice = CreateFileA(nameNoSlash, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		//QMessageBox::information(NULL, "Test", QString("path = %1").arg(nameNoSlash));
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			char *errormessage = NULL;
			//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPSTR)&errormessage, 0, NULL);

			std::wstring errmsg = getWerrmsg(errormessage);

			QMessageBox::critical(NULL, "Volume Error", QString::fromLocal8Bit("An error occurred when attempting to get a handle on %1.").arg(nameWithSlash) + QString("\nError %1: ").arg(GetLastError()) + QString::fromStdWString(errmsg));
			LocalFree(errormessage);
		}
		else
		{
			int arrSz = sizeof(STORAGE_DEVICE_DESCRIPTOR)+512 - 1;
			pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[arrSz];

			pDevDesc->Size = arrSz;

			// get the device number if the drive is
			// removable or (fixed AND on the usb bus)
			if (GetDisksProperty(hDevice, pDevDesc, &deviceInfo) && ((driveType == DRIVE_REMOVABLE) || ((driveType == DRIVE_FIXED) && (pDevDesc->BusType == BusTypeUsb))))
			{
				// ensure that the drive is actually accessible
				// multi-card hubs were reporting "removable" even when empty
				DWORD cbBytesReturned;
				if (DeviceIoControl(hDevice, IOCTL_STORAGE_CHECK_VERIFY2, NULL, 0, NULL, 0, &cbBytesReturned, (LPOVERLAPPED)NULL))
				{
					*pid = deviceInfo.DeviceNumber;
					//QMessageBox::information(NULL, "driveType", QString("%1 id = %2").arg(nameWithSlash).arg(*pid));
					retVal = true;
				}
			}

			delete pDevDesc;
			CloseHandle(hDevice);
		}

		break;
	default:
		retVal = false;
	}

	// free the strings allocated by slashify
	free(nameWithSlash);
	free(nameNoSlash);

	return(retVal);
}

//Lee ADD
void getProductInfo(int deviceID)
{
	HANDLE hDevice;
	// Format physical drive path (may be '\\.\PhysicalDrive0', '\\.\PhysicalDrive1' and so on).
	QString  devicename = QString("\\\\.\\PhysicalDrive%1").arg(deviceID);

	QString strSerialNumber;
	// Get a handle to physical drive
	hDevice = CreateFileA(devicename.toLatin1(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);

	// Set the input data structure
	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	// Get the necessary output buffer size
	STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
	DWORD dwBytesReturned = 0;
	if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		&storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
		&dwBytesReturned, NULL))
	{
		// handle error, do cleanup and return
	}

	// Alloc the output buffer
	const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
	BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
	ZeroMemory(pOutBuffer, dwOutBufferSize);
	// Get the storage device descriptor
	if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
		pOutBuffer, dwOutBufferSize,
		&dwBytesReturned, NULL))
	{
		// handle error, do cleanup and return
	}

	// Now, the output buffer points to a STORAGE_DEVICE_DESCRIPTOR structure
	// followed by additional info like vendor ID, product ID, serial number, and so on.
	STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
	const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
	if (dwSerialNumberOffset != 0)
	{
		// Finally, get the serial number
		strSerialNumber = QString((char*)pOutBuffer + dwSerialNumberOffset);;
		DiskManager::GetInstance()->mProductId = QString((char*)pOutBuffer + pDeviceDescriptor->ProductIdOffset);
		DiskManager::GetInstance()->mVendorId = QString((char*)pOutBuffer + pDeviceDescriptor->VendorIdOffset);
		DiskManager::GetInstance()->mProductRevision = QString((char*)pOutBuffer + pDeviceDescriptor->ProductRevisionOffset);
		//DiskManager::GetInstance()->mSerialNumber = QString((char*)pOutBuffer + pDeviceDescriptor->SerialNumberOffset);
	}


}

DWORD getDeviceNumber(HANDLE deviceHandle)
{
	STORAGE_DEVICE_NUMBER sdn;
	sdn.DeviceNumber = -1;
	DWORD dwBytesReturned = 0;
	if (!DeviceIoControl(deviceHandle,
		IOCTL_STORAGE_GET_DEVICE_NUMBER,
		NULL, 0, &sdn, sizeof(sdn),
		&dwBytesReturned, NULL))
	{
		// handle error - like a bad handle.
		return 11111111;
	}
	return sdn.DeviceNumber;

}

void getSerialNumber(int volumeID)
{
	//UsbDeviceInfo info;

	// get the device handle
	char devicePath[7] = "\\\\.\\@:";
	devicePath[4] = (char)(volumeID + 'A');
	//printf("device path = %s\n", devicePath);
	HANDLE deviceHandle = CreateFileA(devicePath, 0,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, 0, NULL);
	if (deviceHandle == INVALID_HANDLE_VALUE)
		return;

	// to get the device number
	DWORD volumeDeviceNumber = getDeviceNumber(deviceHandle);
	CloseHandle(deviceHandle);

	// Get device interface info s	et handle
	// for all devices attached to the system
	HDEVINFO hDevInfo = SetupDiGetClassDevs(
		&GUID_DEVINTERFACE_DISK, NULL, NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)
		return;

	// Get a context structure for the device interface
	// of a device information set.
	BYTE Buf[1024];
	PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd =
		(PSP_DEVICE_INTERFACE_DETAIL_DATA)Buf;
	SP_DEVICE_INTERFACE_DATA         spdid;
	SP_DEVINFO_DATA                  spdd;

	spdid.cbSize = sizeof(spdid);

	DWORD dwIndex = 0;
	while (true)
	{
		if (!SetupDiEnumDeviceInterfaces(hDevInfo, NULL,
			&GUID_DEVINTERFACE_DISK,
			dwIndex, &spdid))
			break;

		DWORD dwSize = 0;
		//printf("dwindex = %d\n", dwIndex);
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL,
			0, &dwSize, NULL);
		//printf("dwsize = %d\n", dwSize);
		if ((dwSize != 0) && (dwSize <= sizeof(Buf)))
		{
			pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!

			ZeroMemory((PVOID)&spdd, sizeof(spdd));
			spdd.cbSize = sizeof(spdd);

			long res = SetupDiGetDeviceInterfaceDetail(
				hDevInfo, &spdid, pspdidd,
				dwSize, &dwSize, &spdd);
			//printf("res = %d\n", res);
			if (res)
			{
				HANDLE hDrive = CreateFile(pspdidd->DevicePath, 0,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL, OPEN_EXISTING, 0, NULL);
				if (hDrive != INVALID_HANDLE_VALUE)
				{
					DWORD usbDeviceNumber = getDeviceNumber(hDrive);

					if (usbDeviceNumber == volumeDeviceNumber)
					{
						char* usb_data = NULL;
						int data_size = WideCharToMultiByte(CP_ACP, 0, pspdidd->DevicePath, -1, NULL, 0, NULL, NULL);

						usb_data = (char*)malloc(data_size * sizeof(char));
						WideCharToMultiByte(CP_ACP, 0, pspdidd->DevicePath, -1, usb_data, data_size, 0, 0);

						char* serial_number = (char*)malloc(sizeof(char)* data_size);
						int i;
						int check_count = 0;
						int start_index = 0;
						int end_index = 0;
						for (i = 0; i<data_size; i++)
						{
							if (usb_data[i] == '#')
							{
								check_count++;
								if (check_count == 2)
									start_index = i + 1;
								else if (check_count == 3)
								{
									end_index = i - 1;
									break;
								}
								else
									continue;
							}
						}

						for (i = 0; i <= end_index - start_index; i++)
						{
							serial_number[i] = usb_data[start_index + i];
							if (serial_number[i] >= 97 && serial_number[i] <= 128)
								serial_number[i] -= 32;
						}

						serial_number[i] = '\0';
						//printf("%s\n", serial_number);
						DiskManager::GetInstance()->mSerialNumber = QString(serial_number);
						free(usb_data);
						free(serial_number);
					}

				}
				CloseHandle(hDrive);
			}
		}
		dwIndex++;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	return;
}

//~ convert error message in char string to in wchar string, Added by LEE Jeun jeun@wayne-inc.com
std::wstring getWerrmsg(char* errmsg)
{
	char errStr[128];
	strcpy(errStr, errmsg);

	TCHAR WerrStr[128];
	MultiByteToWideChar(CP_ACP, 0, errStr, sizeof(errStr), WerrStr, sizeof(WerrStr));

	std::wstring Werrmsg = WerrStr;
	return Werrmsg;
}
// Added by LEE Jeun jeun@wayne-inc.com ~