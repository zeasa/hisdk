#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "hidv.h"

void hidvDmaWriteNOCAddr(uint64_t offset,size_t size,void * src);
void hidvDmaReadNOCAddr(uint64_t offset,size_t size,void * dest);
void hidvWriteMMapReg32(uint64_t offset, uint32_t src);
uint32_t hidvReadMMapReg32(uint64_t offset);
void hidvDmaSetReadSize(uint32_t size);
void hidvDmaSetWriteSize(uint32_t size);
long int filesize(FILE *fp);
int hidvInitInternal();

static uint8_t nmap_inittable[HIPU200_MEM_NMAP_SIZE] = 
{
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),

    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR1_X, HIPU200_NOC_DDR1_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR1_X, HIPU200_NOC_DDR1_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR1_X, HIPU200_NOC_DDR1_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR1_X, HIPU200_NOC_DDR1_Y),

    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y),
    HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y)
};

static uint8_t mmap_inittable[HIPU200_MEM_MMAP_SIZE] = 
{
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(13),
    HIPU200_NOC_MAKEMMAP_x64M(14),
    HIPU200_NOC_MAKEMMAP_x64M(15),

    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(13),
    HIPU200_NOC_MAKEMMAP_x64M(14),
    HIPU200_NOC_MAKEMMAP_x64M(15),

    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0)
};

#define HIDV_SET_HIPU_MMAP(x)   mmap_inittable[0] = HIPU200_DDR_MMAP_CODE_MONO_OFFS(x); \
                                mmap_inittable[4] = HIPU200_DDR_MMAP_DATA_MONO_OFFS(x); \
    hidvWriteNocNodeMem(HIPU200_NOC_NODEADDR_COR(x), HIPU200_MEM_NMAP_START, HIPU200_MEM_NMAP_SIZE, nmap_inittable); \
    hidvWriteNocNodeMem(HIPU200_NOC_NODEADDR_COR(x), HIPU200_MEM_MMAP_START, HIPU200_MEM_MMAP_SIZE, mmap_inittable)

hisdkRet_t hidvInit()
{
    int ret, i;

    ret = hidvInitInternal(); //open xdma file for read write pcie channel
    if( ret != 0 )
    {
        goto fail;
    }

    hidvWriteChipReg(HIPU200_REG_HPU_CFG_0, HIPU200_CORE_ALL_BIT);// enable clock for all cores

    HIDV_SET_HIPU_MMAP(0); // set mmap for all hipu200 cores
    HIDV_SET_HIPU_MMAP(1);
    HIDV_SET_HIPU_MMAP(2);
    HIDV_SET_HIPU_MMAP(3);
    HIDV_SET_HIPU_MMAP(4);
    HIDV_SET_HIPU_MMAP(5);
    HIDV_SET_HIPU_MMAP(6);
    HIDV_SET_HIPU_MMAP(7);
    HIDV_SET_HIPU_MMAP(8);
    HIDV_SET_HIPU_MMAP(9);
    HIDV_SET_HIPU_MMAP(10);
    HIDV_SET_HIPU_MMAP(11);
    HIDV_SET_HIPU_MMAP(12);

    hidvWriteChipReg(HIPU200_REG_INITPC,  HIPU200_MEM_MMAP_CODE_MONO_START); // init pc

fail:
    return (ret == 0) ? HISDK_RET_SUCCESS : HISDK_RET_ERR_IO;
}

void hidvDestroy()
{
}

void  hidvWriteChipReg(uint32_t offset,uint32_t value)
{
    hidvWriteMMapReg32(offset, value);//enable clock
}

uint32_t hidvReadChipReg(uint32_t offset)
{
    return hidvReadMMapReg32(offset);
}

void hidvWriteNocNodeMem(uint8_t node_xy, uint64_t offset, uint64_t size, void * data)
{
    uint64_t addr = HIPU200_NOC_MAKEADDR(node_xy, offset);

    hidvDmaSetWriteSize(size);
    hidvDmaWriteNOCAddr(addr, size, data);
}

void hidvReadNocNodeMem(uint8_t node_xy, uint64_t offset, uint64_t size, void * data)
{
    uint64_t addr = HIPU200_NOC_MAKEADDR(node_xy, offset);

    hidvDmaSetReadSize(size);//pcie bridge
    hidvDmaReadNOCAddr(addr, size, data);
}

void hidvWriteChipMem(uint64_t addr, uint64_t size, void * data)
{
    hidvDmaSetWriteSize(size);
    hidvDmaWriteNOCAddr(addr, size, data);
}

void hidvReadChipMem(uint64_t addr, uint64_t size, void * data)
{
    hidvDmaSetReadSize(size);//pcie bridge
    hidvDmaReadNOCAddr(addr, size, data);
}

void hidvWriteFileNocNode(uint8_t node_xy, uint64_t offset,const char * filename)
{
    printf("filename: %s",filename);

    FILE * fp = fopen(filename,"rb");
    printf("fp : %p\n",fp);

    uint32_t buffsize = filesize(fp);
    printf("filesize: %d",buffsize);
    if(buffsize%4096!=0){
        buffsize = (4096 - buffsize%4096) + buffsize;
    }

    char * buffer;
    posix_memalign((void **)&buffer, HIPU200_DMA_BUF_ALIGNSIZE, buffsize);
    printf("writing : %s\n",filename);
    fread(buffer, buffsize, 1, fp);
    //for(int i = 0;i<16;i++){
    //    printf("data: %x\n",buffer[i]);
    //}
    hidvWriteNocNodeMem(node_xy,offset,buffsize,buffer);// code: fpga.bin
    free(buffer);
}