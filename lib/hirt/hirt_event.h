#ifndef LIBHIRT_EVENT_H__
#define LIBHIRT_EVENT_H__

#include <pthread.h>

typedef struct hirtEventHandler
{
    pthread_t m_thread;
    hirtThreadFunction_t m_threadfunc;
    void* m_pArg;

    hirtScoreboard_t *m_pScoreboard;
    pthread_mutex_t *m_pMutexScheduler;
} hirtEventHandler_t;

hisdkRet_t hirtEventHandlerCreate(hirtEventHandler_t **ppHandler, 
    hirtScoreboard_t *m_pScoreboard,pthread_mutex_t *m_pMutexScheduler);
hisdkRet_t hirtEventHandlerDestroy(hirtEventHandler_t *pHandler);
void*     hirtEventHandlerThread(void* arg);

#endif /*LIBHIRT_EVENT_H__*/

