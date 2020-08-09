#include "libhirt.h"

static hirtDev_t hirtCurrentDev_g = 0;

/************************************************************************
 * Error handling
 ************************************************************************/
/**
 * @brief Return string pointer that describes
 *     the error code passed in the argument errCode.
 *
 * The function returns a read only string that is corresponding
 * to the argument @p errcode.
 *
 * @param  errCode[in] the error code was returned by previous function call.
 * @return a pointer that points to a constant string.
 */
__R_HOST
const char *hirtGetErrorStr(hirtRet_t errCode)
{
    return "hirtGetErrorStr";
}

/**
 * @brief Get the error code set by any runtime calls.
 *     Its value is meaningful only when the return value indicating an error.
 *
 * @return error code of the last call of runtime functions.
 */
__R_HOST
hirtRet_t hirtGetLastErr(void)
{
    return HIRT_RET_SUCCESS;
}

/*************************************************************************
 * Initialization and destroy
 *************************************************************************/
/**
 * @brief Initialize runtime environment in current process space.
 *
 * Initializes this API must be called before any other runtime API calls.
 *
 * @param  Flags[in] reserved for further use, pass 0 as well.
 * @return hirt_RET_SUCCESS if success, otherwise with the error code.
 */
__R_HOST
hirtRet_t hirtInit(unsigned int Flags)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Destroy everything that allocated by runtime API calls.
 *
 * This API should be called after any other runtime API calls.
 *
 * @return void (None).
 */
__R_HOST
void hirtDestroy(void)
{

}

/******************************************************************************
 * Version and revision
 ******************************************************************************/
/**
 * @brief Return the version of the MLU hardware.
 *
 * Higher version usually offers more features provided by this library.
 *
 * @param  ver[out] pointer to retrieve the version.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtGetHardwareVersion(hirtHardwareVersion_t *ver)
{
    ver->hirtPlatformVersion = 0;
    ver->hirtPlatformRevision = 0;
    ver->hirtCoreVersion = 0;
    ver->hirtCoreRevision = 0;
    return HIRT_RET_SUCCESS;
}

/******************************************************************************
 * Device managment
 ******************************************************************************/
/**
 * @brief Get the device handle by a given device ordinal.
 *
 *  The function returns the device handle given a specific device ordinal.
 *
 * @param  dev[out] pointer to retrieve the device handle.
 * @param  ordinal[in] the device ordinal to get the device handle.
 * @note   the ordinal should be in the range [0~hirtGetDeviceCount() - 1].
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtGetDeviceHandle(hirtDev_t *dev, int ordinal)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Set the device handle for current thread execution context.
 *
 *  It implies that any subsequent runtime API calls are for this device.
 *
 * @param  dev[in] the device handle.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtSetCurrentDevice(hirtDev_t dev)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Get the hirtDevice handle from current thread execution context.
 *
 * The handle has been set by calling hirtSetCurrentDevice().
 *
 * @param  dev[out] pointer to retrieve the device handle.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtGetCurrentDevice(hirtDev_t *dev)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Get capability of a device which is specified by device handle.
 *
 *  The function returns the device capabilities.
 *
 * @param  cap[out] pointer to retrieve the device capability.
 * @param  dev[in] the device handle to get the device capability.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtGetDeviceCapability(hirtCap_t *cap, hirtDev_t dev)
{
    cap->hirtCapsMaxNumberOfStream = 1;
    cap->hirtCapsNumberOfCore = 13;
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Get the number of MLU devices in the system.
 *
 * @param  devnum[out] pointer to retrieve the number of devices.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtGetDeviceCount(unsigned *devnum)
{
    *devnum = 0;
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Get the total memory size in MByte.
 *
 * @param  sz[out] pointer to retrieve the memory amount on device.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtGetDeviceMemorySize(unsigned long *sz)
{
    *sz = 8 * 1024;
    return HIRT_RET_SUCCESS;
}

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
hirtRet_t hirtGetKernelParamsBuffer(hirtKernelParamsBuffer_t **pParams)
{
    hirtKernelParamsBuffer_t *pBuf = NULL;
    hirtRet_t ret = HIRT_RET_SUCCESS;

    pBuf = (hirtKernelParamsBuffer_t *)malloc(sizeof(hirtKernelParamsBuffer_t);
    if(pBuf == NULL)
    {
        ret = HIRT_RET_ERR_INSUFFICIENTMEMORY;
        goto freemem;
    }
    memset(pBuf, 0, sizeof(hirtKernelParamsBuffer_t));
    pBuf->max_param = HIRT_PARAMBUF_MAXSIZE;

    pBuf->host_ptr = (void*)malloc(HIRT_PARAMBUF_MAXSIZE);
    if(pBuf->host_ptr == NULL)
    {
        ret = HIRT_RET_ERR_INSUFFICIENTMEMORY;
        goto freemem;
    }

    return ret;

freemem:
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

    memcpy((void*)(params->host_ptr+params->cur_param), data, nBytes);
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
hirtRet_t hirtInvokeKernel(const hirtKernelFunction_t *function, hirtDim3_t dim,
      hirtKernelParamsBuffer_t params, hirtStream_t stream)
{

}

/*********************************************************************************
 * Memory management
 *********************************************************************************/
/**
 * @brief Allocate nByte bytes and place a pointer to pointer
 *        in pPtr to the allocated host memory. If nBytes is 0, then
 *        hirtMallocHost returns either NULL, or a unique pointer value
 *        that can later be passed to hirtFreeHost.
 *
 * @param pPtr[out]  a pointer to pointer for retrieving allocated host memory.
 * @param nBytes[in] number bytes of memory to be allocated.
 * @param type[in]   memory type to be allocated,
 *                   @see hirt_HOST_MEMORY_TYPE_LOCK and hirt_HOST_MEMORY_TYPE_MAPPED.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtMallocHost(void **pPtr, size_t nBytes, hirtMemType_t type)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Free the memory space pointed by ptr, which must be
 *        returned by a previous call of hirtMallocHost.
 *
 * @param ptr[in]  point to the address of memory to be free.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtFreeHost(void *ptr)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Allocate memory on MLU device.
 *
 * @param pPtr[out] a pointer to pointer for retrieving allocated device memory.
 * @param nBytes[in] allocate size.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtMalloc(void **pPtr, size_t nBytes)
{
    return HIRT_RET_SUCCESS; 
}

/**
 * @brief Deallocate MLU device Memory.
 *
 * @param ptr[in] point to the memory to be free.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtFree(void *ptr)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Copy data from src address to dst address. The copy direction
 *        is specified by input parameter dir. The copy operation is
 *        always performed on current device which is set by hirtSetCurrentDevice.
 *
 * @param dest[in] destination address.
 * @param src[in] source address.
 * @param nBytes[in] number of bytes to be copied.
 * @param dir[in] direction of transfer.
 *                @see  hirt_MEM_TRANS_DIR_HOST2DEV,
 *                      hirt_MEM_TRANS_DIR_DEV2DEV,
 *                      hirt_MEM_TRANS_DIR_DEV2HOST,
 *                      hirt_MEM_TRANS_DIR_HOST2HOST,
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
__R_HOST
hirtRet_t hirtMemcpy(void *dest, const void *src, size_t nBytes, hirtMemTransDir_t dir)
{
    return HIRT_RET_SUCCESS;
}