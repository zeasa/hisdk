#include "hirt_mm.h"


const int HIRT_HIPU200_MEM_CH_NOCADDR_TBL[HIRT_HIPU200_MEM_CH_NUM] = 
{
    HIRT_HIPU200_MEM_CH0_NOCADDR,
    HIRT_HIPU200_MEM_CH1_NOCADDR
};

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
hisdkRet_t hirtHostMalloc(void **pPtr, size_t nBytes/*, hirtMemType_t type*/)
{
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Free the memory space pointed by ptr, which must be
 *        returned by a previous call of hirtMallocHost.
 *
 * @param ptr[in]  point to the address of memory to be hisdkFree.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtHostFree(void *ptr)
{
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Allocate memory on MLU device.
 *
 * @param pPtr[out] a pointer to pointer for retrieving allocated device memory.
 * @param nBytes[in] allocate size.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
#define GMEM_BASE_ADDR      ()
#define GMEM_ALIGN_SIZE     (64)
#define GMEM_ALIGN_CARRY    (GMEM_ALIGN_SIZE)
#define GMEM_ALIGN_MASK     (GMEM_ALIGN_SIZE-1)
#define GMEM_ALIGN(x)       (((x & GMEM_ALIGN_MASK) == 0) ? (x) : ((x & GMEM_ALIGN_MASK) + GMEM_ALIGN_CARRY))
__R_HOST
hisdkRet_t hirtGpuMalloc(hirtGMemAddress_t *pDevAddr, size_t nBytes, hirtGMemType_t memType)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    static hirtGMemAddress_t memUsedData = 0;
    static hirtGMemAddress_t memUsedSha0 = 0;
    static hirtGMemAddress_t memUsedSha1 = 0;

    switch(memType)
    {
    case HIRT_GMEM_TYPE_DATA_BLOCK:
        *pDevAddr = HIPU200_NOC_MAKEADDR(HIPU200_NOC_NODEADDR_DDR0, 0x0) + memUsedData;
        memUsedData += GMEM_ALIGN(nBytes);
        break;
    case HIRT_GMEM_TYPE_SHARE:
        *pDevAddr = HIPU200_NOC_MAKEADDR(HIPU200_NOC_NODEADDR_DDR0, HIRT_HIPU200_MEM_SHA0_OFFSET) + memUsedSha0;
        memUsedSha0 += GMEM_ALIGN(nBytes);
        break;
    default:
        ret = HISDK_RET_ERR_BADPARAMETER;
    }

    HISDK_LOG_INFO(LOG_SYSTEM, "<GpuMalloc:size=%lu", nBytes);
    return ret;
fail:
    return ret;
}

/**
 * @brief Deallocate MLU device Memory.
 *
 * @param ptr[in] point to the memory to be hisdkFree.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGpuFree(hirtGMemAddress_t devAddr)
{
    return HISDK_RET_SUCCESS;
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
hisdkRet_t hirtMemcpy(void *dest, const void *src, size_t nBytes, hirtMemTransDir_t dir)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    
    if(nBytes == 0)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER, "hirtMemcpy size == 0.");
    }

    if(dir == HIRT_MEM_TRANS_DIR_HOST2GPU)
    {
        hidvWriteChipMem((uint64_t)dest, nBytes, const_cast<void*>(src));
    }
    else if(dir == HIRT_MEM_TRANS_DIR_GPU2HOST)
    {
        hidvReadChipMem((uint64_t)src, nBytes, dest);
    }
    else
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER, "hirtMemcpy dir err.");
    }

fail:
    return e;
}

hisdkRet_t hirtMemManagerCreate(hirtMemManager_t **ppMemManager)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hirtMemManager_t *pMemManager;

    pMemManager = (hirtMemManager_t*)hisdkAlloc(sizeof(hirtMemManager_t));
    *ppMemManager = pMemManager;

    for(int i=0; i<HIRT_HIPU200_MEM_CH_NUM; i++)
    {
        for(int j=0; j<HIRT_HIPU200_MEM_CH_BLKNUM; j++)
        {
            pMemManager->m_blocktbl[i][j].m_mbstatus = MB_FREE;
            pMemManager->m_blocktbl[i][j].m_mbbase = 
                MAKE_DEVADDR(HIRT_HIPU200_MEM_CH_NOCADDR_TBL[i], HIRT_HIPU200_MEM_BLK_SIZ);
            pMemManager->m_recordtbl[i*HIRT_HIPU200_MEM_CH_BLKNUM+j].m_blocknum = 0;
        }
    }

    return ret;
}

hisdkRet_t hirtMemManagerDestroy(hirtMemManager_t *pMemManager)
{
    hisdkFree(pMemManager);
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtMemManagerBlkGet(hirtMemManager_t *pMemManager, hirtGMemAddress_t *pAddr, int block_num)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    int i, j, k;

    for(i=0;i<HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_CH_BLKNUM; i++)
    {
        if(pMemManager->m_recordtbl[i].m_blocknum == 0)
        {
            break;
        }
    }
    if(i == HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_CH_BLKNUM)
    {
        ret = HISDK_RET_ERR_NORES;
        goto end;
    }

    for(j=0; j<HIRT_HIPU200_MEM_CH_NUM; j++)
    {
        for(k=0; k<HIRT_HIPU200_MEM_CH_BLKNUM; k++)
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

    if( (j==HIRT_HIPU200_MEM_CH_NUM) && (k==HIRT_HIPU200_MEM_CH_BLKNUM) )
    {
        ret = HISDK_RET_ERR_NORES;
    }

end:
    return ret;
}

hisdkRet_t hirtMemManagerBlkPut(hirtMemManager_t *pMemManager, hirtGMemAddress_t addr)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    int i;
    
    for(i=0; i<HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_CH_BLKNUM; i++)
    {
        if( (pMemManager->m_recordtbl[i].m_mbbase == addr) &&
            (pMemManager->m_recordtbl[i].m_blocknum > 0) )
        {
            for(int j=0; j<pMemManager->m_recordtbl[i].m_blocknum; j++)
            {
                pMemManager->m_blocktbl[pMemManager->m_recordtbl[i].m_ch][pMemManager->m_recordtbl[i].m_beginBlk+j].m_mbstatus = MB_FREE;
            }
            pMemManager->m_recordtbl[i].m_blocknum = 0;
            
            break;
        }
    }

    if(i == HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_CH_BLKNUM)
    {
        ret = HISDK_RET_ERR_NO_EXIST;
    }

    return ret;
}

