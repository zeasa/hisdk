#include "libhirt.h"
#include "libhirt_event.h"

hisdkRet_t hirtEventHandlerCreate(hirtEventHandler_t **ppHandler, 
    hirtScoreboard_t *m_pScoreboard,pthread_mutex_t *m_pMutexScheduler)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hirtEventHandler_t *pHandler;

    pHandler = (hirtEventHandler_t*)malloc(sizeof(hirtEventHandler_t));
    *ppHandler = pHandler;
    pHandler->m_pScoreboard = m_pScoreboard;
    pHandler->m_pMutexScheduler = m_pMutexScheduler;

    pHandler.m_threadfunc = hirtEventHandlerThread;
    if(pthread_create(&pHandler.m_thread, NULL, hirtEventHandlerThread, (void*)pHandler) != 0)
    {
        ret = HISDK_RET_ERR_CREATETHREAD;
        goto err;
    }

    return ret;
    
err:
    free(pHandler);
}

hisdkRet_t hirtEventHandlerDestroy(hirtEventHandler_t *pHandler)
{
    //pthread_exit(NULL);
    free(pHandler);
}

void* hirtEventHandlerThread(void* arg)
{
    hirtEventHandler_t *pHandler = (hirtEventHandler_t *)arg;
    
    while(1)
    {
        //pend reading event kfifo

        //update scoreboard
        pthread_mutex_lock(&pHandler->m_pScoreboard.m_mutex);
        //int freecnt = 0;
        //for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
        //{
        //    if(pScoreboard->m_coreinfo[i].m_status == HIPUCORE_FREE)
        //    {
        //        schedTbl[freecnt] = i;
        //        freecnt++;
        //    }
        //}
        pthread_mutex_unlock(&pHandler->m_pScoreboard.m_mutex);

        //send semaphore to scheduler thread
        sem_post(&pHandler->m_pScoreboard.m_mutex);
    }
}

