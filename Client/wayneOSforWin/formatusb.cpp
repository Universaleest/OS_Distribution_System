// Added by LEE Jeun jeun@wayne-inc.com, referring to rufus
#include "Disk/disk.h"
//#include "View/widgetinstall.h"
#include "formatusb.h"
#include <QDebug>
#include <locale>

std::string FsType[] = { 
	"Unknown",
	"RAW",
	"FAT",
	"FAT32",
	"NTFS",
	"CDFS",
	"UDF",
	"EXFAT",
	"CSVFS",
	"REFS"
};
std::map<DWORD, std::string> deviceType = { 
	{FILE_DEVICE_CD_ROM,"CD ROM"},
	{FILE_DEVICE_DISK,"Disk"},
	{FILE_DEVICE_DVD,"DVD"}
};
std::map<DWORD, std::string> mediaType = { 
	{Unknown,"Unknown"},
	{RemovableMedia,"Removable Media"},
	{FixedMedia,"Fixed Media"}
};
std::map<DWORD, std::string> partitionStyle = { 
	{VDS_PST_MBR,"MBR"},
	{VDS_PST_GPT,"GPT"},
	{VDS_PST_UNKNOWN,"Unknown"}
};

QString getDiskName(int device)
{
	return ("\\\\?\\PhysicalDrive" + QString::number(device));
}

