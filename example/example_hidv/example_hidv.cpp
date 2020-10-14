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

static uint8_t nmap_inittable[HIPU200_MEM_NMAP_SIZE] = 
{
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
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0),
    HIPU200_NOC_MAKEMMAP_x64M(0)
};

int main(int argc, char ** argv)
{
    hidvInit();

    hidvWriteChipReg(HIPU200_REG_HPU_CFG_0, HIPU200_CORE_ALL_BIT);//enable clock

    //const char * code_file = "./binary/code.bin";
    //const char * ifm_file = "./binary/ifm.bin";
    //const char * wt_file = "./binary/wt.bin";
    //const char * bs_file = "./binary/bs.bin";
    //const char * ofm_file = "./binary/fm_conv1_out.bin";
    uint32_t ofmsize = 8*1024;
    uint8_t * ofmbuffer;
    posix_memalign((void **)&ofmbuffer, HIPU200_DMA_BUF_ALIGNSIZE, ofmsize);
    assert(ofmbuffer != NULL);

    uint8_t hipunoc_node_ddr0 = HIPU200_NOC_MAKENMAP(HIPU200_NOC_DDR0_X, HIPU200_NOC_DDR0_Y);
    uint8_t hipunoc_node_cor0 = HIPU200_NOC_MAKENMAP(HIPU200_NOC_COR0_X, HIPU200_NOC_COR0_Y);

    hidvWriteNocNodeMem(hipunoc_node_cor0, HIPU200_MEM_NMAP_START, HIPU200_MEM_NMAP_SIZE, nmap_inittable);//set nmap_inittable for hpu0
    hidvWriteNocNodeMem(hipunoc_node_cor0, HIPU200_MEM_MMAP_START, HIPU200_MEM_MMAP_SIZE, mmap_inittable);//set mmap_inittable for hpu0
    //hidvWriteFileNocNode(hipunoc_node_ddr0, 0x0,     code_file);
    //hidvWriteFileNocNode(hipunoc_node_ddr0, 0x40000, ifm_file);
    //hidvWriteFileNocNode(hipunoc_node_ddr0, 0x80000, wt_file);
    //hidvWriteFileNocNode(hipunoc_node_ddr0, 0x48000, bs_file);

#if 0
    hidvWriteChipReg(HIPU200_REG_INITPC,  0x80020000); // init pc
    hidvWriteChipReg(HIPU200_REG_RESET_0, HIPU200_CORE_ALL_BIT); //reset hpus
    hidvWriteChipReg(HIPU200_REG_RESET_0, ~HIPU200_CORE_0_BIT); //release reset for hpu0

    usleep(1000000);
    hidvWriteChipReg(HIPU200_REG_INT_CLR_0, HIPU200_CORE_ALL_BIT); //wake up all HPU
    
    usleep(1000000);
    hidvReadNocNodeMem(hipunoc_node_ddr0, 0x43000, ofmsize, ofmbuffer);
#endif

    hidvReadNocNodeMem(hipunoc_node_cor0, HIPU200_MEM_NMAP_START, HIPU200_MEM_NMAP_SIZE, ofmbuffer);

    printf("ofm:\n");
    for(int i = 0;i<HIPU200_MEM_NMAP_SIZE;i++)
    {
        printf("in: %x, out: %x\n",nmap_inittable[i], ofmbuffer[i]);
    }

    //FILE * ofm_fp = fopen(ofm_file,"wb");
    //fwrite(ofmbuffer,ofmsize,1,ofm_fp);
    //fclose(ofm_fp);
    free(ofmbuffer);

    return 0;
}