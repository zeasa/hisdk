#ifndef LIBHIRT_SCHEDULER_H__
#define LIBHIRT_SCHEDULER_H__

#include <pthread.h>
#include "hirt.h"
#include "hirt_cqueue.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum
{
    HIPUCORE_FREE = 0,
    HIPUCORE_BUSY,
    HIPUCORE_PEND
} hirtCoreStat_t;

typedef struct
{
    hirtCoreStat_t m_status;
} hirtCoreInfo_t;

typedef struct
{
    hirtCoreInfo_t m_coreinfo[HIRT_HIPU200_CORENUMMAX];
    pthread_mutex_t m_mutex;
} hirtScoreboard_t;

typedef struct
{
    hirtCmdQueue_t *m_pQueue;
    hirtScoreboard_t *m_pScoreboard;
    sem_t m_semEvent;

    pthread_t m_thread;
    hirtThreadFunction_t m_threadfunc;
} hirtScheduler_t;

hisdkRet_t hirtSchedulerCreate(hirtScheduler_t **ppScheduler, hirtCmdQueue_t * const pCmdQueue);
hisdkRet_t hirtSchedulerDestroy(hirtScheduler_t *pScheduler);
void*      hirtSchedulerThread(void *arg);

hisdkRet_t hirtScoreboardCreate(hirtScoreboard_t **ppScoreboard);
hisdkRet_t hirtScoreboardDestroy(hirtScoreboard_t *pScoreboard);

#ifdef __cplusplus
}
#endif
#endif /*LIBHIRT_SCHEDULER_H__*/