#include "libhirt.h"
#include "libhirt_cqueue.h"

/*********************************************************************************
 * Execution control
 *********************************************************************************/
/**
 * @brief Get a parameter buffer for hirtInvokeKernel.
 *
 * @param params[in] pointer to a param buffer
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtAllocKernelParamsBuffer(hirtKernelParamsBuffer_t **pParams)
{
    hirtKernelParamsBuffer_t *pBuf = NULL;
    hirtRet_t ret = HIRT_RET_SUCCESS;

    pBuf = (hirtKernelParamsBuffer_t *)malloc(sizeof(hirtKernelParamsBuffer_t);
    if(pBuf == NULL)
    {
        ret = HIRT_RET_ERR_INSUFFICIENTMEMORY;
        goto freememhost;
    }
    memset(pBuf, 0, sizeof(hirtKernelParamsBuffer_t));
    pBuf->max_param = HIRT_PARAMBUF_MAXSIZE;

    pBuf->pbuf_host = (void*)malloc(HIRT_PARAMBUF_MAXSIZE);
    if(pBuf->pbuf_host == NULL)
    {
        ret = HIRT_RET_ERR_INSUFFICIENTMEMORY;
        goto freememhost;
    }

    if(hirtMalloc(&pBuf->pbuf_dev, HIRT_PARAMBUF_MAXSIZE) != HIRT_RET_SUCCESS)
    {
        ret = HIRT_RET_ERR_INSUFFICIENTMEMORY;
        goto freememdev;
    }

    return ret;

freememdev:
    if(pBuf->pbuf_dev != NULL)
    {
        hirtFree(pBuf->pbuf_dev);
    }

freememhost:
    if(pBuf != NULL)
    {
        free(pBuf);
    }
    *pParams = NULL;
    return ret;
}

/**
 * @brief Add a parameter to a specific parameter buffer.
 *
 * @param params[in] destination parameter buffer
 * @param data[in] pointer to host memory
 * @param nBytes[in] size in bytes
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtKernelParamsBufferAddParam(hirtKernelParamsBuffer_t *params, 
      void *data, size_t nBytes)
{
    if( (params->cur_param+nBytes) > params->max_param )
    {
        return HIRT_RET_ERR_MEMCPY;
    }

    memcpy((void*)(params->pbuf_host+params->cur_param), data, nBytes);
    return HIRT_RET_SUCCESS;
}

__R_HOST
hirtRet_t hirtKernelParamsBufferAddPlaceHolder(hirtKernelParamsBuffer_t *params, 
      size_t nBytes)
{
    params->cur_param += nBytes;
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Destroy a parameter buffer returned by hirtGetKernelParamsBuffer.
 *
 * @param params[in] pointer to a param buffer
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtDestroyKernelParamsBuffer(hirtKernelParamsBuffer_t params)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Invoke a kernel written in Bang with given params on MLU.
 *
 * @param function[in] point to the MLU function.
 * @param dim[in]      how many grid dimentions.
 * @param params[in]   point to arguments.
 * @param c[in]        function type. @see hirtFunctionType_t.
 * @param stream[in]   stream associated to the function call.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 *
 * Notes: Layout of function
 * [nBytes, MLU Binary]
 * First 4 bytes ([function, function+ 3]), an unsigned int (nBytes) represents total bytes of MLU binary
 * Following 4 bytes ([function + 4, function +7]), an unsigned int represents core version (defined in hirt_interal.h)
 * Following 56 bytes ([function + 8, function +63]), empty, left for future extension
 * Following nBytes ([function + 64, function + 64 + nBytes - 1]), MLU binary (binary format, not ascii), just memcpy it
 */
__R_HOST
hirtRet_t hirtInvokeKernel(const hirtKernelFunction_t *function, hirtTaskDim_t dim,
      hirtKernelParamsBuffer_t *pparams, hirtKernelBinBuffer_t **pKernelBin, hirtCmdQueue_t *pqueue)
{
    hirtKernelBinBuffer_t *pKernel;
    size_t param_size;
    void *pKernelGdramPtr;

    //alloc kernelbin struct
    pKernel = (hirtKernelBinBuffer_t *)malloc(sizeof(hirtKernelBinBuffer_t));
    *pKernelBin = pKernel;

    //get kernelfile size
    //malloc kernel gdram in device memory
    pKernel->size = function->function_end - function->function_start;
    hirtMalloc(&pKernel->pbuf_dev, pKernel->size);
    
    //fill some param in parambuf(dim)

    //libhirt_cmdqueue_kernel_put
    libhirt_cmdqueue_kernel_put(pqueue, pparams, pKernel, dim);
}