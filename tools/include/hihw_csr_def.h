#ifndef HIPU200CLIB_CSR_REG_DEF_H__
#define HIPU200CLIB_CSR_REG_DEF_H__

#define CSR_ADDR_NDMA_CTRL                      (0x7C0)
#define CSR_ADDR_NDMA_STATUS                    (0x7C1)
#define CSR_ADDR_NDMA_LCADDR                    (0x7C2)
#define CSR_ADDR_NDMA_RTADDR                    (0x7C3)
#define CSR_ADDR_NDMA_SIZE                      (0x7C4)
#define CSR_ADDR_NDMA_DESTX                     (0x7C5)
#define CSR_ADDR_NDMA_DESTY                     (0x7C6)

#define CSR_ROUND_TYPE                          (0xBC0)
#define CSR_FADD_SHIFT_NUM                      (0xBC1)
#define CSR_FADD_PROT_HIGH                      (0xBC2)
#define CSR_FADD_PROT_LOW                       (0xBC3)
#define CSR_FSUB_SHIFT_NUM                      (0xBC4)
#define CSR_FSUB_PROT_HIGH                      (0xBC5)
#define CSR_FSUB_PROT_LOW                       (0xBC6)
#define CSR_FMUL_SHIFT_NUM                      (0xBC7)
#define CSR_FMUL_PROT_HIGH                      (0xBC8)
#define CSR_FMUL_PROT_LOW                       (0xBC9)

#define CSR_MTX_CLUSTER_START                   (0xBD0)
#define CSR_MTX_CLUSTER_END                     (0xBD1)
#define CSR_MTX_REGION_START                    (0xBD2)
#define CSR_MTX_REGION_END                      (0xBD3)
#define CSR_MTX_BLK_SIZE                        (0xBD4)
#define CSR_MTX_BLK_NUM                         (0xBD5)
#define CSR_MTX_CLS_NUM                         (0xBD6)
#define CSR_MTXRW_BLK_STRIDE                    (0xBD7)
#define CSR_MTXRW_CLS_STRIDE                    (0xBD8)
#define CSR_MTXRO_BLK_STRIDE                    (0xBD9)
#define CSR_MTXRO_CLS_STRIDE                    (0xBDA)
#define CSR_MTX_PAD_TYPE                        (0xBDB)

#define CSR_WR_LUT_FIRST                        (0xBE0)
#define CSR_WR_LUT_INC                          (0xBE1)

#endif /*HIPU200CLIB_CSR_REG_DEF_H__*/
