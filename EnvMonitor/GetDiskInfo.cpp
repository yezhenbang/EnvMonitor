// Acquire disk drive info
//
#include "stdafx.h"

//-----------------------------------------------------------------------------

//��bת��ΪGBΪ��λ
#define GB(x) (x.QuadPart / DGBYTES)

Json::Value getDiskInfo()
{
	//json����
	Json::Value diskInfo;
	//json ÿ������������Ϣ
	Json::Value diskInfoSub;

	int DiskCount = 0;
	DWORD DiskInfo = GetLogicalDrives();
	//����GetLogicalDisks()�������Ի�ȡϵͳ���߼����������������������ص���һ��32λ�޷����������ݡ�  
	while (DiskInfo)//ͨ��ѭ�������鿴ÿһλ�����Ƿ�Ϊ1�����Ϊ1�����Ϊ��,���Ϊ0����̲����ڡ�  
	{
		if (DiskInfo & 1)
		{
			++DiskCount;
		}
		DiskInfo = DiskInfo >> 1;
	}

	//-----------------------------------------------------------------------------------------

	int DStrLength = GetLogicalDriveStrings(0, NULL);
	//ͨ��GetLogicalDiskStrings()������ȡ�����������ַ�����Ϣ���ȡ�  
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
			"δ֪�豸",
			"δ����",
			"���ƶ�����",
			"Ӳ��",
			"����������",
			"����",
			"�ڴ����"
		};

		//---------------------------------------------------------------------------------

		ULARGE_INTEGER freeBytesAvailableToCaller;
		ULARGE_INTEGER totalNumberOfBytes;
		ULARGE_INTEGER totalNumberOfFreeBytes;

		if (GetDiskFreeSpaceEx(tmpDiskStr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes))
		{
			char tmpCh[20];
			sprintf_s(tmpCh, "%s", tmpDiskStr);
			diskInfoSub["*#1�̷�"] = tmpDiskStr;

			diskInfoSub["*#2����"] = diskTypeArr[diskType];

			sprintf_s(tmpCh, "%.2fGB", GB(totalNumberOfBytes));
			diskInfoSub["*#3�ܿռ�"] = tmpCh;

			sprintf_s(tmpCh, "%.2fGB", GB(totalNumberOfFreeBytes));
			diskInfoSub["*#4ʣ��ռ�"] = tmpCh;

		}

		tmpDiskStr += 4;
		diskInfo.append(diskInfoSub);
		diskInfoSub.clear();
		//����"X:\ "(4 char)��������һ�����̸���
	}

	//Json::Value diskInfoSum;
	//diskInfoSum["������Ϣ"] = diskInfo;

	return diskInfo;
}

