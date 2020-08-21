#include <stdlib.h>
#include <semaphore.h>
#include "hirt_fifo.h"
#include "hirt_cqueue.h"


hisdkRet_t hirtCmdQueueCreate(hirtCmdQueue_t **ppQueue)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    hirtCmdQueue_t *pQueue = NULL;
    
    pQueue = (hirtCmdQueue_t *)malloc(sizeof(hirtCmdQueue_t));
    if(pQueue == NULL)
    {
        ret = HISDK_RET_ERR_NOMEM;
        goto fail;
    }
    memset(pQueue, 0, sizeof(hirtCmdQueue_t));
    
    e = hirtFifoCreate(&pQueue->pfifo, HIRT_CMDQUEUE_SIZMAX, sizeof(hirtCmdNode_t));
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_NOMEM;
        goto fail;
    }
        
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueCreate done.");
    *ppQueue = pQueue;
    return ret;
    
fail:
    if(pQueue != NULL)
    {
        free(pQueue);    
    }
    
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueCreate failed.");
    *ppQueue = NULL;
    return ret;
}

hisdkRet_t hirtCmdQueueDestroy(hirtCmdQueue_t *pQueue)
{
    hirtFifoDestroy(pQueue->pfifo);

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueDestroy done.");
    free(pQueue);
}

hisdkRet_t hirtCmdQueueSyncPut(hirtCmdQueue_t *pQueue, sem_t *pSem)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    hirtCmdNode_t node;
    
    node.type = CMDTYPE_SYNC;
    node.sem_cmdsync = pSem;
    
    e = hirtFifoPut(pQueue->pfifo, &node);
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FIFOFULL;
        goto fail;
    }

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueSyncPut done.");
    return ret;
    
fail:
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueSyncPut failed.");
    return ret;
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