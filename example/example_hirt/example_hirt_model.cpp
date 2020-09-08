#include <stdio.h>
#include <iostream>
#include <vector>
#include "hirt.h"
#include "argtable3.h"
#include "hmodel.h"
#include "hirt_model.h"

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
    
	unsigned int seed = time(0);
	// prepare data for conv
	const int ni = 1, ci = 64, hi = 32, wi = 32;
	const int no = 1, co = 64, ho = 32, wo = 32;
	const int kh = 3, kw = 3, stride_h = 1, stride_w = 1;
	const int dilation_h = 1, dilation_w = 1;
	const int pad_h = 2, pad_w = 2;
	//hirtModelCtx_t *pmodel;
    //const std::vector<AddressListEntry> * addresslist;
    //const std::vector<MemoryListEntry>  * memorylist;
    //const std::vector<TaskListEntry>    * tasklist;
    //std::vector<MemoryListEntry>::const_iterator it_mem;
    hirtModelCtx *pCtx;

    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt_model program start...");
    
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

    pCtx = new hirtModelCtx();
    pCtx->loadModel(infiles->filename[0]);
    
#if 0
    HISDK_ERR_FCALLFAIL( hirtLoadModel(&pmodel, infiles->filename[0]) );

    addresslist = &pmodel->phModel->getAddressListEntries();
    memorylist  = &pmodel->phModel->getMemoryListEntries();
    tasklist    = &pmodel->phModel->getTaskListEntries();
    
	// vector<int>::const_iterator iter=v.begin();
	for(it_mem = memorylist->begin(); it_mem != memorylist->end(); ++it_mem)
	{
		cout << it_mem->id << endl;
    }


	float * inputDataCpu;
	inputDataCpu = (float *)hisdkAlloc(ni * ci * hi * wi * sizeof(float));
	for(int index = 0; index < ni * ci * hi * wi; ++index)
	{
		inputDataCpu[index] =((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
	}

	float * weightDataCpu;
	weightDataCpu = (float *)hisdkAlloc(co * ci * kh * kw * sizeof(float));
	for(int index = 0; index < co * ci * kh * kw; ++index)
	{
		weightDataCpu[index] =((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
	}
	
	float * biasDataCpu;
	biasDataCpu = (float *)hisdkAlloc(co * sizeof(float));
	for(int index = 0; index < co; ++index)
	{
		biasDataCpu[index] =(rand_r(&seed) % 100 / 100.0) * 128;
	}

	float * outputDataCpu;
	outputDataCpu = (float *)hisdkAlloc(no * co * ho * wo * sizeof(float));
	float * resultCpu;
	resultCpu = (float *)hisdkAlloc(no * co * ho * wo * sizeof(float));
	for(int index = 0; index < no * co * ho * wo; ++index)
	{
		outputDataCpu[index] = 0.0;
	}

	// prepare input cpu tensor
	himlCpuTensor_t *pTensorInputCpu;
	himlCreateCpuTensor(&pTensorInputCpu, 
		TS_TYPE_TENSOR, 
		DATATYPE_FP32, 
		TS_FORMAT_NCHW, 
		ni, 
		ci, 
		hi, 
		wi);

	// prepare weight cpu tensor
	himlCpuTensor_t *pTensorWeightCpu;
	himlCreateCpuTensor(&pTensorWeightCpu, 
		TS_TYPE_WEIGHT, 
		DATATYPE_FP32, 
		TS_FORMAT_NCHW, 
		co, 
		ci, 
		kh, 
		kw);

	// prepare bias cpu tensor
	himlCpuTensor_t *pTensorBiasCpu;
	himlCreateCpuTensor(&pTensorBiasCpu, 
		TS_TYPE_BIAS, 
		DATATYPE_FP32, 
		TS_FORMAT_NCHW, 
		1, 
		co, 
		1, 
		1);

	// prepare output cpu tensor
	himlCpuTensor_t *pTensorOutputCpu;
	himlCreateCpuTensor(&pTensorOutputCpu, 
		TS_TYPE_TENSOR, 
		DATATYPE_FP32, 
		TS_FORMAT_NCHW, 
		no, 
		co, 
		ho, 
		wo);

	// no data order needed in creating himl tensor
	// prepare input himl tensor
	himlGpuTensor_t *pTensorInputGpu;
	himlCreateGpuTensor(&pTensorInputGpu, 
		TS_TYPE_TENSOR, 
		DATATYPE_FP16, 
		ni, 
		ci, 
		hi, 
		wi);

	// prepare weight himl tensor
	himlGpuTensor_t *pTensorWeightGpu;
	himlCreateGpuTensor(&pTensorWeightGpu, 
		TS_TYPE_WEIGHT, 
		DATATYPE_FP16, 
		co, 
		ci, 
		kh, 
		kw);

	// prepare bias himl tensor
	himlGpuTensor_t *pTensorBiasGpu;
	himlCreateGpuTensor(&pTensorBiasGpu, 
		TS_TYPE_BIAS, 
		DATATYPE_FP16, 
		1, 
		co, 
		1, 
		1);

	// prepare output himl tensor
	himlGpuTensor_t *pTensorOutputGpu;
	himlCreateGpuTensor(&pTensorOutputGpu, 
		TS_TYPE_TENSOR, 
		DATATYPE_FP16, 
		no, 
		co, 
		ho, 
		wo);

	// setup and check op runnable
	//himlCheckBaseOpRunnable(conv_op, himl_C10);

	// compile op
	//himlCompileBaseOp(conv_op, himl_C10, 1);

	// hisdkAlloc himl tensor
	hirtGMemAddress_t gmemInputGpu;
	hirtGMemAddress_t gmemOutputGpu; 
	hirtGMemAddress_t gmemWeightGpu;
	hirtGMemAddress_t gmemBiasGpu;   
	gmemInputGpu  = himlMallocBuffer(pTensorInputGpu);
	gmemOutputGpu = himlMallocBuffer(pTensorOutputGpu);
	gmemWeightGpu = himlMallocBuffer(pTensorWeightGpu);
	gmemBiasGpu   = himlMallocBuffer(pTensorBiasGpu);

	// copy weight to himl tensor
	himlMemcpyTensorToDevice(
	    pTensorWeightCpu, 
		weightDataCpu, 
		pTensorWeightGpu, 
		gmemWeightGpu);
	// copy bias to himl tensor
	himlMemcpyTensorToDevice(
	    pTensorBiasCpu, 
		biasDataCpu, 
		pTensorBiasGpu, 
		gmemBiasGpu);

	// copy input to himl tensor
	himlMemcpyTensorToDevice(
	    pTensorInputCpu, 
		inputDataCpu, 
		pTensorInputGpu, 
		gmemInputGpu);

	//	prepare conv param
	himlConvOpParam_t *pParamConv;
	himlCreateConvOpParam(&pParamConv, 
		stride_h, 
		stride_w, 
		dilation_h, 
		dilation_w, 
		pad_h, 
		pad_w);

	// setup conv operator
	himlBaseOp_t conv_op;
	himlCreateConvOp(&conv_op, 
		pParamConv, 
		pTensorInputGpu, 
		pTensorOutputGpu, 
		pTensorWeightGpu, 
		pTensorBiasGpu);

	/* create stream and func_type and llc */
	//cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
	hirtCmdQueue_t *pCmdQueue;
    hirtCmdQueueCreate(&pCmdQueue);

	// compute conv op
	himlComputeConvOpForward(
	    &conv_op, 
		gmemInputGpu, 
		gmemOutputGpu, 
		FUNCTYPE_C1, 
		pCmdQueue);

	// copy output to cpu tensor
	hirtCmdQueueSync(pCmdQueue);
	
	hirtCmdQueueDestroy(pCmdQueue);

	himlMemcpyTensorToHost(
	    pTensorOutputGpu, 
		gmemOutputGpu, 
		pTensorOutputCpu, 
		outputDataCpu);
	//himlDumpTensor2File("mlu_output", pTensorOutputCpu, himl_TENSOR, outputDataCpu, false);
/*
	himlCpuComputeConvOpForward(pParamConv, 
		pTensorInputCpu, 
		inputDataCpu, 
		pTensorWeightCpu, 
		weightDataCpu, 
		pTensorOutputCpu, 
		resultCpu, 
		pTensorBiasCpu, 
		biasDataCpu);
*/
	//himlDumpTensor2File("cpu_output", pTensorOutputCpu, himl_TENSOR, resultCpu, false);

	// delete conv param
	himlDestroyConvOpParam(pParamConv);

	// delete himl tensors
	himlDestroyGpuTensor(pTensorInputGpu);
	himlDestroyGpuTensor(pTensorWeightGpu);
	himlDestroyGpuTensor(pTensorBiasGpu);
	himlDestroyGpuTensor(pTensorOutputGpu);
	// delete cpu tensors
	himlDestroyCpuTensor(pTensorInputCpu);
	himlDestroyCpuTensor(pTensorWeightCpu);
	himlDestroyCpuTensor(pTensorBiasCpu);
	himlDestroyCpuTensor(pTensorOutputCpu);
	//himlDestroyBaseOp(&conv_op);

	//	delete pointers(including data pointers)
	hisdkFree(inputDataCpu);
	hisdkFree(weightDataCpu);
	hisdkFree(biasDataCpu);
	hisdkFree(outputDataCpu);
	hisdkFree(resultCpu);

#endif
    HISDK_LOG_INFO(LOG_SYSTEM, "example_hirt_model program end...");

fail:

	return 0;
}

