// ---------------------------------------------------------------------------------------------------------------------
// Copyright (c) 1986 - 2020, CAG team, Institute of AI and Robotics, Xi'an Jiaotong University. Proprietary and
// Confidential All Rights Reserved.
// ---------------------------------------------------------------------------------------------------------------------
// NOTICE: All information contained herein is, and remains the property of CAG team, Institute of AI and Robotics,
// Xi'an Jiaotong University. The intellectual and technical concepts contained herein are proprietary to CAG team, and
// may be covered by P.R.C. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
//
// This work may not be copied, modified, re-published, uploaded, executed, or distributed in any way, in any time, in
// any medium, whether in whole or in part, without prior written permission from CAG team, Institute of AI and
// Robotics, Xi'an Jiaotong University.
//
// The copyright notice above does not evidence any actual or intended publication or disclosure of this source code,
// which includes information that is confidential and/or proprietary, and is a trade secret, of CAG team.
// ---------------------------------------------------------------------------------------------------------------------
// FILE NAME  : main.c
// DEPARTMENT : Architecture
// AUTHOR     : wenzhe
// AUTHOR'S EMAIL : wenzhe@xjtu.edu.cn
// ---------------------------------------------------------------------------------------------------------------------
// Ver 1.0  2020--08--01 initial version.
// ---------------------------------------------------------------------------------------------------------------------
#include "../inc/hpu_def.h"
#include "../inc/hpu_api.h"


#define HPU_CORE_NUM            (13)
#define HPU_NOCNODE_DDR0        (0x03)
#define HPU_NOCNODE_DDR1        (0x0F)
#define SBUF_SIZE               (64)

#define MMAP_BLOCKSIZE          (64*1024*1024)
#define MMAP_CODE_MONO_START    (0x80000000)
#define MMAP_CODE_MONO_LEHGTH   (MMAP_BLOCKSIZE)
#define MMAP_CODE_SHAR_START    (MMAP_CODE_MONO_START+MMAP_CODE_MONO_LEHGTH)
#define MMAP_CODE_SHAR_LENGTH   (MMAP_BLOCKSIZE*3)

#define MMAP_DATA_MONO_START    (0x90000000)
#define MMAP_DATA_MONO_LEHGTH   (MMAP_BLOCKSIZE)
#define MMAP_DATA_SHAR_START    (MMAP_DATA_MONO_START+MMAP_DATA_MONO_LEHGTH)
#define MMAP_DATA_SHAR_LENGTH   (MMAP_BLOCKSIZE*3)

#define MMAP_ATOM_START         (0x02010000)
#define MMAP_ATOM_LENGTH        (64)
#define MMAP_MMRA_START         (0x02100000)
#define MMAP_MMRA_LENGTH        (256*1024)
#define KERNEL_PTABLE_ADDR      (MMAP_ATOM_START+MMAP_ATOM_LENGTH-sizeof(void*))
#define KERNEL_RTCODE_ADDR      (KERNEL_PTABLE_ADDR-sizeof(int))

#define NOC_NODE_X(x)           ((x)>>4)
#define NOC_NODE_Y(y)           ((y)&0xF)

typedef unsigned int u32_t;

typedef struct
{
    int typeVersion;
    int tableSiz;
    int parallelism;
    int parallelTable[HPU_CORE_NUM];
} paramTableBase_t;

typedef struct
{
    paramTableBase_t infoBase;

    u32_t srcNocnod_A;
    u32_t srcOffset_A;
    u32_t srcNocnod_B;
    u32_t srcOffset_B;
    u32_t dstNocnod;
    u32_t dstOffset;
    u32_t len;
} paramTableVAdd_t;

void *_kernel_malloc(int size, int type);
void _kernel_sync(int rootCoreNum, int idx);

