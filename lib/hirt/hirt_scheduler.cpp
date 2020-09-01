#include "hirt_scheduler.h"
#include "hirt_mm.h"

hisdkRet_t hirtSchedulerCreate(hirtScheduler_t **ppScheduler, hirtCmdQueue_t * const pCmdQueue)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    hirtScheduler_t *pScheduler = NULL;
    hirtScoreboard_t *pScoreboard = NULL;

    if(pCmdQueue == NULL)
    {
        ret = HISDK_RET_ERR_BADPARAMETER;
        goto fail;
    }

    pScheduler = (hirtScheduler_t*)hisdkAlloc(sizeof(hirtScheduler_t));
    if(pScheduler == NULL)
    {
        ret = HISDK_RET_ERR_NOMEM;
        goto fail;
    }
    hisdkMemset(pScheduler, 0 ,sizeof(hirtScheduler_t));
    pScheduler->m_pQueue = pCmdQueue;
    
    e = hirtScoreboardCreate(&pScoreboard);
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FUNC_CALL;
        goto fail;
    }
    pScheduler->m_pScoreboard = pScoreboard;

    if(sem_init(&pScheduler->m_semEvent, 0, 0) != 0)
    {
        ret = HISDK_RET_ERR_NORES;
        goto fail;
    }

    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "pthread_create[hirtSchedulerThread]");
    if(pthread_create(&pScheduler->m_thread, NULL, hirtSchedulerThread, (void*)pScheduler) != 0)
    {
        ret = HISDK_RET_ERR_CREATETHREAD;
        HISDK_LOG_INFO(LOG_SYSTEM, "%s", "pthread_create failed.");
        goto fail;
    }

    pScheduler->m_threadfunc = (hirtThreadFunction_t)hirtSchedulerThread;
    *ppScheduler = pScheduler;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtSchedulerCreate successfully.");
    return ret;
    
fail:
    sem_destroy(&pScheduler->m_semEvent);
    
    if(pScoreboard != NULL)
    {
        hirtScoreboardDestroy(pScoreboard);
    }

    if(pScheduler != NULL)
    {
        hisdkFree(pScheduler);
    }
    *ppScheduler = NULL;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtSchedulerCreate failed.");
    return ret;
}

hisdkRet_t hirtSchedulerDestroy(hirtScheduler_t *pScheduler)
{
    pthread_join(pScheduler->m_thread, 0);
    
    sem_destroy(&pScheduler->m_semEvent);
    
    if(pScheduler->m_pScoreboard != NULL)
    {
        hirtScoreboardDestroy(pScheduler->m_pScoreboard);
    }

    if(pScheduler != NULL)
    {
        hisdkFree(pScheduler);
    }
    
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtSchedulerDestroy done.");
    return HISDK_RET_SUCCESS;
}

void* hirtSchedulerThread(void* arg)
{
    hirtScheduler_t *pScheduler = (hirtScheduler_t *)arg;
    hirtCmdQueue_t *pQueue = pScheduler->m_pQueue;
    hirtScoreboard_t *pScoreboard = pScheduler->m_pScoreboard;
    hirtCmdNode_t node;
    hirtTaskDim_t dim;
    int schedTbl[HIRT_HIPU200_CORENUMMAX];
    int freecnt = 0;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtSchedulerThread thread entered arg=%p", arg);
    
    while(1)
    {
        //block read the cmd queue
        hirtCmdQueueNodeGet(pQueue, &node);
        
        //depend on the cmdnode type
        switch (node.type)
        {
        case CMDTYPE_KERNEL:
            dim = node.dim;
            //read kernel info<taskDim> read hipucore score board
            while(1)
            {
                //TODO:deal with interlock
                pthread_mutex_lock(&pScoreboard->m_mutex);
                int freecnt = 0;
                for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
                {
                    if(pScoreboard->m_coreinfo[i].m_status == HIPUCORE_FREE)
                    {
                        schedTbl[freecnt] = i;
                        freecnt++;
                    }
                }
                pthread_mutex_unlock(&pScoreboard->m_mutex);

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
            pthread_mutex_lock(&pScoreboard->m_mutex);
            for(int i=0; i<dim; i++)
            {
                pScoreboard->m_coreinfo[schedTbl[i]].m_status == HIPUCORE_FREE;
            }
            pthread_mutex_unlock(&pScoreboard->m_mutex);

            //read kernel and download the kernel into gdram
            hirtMemcpy((void*)&node.buf_kernel->pbuf_gpu, node.buf_kernel->pbuf_host, node.buf_kernel->size, HIRT_MEM_TRANS_DIR_HOST2GPU);
            //copy kernel param from host to device;
            hirtMemcpy((void*)&node.buf_param->pbuf_gpu, node.buf_param->pbuf_host, node.buf_param->max_param, HIRT_MEM_TRANS_DIR_HOST2GPU);

            //config the hipu csr reg <set mmap, set pc>

            //config the hipu csr to make hipucore run

            break;
        case CMDTYPE_SYNC:
            //wait for all hipucores to become idle status
            while(1)
            {
                //TODO:deal with interlock
                pthread_mutex_lock(&pScoreboard->m_mutex);
                int busycnt = 0;
                for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
                {
                    busycnt += (pScoreboard->m_coreinfo[i].m_status != HIPUCORE_FREE) ? 1 : 0;
                }
                pthread_mutex_unlock(&pScoreboard->m_mutex);

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

hisdkRet_t hirtScoreboardCreate(hirtScoreboard_t **ppScoreboard)
{
    hirtScoreboard_t *pScoreboard;
    pScoreboard = (hirtScoreboard_t*)hisdkAlloc(sizeof(hirtScoreboard_t));
    *ppScoreboard = pScoreboard;

    for(int i=0; i<HIRT_HIPU200_CORENUMMAX; i++)
    {
        pScoreboard->m_coreinfo[i].m_status = HIPUCORE_FREE;
    }
    
    pthread_mutex_init(&pScoreboard->m_mutex, NULL);

    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtScoreboardDestroy(hirtScoreboard_t *pScoreboard)
{
    pthread_mutex_destroy(&pScoreboard->m_mutex);

    hisdkFree(pScoreboard);

    return HISDK_RET_SUCCESS;
}
