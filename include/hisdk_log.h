#ifndef _HISDK_LOG_H__
#define _HISDK_LOG_H__

#include <stdio.h>
#include <string.h>
#include "hisdk_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*通用字符串存储大小定义*/
#define STR_COMM_SIZE               128
#define STR_MAX_SIZE                1024

#define MAX_LOG_FILE_NUM		    (3)

#define NUMBER(type)                sizeof(type)/sizeof(type[0])

#define __FILENAME__                (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

/*日志类型*/
enum
{
    LOG_DEBUG = 0,  /*调试日志*/
    LOG_ERROR,      /*错误日志*/
    LOG_WARNING,    /*告警日志*/
    LOG_ACTION,     /*运行日志*/
    LOG_SYSTEM,     /*系统日志*/
    BUTTOM
};

/*将日志输出到终端*/
#define HISDK_LOG_TO_TERM           (0)
/*将日志输出到文件中*/
#define HISDK_LOG_TO_FILE           (1)

#define HISDK_LOG_DEBUG_DISABLE     (0)
#define HISDK_LOG_DEBUG_ENABLE      (1)


/*调试日志宏定义*/
#define DEBUG_PRINT                 (1)
#define LOG_PRINT(fmt, ...) do{\
    	if(DEBUG_PRINT)\
    	{\
    		printf(fmt"  [line:%d] [%s]\n", ##__VA_ARGS__, __LINE__, __FUNCTION__);\
    	}\
    }while(0);

/*错误日志打印(在日志打印模块还未启动时使用)*/
#define LOG_ERR(fmt, ...) do{\
	    printf("[ERROR]  "fmt"  [line:%d] [%s]\n", ##__VA_ARGS__, __LINE__, __FUNCTION__);\
    }while(0);



/*存储日志标记. 0-不存储日志, 1-存储日志*/
extern unsigned long g_ulPrintLogPlaceFlag;
/*是否打印调试日志标记,0-不打印调试日志,1-打印调试日志*/
extern unsigned long g_ulPrintDebugLogFlag;
unsigned long hisdkLOGPrintLog(unsigned char ucType, unsigned char *pucLogInfo);

/*日志打印宏定义*/
#define HISDK_LOG_INFO(type, fmt, ...) do{\
	if(HISDK_LOG_TO_TERM == g_ulPrintLogPlaceFlag) \
	{ \
		if((0 == g_ulPrintDebugLogFlag) && (LOG_DEBUG == type)) \
		{\
			break;\
		}\
    	unsigned char ucLogInfo[STR_MAX_SIZE] = {0}; \
		snprintf((char *)ucLogInfo, sizeof(ucLogInfo) - 1, fmt" [%s.%d][%s()]\n", ##__VA_ARGS__, __FILENAME__, __LINE__, __FUNCTION__); \
		hisdkLOGPrintLog(type, ucLogInfo); \
	} \
	else \
	{ \
		unsigned char ucLogInfo[STR_MAX_SIZE] = {0}; \
		snprintf((char *)ucLogInfo, sizeof(ucLogInfo) - 1, fmt" [%s.%d][%s()]\n", ##__VA_ARGS__, __FILENAME__, __LINE__, __FUNCTION__); \
		hisdkLOGPrintLog(type, ucLogInfo); \
	} \
    }while(0)


/*是否打印调试日志标记,0-不打印调试日志,1-打印调试日志*/
extern void          hisdkLOGSetPrintDebugLogFlag(unsigned long flag);
/*存储日志标记. 0-不存储日志, 1-存储日志*/
extern void          hisdkLOGSetPrintLogPlaceFlag(unsigned long flag);
extern unsigned long hisdkLOGInit(const unsigned char *ucLogFileName, unsigned long ulFileSize);
extern void          hisdkLOGDestroy(void);


#ifdef __cplusplus
}
#endif
#endif //_HISDK_LOG_H__

