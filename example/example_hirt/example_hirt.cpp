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

    hirtCmdQueue_t *pCmdQueue = NULL;
    hirtCmdQueueCreate(&pCmdQueue);

    hirtScheduler_t *pScheduler = NULL;
    hirtSchedulerCreate(&pScheduler, pCmdQueue);

    hirtEventHandler_t *pEventHandler = NULL;
    hirtEventHandlerCreate(&pEventHandler, pScheduler);

#if 0
    int *k_a, *k_b;
    hirtGpuMalloc((void **)&k_a, sizeof(int));
    hirtGpuMalloc((void **)&k_b, sizeof(int));
    hirtMemcpy(k_a, &a, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2GPU);
    hirtMemcpy(k_b, &b, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2GPU);
#endif


    hirtKernelParamsBuffer_t *pParams = NULL;
    hirtKernelParamsBufferCreate(&pParams);
    //hirtKernelParamsBufferAddParam(pParams, &k_a, sizeof(int *));
    hirtKernelBinBuffer_t *pkrnlBin = NULL;
    hirtInvokeKernel("kernel.o", pParams, &pkrnlBin, 1, pCmdQueue);
    hirtCmdQueueSync(pCmdQueue);
    
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

    hirtEventHandlerDestroy(pEventHandler);
    hirtSchedulerDestroy(pScheduler);
    hirtCmdQueueDestroy(pCmdQueue);
    
    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt program end...");
    return 0;
#if 0
fail:
    HISDK_LOG_INFO(LOG_SYSTEM, "program err...");
    return 0;
#endif
}