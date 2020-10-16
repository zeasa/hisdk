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
#define TESTSIZE                (16)

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

void kernel_entry(void) 
{
    unsigned int ka[TESTSIZE] __attribute__((aligned(64)));
    unsigned int kb[TESTSIZE] __attribute__((aligned(64)));
    unsigned int kc[TESTSIZE] __attribute__((aligned(64)));

    int   _coreid = get_hpuid();
    paramTableVAdd_t *_ptable = (paramTableVAdd_t *)KERNEL_PTABLE_ADDR;
    int vlen = _ptable->len;

    int ndma_loc_addr = 0;
    int ndma_rmt_addr = 0;
    int ndma_len = 0;
    int i;

    // load src_a
    ndma_loc_addr = (int)&ka;
    ndma_rmt_addr = _ptable->srcOffset_A;
    ndma_len = TESTSIZE*sizeof(unsigned int);
    ndma_load_data(ndma_loc_addr, ndma_rmt_addr, ndma_len, _ptable->srcNocnod_A>>4, _ptable->srcNocnod_A & 0x0F);
    ndma_wait();

    // load src_b
    ndma_loc_addr = (int)&kb;
    ndma_rmt_addr = _ptable->srcOffset_B;
    ndma_len = TESTSIZE*sizeof(unsigned int);
    ndma_load_data(ndma_loc_addr, ndma_rmt_addr, ndma_len, _ptable->srcNocnod_B>>4, _ptable->srcNocnod_B & 0x0F);
    ndma_wait();

    for (i = 0; i < TESTSIZE; ++i)
    {
        kc[i] = ka[i] + kb[i];
    }

    // save dest
    ndma_loc_addr = (int)&kc;
    ndma_rmt_addr = _ptable->dstOffset;
    ndma_len = TESTSIZE*sizeof(unsigned int);
    ndma_save_data(ndma_loc_addr, ndma_rmt_addr, ndma_len, _ptable->dstNocnod>>4, _ptable->dstNocnod& 0x0F);
    ndma_wait();

    while(1);
}

void kernel_malloc()
{

}

void timer_callback(void) {
    return;
}

void ndma_callback(void) {
    return;
}

