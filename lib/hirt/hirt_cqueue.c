#include <stdlib.h>
#include <semaphore.h>
#include "hirt_fifo.h"
#include "hirt_cqueue.h"


hisdkRet_t hirtCmdQueueCreate(hirtCmdQueue_t **ppQueue)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    hirtCmdQueue_t *pQueue;
    
    pQueue = (hirtCmdQueue_t *)malloc(sizeof(hirtCmdQueue_t));
    memset(pQueue, 0, sizeof(hirtCmdQueue_t));
    
    e = hirtFifoCreate(&pQueue->pfifo, HIRT_CMDQUEUE_SIZMAX, sizeof(hirtCmdNode_t));

    *ppQueue = pQueue;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtCmdQueueCreate done.");
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
    hirtCmdNode_t node;
    node.type = CMDTYPE_SYNC;
    node.sem_cmdsync = pSem;
    hirtFifoPut(pQueue->pfifo, &node);

    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtCmdQueueKernelPut(hirtCmdQueue_t *pQueue, hirtKernelParamsBuffer_t *pParams, 
    hirtKernelBinBuffer_t *pKernelBin, hirtTaskDim_t taskDim)
{
    hirtCmdNode_t node;
    node.type = CMDTYPE_KERNEL;
    node.buf_kernel = pKernelBin;
    node.buf_param = pParams;
    node.dim = taskDim;

    hirtFifoPut(pQueue->pfifo, &node);

    return HISDK_RET_SUCCESS;
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