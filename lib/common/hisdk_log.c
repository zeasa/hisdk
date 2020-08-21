#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>

#include "hisdk_log.h"


/*�����־λ�ñ��,0-������ն�,1-�������־�ļ�*/
unsigned long g_ulPrintLogPlaceFlag = 0;
/*�Ƿ��ӡ������־���,0-����ӡ������־,1-��ӡ������־*/
unsigned long g_ulPrintDebugLogFlag = 0;

/*�洢��־���ļ���*/
static unsigned char g_ucLogFileName[MAX_LOG_FILE_NUM][STR_COMM_SIZE] = { { 0 } };
/*ָ����g_ucLogFileName�е��ĸ��ļ�*/
static unsigned char g_ucLogFileNo = 0;

/*��־�ļ���С*/
static unsigned long g_ulLogFileSize = 0;
/*��־�ļ����*/
static FILE *pFile = NULL;
/*��־�洢������*/
static pthread_mutex_t g_stSaveLogMutexLock;
/*��־ģ���ʼ�����*/
static unsigned long g_ulLogInitFlag = 0;


void hisdkLOGSetPrintLogPlaceFlag(unsigned long flag)
{
    g_ulPrintLogPlaceFlag = flag;
}

void hisdkLOGSetPrintDebugLogFlag(unsigned long flag)
{
    g_ulPrintDebugLogFlag = flag;
}

