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

void hidvCoreSetMMAPCodeData(uint8_t node_xy, uint8_t mmap_code_mono, uint8_t mmap_code_data)
{
    mmap_inittable[0] = mmap_code_mono;
    mmap_inittable[4] = mmap_code_data;
    //printf("hidvCoreSetMMAPCodeData = %x,%x,%x\n", node_xy, mmap_code_mono, mmap_code_data);
    hidvWriteNocNodeMem(node_xy, HIPU200_MEM_NMAP_START, HIPU200_MEM_NMAP_SIZE, nmap_inittable);
    hidvWriteNocNodeMem(node_xy, HIPU200_MEM_MMAP_START, HIPU200_MEM_MMAP_SIZE, mmap_inittable);
}

void hidvCoreSetMMAPCode(uint8_t node_xy, uint8_t mmap_code_mono)
{
    mmap_inittable[0] = mmap_code_mono;
    hidvWriteNocNodeMem(node_xy, HIPU200_MEM_NMAP_START, HIPU200_MEM_NMAP_SIZE, nmap_inittable);
    hidvWriteNocNodeMem(node_xy, HIPU200_MEM_MMAP_START, HIPU200_MEM_MMAP_SIZE, mmap_inittable);
}

static const uint8_t nocNodeXYTable[] =
{
    HIPU200_NOC_NODEADDR_COR(0),
    HIPU200_NOC_NODEADDR_COR(1),
    HIPU200_NOC_NODEADDR_COR(2),
    HIPU200_NOC_NODEADDR_COR(3),
    HIPU200_NOC_NODEADDR_COR(4),
    HIPU200_NOC_NODEADDR_COR(5),
    HIPU200_NOC_NODEADDR_COR(6),
    HIPU200_NOC_NODEADDR_COR(7),
    HIPU200_NOC_NODEADDR_COR(8),
    HIPU200_NOC_NODEADDR_COR(9),
    HIPU200_NOC_NODEADDR_COR(10),
    HIPU200_NOC_NODEADDR_COR(11),
    HIPU200_NOC_NODEADDR_COR(12)
};

uint8_t hidvGetNocNodeXY(uint32_t coreidx)
{
    if(coreidx>=HIPU200_SOC_CORE_NUM)
        return 0xFF;

    return nocNodeXYTable[coreidx];
}

hisdkRet_t hidvInit()
{
    int ret, i;

    ret = hidvInitInternal(); //open xdma file for read write pcie channel
    if( ret != 0 )
    {
        goto fail;
    }

    hidvWriteChipReg(HIPU200_REG_HPU_CFG_0, HIPU200_CORE_ALL_BIT);// enable clock for all cores

    //set mmap nmap for core0 and core1
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(0),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(0) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(0) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(1),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(1) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(1) >> 26);
#if 0    
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(2),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(2) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(2) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(3),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(3) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(3) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(4),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(4) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(4) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(5),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(5) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(5) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(6),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(6) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(6) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(7),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(7) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(7) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(8),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(8) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(8) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(9),  HIPU200_DDR_MMAP_CODE_MONO_OFFS(9) >> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(9) >> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(10), HIPU200_DDR_MMAP_CODE_MONO_OFFS(10)>> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(10)>> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(11), HIPU200_DDR_MMAP_CODE_MONO_OFFS(11)>> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(11)>> 26);
    hidvCoreSetMMAPCodeData(HIPU200_NOC_NODEADDR_COR(12), HIPU200_DDR_MMAP_CODE_MONO_OFFS(12)>> 26,  HIPU200_DDR_MMAP_DATA_MONO_OFFS(12)>> 26);
#endif

    hidvWriteChipReg(HIPU200_REG_INITPC,  HIPU200_MEM_MMAP_CODE_MONO_START); // init pc

    hidvWriteChipReg(HIPU200_REG_RESET_0, HIPU200_CORE_ALL_BIT); //reset all cores
    hidvWriteChipReg(HIPU200_REG_RESET_0, HIPU200_CORE_ALL_BIT & (~(HIPU200_CORE_0_BIT | HIPU200_CORE_1_BIT))); //release core0 and core1