// Delete all partitions in a selected USB by using VDS
BOOL DeletePartitions(IVdsService* pService, int device, std::string& drive_name)
{
	BOOL ret = FALSE;
	HRESULT hr;
	IUnknown* pUnkForProvider;
	IUnknown* pUnkForPack;
	IUnknown* pUnkForDisk;
	IEnumVdsObject* pEnumForProvider;
	IEnumVdsObject* pEnumForPack;
	IEnumVdsObject* pEnumForDisk;
	IVdsProvider* pProvider = NULL;
	IVdsSwProvider* pSwProvider = NULL;
	IVdsPack* pPack = NULL;
	IVdsDisk* pDisk = NULL;
	IVdsAdvancedDisk* pAdvDisk = NULL;
	ULONG ulFetched = 0;
	
	QString selected = getDiskName(device);

	hr = pService->QueryProviders(VDS_QUERY_SOFTWARE_PROVIDERS, &pEnumForProvider);

	if (hr != S_OK)
	{
		qDebug("Could not query VDS Providers!");
		goto out;
	}

	while (pEnumForProvider->Next(1, &pUnkForProvider, &ulFetched) == S_OK)
	{
		hr = pUnkForProvider->QueryInterface(IID_IVdsProvider, (void**)&pProvider);
		_SafeRelease(pUnkForProvider);

		if (hr != S_OK)
		{
			qDebug("Could not get VDS Provider!");
			goto out;
		}

		hr = pProvider->QueryInterface(IID_IVdsSwProvider, (void**)&pSwProvider);
		_SafeRelease(pProvider);

		if (hr != S_OK)
		{
			qDebug("Could not get VDS Software Provider!");
			goto out;
		}

		hr = pSwProvider->QueryPacks(&pEnumForPack);
		_SafeRelease(pSwProvider);

		if (hr != S_OK)
		{
			qDebug("Could not query VDS Packs!");
			goto out;
		}

		while (pEnumForPack->Next(1, &pUnkForPack, &ulFetched) == S_OK)
		{
			hr = pUnkForPack->QueryInterface(IID_IVdsPack, (void**)&pPack);
			_SafeRelease(pUnkForPack);

			if (hr != S_OK)
			{
				qDebug("Could not get VDS Pack!");
				goto out;
			}

			hr = pPack->QueryDisks(&pEnumForDisk);
			_SafeRelease(pPack);

			if (hr != S_OK)
			{
				qDebug("Could not query VDS Disks!");
				goto out;
			}

			while (pEnumForDisk->Next(1, &pUnkForDisk, &ulFetched) == S_OK)
			{

				wchar_t wsGuid[129];
				memset(wsGuid, 0, sizeof(wsGuid));

				VDS_DISK_PROP diskProp;
				VDS_PARTITION_PROP* pPartProp;
				LONG numOfPartitions = 0;

				hr = pUnkForDisk->QueryInterface(IID_IVdsDisk, (void**)&pDisk);
				_SafeRelease(pUnkForDisk);
				
				if (hr != S_OK)
				{
					qDebug("Could not get VDS Disk!");
					goto out;
				}

				hr = pDisk->GetProperties(&diskProp);

				if (hr != S_OK)
				{
					_SafeRelease(pDisk);
					qDebug("Could not query VDS Disk Properties!");
					goto out;
				}
				
				if (_wcsicmp(diskProp.pwszName, selected.toStdWString().c_str()) != 0)
				{
					_SafeRelease(pDisk);
					CoTaskMemFree(diskProp.pwszAdaptorName);
					CoTaskMemFree(diskProp.pwszDevicePath);
					CoTaskMemFree(diskProp.pwszDiskAddress);
					CoTaskMemFree(diskProp.pwszFriendlyName);
					CoTaskMemFree(diskProp.pwszName);
					continue;
				}

				hr = pDisk->QueryInterface(IID_IVdsAdvancedDisk, (void**)&pAdvDisk);
				_SafeRelease(pDisk);

				if (hr != S_OK)
				{
					qDebug("Could not get VDS AdvancedDisk!");
					goto out;
				}

				qDebug("Disk Name: %ls", diskProp.pwszName);
				qDebug("Disk Friendly Name: %ls", diskProp.pwszFriendlyName);
				if (diskProp.PartitionStyle == VDS_PST_GPT)
				{
					::StringFromGUID2(diskProp.DiskGuid, wsGuid, 128);
					qDebug("Disk GUID: %ls", wsGuid);
					memset(wsGuid, 0, sizeof(wsGuid));
				}
				qDebug("Disk Size: %llu bytes", diskProp.ullSize);
				qDebug("Disk Type: %s", deviceType[diskProp.dwDeviceType].c_str());
				qDebug("Media Type: %s", mediaType[diskProp.dwMediaType].c_str());
				qDebug("Partition Style: %s", partitionStyle[diskProp.PartitionStyle].c_str());
				qDebug("Disk Bytes per Sector: %lu", diskProp.ulBytesPerSector);
				qDebug("Disk Sectors per Track: %lu", diskProp.ulSectorsPerTrack);
				qDebug("Disk Tracks per Cylinder: %lu", diskProp.ulTracksPerCylinder);

				hr = pAdvDisk->QueryPartitions(&pPartProp, &numOfPartitions);

				if (hr == S_OK)
				{
					qDebug("Deleting all partitions...");

					for (int i = 0; i < numOfPartitions; ++i)
					{
						qDebug("Partition %lu (offset: %llu, size: %llu bytes)",
							pPartProp[i].ulPartitionNumber, pPartProp[i].ullOffset, pPartProp[i].ullSize);

						qDebug("Partition Style: %s", partitionStyle[pPartProp[i].PartitionStyle].c_str());

						if (pPartProp[i].PartitionStyle == VDS_PST_MBR)
						{
							qDebug("MBR Partition Type: %#.8x", pPartProp[i].Mbr.partitionType);
						}

						else if (pPartProp[i].PartitionStyle == VDS_PST_GPT)
						{
							::StringFromGUID2(pPartProp[i].Gpt.partitionId, wsGuid, 128);
							qDebug("Partition %lu ID: %ls", pPartProp[i].ulPartitionNumber, wsGuid);
							qDebug("GPT Name: %ls", pPartProp[i].Gpt.name);
							qDebug("Attributes: %#.8x", pPartProp[i].Gpt.attributes);
						}

						hr = pAdvDisk->DeletePartition(pPartProp[i].ullOffset, TRUE, TRUE);

						if (hr != S_OK)
						{
							qDebug("Could not delete Patition %lu", pPartProp[i].ulPartitionNumber);
							qDebug("error code: %#.8x", hr);
							ret = FALSE;						
						}
					}

					ret = TRUE;
				}

				else
				{
					qDebug("No partition to delete on disk: %ls", diskProp.pwszName);
					ret = TRUE;
				}

				CoTaskMemFree(diskProp.pwszAdaptorName);
				CoTaskMemFree(diskProp.pwszDevicePath);
				CoTaskMemFree(diskProp.pwszDiskAddress);
				CoTaskMemFree(diskProp.pwszFriendlyName);
				CoTaskMemFree(diskProp.pwszName);
				CoTaskMemFree(pPartProp);
				_SafeRelease(pAdvDisk);

				wchar_t first = 'A';
				DWORD dwCount = 'Z' - 'A' + 1;
				VDS_DRIVE_LETTER_PROP driveLetterProp['Z' - 'A' + 1];
				ZeroMemory(driveLetterProp, sizeof(driveLetterProp));
				hr = pService->QueryDriveLetters(first, dwCount, driveLetterProp);

				if (hr != S_OK)
				{
					qDebug("Could not check which drive letters are in use!");
				}

				wchar_t pwszPath[4];
				memset(pwszPath, 0, sizeof(pwszPath));

				for (int i = 0; i < dwCount; ++i)
				{
					if (driveLetterProp[i].wcLetter < 'C' || driveLetterProp[i].bUsed)
					{
						continue;
					}

					else
					{
						pwszPath[0] = driveLetterProp[i].wcLetter;
						break;
					}
				}

				wchar_t postfix[] = L":\\";
				wcscat(pwszPath, postfix);

				char unusedDrive[4];
				memset(unusedDrive, 0, sizeof(unusedDrive));
				WideCharToMultiByte(CP_ACP, 0, pwszPath, wcslen(pwszPath), unusedDrive, sizeof(unusedDrive), NULL, NULL);
				drive_name = std::string(unusedDrive);
				goto out;
			}
		}
	}

out:
	if(pEnumForProvider != NULL) _SafeRelease(pEnumForProvider);
	if(pEnumForPack != NULL) _SafeRelease(pEnumForPack);
	if(pEnumForDisk != NULL) _SafeRelease(pEnumForDisk);

	return ret;
}

