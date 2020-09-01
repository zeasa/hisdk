#include "hirt_fifo.h"


hisdkRet_t hirtFifoCreate(hirtFifo_t **ppFifo, int item_num, int item_siz)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hirtFifo_t *pFifo;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtFifoCreate : item_num=%d,item_siz=%d", item_num, item_siz);
    
    pFifo = (hirtFifo_t *)hisdkAlloc(sizeof(hirtFifo_t));
    pFifo->item_num = item_num;
    pFifo->item_siz = item_siz;
    pFifo->pWrite = 0;
    pFifo->pRead = 0;
    pFifo->is_full = 0;
    pFifo->buf = (void *)hisdkAlloc(item_num * item_siz);
    sem_init(&pFifo->sem_read, 0, 0);

    *ppFifo = pFifo;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtFifoCreate done.");
    return ret;
}

hisdkRet_t hirtFifoDestroy(hirtFifo_t* fifo)
{
    sem_destroy(&fifo->sem_read);
    hisdkFree(fifo->buf);
    hisdkFree(fifo);
    
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtFifoPut(hirtFifo_t* fifo, void* pdata)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    
    if(fifo->is_full)
    {
        ret =  HISDK_RET_ERR_FIFOFULL;
        goto fail;
    }

    memcpy(((char *)(fifo->buf) + fifo->pWrite * fifo->item_siz), pdata, fifo->item_siz);
    fifo->pWrite++;
    if(fifo->pWrite >= fifo->item_num)
    {
        fifo->pWrite = 0;
    }
    if(fifo->pWrite == fifo->pRead)
    {
        fifo->is_full = 1;
    }

    return ret;
    
fail:
    return ret;
}

hisdkRet_t hirtFifoGet(hirtFifo_t* fifo, void* pbuf)
{
    sem_wait(&fifo->sem_read);
    
    if( (fifo->pWrite == fifo->pRead) && (!fifo->is_full) )
    {
        return HISDK_RET_ERR_FIFOEMPTY;
    }

    memcpy(pbuf, ((char *)(fifo->buf) + fifo->pRead * fifo->item_siz), fifo->item_siz);
    fifo->pRead++;
    if(fifo->pRead >= fifo->item_num)
    {
        fifo->pRead = 0;
    }
    fifo->is_full = 0;

    return HISDK_RET_SUCCESS;
}

int hirtFifoIsFull(hirtFifo_t* fifo)
{
    return fifo->is_full;
}
