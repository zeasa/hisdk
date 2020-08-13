#ifndef LIBHIRT_SCHEDULER_H__
#define LIBHIRT_SCHEDULER_H__

#include "libhirt.h"

typedef enum hirtCoreStat
{
    HIPUCORE_FREE = 0,
    HIPUCORE_BUSY,
    HIPUCORE_PEND
} hirtCoreStat_t;

typedef struct hirtCoreInfo
{
    hirtCoreStat_t m_status;
} hirtCoreInfo_t;

typedef struct hirtScoreboard
{
    hirtCoreInfo_t m_coreinfo[HIRT_HIPU200_CORENUMMAX];
    pthread_mutex_t m_mutex;
} hirtScoreboard_t;

typedef struct hirtScheduler
{
    hirtCmdQueue_t *m_pQueue;
    hirtScoreboard_t *m_pScoreboard;
    sem_t m_semEvent;

    pthread_t m_thread;
    hirtThreadFunction_t m_threadfunc;
} hirtScheduler_t;

hirtRet_t libhirt_scoreboard_create(hirtScoreboard_t **ppScoreboard);
hirtRet_t libhirt_scoreboard_destroy(hirtScoreboard_t *pScoreboard);
hirtRet_t libhirt_scheduler_create(hirtScheduler_t **ppScheduler);
hirtRet_t libhirt_scheduler_destroy(hirtScheduler_t *pScheduler);
void*     libhirt_scheduler_thread(void *arg);

#endif /*LIBHIRT_SCHEDULER_H__*/