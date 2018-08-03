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
		memInfo["*#0�������ڴ�"] = tmpInfo;
		//*#X ���Թ涨���ʱ˳�򣬿�ѡ
		sprintf_s(tmpInfo, "%.2fGB", statusex.ullAvailPhys / DGBYTES);
		memInfo["*#1���������ڴ�"] = tmpInfo;

		sprintf_s(tmpInfo, "%d%%", statusex.dwMemoryLoad);
		memInfo["*#2�ڴ�ʹ����"] = tmpInfo;

		sprintf_s(tmpInfo, "%.2fGB", statusex.ullTotalVirtual / DGBYTES);
		memInfo["*#3�������ڴ�"] = tmpInfo;

		sprintf_s(tmpInfo, "%.2fGB", statusex.ullAvailVirtual / DGBYTES);
		memInfo["*#4���������ڴ�"] = tmpInfo;

		//Json::Value memInfoSum
		//memInfoSum["�ڴ���Ϣ"] = memInfo;
		//return memInfoSum;
	}

	return memInfo;
}

