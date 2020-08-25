#ifndef LIBHIRT_EVENT_H__
#define LIBHIRT_EVENT_H__

#include <pthread.h>
#include <semaphore.h>
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
    sem_t *m_pSemScheduler;
} hirtEventHandler_t;

hisdkRet_t hirtEventHandlerCreate(hirtEventHandler_t **ppHandler, hirtScheduler_t *pScheduler);
hisdkRet_t hirtEventHandlerDestroy(hirtEventHandler_t *pHandler);
void*      hirtEventHandlerThread(void* arg);

#ifdef __cplusplus
}
#endif
#endif /*LIBHIRT_EVENT_H__*/

