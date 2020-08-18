#include "hikl.h"
#include "hihw.h"
#include "hikl_param.h"
#include "libhikl_stdlib.h"

void _kernel_type_0(uint32 ifm, uint32 wt, uint32 ofm) {
    register uint32 a asm("t1") = ifm;
    register uint32 b asm("t2") = wt;
    register uint32 c asm("t3") = ofm;
    _clear_vreg(vr1);
    mmac(VPR_NONE, vr1, t1, t2, vr1);
    // can follow other vector operations ...
    vsw(vr1, t3, 0);
    return;
}

void _kernel_type_1(uint32 ifm, uint32 wt, uint32 ofm){return; }
void _kernel_type_2(uint32 ifm, uint32 wt, uint32 ofm){return; }
void _kernel_type_3(uint32 ifm, uint32 wt, uint32 ofm){return; }

int libhikl_op_conv1(void* pParam){ return 0;   }
int libhikl_op_conv2(void* pParam){ return 0;   }
int libhikl_op_conv3(void* pParam){ return 0;   }

int libhikl_op_conv_2d(hikl_addr_t *ifm_addr, hikl_addr_t *wt_addr, hikl_addr_t *ofm_addr, conv_shape_t *cshape, pad_shape_t *pshape)
{
    uint32 ifm_row_size, ofm_row_size, ifm_row_stride, ofm_row_stride = 0;
    uint32 chn_brick_num, blk_mtx_num, blk_stride_num = 0;
    uint32 w_iter_num, h_iter_num, c_iter_num = 0;
    uint32 wt_offset, wt_start, ifm_start, pad_start, pad_end, ofm_start = 0;
    uint32 ifm_reserve_size, wt_reserve_size, ofm_reserve_start = 0;
    uint32 ifm_dma_row_offset = 0;
    uint32 ndma_poll = 0;

    ifm_row_size = cshape->ifm_w * cshape->ifm_c;               // in Bytes
    ofm_row_size = cshape->ifm_w * cshape->ofm_c;               // in Bytes
    ifm_reserve_size = ifm_row_size*(cshape->k_h+1);            // in Bytes, Reserve MMA for (k_h+1) complete ifm rows
    ofm_reserve_start = MMA_BEGIN + ifm_reserve_size;
    LIBHIKL_ASSERT((ifm_reserve_size + ofm_row_size) <= MMA_BYTE_SIZE);// check memory overflow of MMA

    // Load top ifm rows that are required for 1st iteration
    ifm_dma_row_offset += cshape->k_h - pshape->top;
    ndma_cfg(ifm_addr->x_pos, ifm_addr->y_pos, ifm_addr->lcaddr, MMA_BEGIN, ifm_row_size*(cshape->k_h - pshape->top));
    ndma_start(DMA_RX_CMD);
    ndma_over_poll(ndma_poll);

    // Load the entire weight
    wt_reserve_size = cshape->k_w * cshape->k_h * cshape->ifm_c * cshape->ofm_c; // in Bytes
    LIBHIKL_ASSERT(wt_reserve_size <= MMB_BYTE_SIZE); // check memory overflow of MMB
    ndma_cfg(wt_addr->x_pos, wt_addr->y_pos, wt_addr->lcaddr, MMB_BEGIN, wt_reserve_size);
    ndma_start(DMA_RX_CMD);

    // Set ifm region size as the ifm_reserve range
    _set_mmac_region_start(BytetoMM(MMA_BEGIN));
    _set_mmac_region_end(BytetoMM(MMA_BEGIN + ifm_reserve_size) - 1);

    // As rows are tightly laid, stirde equals to row length
    ifm_row_stride = BytetoMM(ifm_row_size);
    ofm_row_stride = BytetoMM(ofm_row_size);
    _set_mmac_fm_blk_stride(ifm_row_stride);

    // the amount of 8-length bricks along ifm channel
    chn_brick_num = cshape->ifm_c/MTX_SCALE;    
    
    // Calculate the amount of adjacent ifm matrix in one row that need to compute
    blk_mtx_num = chn_brick_num * cshape->k_w;  // k_w adjacent pixels in a row
    _set_mmac_fm_blk_size(blk_mtx_num);

    h_iter_num = cshape->ifm_h + pshape->top + pshape->bottom - cshape->k_h + 1;
    c_iter_num = cshape->ofm_c / MTX_SCALE;
    w_iter_num = cshape->ifm_w / MTX_SCALE;

    ndma_over_poll(ndma_poll);   // wait for weight load over

    // Iterate for each ofm row
    for(int i=0; i<h_iter_num; i++){

        // Pre-load the next ifm row if not we are at bottom
        if((i + cshape->k_h) < (cshape->ifm_h + pshape->top))
        {
            ndma_cfg(ifm_addr->x_pos, ifm_addr->y_pos, ifm_addr->lcaddr + ifm_row_size * ifm_dma_row_offset, 
                        MMA_BEGIN + ((ifm_dma_row_offset%(cshape->k_h+1)) * ifm_row_size), ifm_row_size);
            ndma_start(DMA_RX_CMD);
        }
        
        // Calculate the number and range of ifm/wt rows
        if(i < pshape->top)
        {
            pad_start = BytetoMM(MMA_BEGIN);  // always start from the first ifm row, pad automatically increments inside mmac
            pad_end = pad_start + ifm_row_stride - 1;       // always the end of the first ifm row
            blk_stride_num = i + cshape->k_h - pshape->top; // always less than the full kernel height
            wt_offset = blk_mtx_num * (pshape->top - i);   // skip top rows of kernel
        }
        else
        { 
            pad_start = BytetoMM(MMA_BEGIN) + (i - pshape->top) * ifm_row_stride; // from the middle of ifm
            pad_end = pad_start + ifm_row_stride - 1;
            wt_offset = 0; // always start from kernel top

            if((i + cshape->k_h) > (cshape->ifm_h + pshape->top))   // at bottom
                blk_stride_num = cshape->ifm_h + pshape->top - i;   // always less than the full kernel height

            else // at middle
                blk_stride_num = cshape->k_h;   // always equal the full kernel height
        }

        _set_mmac_fm_cls_start(pad_start);
        _set_mmac_fm_cls_end(pad_end);
        _set_mmac_fm_blk_num(blk_stride_num);

        // iterate for each 8-kernel group
        for(int k=0; k<c_iter_num; k++){
            
            wt_start = BytetoMM(MMB_BEGIN) + k * cshape->k_h * blk_mtx_num + wt_offset;

            for(int j=0; j<w_iter_num; j++)
            {
                ifm_start = pad_start + (j - pshape->side) * chn_brick_num;
                ofm_start = BytetoMM(ofm_reserve_start) + i*ofm_row_stride + j*c_iter_num + k;
                _kernel_type_0(ifm_start, wt_start, ofm_start);
            }
        }

        // End of one ofm row, wait for next ifm row
        if((i + cshape->k_h) < (cshape->ifm_h + pshape->top)){
            ndma_over_poll(ndma_poll);
            ifm_dma_row_offset++;
        }
    }

    // Write back entire ofm
    ndma_cfg(ofm_addr->x_pos, ofm_addr->y_pos, ofm_addr->lcaddr, ofm_reserve_start, h_iter_num * ofm_row_size);
    ndma_start(DMA_RX_CMD);
    ndma_over_poll(ndma_poll);

    return 0;
}