// Load VDS
int getVds(int device, std::string& drive_name)
{
	setlocale(LC_ALL, "");
	HRESULT hr;
	IVdsService* pService = NULL;
	IVdsServiceLoader* pLoader = NULL;
	BOOL bResult = FALSE;
	char driveName[] = "?:\\";
	std::vector<char> driveLetters;

	hr = CoCreateInstance(
		CLSID_VdsLoader,
		NULL,
		CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER,
		IID_IVdsServiceLoader,
		(void**)&pLoader
	);

	if (SUCCEEDED(hr))
	{
		hr = pLoader->LoadService(NULL, &pService);
		_SafeRelease(pLoader);

		if (SUCCEEDED(hr))
		{
			hr = pService->WaitForServiceReady();

			if (SUCCEEDED(hr))
			{
				driveLetters = getDriveLetters(device);

				if (driveLetters.size() == 0)
				{
					qDebug("Failed to get drive letters!");
					return -1;
				}

				for (int i = 0; i < driveLetters.size(); ++i)
				{
					driveName[0] = driveLetters[i];

					if (!DeleteVolumeMountPointA(driveName))
					{
						qDebug("Failed to delete mountpoint %s:", driveName);
					}
				}

				bResult = DeletePartitions(pService, device, drive_name);
				_SafeRelease(pService);

				return 0;
			}
		}
	}
	
	qDebug("Could not load VDS service!");
	return -1;
}

/* copied from rufus
 * Mount the volume identified by drive_guid to mountpoint drive_name.
 * If volume_name is already mounted, but with a different letter than the
 * one requested then drive_name is updated to use that letter.
 */
