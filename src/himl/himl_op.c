#include"himl.h"

hisdkRet_t himlCreateConvOp(
    himlBaseOp_t *op,
    himlConvOpParam_t *param,
    himlDevTensor_t *inputTensor,
    himlDevTensor_t *outputTensor,
    himlDevTensor_t *filterTensor,
    himlDevTensor_t *biasTensor)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hirtKernelParamsBuffer_t *pKernelParams;
    const char kernel_symbal[] = "libhikl_op_conv";

    HISDK_LOG_INFO(LOG_DEBUG, "%s", "himlCreateConvOp()");

    /*construct the parambuf*/
    hirtAllocKernelParamsBuffer(hirtKernelParamsBuffer_t &pKernelParams);
    hirtKernelParamsBufferAddPlaceHolder(pKernelParams, sizeof(hirtKernelParamTable_t));
    hirtKernelParamsBufferAddParam(pKernelParams, inputTensor,  sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, outputTensor, sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, filterTensor, sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, biasTensor,   sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, param,        sizeof(himlConvOpParam_t));

    memcpy(op->kernel_symbal_name, kernel_symbal, sizeof(kernel_symbal));

    return ret;
}

hisdkRet_t himlComputeConvOpForward(
    himlBaseOp_t *op,
    himlDevTensor_t *pTensorInput,
    himlDevTensor_t *pTensorOutput,
    himlFuncType_t type,
    hirtCmdQueue_t *pQueue)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    hirtKernelBinBuffer_t *kernel_binbuf;

    HISDK_LOG_INFO(LOG_DEBUG, "%s", "himlComputeConvOpForward()");

    /*alloc the kernelbin gmem and load the kernelbin into the gmembin*/
    
    e = hirtInvokeKernel(op->kernel_parambuf, type, op->kernel_parambuf, &kernel_binbuf, pQueue);
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FUNC_CALL;
        goto fail;
    }
    
fail:
    return ret;
}

