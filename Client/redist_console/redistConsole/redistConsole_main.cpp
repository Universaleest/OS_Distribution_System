// Created by LEE Jeun jeun@wayne-inc.com
/*#include <iostream>
#include <string>
#include <cstdlib>
#include <Windows.h>
#include <atlstr.h>
#include <Msi.h>*/
#include "redistConsole.h"
#include "resource.h"

// #define DIR_LEN MAX_PATH+1

// const CString KEY = "SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio";

int main()
{
	SetConsoleTitle(_T("Wayne OS Installer 1.4.2"));
	LONG result;
	BOOL STATE;
	HKEY hKey;
	CString path;
	if (Is64BitWindows())
	{
		path = KEY64;
	}
	else
	{
		path = KEY32;
	}
	/*result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JUDGE3264, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS) path = KEY32;
	else path = KEY64;*/
	CString ver = path + MSVCRKEY13;
	// CString csProduct = "{8122DAB1-ED4D-3676-BB0A-CA368196543E}"; // vcredist 2013 x86 minimum 12.0.40664
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ver, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS)
	{
		showStatus(ver, result);
		//PROCESS_INFORMATION pi = runExApp(MSVCR13, STATE);
		//checkAppRunning(pi);
		SHELLEXECUTEINFO si = runExApp(MSVCR13, STATE);
		checkAppRunning(si);
		isInstalled(hKey, ver);
	}
	else showStatus(ver, result);
	ver = path + MSVCRKEY15;
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ver, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS)
	{
		showStatus(ver, result);
		//PROCESS_INFORMATION pi = runExApp(MSVCR15, STATE);
		//checkAppRunning(pi);
		SHELLEXECUTEINFO si = runExApp(MSVCR15, STATE);
		checkAppRunning(si);
		isInstalled(hKey, ver);
	}
	else showStatus(ver, result);
	runExApp(MAIN_INSTALLER, STATE);
	checkMainRunning(STATE);
	return 0;
	// INSTALLSTATE t = MsiQueryProductState(csProduct);
	/*if(result != ERROR_SUCCESS) // if (t != INSTALLSTATE_DEFAULT)
	{
		std::cout << "Microsoft visual c++ 2013 redistributable(x86) is installing..." << std::endl;
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi = { 0, };
		si.cb = sizeof(si);
		TCHAR command[DIR_LEN];
		GetModuleFileName(NULL, command, sizeof(command));
		PathRemoveFileSpec(command);
		TCHAR exe[] = _T("\\vcredist\\vcredist_x86_2013.exe");
		StrCatBuff(command, exe, sizeof(command));
		BOOL state;
		state = CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		unsigned long l;
		if (pi.hProcess)
		{
			GetExitCodeProcess(pi.hProcess, &l);
			while (l == STILL_ACTIVE)
			{
				GetExitCodeProcess(pi.hProcess, &l);
			}
		}
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, ver, 0, KEY_READ, &hKey) == ERROR_SUCCESS) //if (MsiQueryProductState(csProduct) == INSTALLSTATE_DEFAULT) 
		{ 
			std::cout << "Succeed to install Microsoft visual c++ 2013 redistributable(x86)!" << std::endl; 
		}
		else
		{
			std::cout << "Fail to install Microsoft visual c++ 2013 redistributable(x86)!" << std::endl;
			return 0;
		}
	}
	else std::cout << "This PC already had Microsoft visual c++ 2013 redistributable(x86)" << std::endl;
	// csProduct = "{BBF2AC74-720C-3CB3-8291-5E34039232FA}"; // vcredist 2015 x86 minimum 14.0.24215
	// csProduct = "{7B77DE7F-5219-435E-9CE1-FC77F1D4CCAD}"; // vcredist 2017 x86 minimum 14.16.27012
	//t = MsiQueryProductState(csProduct);
	ver = KEY + "\\14.0\\VC\\Runtimes\\x86";
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ver, 0, KEY_READ, &hKey);
	if(result != ERROR_SUCCESS)//if (t != INSTALLSTATE_DEFAULT)
	{
		std::cout << "Microsoft visual c++ 2015 redistributable(x86) is installing..." << std::endl;
		// std::cout << "Microsoft visual c++ 2017 redistributable(x86) is installing..." << std::endl;
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi = { 0, };
		si.cb = sizeof(si);
		TCHAR command[DIR_LEN];
		GetModuleFileName(NULL, command, sizeof(command));
		PathRemoveFileSpec(command);
		TCHAR exe[] = _T("\\vcredist\\vcredist_x86_2015.exe");
		// TCHAR exe[] = _T("\\vcredist\\vcredist_x86_2017.exe");
		StrCatBuff(command, exe, sizeof(command));
		BOOL state;
		state = CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		unsigned long l;
		if (pi.hProcess)
		{
			GetExitCodeProcess(pi.hProcess, &l);
			while (l == STILL_ACTIVE)
			{
				GetExitCodeProcess(pi.hProcess, &l);
			}
		}
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, ver, 0, KEY_READ, &hKey) == ERROR_SUCCESS) // if (MsiQueryProductState(csProduct) == INSTALLSTATE_DEFAULT)
		{
			std::cout << "Succeed to install Microsoft visual c++ 2015 redistributable(x86)!" << std::endl;
			// std::cout << "Succeed to install Microsoft visual c++ 2017 redistributable(x86)!" << std::endl;
		}
		else
		{
			std::cout << "Fail to install Microsoft visual c++ 2015 redistributable(x86)!" << std::endl;
			// std::cout << "Fail to install Microsoft visual c++ 2017 redistributable(x86)!" << std::endl;
			return 0;
		}
	}
	else std::cout << "This PC already had Microsoft visual c++ 2015 redistributable(x86)" << std::endl;
	// else std::cout << "This PC already had Microsoft visual c++ 2017 redistributable(x86)" << std::endl;
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	TCHAR command[DIR_LEN];
	GetModuleFileName(NULL, command, sizeof(command));
	PathRemoveFileSpec(command);
	TCHAR exe[] = _T("\\main\\wayneOSforWin.exe");
	StrCatBuff(command, exe, sizeof(command));
	BOOL state;
	state = CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if(state) std::cout << "main installer will be running!" << std::endl;
	else std::cout << "It fails to run the main installer!" << std::endl;
	return 0;*/
}