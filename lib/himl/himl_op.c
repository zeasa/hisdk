#include "himl.h"


hisdkRet_t himlCreateConvOp(
    himlBaseOp_t *op,
    himlConvOpParam_t *param,
    himlGpuTensor_t *inputTensor,
    himlGpuTensor_t *outputTensor,
    himlGpuTensor_t *filterTensor,
    himlGpuTensor_t *biasTensor)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtKernelParamsBuffer_t *pKernelParams;
    const char kernel_symbal[] = "libhikl_op_conv";

    HISDK_LOG_INFO(LOG_DEBUG, "%s", "himlCreateConvOp()");

    /*construct the parambuf*/
    hirtKernelParamsBufferCreate(&pKernelParams);
    hirtKernelParamsBufferAddPlaceHolder(pKernelParams, sizeof(hirtKernelParamTable_t));
    hirtKernelParamsBufferAddParam(pKernelParams, inputTensor,  sizeof(himlGpuTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, outputTensor, sizeof(himlGpuTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, filterTensor, sizeof(himlGpuTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, biasTensor,   sizeof(himlGpuTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, param,        sizeof(himlConvOpParam_t));

    hisdkMemcpy((void*)op->kernel_symbal_name, (void*)kernel_symbal, sizeof(kernel_symbal));

    return e;

fail:
    return e;
}

hisdkRet_t himlComputeConvOpForward(
    himlBaseOp_t *op,
    hirtGMemAddress_t input,
    hirtGMemAddress_t output,
    himlFuncType_t type,
    hirtCmdQueue_t *pQueue)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtKernelBinBuffer_t *kernel_binbuf;

    HISDK_LOG_INFO(LOG_DEBUG, "%s", "himlComputeConvOpForward()");

    /*alloc the kernelbin gmem and load the kernelbin into the gmembin*/
    
    HISDK_ERR_FCALLFAIL( hirtInvokeKernel(op->kernel_symbal_name, op->kernel_parambuf, &kernel_binbuf, type, pQueue) );

    return e;
    
fail:
    return e;
}

