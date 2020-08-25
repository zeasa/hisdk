#include "hirt_event.h"


hisdkRet_t hirtEventHandlerCreate(hirtEventHandler_t **ppHandler, hirtScheduler_t *pScheduler)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtEventHandler_t *pHandler = NULL;

    pHandler = (hirtEventHandler_t*)hisdkAlloc(sizeof(hirtEventHandler_t));
    if(pHandler == NULL)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "eventhandler alloc failed!");
    }
    
    pHandler->m_pScoreboard = pScheduler->m_pScoreboard;
    pHandler->m_pSemScheduler = &pScheduler->m_semEvent;
    pHandler->m_threadfunc = (hirtThreadFunction_t)hirtEventHandlerThread;
    
    if(pthread_create(&pHandler->m_thread, NULL, hirtEventHandlerThread, (void*)pHandler) != 0)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_CREATETHREAD, "eventhandler thread create failed!");
    }

    *ppHandler = pHandler;
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtEventHandlerCreate done.");
    return e;
    
fail:
    if(pHandler != NULL)
    {
        hisdkFree(pHandler);
    }
    
    *ppHandler = NULL;
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtEventHandlerCreate failed.");
    return e;
}

hisdkRet_t hirtEventHandlerDestroy(hirtEventHandler_t *pHandler)
{
    hisdkFree(pHandler);
}

void* hirtEventHandlerThread(void* arg)
{
    hirtEventHandler_t *pHandler = (hirtEventHandler_t *)arg;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtEventHandlerThread thread entered arg=%p", arg);
    
    while(1)
    {
        //pend reading event kfifo

        //update scoreboard
        pthread_mutex_lock(&pHandler->m_pScoreboard->m_mutex);
        //int freecnt = 0;
        //for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
        //{
        //    if(pScoreboard->m_coreinfo[i].m_status == HIPUCORE_FREE)
        //    {
        //        schedTbl[freecnt] = i;
        //        freecnt++;
        //    }
        //}
        pthread_mutex_unlock(&pHandler->m_pScoreboard->m_mutex);

        //send semaphore to scheduler thread
        sem_post(pHandler->m_pSemScheduler);
    }

    return 0;
}

