#include "libhirt.h"

/*********************************************************************************
 * Memory management
 *********************************************************************************/
/**
 * @brief Allocate nByte bytes and place a pointer to pointer
 *        in pPtr to the allocated host memory. If nBytes is 0, then
 *        hirtMallocHost returns either NULL, or a unique pointer value
 *        that can later be passed to hirtFreeHost.
 *
 * @param pPtr[out]  a pointer to pointer for retrieving allocated host memory.
 * @param nBytes[in] number bytes of memory to be allocated.
 * @param type[in]   memory type to be allocated,
 *                   @see hirt_HOST_MEMORY_TYPE_LOCK and hirt_HOST_MEMORY_TYPE_MAPPED.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtMallocHost(void **pPtr, size_t nBytes, hirtMemType_t type)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Free the memory space pointed by ptr, which must be
 *        returned by a previous call of hirtMallocHost.
 *
 * @param ptr[in]  point to the address of memory to be free.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtFreeHost(void *ptr)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Allocate memory on MLU device.
 *
 * @param pPtr[out] a pointer to pointer for retrieving allocated device memory.
 * @param nBytes[in] allocate size.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtMalloc(void **pPtr, size_t nBytes)
{
    return HIRT_RET_SUCCESS; 
}

/**
 * @brief Deallocate MLU device Memory.
 *
 * @param ptr[in] point to the memory to be free.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtFree(void *ptr)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Copy data from src address to dst address. The copy direction
 *        is specified by input parameter dir. The copy operation is
 *        always performed on current device which is set by hirtSetCurrentDevice.
 *
 * @param dest[in] destination address.
 * @param src[in] source address.
 * @param nBytes[in] number of bytes to be copied.
 * @param dir[in] direction of transfer.
 *                @see  hirt_MEM_TRANS_DIR_HOST2DEV,
 *                      hirt_MEM_TRANS_DIR_DEV2DEV,
 *                      hirt_MEM_TRANS_DIR_DEV2HOST,
 *                      hirt_MEM_TRANS_DIR_HOST2HOST,
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtMemcpy(void *dest, const void *src, size_t nBytes, hirtMemTransDir_t dir)
{
    return HIRT_RET_SUCCESS;
}


hirtRet_t libhirt_memmamager_create(hirtMemManager_t **ppMemManager)
{
    hirtRet_t ret = HIRT_RET_SUCCESS;
    hirtMemManager_t *pMemManager;

    pMemManager = (hirtMemManager_t*)malloc(sizeof(hirtMemManager_t));
    *ppMemManager = pMemManager;

    for(int i=0; i<HIRT_HIPU200_MEM_CH_NUM; i++)
    {
        for(int j=0; j<HIRT_HIPU200_MEM_BLK_NUM; j++)
        {
            pMemManager->m_blocktbl[i][j].m_mbstatus = MB_FREE;
            pMemManager->m_blocktbl[i][j].m_mbbase = 
                MAKE_DEVADDRESS(HIRT_HIPU200_MEM_CH_NOCADDR_TBL[i], HIRT_HIPU200_MEM_BLK_SIZ);
            pMemManager->m_recordtbl[i*HIRT_HIPU200_MEM_BLK_NUM+j].m_blocknum = 0;
        }
    }

    return ret;
}

hirtRet_t libhirt_memmamager_destroy(hirtMemManager_t *pMemManager)
{
    free(pMemManager);
    return HIRT_RET_SUCCESS;
}

hirtRet_t libhirt_memmamager_block_get(hirtMemManager_t *pMemManager, hirtGMemAddress_t *pAddr, int block_num)
{
    hirtRet_t ret = HIRT_RET_SUCCESS;
    int i;

    for(i=0;i<HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_BLK_NUM; i++)
    {
        if(pMemManager->m_recordtbl[i].m_blocknum == 0)
        {
            break;
        }
    }
    if(i == HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_BLK_NUM)
    {
        ret = HIRT_RET_ERR_NORES;
        goto end;
    }

    for(int j=0; j<HIRT_HIPU200_MEM_CH_NUM; j++)
    {
        for(int k=0; k<HIRT_HIPU200_MEM_BLK_NUM; k++)
        {
            int cnt=1;
            if(pMemManager->m_blocktbl[j][k].m_mbstatus == MB_FREE)
            {
                for(int m=1;m<block_num;m++)
                {
                    if(pMemManager->m_blocktbl[j][k+m].m_mbstatus == MB_FREE)
                    {
                        cnt++;
                    }
                }
                if(cnt == block_num)
                {
                    //found
                    for(int n=0;n<block_num;n++)
                    {
                        pMemManager->m_blocktbl[j][k].m_mbstatus = MB_USED;
                    }
                }
                pMemManager->m_recordtbl[i].m_ch = j;
                pMemManager->m_recordtbl[i].m_blocknum = block_num;
                pMemManager->m_recordtbl[i].m_beginBlk = k;
                pMemManager->m_recordtbl[i].m_mbbase = MAKE_DEVADDR_BLK(HIRT_HIPU200_MEM_CH_NOCADDR_TBL[j], k);
                goto end;
            }
        }
    }

    if( (j==HIRT_HIPU200_MEM_CH_NUM) && (k==HIRT_HIPU200_MEM_BLK_NUM) )
    {
        ret = HIRT_RET_ERR_NORES;
    }

end:
    return ret;
}

hirtRet_t libhirt_memmanager_block_put(hirtMemManager_t *pMemManager, hirtGMemAddress_t addr)
{
    hirtRet_t ret = HIRT_RET_SUCCESS;
    int i;
    
    for(i=0; i<HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_BLK_NUM; i++)
    {
        if( (pMemManager->m_recordtbl[i].m_mbbase == addr) &&
            (pMemManager->m_recordtbl[i].m_blocknum > 0) )
        {
            for(int j=0; j<pMemManager->m_recordtbl[i].m_blocknum; j++)
            {
                pMemManager->m_blocktbl[pMemManager->m_recordtbl[i].m_ch][pMemManager->m_recordtbl[i].m_beginBlk+j] = MB_FREE;
            }
            pMemManager->m_recordtbl[i].m_blocknum = 0;
            
            break;
        }
    }

    if(i == HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_BLK_NUM)
    {
        ret = HIRT_RET_ERR_NO_EXIST;
    }

    return ret;
}