BOOL MountVolume(char* drive_name, char* volume_name)
{
	const char* global_root = "\\\\?\\GLOBALROOT";
	const int global_root_len = strlen(global_root);
	char mounted_guid[52];
	char mounted_letter[27] = { 0 };
	DWORD size;

	if ((drive_name == NULL) || (volume_name == NULL) || (drive_name[0] == ':') ||
		strncmp(volume_name, global_root, global_root_len) == 0)
	{
		return FALSE;
	}

	if ((GetVolumePathNamesForVolumeNameA(volume_name, mounted_letter, sizeof(mounted_letter), &size)) 
		&& (size > 1) && (mounted_letter[0] != drive_name[0]))
	{
		qDebug("%s is already mounted as %c: instead of %c: - Will now use this target instead...",
			volume_name, mounted_letter[0], drive_name[0]);
		drive_name[0] = mounted_letter[0];
		return TRUE;
	}

	if (!SetVolumeMountPointA(drive_name, volume_name))
	{
		if (GetLastError() == ERROR_DIR_NOT_EMPTY)
		{
			if (!GetVolumeNameForVolumeMountPointA(drive_name, mounted_guid, sizeof(mounted_guid)))
			{
				qDebug("%s is already mounted, but volume GUID could not be checked",
					drive_name);
			}

			else if (strcmp(volume_name, mounted_guid) != 0)
			{
				qDebug("%s is mounted, but volume GUID doesn't match: expected %s, got %s",
					drive_name, volume_name, mounted_guid);
			}

			else
			{
				qDebug("%s is already mounted as %c", volume_name, drive_name[0]);
				return TRUE;
			}

			qDebug("Retrying after dismount...");
			
			if (!DeleteVolumeMountPointA(drive_name))
			{
				qDebug("Warning: Could not delete volume mountpoint");
			}

			if (SetVolumeMountPointA(drive_name, volume_name))
			{
				return TRUE;
			}

			if ((GetLastError() == ERROR_DIR_NOT_EMPTY) &&
				GetVolumeNameForVolumeMountPointA(drive_name, mounted_guid, sizeof(mounted_guid)) &&
				(strcmp(volume_name, mounted_guid) == 0))
			{
				qDebug("%s was remounted as %c", volume_name, drive_name[0]);
				return TRUE;
			}
		}
		return FALSE;
	}
	return TRUE;
}

// return the array of drive letters of a selected USB
std::vector<char> getDriveLetters(int deviceId)
{
	std::vector<char> ret;
	DWORD size = 0;
	ULONG pid = 0;
	BOOL bResult = FALSE;
	char logical_volume[8];
	char prefix[] = "\\\\.\\";
	char drives[26 * 4 + 1];
	char drive_label[4];
	int idx = 0;

	memset(logical_volume, 0, sizeof(logical_volume));
	memset(drives, 0, sizeof(drives));
	memset(drive_label, 0, sizeof(drive_label));

	size = GetLogicalDriveStringsA(sizeof(drives), drives);

	for (int i = 0; i < sizeof(drives); ++i)
	{
		drive_label[idx] = drives[i];

		if (idx == 0 && drive_label[idx] == 0)
		{
			break;
		}

		++idx;

		if (idx > 3)
		{
			std::string current_Logical_volume = std::string(prefix) + std::string(drive_label);
			strncpy(logical_volume, current_Logical_volume.c_str(), current_Logical_volume.size());
			bResult = checkDriveType(logical_volume, &pid);

			if ((bResult) && (pid == deviceId))
			{
				ret.push_back(drive_label[0]);
			}

			memset(logical_volume, 0, sizeof(logical_volume));
			memset(drive_label, 0, sizeof(drive_label));
		}

		idx = idx > 3 ? 0 : idx;
	}

	return ret;
}

/* Modified by LEE Jeun jeun@wayne-inc.com, referring to rufus
 * Return the GUID volume name for the disk and partition specified, or NULL if not found.
 * See http://msdn.microsoft.com/en-us/library/cc542456.aspx
 * If PartitionOffset is 0, the offset is ignored and the first partition found is returned.
 */
