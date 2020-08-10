// Added by LEE Jeun jeun@wayne-inc.com, referring to rufus
#ifndef _FORMAT_USB
#define _FORMAT_USB

#include <Windows.h>
#include <initguid.h>
#include <vds.h>
#include <cstdio>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include "Info/infomanager.h"
#include "View/viewmanager.h"
#include <cfgmgr32.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "cfgmgr32.lib")

#define _SafeRelease(x){if(x!=NULL){x->Release();x=NULL;}}
#define MAX_PARTITIONS 16

extern std::string diskStatus[];
extern std::string volStatus[];
extern std::string volHealth[];
extern std::string FsType[];
extern std::map<DWORD, std::string> deviceType;
extern std::map<DWORD, std::string> mediaType;
extern std::map<DWORD, std::string> partitionStyle;

typedef struct {
	DWORD NumberOfDiskExtents;
	DISK_EXTENT Extents[8];
} VOLUME_DISK_EXTENTS_REDEF;

QString getDiskName(int device);
BOOL DeletePartitions(IVdsService* pService, int device, std::string& drive_name);
int getVds(int devcie, std::string& drive_name);
BOOL MountVolume(char* drive_name, char* volume_name);
std::vector<char> getDriveLetters(int deviceId);
std::string getLogicalName(int device, uint64_t partitionOffset, BOOL bKeep);
std::string AltGetLogicalName(int device, uint64_t partitionOffset, BOOL bKeep);
BOOL WaitForLogical(int deviceId, uint64_t partitionOffset);
BOOL refreshLayout();
BOOL scanForHwChanges();

#endif // !_FORMAT_USB

