#ifndef REDIST_CONSOLE_H
#define REDIST_CONSOLE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <Windows.h>
#include <atlstr.h>
#include <Msi.h>

#define DIR_LEN MAX_PATH+1

const CString JUDGE3264 = "SOFTWARE\\WOW6432Node";
const CString KEY32 = "SOFTWARE\\Microsoft\\VisualStudio";
const CString KEY64 = "SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio";
const CString MSVCR13 = "\\vcredist\\vcredist_x86_2013.exe";
const CString MSVCR15 = "\\vcredist\\vcredist_x86_2015.exe";
const CString MAIN_INSTALLER = "\\main\\wayneOSforWin.exe";
const CString MSVCRKEY13 = "\\12.0\\VC\\Runtimes\\x86";
const CString MSVCRKEY15 = "\\14.0\\VC\\Runtimes\\x86";
const std::string INSTALL = " is installing...";
const std::string SUCCESS = "Succeed to install ";
const std::string FAIL = "Fail to install ";
const std::string ALREADY_INSTALLED = "This PC already had ";

const std::string showVersion(int version);
int checkVersion(CString ver);
void showStatus(CString ver, LONG result);
//PROCESS_INFORMATION runExApp(CString ver, BOOL& STATE);
SHELLEXECUTEINFO runExApp(CString ver, BOOL& STATE);
//void checkAppRunning(PROCESS_INFORMATION& pi);
void checkAppRunning(SHELLEXECUTEINFO& si);
void isInstalled(HKEY& hKey, CString ver);
void checkMainRunning(BOOL state);
BOOL IsCurrentProcess64bit();
BOOL IsCurrentProcessWow64();
BOOL Is64BitWindows();

#endif