void kernel_entry(void)
{
    unsigned char buf_sram_a[SBUF_SIZE] __attribute__((aligned(64)));
    unsigned char buf_sram_b[SBUF_SIZE] __attribute__((aligned(64)));
    unsigned char buf_sram_c[SBUF_SIZE] __attribute__((aligned(64)));
    unsigned int *_flags = (unsigned int *)MMAP_ATOM_START;
    _flags[0] = 0x70;

    int _coreid  = get_hpuid();/*get core id number*/
    int _taskid;
    int i;
    int *_rtcode = (int *)KERNEL_RTCODE_ADDR;                          /*kernel return code to host runtime*/
    paramTableVAdd_t *_ptable = *((paramTableVAdd_t **)KERNEL_PTABLE_ADDR);/*get kernel param table from runtime*/

    int vlen = _ptable->len; /*kernel input vector length*/
    int cnum = _ptable->infoBase.parallelism;
    
    _flags[1] = _ptable->srcNocnod_A;
    _flags[2] = _ptable->srcOffset_A;
    _flags[3] = _ptable->srcNocnod_B;
    _flags[4] = _ptable->srcOffset_B;
    _flags[5] = _ptable->dstNocnod;
    _flags[6] = _ptable->dstOffset;
    

    /*judge if the parallelism is bigger than the maxcorenum*/
    if( (cnum>HPU_CORE_NUM) || (cnum<1) )
    {
        *_rtcode = 0xdead0001;
        goto fail;
    }
    
    /*find the taskid of the current core*/
    for (i = 0; i < cnum; ++i)
    {
        if(_ptable->infoBase.parallelTable[i] == _coreid)
        {
            _taskid = i;
            break;
        }
    }

    /*judge if the taskid can be found in the paramtable*/
    if(i == cnum)
    {
        *_rtcode = 0xdead0002;
        goto fail;
    }

    if(vlen < 1)
    {
        *_rtcode = 0xdead0003;
        goto fail;
    }

    int vparts = (vlen == 1) ? 1 : ((vlen - 1) / cnum + 1);
    int vparts_sub = (vparts - 1) / SBUF_SIZE + 1;
    int ndma_loc_addr, ndma_rmt_addr, ndma_len, itemnum;

    for (int i = 0; i < vparts_sub; ++i)
    {
        if(((i+1) * SBUF_SIZE) <= vparts)
        {
            itemnum = SBUF_SIZE;
        }
        else
        {
            itemnum = vparts - i * SBUF_SIZE;
        }
        ndma_len = itemnum;

        // dma load data from ddr input buffer to local sram buffer
        ndma_loc_addr = (int)buf_sram_a;
        ndma_rmt_addr = _ptable->srcOffset_A + _taskid * vparts + i * SBUF_SIZE;
        _flags[1] = ndma_loc_addr;
        _flags[2] = ndma_rmt_addr;
        _flags[7] = 0x11;
        ndma_load_data(ndma_loc_addr, ndma_rmt_addr, ndma_len, NOC_NODE_X(_ptable->srcNocnod_A), NOC_NODE_Y(_ptable->srcNocnod_A));
        ndma_wait();

        // dma load data from ddr input buffer to local sram buffer 
        ndma_loc_addr = (int)buf_sram_b;
        ndma_rmt_addr = _ptable->srcOffset_B + _taskid * vparts + i * SBUF_SIZE;
        _flags[1] = ndma_loc_addr;
        _flags[2] = ndma_rmt_addr;
        _flags[7] = 0x22;
        ndma_load_data(ndma_loc_addr, ndma_rmt_addr, ndma_len, NOC_NODE_X(_ptable->srcNocnod_B), NOC_NODE_Y(_ptable->srcNocnod_B));
        ndma_wait();

        // do the actual calculation
        for (i = 0; i < itemnum; ++i)
        {
            buf_sram_c[i] = buf_sram_a[i] + buf_sram_b[i];
        }

        // dma save data from local sram buffer to ddr output buffer
        ndma_loc_addr = (int)buf_sram_c;
        ndma_rmt_addr = _ptable->dstOffset + _taskid * vparts + i * SBUF_SIZE;
        _flags[1] = ndma_loc_addr;
        _flags[2] = ndma_rmt_addr;
        _flags[7] = 0x33;
        ndma_save_data(ndma_loc_addr, ndma_rmt_addr, ndma_len, NOC_NODE_X(_ptable->dstNocnod), NOC_NODE_Y(_ptable->dstNocnod));
        ndma_wait();
    }

    // synchronize all cores within current task group
    _kernel_sync(0, 0);
    // return code = ok
    *_rtcode = 0x12345678;
    _flags[7] = 0x12345678;

fail:
    /*send complete interrupt to runtime system
      and wait for termination done by runtime*/
    while (1)
    {
        ;
    }
}

//alloc some local sram buffer for calculation buffer
void* _kernel_malloc(int size, int type)
{

}

//multicore synchronization with rootcore dtcm
void _kernel_sync(int rootCoreNum, int idx)
{

}

void _kernel_printf()
{

}

void timer_callback(void) {
    return;
}

void ndma_callback(void) {
    return;
}

