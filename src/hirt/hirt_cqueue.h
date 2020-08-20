
#ifndef LIBHIRT_CMDQUEUE_H__
#define LIBHIRT_CMDQUEUE_H__

#include <pthread>
#include "libhirt.h"

typedef enum hirtCmdType
{
    CMDTYPE_KERNEL = 0,
    CMDTYPE_SYNC,
    CMDTYPE_NOTIFIER
} hirtCmdType_t;

typedef struct hirtCmdNode
{
    hirtCmdType_t type;
    hirtTaskDim_t dim;
    
    hirtKernelParamsBuffer_t buf_param;
    hirtKernelBinBuffer_t buf_kernel;

    sem_t *sem_cmdsync;
} hirtCmdNode_t;

typedef struct hirtCmdQueue
{
    libhirt_fifo_t *pfifo;
} hirtCmdQueue_t;

hisdkRet_t libhirt_cmdqueue_create(hirtCmdQueue_t **ppQueue);
hisdkRet_t libhirt_cmdqueue_destory(hirtCmdQueue_t *pQueue);
hisdkRet_t libhirt_cmdqueue_sync_put(hirtCmdQueue_t *pQueue, sem_t *pSem);
hisdkRet_t libhirt_cmdqueue_kernel_put(hirtCmdQueue_t *pQueue, hirtKernelParamsBuffer_t *pParams, hirtKernelBinBuffer_t *pKernelBin);
hisdkRet_t libhirt_cmdqueue_get(hirtCmdQueue_t *pQueue, hirtCmdNode_t *pNode);
hisdkRet_t hirtSyncQueue(hirtCmdQueue *pQueue);


#endif /*LIBHIRT_CMDQUEUE_H__*/
