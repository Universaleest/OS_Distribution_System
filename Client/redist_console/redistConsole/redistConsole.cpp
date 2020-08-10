#include "redistConsole.h"

const std::string showVersion(int version)
{
	return version == 13 ? std::string("Microsoft Visual C++ 2013 Redistributable(x86)") : std::string("Microsoft Visual C++ 2015 Redistributable(x86)");
}

int checkVersion(CString ver)
{
	return (ver.Find(_T("12.0"), 0) != -1) ? 13 : 15;
}

void showStatus(CString ver, LONG result)
{
	if (result != ERROR_SUCCESS)
	{
		std::cout << showVersion(checkVersion(ver)) << INSTALL << std::endl;
	}
	else std::cout << ALREADY_INSTALLED << showVersion(checkVersion(ver)) << std::endl;
}

SHELLEXECUTEINFO runExApp(CString ver, BOOL& STATE)
{
	//STARTUPINFO si = { 0, };
	//PROCESS_INFORMATION pi = { 0, };
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;
	//ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	//ZeroMemory(&si, sizeof(STARTUPINFO));
	//si.cb = sizeof(STARTUPINFO);
	//TCHAR command[DIR_LEN];
	STATE = FALSE;
	CString command;
	//GetModuleFileName(NULL, command, sizeof(command));
	GetModuleFileName(NULL, command.GetBuffer(DIR_LEN), DIR_LEN);
	command.ReleaseBuffer();
	std::wcout << command.GetString() << "\n";
	//PathRemoveFileSpec(command);
	PathRemoveFileSpec(command.GetBuffer(command.GetLength()));
	command.ReleaseBuffer();
	std::wcout << command.GetString() << "\n";
	//TCHAR* exe = new TCHAR[ver.GetLength()+1];
	CString exe = ver.GetString();
	//memset(exe, 0, ver.GetLength()+1);
	//StrCpy(exe, ver.GetString());
	//StrCatBuff(command, exe, sizeof(command));
	PathAppend(command.GetBuffer(DIR_LEN), exe);
	command.ReleaseBuffer();
	std::wcout << command.GetString() << "\n";
	
	SHELLEXECUTEINFO si;
	ZeroMemory(&si, sizeof(SHELLEXECUTEINFO));
	si.cbSize = sizeof(SHELLEXECUTEINFO);
	si.hwnd = NULL;
	si.fMask = SEE_MASK_NOCLOSEPROCESS;
	si.lpVerb = _T("runas");
	si.lpFile = command;
	si.nShow = SW_SHOWNORMAL;
	si.lpDirectory = NULL;
	STATE = ShellExecuteEx(&si);

	/*std::wcout << command << "\n";
	HINSTANCE hInstance = (HINSTANCE)ShellExecute(NULL, _T("open"), command, NULL, NULL, SW_SHOW);
	if (hInstance <= (HINSTANCE)HINSTANCE_ERROR)
	{
		std::cout << "ShellExcute error!\r\n";
	}*/
	
	if (!STATE)
	{
		std::cout << "cannot create process" << "\n";
		std::cout << GetLastError() << "\n";
		Sleep(10000);
		exit(-1);
	}
	//BOOL state = FALSE;
	//state = CreateProcess(NULL, command.GetBuffer(command.GetLength()), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	//command.ReleaseBuffer();
	/*if (!state)
	{
		std::cout << "cannot create process" << "\n";
		std::cout << GetLastError() << "\n";
		Sleep(10000);
		exit(-1);
	}*/
	//STATE = state;
	//delete [] exe;
	//return pi;
	return si;
}

/*void checkAppRunning(PROCESS_INFORMATION& pi)
{
	unsigned long l;
	if (pi.hProcess)
	{
		GetExitCodeProcess(pi.hProcess, &l);
		while (l == STILL_ACTIVE)
		{
			GetExitCodeProcess(pi.hProcess, &l);
		}
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}*/

void checkAppRunning(SHELLEXECUTEINFO& si)
{
	DWORD dwExitCode = 0;
	/*if (si.hProcess)
	{
		GetExitCodeProcess(si.hProcess, &dwExitCode);
		
		while (dwExitCode == STILL_ACTIVE)
		{
			GetExitCodeProcess(si.hProcess, &dwExitCode);
		}
	}*/
	WaitForSingleObject(si.hProcess, INFINITE);
	GetExitCodeProcess(si.hProcess, &dwExitCode);
	CloseHandle(si.hProcess);
}


void isInstalled(HKEY& hKey, CString ver)
{
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ver, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		std::cout << SUCCESS << showVersion(checkVersion(ver)) << std::endl;
	}
	else
	{
		std::cout << FAIL << showVersion(checkVersion(ver)) << std::endl;
		exit(0);
	}
}

void checkMainRunning(BOOL state)
{
	state ? std::cout << "main installer will be running!" << std::endl : std::cout << "It fails to run the main installer!" << std::endl;
}

BOOL IsCurrentProcess64bit()
{
#if defined(_WIN64)
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL IsCurrentProcessWow64()
{
	BOOL bIsWow64 = FALSE;
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (!fnIsWow64Process)
	{
		return FALSE;
	}
	
	return fnIsWow64Process(GetCurrentProcess(), &bIsWow64) && bIsWow64;
}

BOOL Is64BitWindows()
{
	if (IsCurrentProcess64bit())
	{
		return TRUE;
	}

	return IsCurrentProcessWow64();
}