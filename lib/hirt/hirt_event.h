#ifndef LIBHIRT_EVENT_H__
#define LIBHIRT_EVENT_H__

#include <pthread.h>
#include "hirt.h"
#include "hirt_scheduler.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct
{
    pthread_t m_thread;
    hirtThreadFunction_t m_threadfunc;
    void* m_pArg;

    hirtScoreboard_t *m_pScoreboard;
    pthread_mutex_t *m_pMutexScheduler;
} hirtEventHandler_t;

hisdkRet_t hirtEventHandlerCreate(hirtEventHandler_t **ppHandler, 
    hirtScoreboard_t *m_pScoreboard, pthread_mutex_t *m_pMutexScheduler);
hisdkRet_t hirtEventHandlerDestroy(hirtEventHandler_t *pHandler);
void*     hirtEventHandlerThread(void* arg);

#ifdef __cplusplus
}
#endif
#endif /*LIBHIRT_EVENT_H__*/

