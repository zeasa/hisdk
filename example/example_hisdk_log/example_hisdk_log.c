#include <stdio.h>
#include "hisdk_log.h"

int test_func1()
{
	HISDK_LOG_INFO(LOG_DEBUG, "%s", "hello world!!");
	return 0;
}

int main(int argc, char *argv[])
{
	hisdkLOGSetPrintDebugLogFlag(HISDK_LOG_DEBUG_ENABLE);//打印调试信息
	hisdkLOGSetPrintLogPlaceFlag(HISDK_LOG_TO_TERM);  //保存打印信息到终端
    //hisdkLOGSetPrintLogPlaceFlag(HISDK_LOG_TO_FILE);//保存打印信息到文件
	hisdkLOGInit("log", 8000); //(logfilename, filemaxsize)

	HISDK_LOG_INFO(LOG_DEBUG, "%s", "Init log!!");

	test_func1();

	HISDK_LOG_INFO(LOG_DEBUG, "%s", "Destroy log!!");
	hisdkLOGDestroy();
}