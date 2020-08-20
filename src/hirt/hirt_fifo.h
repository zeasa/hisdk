#ifndef LIBHIRT_FIFO_H__
#define LIBHIRT_FIFO_H__

#include <queue>
#include <pthread>
#include <semaphore.h>
#include "libhirt.h"

typedef struct _libhirt_fifo
{
    int   pWrite;
    int   pRead;
    int   item_num;
    int   item_siz;
    int   is_full;
    void  *buf;
    sem_t sem_read;
    //  sem_t sem_write;
} libhirt_fifo_t;

libhirt_fifo_t *libhirt_fifo_create(int item_num, int item_siz);
hisdkRet_t libhirt_fifo_destroy(libhirt_fifo_t *fifo);
hisdkRet_t libhirt_fifo_put(libhirt_fifo_t *fifo, void *pdata);
hisdkRet_t libhirt_fifo_get(libhirt_fifo_t *fifo, void *pbuf);
int libhirt_fifo_isfull(libhirt_fifo_t *fifo);

#endif /*LIBHIRT_FIFO_H__*/

