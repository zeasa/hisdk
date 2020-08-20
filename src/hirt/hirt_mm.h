#ifndef LIBHIRT_MM_H__
#define LIBHIRT_MM_H__

typedef enum hirtMemBlockStat
{
    MB_FREE = 0,
    MB_USED = 1
} hirtMemBlockStat_t;

typedef struct hirtMemBlockInfo
{
    hirtMemBlockStat_t m_mbstatus;
    hirtGMemAddress_t  m_mbbase;
} hirtMemBlockInfo_t;

typedef struct hirtMemAllocRecord
{
    hirtGMemAddress_t m_mbbase;
    int m_blocknum;
    int m_ch;
    int m_beginBlk;
} hirtMemAllocRecord_t;

typedef struct hirtMemManager
{
    hirtMemBlockInfo_t m_blocktbl[HIRT_HIPU200_MEM_CH_NUM][HIRT_HIPU200_MEM_BLK_NUM];
    hirtMemAllocRecord_t m_recordtbl[HIRT_HIPU200_MEM_CH_NUM*HIRT_HIPU200_MEM_BLK_NUM];
} hirtMemManager_t;

hisdkRet_t libhirt_memmamager_create(hirtMemManager_t **ppMemManager);
hisdkRet_t libhirt_memmamager_destroy(hirtMemManager_t *pMemManager);
hisdkRet_t libhirt_memmamager_block_get(hirtMemManager_t *pMemManager, hirtGMemAddress_t *pAddr, int block_num);
hisdkRet_t libhirt_memmanager_block_put(hirtMemManager_t *pMemManager, hirtGMemAddress_t addr);

#endif /*LIBHIRT_FIFO_H__*/

