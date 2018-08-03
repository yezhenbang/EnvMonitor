// EnvMonitor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	Json::Value envInfo;

	envInfo["*#1OS-Info"] = getOsInfo();

	envInfo["*#2CPU-Info"] = getCpuInfo();

	envInfo["*#3Memory-Info"] = getMemoryInfo();

	envInfo["*#4Disk-Info"] = getDiskInfo();

	//Json::FastWriter fastWriter;
	//std::cout << fastWriter.write(envInfo) << std::endl;

	// pretty print json
	Json::StyledWriter styledWriter;
	std::cout << styledWriter.write(envInfo) << std::endl;

    return 0;
}

