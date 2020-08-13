#include <semaphore.h>
#include "libhirt_fifo.h"
#include "libhirt_cqueue.h"


hirtRet_t libhirt_cmdqueue_create(hirtCmdQueue_t *pqueue)
{
    pqueue = (hirtCmdQueue_t *)malloc(sizeof(hirtCmdQueue_t));
    pqueue->pfifo = libhirt_fifo_create(HIRT_CMDQUEUE_SIZMAX, sizeof(hirtCmdNode_t));
}

hirtRet_t libhirt_cmdqueue_destory(hirtCmdQueue_t *pqueue)
{
    libhirt_fifo_destroy(pqueue->pfifo);
    free(pqueue);
}

hirtRet_t libhirt_cmdqueue_sync_put(hirtCmdQueue_t *pqueue, sem_t *psem)
{
    hirtCmdNode_t node;
    node.type = CMDTYPE_SYNC;
    node.sem_cmdsync = psem;
    libhirt_fifo_put(pqueue->pfifo, &node);

    return HIRT_RET_SUCCESS;
}

hirtRet_t libhirt_cmdqueue_kernel_put(hirtCmdQueue_t *pQueue, hirtKernelParamsBuffer_t *pParams, 
    hirtKernelBinBuffer_t *pKernelBin, hirtTaskDim_t taskDim)
{
    hirtCmdNode_t node;
    node.type = CMDTYPE_KERNEL;
    node.buf_kernel = pKernelBin;
    node.buf_param = pParams;
    node.dim = taskDim;

    libhirt_fifo_put(pQueue->pfifo, &node);

    return HIRT_RET_SUCCESS;
}

hirtRet_t libhirt_cmdqueue_get(hirtCmdQueue_t *pQueue, hirtCmdNode_t *pNode)
{
    libhirt_fifo_get(pQueue->pfifo, (void *)pNode);
}

hirtRet_t hirtSyncQueue(hirtCmdQueue *pqueue)
{
    sem_t sem_sync;

    sem_init(&sem_sync, 0, 0);

    libhirt_cmdqueue_sync_put(pqueue, &sem_sync);

    sem_wait(&sem_sync);

    return HIRT_RET_SUCCESS;
}