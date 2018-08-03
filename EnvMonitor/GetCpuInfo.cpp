// Acquire CPU info

#include "stdafx.h"

//-----------------------------------------------------------------------------

__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

	return (b - a);
}
//WIN CPU使用情况
__int64 getCpuUsage() {
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;

	hEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled

	WaitForSingleObject(hEvent, 300);
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	__int64 idle = CompareFileTime(preidleTime, idleTime);
	__int64 kernel = CompareFileTime(prekernelTime, kernelTime);
	__int64 user = CompareFileTime(preuserTime, userTime);

	__int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
	//__int64 cpuidle = (idle) * 100 / (kernel + user);

	return cpu;
}


//** use asm , fxxx
// ---- get cpu info ---- //
// save 4 register variables
DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;

// init cpu in assembly language
void initCpu(DWORD veax)
{
	__asm
	{
		mov eax, veax
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}
}

long getCpuFreq()
{
	int start, over;
	_asm
	{
		RDTSC
		mov start, eax
	}
	Sleep(50);
	_asm
	{
		RDTSC
		mov over, eax
	}
	return (over - start) / 50000;
}

/*
std::string getManufactureID()
{
char manuID[25];
memset(manuID, 0, sizeof(manuID));

initCpu(0);
memcpy(manuID + 0, &debx, 4); // copy to array
memcpy(manuID + 4, &dedx, 4);
memcpy(manuID + 8, &decx, 4);

return manuID;
}
*/

std::string getCpuType()
{
	const DWORD id = 0x80000002; // start 0x80000002 end to 0x80000004
	char cpuType[49];
	memset(cpuType, 0, sizeof(cpuType));

	for (DWORD t = 0; t < 3; t++)
	{
		initCpu(id + t);

		memcpy(cpuType + 16 * t + 0, &deax, 4);
		memcpy(cpuType + 16 * t + 4, &debx, 4);
		memcpy(cpuType + 16 * t + 8, &decx, 4);
		memcpy(cpuType + 16 * t + 12, &dedx, 4);
	}

	return cpuType;
}

Json::Value getCpuInfo()
{
	Json::Value cpuInfo;

	SYSTEM_INFO sysInfo;     //获取系统信息
	GetNativeSystemInfo(&sysInfo);

	std::string processorArchitecture;

	switch (sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		processorArchitecture = "x64 (AMD or Intel)";
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		processorArchitecture = "ARM";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		processorArchitecture = "ARM64";
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		processorArchitecture = "Intel Itanium-based";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		processorArchitecture = "x86";
		break;
	default:
		processorArchitecture = "UNKNOWN";
		break;
	}

	cpuInfo["*#1CPU型号"] = getCpuType();

	cpuInfo["*#2CPU架构"] = processorArchitecture;

	cpuInfo["*#3CPU核心数"] = (long)sysInfo.dwNumberOfProcessors;

	char cpuFreq[10];
	sprintf_s(cpuFreq, "%dMHz", getCpuFreq());
	cpuInfo["*#4CPU主频"] = cpuFreq;

	char cpuUsage[10];
	sprintf_s(cpuUsage, "%d%%", getCpuUsage());
	cpuInfo["*#5CPU占用"] = cpuUsage;

	//Json::Value cpuInfoSum;
	//cpuInfoSum["CPU信息"] = cpuInfo;

	/*
	char cpuType[10];
	switch (sysInfo.dwProcessorType)
	{
	case PROCESSOR_INTEL_386:
	case PROCESSOR_INTEL_486:
	case PROCESSOR_INTEL_PENTIUM:
	case PROCESSOR_INTEL_IA64:
	case PROCESSOR_AMD_X8664:
	default:
		break;
	}
	cpuInfoSum["CPUType"] = cpuType;
	*/

	return cpuInfo;
}

