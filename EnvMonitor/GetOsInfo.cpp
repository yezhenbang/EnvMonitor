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
BOOL IsWow64() //�ж�64λ����ϵͳ
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
	SYSTEM_INFO sysInfo;     //��ȡϵͳ��Ϣ
	GetNativeSystemInfo(&sysInfo);

	std::string sysVersionStr;
	//���ж��Ƿ�Ϊwin8.1��win10
	//����ntdll.dll�е�RtlGetNtVersionNumbers������ȡ
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
		//�ж�win8.1���µİ汾
		//�ɰ汾ϵͳ��GetVersionEx�ж�
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
				sysVersionStr = "δ֪����ϵͳ";
			}
		}
		else
			sysVersionStr = "�汾��ȡʧ��";
	}

	bool is64Bit;
	//�ж��Ƿ�64λ����ϵͳ
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

	osInfo["*#1ϵͳ�汾"] = sysVersionStr;
	//"*#X" ���Թ涨��ʽ��Ϊstring���˳��ѡ��
	osInfo["*#2ϵͳλ��"] = is64Bit ? "64Bit" : "32Bit";

	osInfo["*#3ϵͳҳ���С"] = (long)sysInfo.dwPageSize;

	//Json::Value osInfoSum;
	//osInfoSum["ϵͳ��Ϣ"] = osInfo;

	return osInfo;
}