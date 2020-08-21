#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hirt.h"
#include "hirt_device.h"
#include "hirt_cqueue.h"
#include "hirt_scheduler.h"
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
    void *argtable[] = {
        help    = arg_lit0("h", "help",    "display this help and exit"),
        version = arg_lit0("v", "version", "display version info and exit"),
        debug   = arg_lit0("d", "debug",   "enable log of debug information"),
        logfile = arg_file0("f", "logfile", "<output>", "log file (default is \"-\")"),
        end     = arg_end(20)
    };

    HISDK_LOG_INFO(LOG_SYSTEM, "program start...");
    
    hirtInit(0);
    hirtDev_t dev;
    hirtGetDeviceHandle(&dev, 0);
    hirtSetCurrentDevice(dev);

    //int a, b, out;
    //srand((unsigned)time(NULL));
    //a = (int)rand();
    //b = (int)rand();

    hirtCmdQueue_t *pCmdQueue;
    hirtCmdQueueCreate(&pCmdQueue);

    hirtScheduler_t *pScheduler;
    hirtSchedulerCreate(&pScheduler, pCmdQueue);

#if 0
    int *k_a, *k_b;
    hirtDevMalloc((void **)&k_a, sizeof(int));
    hirtDevMalloc((void **)&k_b, sizeof(int));
    hirtMemcpy(k_a, &a, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2DEV);
    hirtMemcpy(k_b, &b, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2DEV);

    hirtKernelParamsBuffer_t *pParams;
    hirtGetKernelParamsBuffer(&pParams);
    hirtKernelParamsBufferAddParams(pParams, &k_a, sizeof(int *));
    //hirtKernelParamsBufferAddParams(pParams, &k_b, sizeof(int *));


    hirtInvokeKernel(&kernel0, 3, pParams, queue);
    hirtInvokeKernel(&kernel1, 3, pParams, queue);
    hirtInvokeKernel(&kernel2, 3, pParams, queue);
    hirtCmdQueueSync(queue);
    
    hirtInvokeKernel(&kernel3, 3, pParams, queue);

    hirtMemcpy(&out, k_a, sizeof(int), HIRT_MEM_TRANS_DIR_DEV2HOST);
    hirtDevFree(k_a);
    hirtDevFree(k_b);
    hirtDestroyKernelParamsBuffer(pParams);
    hirtDestroyQueue(queue);
    hirtDestroy();
#endif

    hirtSchedulerDestroy(pScheduler);
    hirtCmdQueueDestroy(pCmdQueue);
    
    HISDK_LOG_INFO(LOG_SYSTEM, "program end...");
    return 0;
}