// Acquire memory info 
//
#include "stdafx.h"

//-----------------------------------------------------------------------------

static const int kMaxInfoBuffer = 256;

Json::Value getMemoryInfo()
{
	Json::Value memInfo;

	MEMORYSTATUSEX statusex;
	statusex.dwLength = sizeof(statusex);
	if (GlobalMemoryStatusEx(&statusex))
	{
		
		char tmpInfo[40];
		sprintf_s(tmpInfo, "%.2fGB", statusex.ullTotalPhys / DGBYTES);
		memInfo["*#0总物理内存"] = tmpInfo;
		//*#X 用以规定输出时顺序，可选
		sprintf_s(tmpInfo, "%.2fGB", statusex.ullAvailPhys / DGBYTES);
		memInfo["*#1可用物理内存"] = tmpInfo;

		sprintf_s(tmpInfo, "%d%%", statusex.dwMemoryLoad);
		memInfo["*#2内存使用率"] = tmpInfo;

		sprintf_s(tmpInfo, "%.2fGB", statusex.ullTotalVirtual / DGBYTES);
		memInfo["*#3总虚拟内存"] = tmpInfo;

		sprintf_s(tmpInfo, "%.2fGB", statusex.ullAvailVirtual / DGBYTES);
		memInfo["*#4可用虚拟内存"] = tmpInfo;

		//Json::Value memInfoSum
		//memInfoSum["内存信息"] = memInfo;
		//return memInfoSum;
	}

	return memInfo;
}

