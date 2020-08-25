

/* test for primitive operator */
/*
 * define tensors: input, weight, bias and output
 * op name : conv
 * input size:	   ni x ci x hi x wi
 * weight size:    co x ci x kh x kw
 * bias size:	   1 x co x 1 x 1
 * output size:    no x co x ho x wo
*/
#include "himl.h"

int main(int argc, char *argv[])
{
    HISDK_LOG_INFO(LOG_SYSTEM, "example_himl program start...");
    
	unsigned int seed = time(0);

	// prepare data for conv
	const int ni = 1, ci = 64, hi = 32, wi = 32;
	const int no = 1, co = 64, ho = 32, wo = 32;
	const int kh = 3, kw = 3, stride_h = 1, stride_w = 1;
	const int dilation_h = 1, dilation_w = 1;
	const int pad_h = 2, pad_w = 2;

	float * inputDataCpu =(float *)hisdkAlloc(ni * ci * hi * wi * sizeof(float));
	for(int index = 0; index < ni * ci * hi * wi; ++index)
	{
		inputDataCpu[index] =((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
	}

	float * weightDataCpu =(float *)hisdkAlloc(co * ci * kh * kw * sizeof(float));
	for(int index = 0; index < co * ci * kh * kw; ++index)
	{
		weightDataCpu[index] =((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
	}
	
	float * biasDataCpu =(float *)hisdkAlloc(co * sizeof(float));
	for(int index = 0; index < co; ++index)
	{
		biasDataCpu[index] =(rand_r(&seed) % 100 / 100.0) * 128;
	}

	float * outputDataCpu =(float *)hisdkAlloc(no * co * ho * wo * sizeof(float));
	float * resultCpu     =(float *)hisdkAlloc(no * co * ho * wo * sizeof(float));
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
	hirtGMemAddress_t gmemInputGpu  = himlMallocBuffer(pTensorInputGpu);
	hirtGMemAddress_t gmemOutputGpu = himlMallocBuffer(pTensorOutputGpu);
	hirtGMemAddress_t gmemWeightGpu = himlMallocBuffer(pTensorWeightGpu);
	hirtGMemAddress_t gmemBiasGpu   = himlMallocBuffer(pTensorBiasGpu);

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
		1, 
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

    HISDK_LOG_INFO(LOG_SYSTEM, "example_himl program end...");
	return 0;
}

