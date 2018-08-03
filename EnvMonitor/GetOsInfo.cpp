// Acquire os info
#include "stdafx.h"

//-----------------------------------------------------------------------------

/*
char* format(DWORD value)
{
char* ch;
ch = new char[20];
sprintf_s(ch, 20, "%d", value);
return ch;
}

char* format(double value)
{
char* ch;
ch = new char[20];
sprintf_s(ch, 20, "%.2f", value);
return ch;
}
*/

/*
BOOL IsWow64() //判断64位操作系统
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	return bIsWow64;
}
*/

Json::Value getOsInfo()
{
	SYSTEM_INFO sysInfo;     //获取系统信息
	GetNativeSystemInfo(&sysInfo);

	std::string sysVersionStr;
	//先判断是否为win8.1或win10
	//调用ntdll.dll中的RtlGetNtVersionNumbers方法获取
	typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(L"ntdll.dll");
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");
	proc(&dwMajor, &dwMinor, &dwBuildNumber);
	if (dwMajor == 6 && dwMinor == 3)	//win 8.1
	{
		sysVersionStr = "Microsoft Windows 8.1";
	}
	else if (dwMajor == 10 && dwMinor == 0)	//win 10
	{
		sysVersionStr = "Microsoft Windows 10";
	}
	else
	{
		//判断win8.1以下的版本
		//旧版本系统用GetVersionEx判断
		OSVERSIONINFOEX os = { sizeof(OSVERSIONINFOEX) };
		
		#pragma warning(disable:4996)
		if (GetVersionEx((OSVERSIONINFO *)&os))
		{
			switch (os.dwMajorVersion)
			{
			case 4:
				switch (os.dwMinorVersion)
				{
				case 0:
					if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
						sysVersionStr = "Microsoft Windows NT 4.0";
					else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
						sysVersionStr = "Microsoft Windows 95";
					break;
				case 10:
					sysVersionStr = "Microsoft Windows 98";
					break;
				case 90:
					sysVersionStr = "Microsoft Windows Me";
					break;
				}
				break;
			case 5:
				switch (os.dwMinorVersion)
				{
				case 0:
					sysVersionStr = "Microsoft Windows 2000";
					break;
				case 1:
					sysVersionStr = "Microsoft Windows XP";
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION &&
						sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
						sysVersionStr = "Microsoft Windows XP Professional x64 Edition";
					else if (GetSystemMetrics(SM_SERVERR2) == 0)
						sysVersionStr = "Microsoft Windows Server 2003";
					else if (GetSystemMetrics(SM_SERVERR2) != 0)
						sysVersionStr = "Microsoft Windows Server 2003 R2";
					break;
				}
				break;
			case 6:
				switch (os.dwMinorVersion)
				{
				case 0:
					if (os.wProductType == VER_NT_WORKSTATION)
						sysVersionStr = "Microsoft Windows Vista";
					else
						sysVersionStr = "Microsoft Windows Server 2008";
					break;
				case 1:
					if (os.wProductType == VER_NT_WORKSTATION)
						sysVersionStr = "Microsoft Windows 7";
					else
						sysVersionStr = "Microsoft Windows Server 2008 R2";
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION)
						sysVersionStr = "Microsoft Windows 8";
					else
						sysVersionStr = "Microsoft Windows Server 2012";
					break;
				}
				break;
			default:
				sysVersionStr = "未知操作系统";
			}
		}
		else
			sysVersionStr = "版本获取失败";
	}

	bool is64Bit;
	//判断是否64位操作系统
	// if (IsWow64())
	if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64 ||
		sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		is64Bit = true;
	}
	else
	{
		is64Bit = false;
	}
	
	Json::Value osInfo;

	osInfo["*#1系统版本"] = sysVersionStr;
	//"*#X" 用以规定格式化为string后的顺序，选用
	osInfo["*#2系统位数"] = is64Bit ? "64Bit" : "32Bit";

	osInfo["*#3系统页面大小"] = (long)sysInfo.dwPageSize;

	//Json::Value osInfoSum;
	//osInfoSum["系统信息"] = osInfo;

	return osInfo;
}