std::string getLogicalName(int device, uint64_t partitionOffset, BOOL bKeep)
{
	static const char* ignore_device[] = { "\\Device\\CdRom","\\Device\\Floppy" };
	static const char* volume_start = "\\\\?\\";
	char volume_name[MAX_PATH], path[MAX_PATH];
	std::string ret;
	BOOL bPrintHeader = TRUE;
	HANDLE hDrive = INVALID_HANDLE_VALUE, hVolume = INVALID_HANDLE_VALUE;
	VOLUME_DISK_EXTENTS_REDEF DiskExtents;
	DWORD size;
	std::vector<std::string> found_name;
	uint64_t found_offset[MAX_PARTITIONS] = { 0, };
	uint32_t i, j;
	size_t len;
	UINT driveType;

	for (int i = 0; hDrive == INVALID_HANDLE_VALUE; ++i)
	{
		if (i == 0)
		{
			hVolume = FindFirstVolumeA(volume_name, sizeof(volume_name));
			
			if (hVolume == INVALID_HANDLE_VALUE)
			{
				qDebug("Could not access fisrt GUID volume!");
				goto out;
			}
		}

		else
		{
			if (!FindNextVolumeA(hVolume, volume_name, sizeof(volume_name)))
			{
				if (GetLastError() != ERROR_NO_MORE_FILES)
				{
					qDebug("Could not access next GUID volume!");
				}
				break;
			}
		}

		len = strlen(volume_name);
		assert(len > 4);
		assert(_strnicmp((volume_name == NULL ? "<NULL>" : volume_name), (volume_start == NULL ? "<NULL>" : volume_start), 4) == 0);
		assert(volume_name[len - 1] == '\\');

		driveType = GetDriveTypeA(volume_name);
		if ((driveType != DRIVE_REMOVABLE) && (driveType != DRIVE_FIXED))
		{
			continue;
		}

		volume_name[len - 1] = 0;

		if (QueryDosDeviceA(&volume_name[4], path, sizeof(path)) == 0)
		{
			qDebug("Failed to get device path for GUID volume '%s'", volume_name);
			continue;
		}

		for (j = 0; (j < ARRAYSIZE(ignore_device)) &&
			(_strnicmp(path, ignore_device[j], strlen(ignore_device[j])) != 0); ++j);
	
		if (j < ARRAYSIZE(ignore_device))
		{
			qDebug("Skipping GUID volume for '%s'", path);
			continue;
		}

		hDrive = CreateFileA(volume_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hDrive == INVALID_HANDLE_VALUE)
		{
			qDebug("Could not open GUID volume '%s'", volume_name);
			continue;
		}

		if ((!DeviceIoControl(hDrive, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
			&DiskExtents, sizeof(DiskExtents), &size, NULL)) || (size <= 0))
		{
			qDebug("Could not get Disk Extents!");
			CloseHandle(hDrive);
			hDrive = INVALID_HANDLE_VALUE;
			continue;
		}

		CloseHandle(hDrive);
		hDrive = INVALID_HANDLE_VALUE;

		if (DiskExtents.NumberOfDiskExtents == 0)
		{
			qDebug("Ignoring volume '%s' because it has no extents...", volume_name);
			continue;
		}

		if (DiskExtents.NumberOfDiskExtents != 1)
		{
			qDebug("Ignoring volume '%s' because it has more than one extent...", volume_name);
			continue;
		}

		if (DiskExtents.Extents[0].DiskNumber != device)
		{
			continue;
		}

		if (found_name.size() == MAX_PARTITIONS)
		{
			qDebug("Error: Trying to process a disk with more than %d partitions!", MAX_PARTITIONS);
			goto out;
		}

		if (bKeep)
		{
			volume_name[len - 1] = '\\';
		}
		found_offset[found_name.size()] = DiskExtents.Extents[0].StartingOffset.QuadPart;
		found_name.push_back(volume_name);
		qDebug("- %s @%lld", volume_name, DiskExtents.Extents[0].StartingOffset.QuadPart);
	}

	if (found_name.size() == 0)
	{
		goto out;
	}

	for (i = 0; (i < found_name.size()) && (partitionOffset != 0) && (partitionOffset != found_offset[i]); ++i);

	if (i < found_name.size())
	{
		ret = found_name[i];
	}

	else
	{
		ret = AltGetLogicalName(device, partitionOffset, bKeep);
		
		if (ret.size() != 0 && ret.find(' ') != std::string::npos)
		{
			qDebug("Warning: Using physical device to access partition data");
		}
	}

out:
	if (hVolume != INVALID_HANDLE_VALUE)
	{
		FindVolumeClose(hVolume);
	}
	return ret;
}