fail:
    return (ret == 0) ? HISDK_RET_SUCCESS : HISDK_RET_ERR_IO;
}

#define HIPU200_SOC_CORE_NUM            (13)
#define HIPU200_DMA_BUF_ALIGNSIZE       (1024)

#define HIPU200_CORE_0                  (0)
#define HIPU200_CORE_1                  (1)
#define HIPU200_CORE_2                  (2)
#define HIPU200_CORE_3                  (3)
#define HIPU200_CORE_4                  (4)
#define HIPU200_CORE_5                  (5)
#define HIPU200_CORE_6                  (6)
#define HIPU200_CORE_7                  (7)
#define HIPU200_CORE_8                  (8)
#define HIPU200_CORE_9                  (9)
#define HIPU200_CORE_10                 (10)
#define HIPU200_CORE_11                 (11)
#define HIPU200_CORE_12                 (12)

#define HIPU200_CORE_0_BIT              (0x01 << HIPU200_CORE_0 )
#define HIPU200_CORE_1_BIT              (0x01 << HIPU200_CORE_1 )
#define HIPU200_CORE_2_BIT              (0x01 << HIPU200_CORE_2 )
#define HIPU200_CORE_3_BIT              (0x01 << HIPU200_CORE_3 )
#define HIPU200_CORE_4_BIT              (0x01 << HIPU200_CORE_4 )
#define HIPU200_CORE_5_BIT              (0x01 << HIPU200_CORE_5 )
#define HIPU200_CORE_6_BIT              (0x01 << HIPU200_CORE_6 )
#define HIPU200_CORE_7_BIT              (0x01 << HIPU200_CORE_7 )
#define HIPU200_CORE_8_BIT              (0x01 << HIPU200_CORE_8 )
#define HIPU200_CORE_9_BIT              (0x01 << HIPU200_CORE_9 )
#define HIPU200_CORE_10_BIT             (0x01 << HIPU200_CORE_10)
#define HIPU200_CORE_11_BIT             (0x01 << HIPU200_CORE_11)
#define HIPU200_CORE_12_BIT             (0x01 << HIPU200_CORE_12)

#define HIPU200_CORE_BIT(x)             (0x01 << x)

void hidvCoreResetPC(uint32_t coreBitmap)
{

}

void hidvCoreActivate(uint32_t coreBitmap)
{
    uint32_t regval;
    regval = hidvReadChipReg(HIPU200_REG_RESET_0);
    regval = regval & ~(coreBitmap & HIPU200_CORE_ALL_BIT);
    hidvWriteChipReg(HIPU200_REG_RESET_0, regval);
}

void hidvCoreDeactivate(uint32_t coreBitmap)
{
    uint32_t regval;
    regval = hidvReadChipReg(HIPU200_REG_RESET_0);
    regval = regval | (coreBitmap & HIPU200_CORE_ALL_BIT);
    hidvWriteChipReg(HIPU200_REG_RESET_0, regval);
}

void hidvCoreWakeup(uint32_t coreBitmap)
{
    hidvWriteChipReg(HIPU200_REG_INT_CLR_0, (coreBitmap & HIPU200_CORE_ALL_BIT));
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

    hidvWriteChipMem(addr, size, data);
}

void hidvReadNocNodeMem(uint8_t node_xy, uint64_t offset, uint64_t size, void * data)
{
    uint64_t addr = HIPU200_NOC_MAKEADDR(node_xy, offset);

    hidvReadChipMem(addr, size, data);
}

void hidvWriteChipMem(uint64_t addr, uint64_t size, void * data)
{
    //printf("hidvWriteChipMem = %lx, %lx\n", addr, size);
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