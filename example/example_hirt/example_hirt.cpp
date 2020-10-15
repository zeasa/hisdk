#include <stdio.h>
#include <stdlib.h>
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

    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt program start...");

    hirtInit(0);
    hirtDev_t dev = 0;
    hirtGetDeviceHandle(&dev, 0);
    hirtSetCurrentDevice(dev);

    //int a, b, out;
    //srand((unsigned)time(NULL));
    //a = (int)rand();
    //b = (int)rand();
#if 0
    hirtCmdQueue_t *pCmdQueue = NULL;
    hirtCmdQueueCreate(&pCmdQueue);

    hirtScheduler_t *pScheduler = NULL;
    hirtSchedulerCreate(&pScheduler, pCmdQueue);

    hirtEventHandler_t *pEventHandler = NULL;
    hirtEventHandlerCreate(&pEventHandler, pScheduler);
#endif

#define TESTLEN     16
    unsigned char buf1[TESTLEN];
    unsigned char buf2[TESTLEN];
    for (unsigned char i = 0; i < TESTLEN; i++)
    {
        buf1[i] = i;
        buf2[i] = (TESTLEN-1) - i;
    }

    hirtGMemAddress_t k_a, k_b;
    hirtGpuMalloc(&k_a, 1024);
    hirtGpuMalloc(&k_b, 1024);
    hirtMemcpy((void*)k_a, buf1, TESTLEN, HIRT_MEM_TRANS_DIR_HOST2GPU);
    hirtMemcpy((void*)k_b, buf2, TESTLEN, HIRT_MEM_TRANS_DIR_HOST2GPU);

    hirtMemcpy(buf1, (void*)k_b, TESTLEN, HIRT_MEM_TRANS_DIR_GPU2HOST);
    hirtMemcpy(buf2, (void*)k_a, TESTLEN, HIRT_MEM_TRANS_DIR_GPU2HOST);

    for (unsigned char i = 0; i < TESTLEN; i++)
    {
        printf("%x,%x\n", buf1[i], buf2[i]);
    }

#if 0
    hirtKernelParamsBuffer_t *pParams = NULL;
    hirtKernelParamsBufferCreate(&pParams);
    //hirtKernelParamsBufferAddParam(pParams, &k_a, sizeof(int *));
    hirtKernelBinBuffer_t *pkrnlBin = NULL;
    hirtInvokeKernel("kernel.o", pParams, &pkrnlBin, 1, pCmdQueue);
    hirtCmdQueueSync(pCmdQueue);
#endif
#if 0    
    hirtInvokeKernel(&kernel1, 3, pParams, queue);
    hirtInvokeKernel(&kernel2, 3, pParams, queue);
    hirtMemcpy(&out, k_a, sizeof(int), HIRT_MEM_TRANS_DIR_GPU2HOST);
    hirtGpuFree(k_a);
    hirtGpuFree(k_b);
    hirtDestroyKernelParamsBuffer(pParams);
    hirtDestroyQueue(queue);
    hirtDestroy();
#endif
#if 0
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