/* Modified by LEE Jeun jeun@wayne-inc.com, referring to rufus
 * Alternative version of the above, needed because some volumes, such as ESPs, are not listed
 * by Windows, be it with VDS or other APIs.
 * For these, we return the "\\?\GLOBALROOT\Device\HarddiskVolume#" identifier that matches
 * our "Harddisk#Partition#", as reported by QueryDosDevice().
*/
std::string AltGetLogicalName(int device, uint64_t partitionOffset, BOOL bKeep)
{
	const char* global_root = "\\\\?\\GLOBALROOT";
	const int global_root_len = strlen(global_root);
	BOOL matching_drive = (InfoManager::GetInstance()->mDeviceId == device), r = FALSE;
	DWORD i, size;
	std::string ret;
	char volumeName[MAX_PATH], path[64];
	BYTE geometry[256] = { 0 }, layout[4096] = { 0 };
	PDISK_GEOMETRY_EX diskGeometry = (PDISK_GEOMETRY_EX)(void*)geometry;
	PDRIVE_LAYOUT_INFORMATION_EX driveLayout = (PDRIVE_LAYOUT_INFORMATION_EX)(void*)layout;
	HANDLE hDisk = INVALID_HANDLE_VALUE;
	QString diskName = getDiskName(device);
	hDisk = CreateFileA(diskName.toStdString().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);
	r = DeviceIoControl(hDisk, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL,
		0, geometry, sizeof(geometry), &size, NULL);

	if (!r)
	{
		qDebug("Could not get disk geometry of a selected USB!");
		CloseHandle(hDisk);
		hDisk = INVALID_HANDLE_VALUE;
		return ret;
	}

	r = DeviceIoControl(hDisk, IOCTL_DISK_GET_DRIVE_LAYOUT_EX, NULL,
		0, layout, sizeof(layout), &size, NULL);

	if (!r)
	{
		qDebug("Could not get drive layout of a selected USB!");
		CloseHandle(hDisk);
		hDisk = INVALID_HANDLE_VALUE;
		return ret;
	}

	CloseHandle(hDisk);
	hDisk = INVALID_HANDLE_VALUE;

	if (partitionOffset == 0)
	{
		i = 0;
	}

	else if (matching_drive)
	{
		for (i = 0; (i < MAX_PARTITIONS) &&
			(partitionOffset != driveLayout->PartitionEntry[i].StartingOffset.QuadPart); ++i);

		if (i >= MAX_PARTITIONS)
		{
			qDebug("Error: Could not find a partition at offset %lld on this USB!", partitionOffset);
			return ret;
		}
	}

	else
	{
		qDebug("Error: Searching for a partition on a non matching disk!");
		return ret;
	}

	memset(volumeName, 0, sizeof(volumeName));
	memset(path, 0, sizeof(path));

	strncpy(volumeName, global_root, global_root_len);
	QString qstrPath = "Harddisk" + QString::number(device) +
		"Partition" + QString::number(i + 1);

	if (qstrPath.size() >= sizeof(path))
	{
		qDebug("Harddisk#Partition# string is too long!");
		return ret;
	}

	strncpy(path, qstrPath.toStdString().c_str(), qstrPath.size());

	if (!QueryDosDeviceA(path, &volumeName[global_root_len],
		(DWORD)(MAX_PATH - global_root_len)) || strlen(volumeName) < 20)
	{
		qDebug("Could not find a DOS volume name for '%s'!", path);

		if (!matching_drive)
		{
			return ret;
		}

		QString physicalName = "\\\\.\\PhysicalDrive" + QString::number(device) + (bKeep ? "\\" : "") + " "
			+ QString::number(driveLayout->PartitionEntry[i].StartingOffset.QuadPart) + " " + QString::number(driveLayout->PartitionEntry[i].PartitionLength.QuadPart);
		strncat(volumeName, physicalName.toStdString().c_str(), physicalName.size());
		qDebug("volume name from AltGetLogicalName(): %s", volumeName);
	}

	else if (bKeep)
	{
		strncat(volumeName, "\\", strlen("\\"));
	}

	ret = std::string(volumeName);
	return ret;
}

