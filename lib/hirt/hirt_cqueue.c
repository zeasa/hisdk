#include "hirt_cqueue.h"

hisdkRet_t hirtCmdQueueCreate(hirtCmdQueue_t **ppQueue)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtCmdQueue_t *pQueue = NULL;
    
    pQueue = (hirtCmdQueue_t *)hisdkAlloc(sizeof(hirtCmdQueue_t));
    if(pQueue == NULL)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "queue alloc failed!");
    }
    hisdkMemset(pQueue, 0, sizeof(hirtCmdQueue_t));
    
    HISDK_ERR_FCALLFAIL( hirtFifoCreate(&pQueue->pfifo, HIRT_CMDQUEUE_SIZMAX, sizeof(hirtCmdNode_t)) );
        
    HISDK_LOG_INFO(LOG_DEBUG, "hirtCmdQueueCreate done.");
    *ppQueue = pQueue;
    return e;

fail:
    if(pQueue != NULL)
    {
        hisdkFree(pQueue);    
    }
    
    HISDK_LOG_INFO(LOG_DEBUG, "hirtCmdQueueCreate failed.");
    *ppQueue = NULL;
    return e;
}

hisdkRet_t hirtCmdQueueDestroy(hirtCmdQueue_t *pQueue)
{
    hirtFifoDestroy(pQueue->pfifo);

    HISDK_LOG_INFO(LOG_DEBUG, "hirtCmdQueueDestroy done.");
    hisdkFree(pQueue);
}

hisdkRet_t hirtCmdQueueSyncPut(hirtCmdQueue_t *pQueue, sem_t *pSem)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtCmdNode_t node;
    
    node.type = CMDTYPE_SYNC;
    node.sem_cmdsync = pSem;
    
    HISDK_ERR_FCALLFAIL( hirtFifoPut(pQueue->pfifo, &node) );

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueSyncPut done.");
    return e;
    
fail:
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueSyncPut failed.");
    return e;
}

hisdkRet_t hirtCmdQueueKernelPut(hirtCmdQueue_t *pQueue, hirtKernelParamsBuffer_t *pParams, 
    hirtKernelBinBuffer_t *pKernelBin, hirtTaskDim_t taskDim)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    hirtCmdNode_t node;
    
    node.type = CMDTYPE_KERNEL;
    node.buf_kernel = pKernelBin;
    node.buf_param = pParams;
    node.dim = taskDim;

    e = hirtFifoPut(pQueue->pfifo, &node);
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FIFOFULL;
        goto fail;
    }

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueKernelPut done.");
    return ret;
    
fail:
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueKernelPut failed.");
    return ret;
}

hisdkRet_t hirtCmdQueueNodeGet(hirtCmdQueue_t *pQueue, hirtCmdNode_t *pNode)
{
    hirtFifoGet(pQueue->pfifo, (void *)pNode);
}

hisdkRet_t hirtCmdQueueSync(hirtCmdQueue_t *pQueue)
{
    sem_t sem_sync;

    sem_init(&sem_sync, 0, 0);

    hirtCmdQueueSyncPut(pQueue, &sem_sync);

    sem_wait(&sem_sync);

    return HISDK_RET_SUCCESS;
}