/*****************************************************************
** ������: get_file_size
** �䡡��: char *path
** �䡡��:
** ��������:��ȡָ���ļ���С
** ����ֵ: long
****************************************************************/
static long get_file_size(const char *path)
{
    long filesize = -1;
    struct stat statbuff;

    if (stat(path, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }
    return filesize;
}

/*****************************************************************
** ������: unsigned long hisdkLOGPrintLogTime
** �䡡��:  unsigned long ulBufLen �洢ʱ��Ŀռ䳤��
** �䡡��:unsigned char *ucTime  �洢ʱ��
** ��������:��־���
** ����ֵ:unsigned long
****************************************************************/
unsigned long hisdkLOGPrintLogTime(unsigned char *ucTime, unsigned long ulBufLen)
{
    struct tm *pstTmSec;
    struct timeval stTmMsec;

    if (NULL == ucTime)
    {
        return -1;
    }
    gettimeofday(&stTmMsec, NULL);
    pstTmSec = localtime(&stTmMsec.tv_sec);
    snprintf((char *)ucTime, ulBufLen - 1, "%04d-%02d-%02d %02d:%02d:%02d %03ldms",
             pstTmSec->tm_year + 1900, pstTmSec->tm_mon + 1, pstTmSec->tm_mday, pstTmSec->tm_hour,
             pstTmSec->tm_min, pstTmSec->tm_sec, stTmMsec.tv_usec / 1000);

    return 0;
}

/*****************************************************************
** ������: 	unsigned char *hisdkLOGLogTypeToStr
** �䡡��:  	unsigned char ucType  ��־����
                        unsigned long ulBufLen �洢��־�����ַ����ռ�ĳ���
** �䡡��:unsigned char *pucTypeString ������־���ͽ���ת������Ӧ���ַ���
** ��������:������־����ת������Ӧ���ַ���
** ����ֵ:unsigned long
****************************************************************/
unsigned long hisdkLOGLogTypeToStr(unsigned char ucType, unsigned char *pucTypeString, unsigned long ulBufLen)
{
    if (NULL == pucTypeString)
    {
        return -1;
    }
/*��ֹ����Խ��*/
    ulBufLen -= 1;

    switch (ucType)
    {
    case LOG_DEBUG:
    {
        strncpy((char *)pucTypeString, "DEBUG", ulBufLen);
        break;
    }
    case LOG_ERROR:
    {
        strncpy((char *)pucTypeString, "ERROR", ulBufLen);
        break;
    }
    case LOG_WARNING:
    {
        strncpy((char *)pucTypeString, "WARNING", ulBufLen);
        break;
    }
    case LOG_ACTION:
    {
        strncpy((char *)pucTypeString, "ACTION", ulBufLen);
        break;
    }
    case LOG_SYSTEM:
    {
        strncpy((char *)pucTypeString, "SYSTEM", ulBufLen);
        break;
    }
    default:
    {
        strncpy((char *)pucTypeString, "UNKNOWN", ulBufLen);
        break;
    }
    }
    return 0;
}

/*****************************************************************
** ������: unsigned long hisdkLOGOpenLogFile
** �䡡��:  void
** �䡡��:void
** ��������:����־�ļ�
** ����ֵ:unsigned long
****************************************************************/
unsigned long hisdkLOGOpenLogFile(void)
{
    char *path = (char *)g_ucLogFileName[g_ucLogFileNo];
    char *flag = NULL;
    int len = 0;

/*�ж��ļ��Ƿ��Ѿ���*/
    if (NULL != pFile)
    {
        LOG_PRINT("[ACTION] file opened!");
        return 0;
    }
/*�ж��ļ����Ƿ��ж���*/
    if (NULL == path)
    {
        LOG_PRINT("[ERROR] file name is NULL.");
        return -1;
    }

/*�ж��ļ��Ƿ����*/
    if (!access(path, 0))
    {
        /*��ȡ�ļ���С*/
        if (0 > (len = get_file_size(path)))
        {
            LOG_PRINT("[ERROR] get file size failed!");
            return -1;
        }
    }
    flag = (len > 0 && len < g_ulLogFileSize) ? "a" : "w";

/*���ļ�*/
    pFile = fopen(path, flag);
    if (NULL == pFile)
    {
        LOG_PRINT("[ERROR] open file failed!");
        return -1;
    }
    LOG_PRINT("[DEBUG] open file name = %s", path);
    return 0;
}

/*****************************************************************
** ������: hisdkLOGPrintLog
** �䡡��:  unsigned char *ucLogInfo  ��Ҫ��ӡ���ߴ洢����־��Ϣ
                        unsigned char ucType ��־����
** �䡡��:void
** ��������:��־���
** ����ֵ:unsigned long
****************************************************************/
unsigned long hisdkLOGPrintLog(unsigned char ucType, unsigned char *pucLogInfo)
{
    unsigned long ulResult = 0;
    unsigned long ulFileLen = 0;
    unsigned char ucTime[STR_COMM_SIZE] = { 0 };
    unsigned char ucLogTypeStr[STR_COMM_SIZE] = { 0 };
    unsigned char ucLogInfo[STR_MAX_SIZE] = { 0 };

    if (NULL == pucLogInfo)
    {
        return -1;
    }

/*����־����ת�����ַ���*/
    ulResult = hisdkLOGLogTypeToStr(ucType, ucLogTypeStr, sizeof(ucLogTypeStr));
/*��ȡ������־��ʱ��*/
    ulResult += hisdkLOGPrintLogTime(ucTime, sizeof(ucTime));
    if (0 != ulResult)
    {
        return -1;
    }
    snprintf((char *)ucLogInfo, sizeof(ucLogInfo) - 1, "[%s][%s] %s", ucTime, ucLogTypeStr, pucLogInfo);
/*�ж��Ƿ��ӡ������־*/
    if (HISDK_LOG_TO_TERM == g_ulPrintLogPlaceFlag)
    {
        printf("%s", ucLogInfo);
        return 0;
    }
/*���������ļ�����*/
    pthread_mutex_lock(&g_stSaveLogMutexLock);
/*����־�ļ�*/
    (void)hisdkLOGOpenLogFile();
    if (NULL != pFile)
    {
        fputs((char *)ucLogInfo, pFile);
        ulFileLen = ftell(pFile);
        LOG_PRINT("file len = %ld", ulFileLen);
        if (ulFileLen >= g_ulLogFileSize)
        {
            fclose(pFile);
            pFile = NULL;
            g_ucLogFileNo = (g_ucLogFileNo + 1) % MAX_LOG_FILE_NUM;
        }
    }
    pthread_mutex_unlock(&g_stSaveLogMutexLock);
    return 0;
}

/*****************************************************************
** ������: hisdkLOGInit
** �䡡��:  const unsigned char* ucLogFileName  ����������־���ļ���
                        unsigned long ulFileSize �洢��־���ļ���С
** �䡡��:void
** ��������:��־��ӡ��ʼ��
** ����ֵ:unsigned long
****************************************************************/
unsigned long hisdkLOGInit(const unsigned char *ucLogFileName, unsigned long ulFileSize)
{
    unsigned int i = 0;
/*�жϲ����ĺϷ���*/
    if ((NULL == ucLogFileName) || !(ulFileSize > 0))
    {
        return -1;
    }
/*�ж��Ƿ���־�������־�ļ�*/
    if ((HISDK_LOG_TO_FILE != g_ulPrintLogPlaceFlag) || (0 != g_ulLogInitFlag))
    {
        //printf("g_ulPrintLogPlaceFlag = %ld g_ulLogInitFlag = %ld\n", g_ulPrintLogPlaceFlag, g_ulLogInitFlag);
        LOG_PRINT("print log to termination!!");
        return 0;
    }

/*��¼��־ģ���Ѿ�����ʼ��(��ֹ��ģ�鱻�ظ���ʼ��)*/
    g_ulLogInitFlag = 1;

/*���ɴ洢��־���ļ���*/
    for (i = 0; i < NUMBER(g_ucLogFileName); i++)
    {
        snprintf((char *)g_ucLogFileName[i], sizeof(g_ucLogFileName[i]) - 1, "%s_%02d", ucLogFileName, i);
        LOG_PRINT("Log File: %s", g_ucLogFileName[i]);
        printf("Log File: %s\n", g_ucLogFileName[i]);
    }
/*������־�ļ���С*/
    g_ulLogFileSize = ulFileSize;
    pthread_mutex_init(&g_stSaveLogMutexLock, NULL);

    return 0;
}

/*****************************************************************
** ������: hisdkLOGDestroy
** �䡡��:  void
** �䡡��:void
** ��������:��־��ӡ��Դ�ͷ�
** ����ֵ:unsigned long
****************************************************************/
void hisdkLOGDestroy(void)
{
    if (pFile != NULL)
    {
        fclose(pFile);
        pFile = NULL;
    }
    pthread_mutex_destroy(&g_stSaveLogMutexLock);
    return;
}



