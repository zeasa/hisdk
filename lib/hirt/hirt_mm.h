#ifndef LIBHIRT_MM_H__
#define LIBHIRT_MM_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum
{
    MB_FREE = 0,
    MB_USED = 1
} hirtMemBlockStat_t;

typedef struct
{
    hirtMemBlockStat_t m_mbstatus;
    hirtGMemAddress_t  m_mbbase;
} hirtMemBlockInfo_t;

typedef struct
{
    hirtGMemAddress_t m_mbbase;
    int m_blocknum;
    int m_ch;
    int m_beginBlk;
} hirtMemAllocRecord_t;

typedef struct
{
    hirtMemBlockInfo_t m_blocktbl[HIRT_HIPU200_MEM_CH_NUM][HIRT_HIPU200_MEM_BLK_NUM];
    hirtMemAllocRecord_t m_recordtbl[HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_BLK_NUM];
} hirtMemManager_t;

hisdkRet_t hirtMemManagerCreate(hirtMemManager_t **ppMemManager);
hisdkRet_t hirtMemManagerDestroy(hirtMemManager_t *pMemManager);
hisdkRet_t hirtMemManagerBlkGet(hirtMemManager_t *pMemManager, hirtGMemAddress_t *pAddr, int block_num);
hisdkRet_t hirtMemManagerBlkPut(hirtMemManager_t *pMemManager, hirtGMemAddress_t addr);

__R_HOST
hisdkRet_t hirtHostMalloc(void **pPtr, size_t nBytes/*, hirtMemType_t type*/);
__R_HOST
hisdkRet_t hirtHostFree(void *ptr);
__R_HOST
hisdkRet_t hirtDevMalloc(void **pPtr, size_t nBytes);
__R_HOST
hisdkRet_t hirtDevFree(void *ptr);
__R_HOST
hisdkRet_t hirtMemcpy(void *dest, const void *src, size_t nBytes, hirtMemTransDir_t dir);

#ifdef __cplusplus
}
#endif
#endif /*LIBHIRT_FIFO_H__*/

