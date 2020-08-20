&#include "libhirt.h"
#include "libhirt_scheduler.h"
#include "libhirt_cqueue.h"

hisdkRet_t libhirt_scheduler_create(hirtScheduler_t **ppScheduler)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hirtScheduler_t *pScheduler;

    pScheduler = (hirtScheduler_t*)malloc(sizeof(hirtScheduler_t));
    *ppScheduler = pScheduler;

    sem_init(&pScheduler->m_semEvent, 0, 0);

    if(pthread_create(&pScheduler.m_thread, NULL, libhirt_scheduler_thread, (void*)pScheduler) != 0)
    {
        ret = HISDK_RET_ERR_CREATETHREAD;
        goto err;
    }

err:
    sem_destroy(&pScheduler->m_semEvent);
    free(pScheduler);
    return ret;
}

hisdkRet_t libhirt_scheduler_destroy(hirtScheduler_t *pScheduler)
{
    sem_destroy(&pScheduler->m_semEvent);
    free(pScheduler);

    return HISDK_RET_SUCCESS;
}

void* libhirt_scheduler_thread(void* arg)
{
    hirtScheduler_t *pScheduler = (hirtScheduler_t *)arg;
    hirtCmdQueue_t *pQueue = pScheduler->m_pQueue;
    hirtScoreboard_t *pScoreboard = pScheduler->m_pScoreboard;
    hirtCmdNode_t node;
    hirtTaskDim_t dim;
    int schedTbl[HIRT_HIPU200_CORENUMMAX];
    
    while(true)
    {
        //block read the cmd queue
        libhirt_cmdqueue_get(pQueue, &node);

        //depend on the cmdnode type
        switch (node.type)
        {
        case CMDTYPE_KERNEL:
            dim = node.dim;
            //read kernel info<taskDim> read hipucore score board
            while(1)
            {
                //TODO:deal with interlock
                pthread_mutex_lock(&pScoreboard->mutex);
                int freecnt = 0;
                for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
                {
                    if(pScoreboard->m_coreinfo[i].m_status == HIPUCORE_FREE)
                    {
                        schedTbl[freecnt] = i;
                        freecnt++;
                    }
                }
                pthread_mutex_unlock(&pScoreboard->mutex);

                if(freecnt>=dim)
                {
                    break;
                }
                else
                {
                    sem_wait(&pScheduler->m_semEvent);
                }
            }

            //update scoreboard
            pthread_mutex_lock(&pScoreboard->mutex);
            int freecnt = 0;
            for(int i=0; i<dim; i++)
            {
                pScoreboard->m_coreinfo[schedTbl[i]].m_status == HIPUCORE_FREE;
            }
            pthread_mutex_unlock(&pScoreboard->mutex);

            //read kernel and download the kernel into gdram
            hirtMemcpy(node.buf_kernel.pbuf_dev, node.buf_kernel.pbuf_host, node.buf_kernel.size);
            //copy kernel param from host to device;
            hirtMemcpy(node.buf_param.pbuf_dev, node.buf_param.pbuf_host, node.buf_param.max_param);

            //config the hipu csr reg <set mmap, set pc>

            //config the hipu csr to make hipucore run

            break;
        case CMDTYPE_SYNC:
            //wait for all hipucores to become idle status
            while(1)
            {
                //TODO:deal with interlock
                pthread_mutex_lock(&pScoreboard->mutex);
                int busycnt = 0;
                for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
                {
                    busycnt += (pScoreboard->m_coreinfo[i].m_status != HIPUCORE_FREE) ? 1 : 0;
                }
                pthread_mutex_unlock(&pScoreboard->mutex);

                if(busycnt==0)
                {
                    break;
                }
                else
                {
                    sem_wait(&pScheduler->m_semEvent);
                }
            }
            
            break;
        case CMDTYPE_NOTIFIER:
            break;
        default:
            break;
        }
    }

    return NULL;
}

hisdkRet_t libhirt_scoreboard_create(hirtScoreboard_t **ppScoreboard)
{
    hirtScoreboard_t *pScoreboard;
    pScoreboard = (hirtScoreboard_t*)malloc(sizeof(hirtScoreboard_t));
    *ppScoreboard = pScoreboard;

    for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
    {
        pScoreboard->m_coreinfo[i].m_status = HIPUCORE_FREE;
    }
    
    pthread_mutex_init(&pScoreboard->mutex, NULL);

    return HISDK_RET_SUCCESS;
}

hisdkRet_t libhirt_scoreboard_destroy(hirtScoreboard_t *pScoreboard)
{
    pthread_mutex_destroy(&pScoreboard->mutex);

    free(pScoreboard);

    return HISDK_RET_SUCCESS;
}
