#include <stdio.h>
#include <iostream>
#include <vector>
#include "argtable3.h"
#include "hirt.h"
#include "hirt_model.h"
#include "hirt_mm.h"
#include "hirt_device.h"
#include "hirt_cqueue.h"
#include "hirt_kernel.h"
#include "hirt_scheduler.h"
#include "hirt_event.h"
#include "hirt_utils.h"
#include "hisdk_log.h"
#include "hmodel.h"

struct arg_lit *help;
struct arg_lit *version;
struct arg_file *infiles;
struct arg_end *argend;

using namespace hisdk;
using namespace std;
/* test for primitive operator */
/*
 * define tensors: input, weight, bias and output
 * op name : conv
 * input size:	   ni x ci x hi x wi
 * weight size:    co x ci x kh x kw
 * bias size:	   1 x co x 1 x 1
 * output size:    no x co x ho x wo
*/

int main(int argc, char *argv[])
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    void *argtable[] = {
        help    = arg_lit0("h", "help",    "display this help and exit"),
        version = arg_lit0("v", "version", "display version info and exit"),
        infiles = arg_filen(NULL,NULL,NULL,1,argc+2,       "input file(s)"),
        argend  = arg_end(20)
    };
    char *progname = argv[0];

	// prepare data for conv
	//const int ni = 1, ci = 64, hi = 32, wi = 32;
	//const int no = 1, co = 64, ho = 32, wo = 32;
	//const int kh = 3, kw = 3, stride_h = 1, stride_w = 1;
	//const int dilation_h = 1, dilation_w = 1;
	//const int pad_h = 2, pad_w = 2;

    hirtModelCtx *pCtx;
    unsigned char* pBufInputImg;
    unsigned char* pBufOutputImg;
	u64_t inputBufSize;
	u64_t outputBufSize;
    hirtCmdQueue_t *pCmdQueue = NULL;
    hirtDev_t dev = 0;

	HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt_model program start...");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
        /* NULL entries were detected, some allocations must have failed */
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "%s: insufficient memory.", progname);
    }
    /* If the parser returned any errors then display them and exit */
    if (arg_parse(argc,argv,argtable) > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, argend, progname);
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_BADPARAMETER, "Try '%s --help' for more information.", progname);
    }
    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf("List information about the FILE(s) "
               "(the current directory by default).\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        goto fail;
    }
    /* special case: '--version' takes precedence error reporting */
    if (version->count > 0)
    {
        printf("'%s' example program for the \"argtable\" command line argument parser.\n",progname);
        printf("September 2003, Stewart Heitmann\n");
        goto fail;
    }
    /* test hmodel deserialization */
    if(infiles->count != 1)
    {
        HISDK_LOG_INFO(LOG_SYSTEM, "parameter error, program exit...");
    }


    /*prepare gpu execution envirement*/
    hirtInit(0);
    hirtGetDeviceHandle(&dev, 0);
    hirtSetCurrentDevice(dev);
    
    hirtCmdQueueCreate(&pCmdQueue);
#if 0    
    hirtScheduler_t *pScheduler = NULL;
    hirtSchedulerCreate(&pScheduler, pCmdQueue);
    hirtEventHandler_t *pEventHandler = NULL;
    hirtEventHandlerCreate(&pEventHandler, pScheduler);
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

    pCtx = new hirtModelCtx();
    pCtx->loadModel(infiles->filename[0]);

	inputBufSize  = pCtx->getInputTensorBufferSize();
	outputBufSize = pCtx->getOutputTensorBufferSize();
    pBufInputImg  = new unsigned char[inputBufSize];
    pBufOutputImg = new unsigned char[outputBufSize];
	HISDK_LOG_INFO(LOG_SYSTEM, "inBuffer.size=%lu, outBuffer.size=%lu", inputBufSize, outputBufSize);

	pCtx->submit(pBufInputImg, pBufOutputImg, pCmdQueue);
    
#if 0
	/*deinit the gpu execution envirement*/
    hirtEventHandlerDestroy(pEventHandler);
    hirtSchedulerDestroy(pScheduler);
    hirtCmdQueueDestroy(pCmdQueue);
    hirtDestroy();
#endif

    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt_model program end...");

fail:

	return 0;
}

