#include "libhirt_fifo.h"

libhirt_fifo_t* libhirt_fifo_create(int item_num, int item_siz)
{
    libhirt_fifo_t *fifo = (libhirt_fifo_t *)malloc(sizeof(libhirt_fifo_t));
    fifo->item_num = item_num;
    fifo->item_siz = item_siz;
    fifo->pWrite = 0;
    fifo->pRead = 0;
    fifo->is_full = 0;
    fifo->buf = (void *)malloc(item_num * item_siz);
    sem_init(&fifo->sem_read, 0, 0);

    return fifo;
}

hirtRet_t libhirt_fifo_destroy(libhirt_fifo_t* fifo)
{
    sem_destroy(&fifo->sem_read);
    free(fifo->buf);
    free(fifo);
    return HIRT_RET_SUCCESS;
}

hirtRet_t libhirt_fifo_put(libhirt_fifo_t* fifo, void* pdata)
{
    if(fifo->is_full)
    {
        return HIRT_RET_ERR_FIFOFULL;
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

    return HIRT_RET_SUCCESS;
}

hirtRet_t libhirt_fifo_get(libhirt_fifo_t* fifo, void* pbuf)
{
    sem_wait(&fifo->sem_read);
    
    if( (fifo->pWrite == fifo->pRead) && (!fifo->is_full) )
    {
        return HIRT_RET_ERR_FIFOEMPTY;
    }

    memcpy(pbuf, ((char *)(fifo->buf) + fifo->pRead * fifo->item_siz), fifo->item_siz);
    fifo->pRead++;
    if(fifo->pRead >= fifo->item_num)
    {
        fifo->pRead = 0;
    }
    fifo->is_full = 0;

    return HIRT_RET_SUCCESS;
}

int libhirt_fifo_isfull(libhirt_fifo_t* fifo)
{
    return fifo->is_full;
}
