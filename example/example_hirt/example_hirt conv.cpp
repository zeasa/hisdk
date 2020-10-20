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

#define IFM_FILENAME        "ifm.bin"
#define WT_FILENAME         "wt.bin"
#define BS_FILENAME         "bs.bin"
#define OFM_FILENAME        "ofm.bin"
#define KERNEL_FILENAME     "kernel_entry.bin"

typedef unsigned char   uint8;
typedef unsigned int    uint32;

typedef struct{
    uint32 ifm_h;
    uint32 ifm_w;
    uint32 ifm_c;
    uint32 ofm_c;
    uint32 k_h;
    uint32 k_w;
}conv_shape_t;

typedef struct{
    uint8 top;
    uint8 bottom;
    uint8 side;
}pad_shape_t;

typedef struct{
    uint8 w_strd;
    uint8 h_strd;
}stride_shape_t;

typedef struct{
    uint8 w_dilat;
    uint8 h_dilat;
}dilate_shape_t;

typedef struct{
    uint32 grp_num;
}chn_group_t;

typedef struct{
    conv_shape_t cshape;
    pad_shape_t pshape;
    stride_shape_t  strd_shape;
    dilate_shape_t  dilat_shape;
    chn_group_t cgroup_num;
}conv2d_params_t;

typedef struct
{
    int typeVersion;
    int tableSiz;
    int parallelism;
    int parallelTable[13];
} paramTableBase_t;

typedef struct 
{
    uint32 ifmNocnod;
    uint32 ifmOffset;
    uint32 wtNocnod;
    uint32 wtOffset;
    uint32 bsNocnod;
    uint32 bsOffset;
    uint32 ofmNocnod;
    uint32 ofmOffset;
} conv2d_mems_t;

typedef struct
{
    paramTableBase_t infoBase;
    conv2d_mems_t    convMems;
    conv2d_params_t  convParam;
} paramTableConv2d_t;

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

    hirtCmdQueue_t *pCmdQueue = NULL;
    hirtCmdQueueCreate(&pCmdQueue);

    //hirtScheduler_t *pScheduler = NULL;
    //hirtSchedulerCreate(&pScheduler, pCmdQueue);
    //hirtEventHandler_t *pEventHandler = NULL;
    //hirtEventHandlerCreate(&pEventHandler, pScheduler);

    int filesize_ifm = hisdkPortOsGetFileSize(IFM_FILENAME);
    int filesize_wt  = hisdkPortOsGetFileSize(WT_FILENAME);
    int filesize_bs  = hisdkPortOsGetFileSize(BS_FILENAME);
    int filesize_ofm = 1024;

    hirtGMemAddress_t gmemIfm, gmemWt, gmemBs, gmemOfm;
    hirtGpuMalloc(&gmemIfm, filesize_ifm);
    hirtGpuMalloc(&gmemWt,  filesize_wt);
    hirtGpuMalloc(&gmemBs,  filesize_bs);
    hirtGpuMalloc(&gmemOfm, filesize_bs);

    unsigned char *filebuf_ifm = (unsigned char*)malloc(filesize_ifm);
    unsigned char *filebuf_wt  = (unsigned char*)malloc(filesize_wt);
    unsigned char *filebuf_bs  = (unsigned char*)malloc(filesize_bs);

    hirtMemcpy((void*)gmemIfm, filebuf_ifm, filesize_ifm, HIRT_MEM_TRANS_DIR_HOST2GPU);
    hirtMemcpy((void*)gmemWt,  filebuf_wt,  filesize_wt,  HIRT_MEM_TRANS_DIR_HOST2GPU);
    hirtMemcpy((void*)gmemBs,  filebuf_bs,  filesize_bs,  HIRT_MEM_TRANS_DIR_HOST2GPU);

    conv2d_mems_t convMems;
    conv2d_params_t convParam =
    {
        .cshape{
            .ifm_h = 32,
            .ifm_w = 32,
            .ifm_c = 32,
            .ofm_c = 32,
            .k_h   = 3,
            .k_w   = 3
        },
        .pshape{
            .top   = 1,
            .bottom= 1,
            .side  = 1,
        },
        .strd_shape{
            .w_strd = 1,
            .h_strd = 1
        },
        .dilat_shape{
            .w_dilat = 1,
            .h_dilat = 1
        },
        .cgroup_num{
            .grp_num = 1
        },
    };

    convMems.ifmNocnod = gmemIfm >> 32;
    convMems.ifmOffset = gmemIfm & 0xFFFFFFFF;
    convMems.wtNocnod  = gmemWt  >> 32;
    convMems.wtOffset  = gmemWt  & 0xFFFFFFFF;
    convMems.bsNocnod  = gmemBs  >> 32;
    convMems.bsOffset  = gmemBs  & 0xFFFFFFFF;
    convMems.ofmNocnod = gmemOfm >> 32;
    convMems.ofmOffset = gmemOfm & 0xFFFFFFFF;

    hirtKernelParamsBuffer_t *pParams = NULL;
    hirtKernelParamsBufferCreate(&pParams);
    hirtKernelParamsBufferAddParam(pParams, (void*)&convMems,  sizeof(conv2d_mems_t));
    hirtKernelParamsBufferAddParam(pParams, (void*)&convParam, sizeof(conv2d_params_t));
    hirtKernelBinBuffer_t *pkrnlBin = NULL;
    hirtInvokeKernel(KERNEL_FILENAME, pParams, &pkrnlBin, 2, pCmdQueue);

    hirtCmdQueueSync(pCmdQueue);

    hirtMemcpy(&out, k_a, sizeof(int), HIRT_MEM_TRANS_DIR_GPU2HOST);

    hirtGpuFree(gmemIfm);
    hirtGpuFree(gmemWt);
    hirtGpuFree(gmemBs);
    hirtKernelParamsBufferDestroy(pParams);
    hirtCmdQueueDestroy(pCmdQueue);
    hirtDestroy();

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