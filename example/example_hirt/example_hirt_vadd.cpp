#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "hirt.h"
#include "hirt_device.h"
#include "hirt_cqueue.h"
#include "hirt_kernel.h"
#include "hirt_scheduler.h"
#include "hirt_event.h"
#include "hirt_utils.h"
#include "hirt_mm.h"
#include "hisdk_log.h"
#include "argtable3.h"

typedef unsigned int u32_t;

typedef struct
{
    u32_t typeVersion;
    u32_t tableSiz;
    u32_t parallelism;
    u32_t parallelTable[13];
} paramTableBase_t;

typedef struct
{
    u32_t srcNocnod_A;
    u32_t srcOffset_A;
    u32_t srcNocnod_B;
    u32_t srcOffset_B;
    u32_t dstNocnod;
    u32_t dstOffset;
    u32_t len;
} paramTableVAdd_t;

struct arg_lit  *help;
struct arg_lit  *version;
struct arg_lit  *debug;
struct arg_file *logfile;
struct arg_end  *end;

int main(int argc, char *argv[])
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    void *argtable[] = {
        help    = arg_lit0("h", "help",    "display this help and exit"),
        version = arg_lit0("v", "version", "display version info and exit"),
        debug   = arg_lit0("d", "debug",   "enable log of debug information"),
        logfile = arg_file0("f", "logfile", "<output>", "log file (default is \"-\")"),
        end     = arg_end(20)
    };

    paramTableVAdd_t ptable_vadd;

    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt program start...");

    hirtInit(0);
    hirtDev_t dev = 0;
    hirtGetDeviceHandle(&dev, 0);
    hirtSetCurrentDevice(dev);

    hirtCmdQueue_t *pCmdQueue = NULL;
    hirtCmdQueueCreate(&pCmdQueue);
    hirtScheduler_t *pScheduler = NULL;
    hirtSchedulerCreate(&pScheduler, pCmdQueue);

#if 0
    hirtEventHandler_t *pEventHandler = NULL;
    hirtEventHandlerCreate(&pEventHandler, pScheduler);
#endif

#define TESTLEN     16
    unsigned char buf_a[TESTLEN];
    unsigned char buf_b[TESTLEN];
    unsigned char buf_c[TESTLEN];
    for (unsigned char i = 0; i < TESTLEN; i++)
    {
        buf_a[i] = 1;
        buf_b[i] = 2;
        buf_c[i] = 0;
    }

    hirtGMemAddress_t gaddr_a, gaddr_b, gaddr_c;
    hirtGpuMalloc(&gaddr_a, 1024);
    hirtGpuMalloc(&gaddr_b, 1024);
    hirtGpuMalloc(&gaddr_c, 1024);
    hirtMemcpy((void*)gaddr_a, buf_a, TESTLEN, HIRT_MEM_TRANS_DIR_HOST2GPU);
    hirtMemcpy((void*)gaddr_b, buf_b, TESTLEN, HIRT_MEM_TRANS_DIR_HOST2GPU);


    hirtKernelParamsBuffer_t *pParams = NULL;
    hirtKernelParamsBufferCreate(&pParams);
    hirtKernelParamsBufferAddPlaceHolder(pParams, sizeof(paramTableBase_t));
    ptable_vadd.srcNocnod_A = (u32_t)(gaddr_a >> 32);
    ptable_vadd.srcOffset_A = (u32_t)(gaddr_a & 0xFFFFFFFF);
    ptable_vadd.srcNocnod_B = (u32_t)(gaddr_b >> 32);;
    ptable_vadd.srcOffset_B = (u32_t)(gaddr_b & 0xFFFFFFFF);;
    ptable_vadd.dstNocnod   = (u32_t)(gaddr_c >> 32);;
    ptable_vadd.dstNocnod   = (u32_t)(gaddr_c & 0xFFFFFFFF);;
    ptable_vadd.len = TESTLEN;
    hirtKernelParamsBufferAddParam(pParams, (void*)&ptable_vadd, sizeof(paramTableVAdd_t));

    hirtKernelBinBuffer_t *pkrnlBin = NULL;
    hirtInvokeKernel("kernel_vadd_1c.bin", pParams, &pkrnlBin, 1, pCmdQueue);
    //hirtCmdQueueSync(pCmdQueue);

    usleep(2000*1000);
    hirtMemcpy(buf_c, (void*)gaddr_c, TESTLEN, HIRT_MEM_TRANS_DIR_GPU2HOST);

    for (unsigned char i = 0; i < TESTLEN; i++)
    {
        printf("buf_c[%d]=0x%x\n", i, buf_c[i]);
    }

#if 0    
    hirtGpuFree(k_a);
    hirtGpuFree(k_b);
    hirtDestroyKernelParamsBuffer(pParams);
    hirtDestroyQueue(queue);
    hirtDestroy();
    hirtEventHandlerDestroy(pEventHandler);
    hirtSchedulerDestroy(pScheduler);
    hirtCmdQueueDestroy(pCmdQueue);
#endif

    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt program end...");
    return 0;

#if 0
fail:
    HISDK_LOG_INFO(LOG_SYSTEM, "program err...");
    return 0;
#endif
}