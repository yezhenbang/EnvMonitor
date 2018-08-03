// Acquire disk drive info
//
#include "stdafx.h"

//-----------------------------------------------------------------------------

//从b转换为GB为单位
#define GB(x) (x.QuadPart / DGBYTES)

Json::Value getDiskInfo()
{
	//json数组
	Json::Value diskInfo;
	//json 每个驱动器的信息
	Json::Value diskInfoSub;

	int DiskCount = 0;
	DWORD DiskInfo = GetLogicalDrives();
	//利用GetLogicalDisks()函数可以获取系统中逻辑驱动器的数量，函数返回的是一个32位无符号整型数据。  
	while (DiskInfo)//通过循环操作查看每一位数据是否为1，如果为1则磁盘为真,如果为0则磁盘不存在。  
	{
		if (DiskInfo & 1)
		{
			++DiskCount;
		}
		DiskInfo = DiskInfo >> 1;
	}

	//-----------------------------------------------------------------------------------------

	int DStrLength = GetLogicalDriveStrings(0, NULL);
	//通过GetLogicalDiskStrings()函数获取所有驱动器字符串信息长度。  
	TCHAR* DiskStr = new TCHAR[DStrLength];
	GetLogicalDriveStrings(DStrLength, DiskStr);
	
	TCHAR* tmpDiskStr = DiskStr;
	for (int i = 0; i < DiskCount; i++)
	{
		
		UINT diskType = GetDriveType(tmpDiskStr);
		/*
		char *diskTypeArr[] = {
		"UNKNOWN",
		"NO_ROOT_DIR",
		"REMOVABLE",
		"FIXED",
		"REMOTE",
		"CDROM",
		"RAMDISK"
		};
		*/
		char *diskTypeArr[] = {
			"未知设备",
			"未分区",
			"可移动磁盘",
			"硬盘",
			"网络驱动器",
			"光驱",
			"内存磁盘"
		};

		//---------------------------------------------------------------------------------

		ULARGE_INTEGER freeBytesAvailableToCaller;
		ULARGE_INTEGER totalNumberOfBytes;
		ULARGE_INTEGER totalNumberOfFreeBytes;

		if (GetDiskFreeSpaceEx(tmpDiskStr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes))
		{
			char tmpCh[20];
			sprintf_s(tmpCh, "%s", tmpDiskStr);
			diskInfoSub["*#1盘符"] = tmpDiskStr;

			diskInfoSub["*#2类型"] = diskTypeArr[diskType];

			sprintf_s(tmpCh, "%.2fGB", GB(totalNumberOfBytes));
			diskInfoSub["*#3总空间"] = tmpCh;

			sprintf_s(tmpCh, "%.2fGB", GB(totalNumberOfFreeBytes));
			diskInfoSub["*#4剩余空间"] = tmpCh;

		}

		tmpDiskStr += 4;
		diskInfo.append(diskInfoSub);
		diskInfoSub.clear();
		//跳过"X:\ "(4 char)，检索下一个磁盘根名
	}

	//Json::Value diskInfoSum;
	//diskInfoSum["磁盘信息"] = diskInfo;

	return diskInfo;
}

