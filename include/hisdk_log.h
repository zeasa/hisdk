#ifndef _HISDK_LOG_H__
#define _HISDK_LOG_H__

#include <string.h>
#include "hisdk.h"

/*ͨ���ַ����洢��С����*/
#define STR_COMM_SIZE               128
#define STR_MAX_SIZE                1024

#define MAX_LOG_FILE_NUM		    (3)

#define NUMBER(type)                sizeof(type)/sizeof(type[0])

#define __FILENAME__                (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

/*��־����*/
enum
{
    LOG_DEBUG = 0,  /*������־*/
    LOG_ERROR,      /*������־*/
    LOG_WARNING,    /*�澯��־*/
    LOG_ACTION,     /*������־*/
    LOG_SYSTEM,     /*ϵͳ��־*/
    BUTTOM
};

/*����־������ն�*/
#define HISDK_LOG_TO_TERM           (0)
/*����־������ļ���*/
#define HISDK_LOG_TO_FILE           (1)

#define HISDK_LOG_DEBUG_DISABLE     (0)
#define HISDK_LOG_DEBUG_ENABLE      (1)


/*������־�궨��*/
#define DEBUG_PRINT                 (1)
#define LOG_PRINT(fmt, ...) do{\
    	if(DEBUG_PRINT)\
    	{\
    		printf(fmt"  [line:%d] [%s]\n", ##__VA_ARGS__, __LINE__, __FUNCTION__);\
    	}\
    }while(0);

/*������־��ӡ(����־��ӡģ�黹δ����ʱʹ��)*/
#define LOG_ERR(fmt, ...) do{\
	    printf("[ERROR]  "fmt"  [line:%d] [%s]\n", ##__VA_ARGS__, __LINE__, __FUNCTION__);\
    }while(0);



/*�洢��־���. 0-���洢��־, 1-�洢��־*/
extern unsigned long g_ulPrintLogPlaceFlag;
/*�Ƿ��ӡ������־���,0-����ӡ������־,1-��ӡ������־*/
extern unsigned long g_ulPrintDebugLogFlag;
unsigned long hisdkLOGPrintLog(unsigned char ucType, unsigned char *pucLogInfo);


/*��־��ӡ�궨��*/
#define HISDK_LOG_INFO(type, fmt, ...) do{\
	if(HISDK_LOG_TO_TERM == g_ulPrintLogPlaceFlag) \
	{ \
		if((0 == g_ulPrintDebugLogFlag) && (LOG_DEBUG == type)) \
		{\
			break;\
		}\
    	unsigned char ucLogInfo[STR_MAX_SIZE] = {0}; \
		snprintf((char *)ucLogInfo, sizeof(ucLogInfo) - 1, fmt"  [%s] [line:%d] [%s]\n", ##__VA_ARGS__, __FILENAME__, __LINE__, __FUNCTION__); \
		hisdkLOGPrintLog(type, ucLogInfo); \
	} \
	else \
	{ \
		unsigned char ucLogInfo[STR_MAX_SIZE] = {0}; \
		snprintf((char *)ucLogInfo, sizeof(ucLogInfo) - 1, fmt"  [%s] [line:%d] [%s]\n", ##__VA_ARGS__, __FILENAME__, __LINE__, __FUNCTION__); \
		hisdkLOGPrintLog(type, ucLogInfo); \
	} \
    }while(0)


/*�Ƿ��ӡ������־���,0-����ӡ������־,1-��ӡ������־*/
extern void          hisdkLOGSetPrintDebugLogFlag(unsigned long flag);
/*�洢��־���. 0-���洢��־, 1-�洢��־*/
extern void          hisdkLOGSetPrintLogPlaceFlag(unsigned long flag);
extern unsigned long hisdkLOGInit(const unsigned char *ucLogFileName, unsigned long ulFileSize);
extern void          hisdkLOGDestroy(void);

#endif //_HISDK_LOG_H__