// Modified by LEE Jeun jeun@wayne-inc.com, referring to rufus
/* Wait for a logical drive to reappear - Used when a drive has just been repartitioned */
BOOL WaitForLogical(int deviceId, uint64_t partitionOffset)
{
	const char* global_root = "\\\\?\\GLOBALROOT";
	const int global_root_len = strlen(global_root);
	const int DRIVE_ACCESS_TIMEOUT = 15000;
	const int DRIVE_ACCESS_RETRIES = 150;
	uint64_t endTime;
	std::string logicalPath;

	endTime = GetTickCount64() + DRIVE_ACCESS_TIMEOUT;

	do
	{
		QCoreApplication::processEvents();

		logicalPath = getLogicalName(deviceId, partitionOffset, FALSE);

		if (!logicalPath.empty() && strncmp(logicalPath.c_str(), global_root, global_root_len) != 0)
		{
			return TRUE;
		}

		if (ViewManager::GetInstance()->flag == ViewManager::GetInstance()->EXIT_EVENT)
		{
			return FALSE;
		}

		Sleep(DRIVE_ACCESS_TIMEOUT / DRIVE_ACCESS_RETRIES);
	} while (GetTickCount64() < endTime);

	qDebug("Timeout while waiting for logical drive");
	return FALSE;
}

// Modified by LEE Jeun@wayne-inc.com, referring to rufus
// Use VDS to refresh the drive layout
BOOL refreshLayout()
{
	BOOL r = FALSE;
	HRESULT hr;
	IVdsServiceLoader* pLoader;
	IVdsService* pService;
	IEnumVdsObject* pEnum;

	hr = CoCreateInstance(CLSID_VdsLoader, NULL, CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER,
		IID_IVdsServiceLoader, (void**)&pLoader);
	if (hr != S_OK)
	{
		qDebug("Could not create VDS Loader Instance: %#.8x", hr);
		goto out;
	}

	hr = pLoader->LoadService(NULL, &pService);
	_SafeRelease(pLoader);

	if (hr != S_OK)
	{
		qDebug("Could not lad VDS Service: %#.8x", hr);
		goto out;
	}

	hr = pService->WaitForServiceReady();
	if (hr != S_OK)
	{
		qDebug("VDS Service is not ready: %#.8x", hr);
		goto out;
	}

	hr = pService->QueryProviders(VDS_QUERY_SOFTWARE_PROVIDERS, &pEnum);
	if (hr != S_OK)
	{
		qDebug("Could not query VDS Service Provider: %#.8x", hr);
		goto out;
	}

	hr = pService->CleanupObsoleteMountPoints();
	if (hr != S_OK)
	{
		qDebug("Could not clean up VDS mountpoints: %#.8x", hr);
		goto out;
	}

	hr = pService->Refresh();
	if (hr != S_OK)
	{
		qDebug("Could not refresh VDS layout: %#.8x", hr);
		goto out;
	}

	hr = pService->Reenumerate();
	if (hr != S_OK)
	{
		qDebug("Could not refresh VDS layout: %#.8x", hr);
		goto out;
	}

	_SafeRelease(pService);
	r = TRUE;

out:
	return r;
}

BOOL scanForHwChanges()
{
	DEVINST devInst;
	CONFIGRET status;

	status = CM_Locate_DevNode(&devInst, NULL, CM_LOCATE_DEVNODE_NORMAL);
	if (status != CR_SUCCESS)
	{
		qDebug("CM_Locate_DevNode failed: %#.8x\n", status);
		return FALSE;
	}

	status = CM_Reenumerate_DevNode(devInst, 0);
	if (status != CR_SUCCESS)
	{
		qDebug("CM_Reenumerate_DevNode failed: %#.8x\n", status);
		return FALSE;
	}

	return TRUE;
}