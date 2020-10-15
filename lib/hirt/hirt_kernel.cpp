#include "hirt_kernel.h"
#include "hirt_mm.h"


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
hisdkRet_t hirtKernelParamsBufferCreate(hirtKernelParamsBuffer_t **ppParams)
{
    hirtKernelParamsBuffer_t *pBuf = NULL;
    hisdkRet_t ret = HISDK_RET_SUCCESS;

    pBuf = (hirtKernelParamsBuffer_t *)hisdkAlloc(sizeof(hirtKernelParamsBuffer_t));
    if(pBuf == NULL)
    {
        ret = HISDK_RET_ERR_INSUFFICIENTMEMORY;
        goto fail;
    }
    hisdkMemset(pBuf, 0, sizeof(hirtKernelParamsBuffer_t));
    pBuf->max_param = HIRT_PARAMBUF_MAXSIZE;

    pBuf->pbuf_host = (void*)hisdkAlloc(pBuf->max_param);
    if(pBuf->pbuf_host == NULL)
    {
        ret = HISDK_RET_ERR_INSUFFICIENTMEMORY;
        goto fail;
    }
    hisdkMemset(pBuf->pbuf_host, 0, pBuf->max_param);
    
    if(hirtGpuMalloc(&pBuf->pbuf_gpu, HIRT_PARAMBUF_MAXSIZE) != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_INSUFFICIENTMEMORY;
        goto fail;
    }

    *ppParams = pBuf;
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtKernelParamsBufferCreate done.");
    return ret;

fail:
    if(pBuf->pbuf_gpu != 0)
    {
        hirtGpuFree(pBuf->pbuf_gpu);
    }

    if(pBuf->pbuf_host != NULL)
    {
        hisdkFree(pBuf->pbuf_host);
    }
    
    if(pBuf != NULL)
    {
        hisdkFree(pBuf);
    }
    
    *ppParams = NULL;
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtKernelParamsBufferCreate failed.");
    return ret;
}

__R_HOST
hisdkRet_t hirtKernelParamsBufferDestroy(hirtKernelParamsBuffer_t *pParams)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtKernelParamsBufferDestroy done.");
    return ret;
}

__R_HOST
hisdkRet_t hirtKernelParamsBufferAddParam(hirtKernelParamsBuffer_t *pParams, 
      void *data, size_t nBytes)
{
    if( (pParams->cur_param+nBytes) > pParams->max_param )
    {
        return HISDK_RET_ERR_MEMCPY;
    }

    memcpy((void*)((uint64_t)pParams->pbuf_host+pParams->cur_param), data, nBytes);
    return HISDK_RET_SUCCESS;
}

__R_HOST
hisdkRet_t hirtKernelParamsBufferAddPlaceHolder(hirtKernelParamsBuffer_t *pParams, 
      size_t nBytes)
{
    pParams->cur_param += nBytes;
    return HISDK_RET_SUCCESS;
}


__R_HOST static
hisdkRet_t hirtLoadKernelFromFile(const char *kernel_filename, unsigned char **ppbuf, size_t *pSize)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t rc;
    hisdkFile_t *file = NULL;
    hisdkStatType_t   finfo;
    size_t file_size;
    unsigned char *pbuf = NULL;
    size_t actually_read = 0;

    if(strlen(kernel_filename) == 0)
    {
        ret = HISDK_RET_ERR_FILEOPERATIONFAILED;
        goto fail;
    }

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtLoadKernelFromFile file=%s", kernel_filename);
    
    rc = hisdkPortOsFopen(kernel_filename, HISDK_OPEN_READ, &file);
    if(rc != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FILENOTFOUND;
        goto fail;
    }

    rc = hisdkPortOsFstat(file, &finfo);
    if(rc != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FILEOPERATIONFAILED;
        goto fail;
    }

    file_size = hisdkPortOsStatGetSize(&finfo);
    if(!file_size)
    {
        ret = HISDK_RET_ERR_FILEOPERATIONFAILED;
    }

    pbuf = (unsigned char*)hisdkAlloc(file_size);
    if(pbuf == NULL)
    {
        ret = HISDK_RET_ERR_NOMEM;
        goto fail;
    }

    hisdkPortOsFseek(file, 0, HisdkSeek_Set);

    rc = hisdkPortOsFread(file, pbuf, file_size, &actually_read);
    if(rc != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FILEOPERATIONFAILED;
        goto fail;
    }

    hisdkPortOsFclose(file);
    if(actually_read != file_size)
    {
        ret = HISDK_RET_ERR_FILEOPERATIONFAILED;
        goto fail;
    }

    *ppbuf = pbuf;
    *pSize = file_size;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtLoadKernelFromFile done, size=%zu", file_size);
    return ret;
    
fail:
    if(pbuf != NULL)
    {
        hisdkFree(pbuf);
    }

    if(file != NULL)
    {
        hisdkPortOsFclose(file);
    }

    *ppbuf = NULL;
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtLoadKernelFromFile failed.");
    return ret;
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
hisdkRet_t hirtInvokeKernel(const char* function, 
      hirtKernelParamsBuffer_t *pParams, hirtKernelBinBuffer_t **ppKernelBin, 
      hirtTaskDim_t dim, hirtCmdQueue_t *pQueue)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t rc;
    unsigned char* pbuf;
    size_t filesize;
    size_t param_size;
    hirtKernelBinBuffer_t *pKernelBin = NULL;
    void *pKernelGdramPtr;

    //alloc kernelbin struct
    pKernelBin = (hirtKernelBinBuffer_t *)hisdkAlloc(sizeof(hirtKernelBinBuffer_t));
    if(pKernelBin == NULL)
    {
        ret = HISDK_RET_ERR_NOMEM;
        goto fail;
    }
    hisdkMemset(pKernelBin, 0, sizeof(hirtKernelBinBuffer_t));

    //get kernelfile size
    rc = hirtLoadKernelFromFile(function, &pbuf, &filesize);
    if(rc != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_FILENOTFOUND;
        goto fail;
    }
    
    //hisdkAlloc kernel gdram in device memory
    pKernelBin->size = filesize;
    rc = hirtGpuMalloc(&pKernelBin->pbuf_gpu, pKernelBin->size);
    if(rc != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_NODEV;
        goto fail;
    }
    pKernelBin->pbuf_host = pbuf;
    //fill some param in parambuf(dim)

    //hirtCmdQueueKernelPut
    hirtCmdQueueKernelPut(pQueue, pParams, pKernelBin, dim);

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtInvokeKernel done.");
    *ppKernelBin = pKernelBin;
    return ret;
    
fail:
    if(pKernelBin->pbuf_gpu != 0)
    {
        hirtGpuFree(pKernelBin->pbuf_gpu);
    }
    
    if(pKernelBin != NULL)
    {
        hisdkFree(pKernelBin);
    }

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtInvokeKernel failed.");
    *ppKernelBin == NULL;
    return ret;
}