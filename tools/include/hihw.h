#ifndef HIPU200CLIB_HIHW_H__
#define HIPU200CLIB_HIHW_H__

#include "hihw_csr_def.h"
#include "hihw_ext_isa.h"
#include "hihw_reg_def.h"

#define MTX_SCALE       (8)
#define VEC_SCALE       (64)
#define MMA_BEGIN       (0)           // in bytes, 0 - 256 KB
#define MMB_BEGIN       (2 << 18)     // in bytes, 256 - 512 KB
#define MMA_BYTE_SIZE   (2 << 18)     // 256KB
#define MMB_BYTE_SIZE   (2 << 18)     // 256KB

#define MMtoByte(x)     ((x)<<6)        // Each MM word = 64 Bytes
#define BytetoMM(x)     ((x)>>6)

#define DMA_TX_CMD                      0
#define DMA_RX_CMD                      1
#define DMA_SWAP                        2
#define DMA_STATUS_MASK                 (0x00000001)

#define _set_mmac_region_start(x)      csrw(CSR_MTX_REGION_START, x)
#define _set_mmac_region_end(x)        csrw(CSR_MTX_REGION_END, x)
#define _set_mmac_fm_blk_size(x)       csrw(CSR_MTX_BLK_SIZE, x)
#define _set_mmac_fm_blk_num(x)        csrw(CSR_MTX_BLK_NUM, x)
#define _set_mmac_fm_cls_start(x)      csrw(CSR_MTX_CLUSTER_START, x)
#define _set_mmac_fm_cls_end(x)        csrw(CSR_MTX_CLUSTER_END, x)
#define _set_mmac_fm_cls_num(x)        csrw(CSR_MTX_CLS_NUM, x)
#define _set_mmac_fm_blk_stride(x)     csrw(CSR_MTXRW_BLK_STRIDE, x)
#define _set_mmac_fm_cls_stride(x)     csrw(CSR_MTXRW_CLS_STRIDE, x)
#define _set_mmac_wt_blk_stride(x)     csrw(CSR_MTXRO_BLK_STRIDE, x)
#define _set_mmac_wt_cls_stride(x)     csrw(CSR_MTXRO_CLS_STRIDE, x)

#define _clear_vreg(x)                 vsub_vv(0,x,x,x)

// 00: tx 01: rx 10:swap
#define ndma_start(x)                   csrw(CSR_ADDR_NDMA_CTRL, x)
#define ndma_over_poll(x)   do{   \
                                csrr(x, CSR_ADDR_NDMA_STATUS);  \
                            }while(((x) & DMA_STATUS_MASK) == 0); \
                            csrw(CSR_ADDR_NDMA_STATUS, 0);      \
                            x = 0;                              \

#define ndma_cfg(x, y, r, l, s)         csrw(CSR_ADDR_NDMA_RTADDR, (r));  \
                                        csrw(CSR_ADDR_NDMA_DESTX, (x));  \
                                        csrw(CSR_ADDR_NDMA_DESTY, (y));  \
                                        csrw(CSR_ADDR_NDMA_LCADDR, (l));  \
                                        csrw(CSR_ADDR_NDMA_SIZE, (s));  

#endif /*HIPU200CLIB_HIHW_H__*/