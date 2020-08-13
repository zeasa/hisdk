#include "libhirt.h"

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