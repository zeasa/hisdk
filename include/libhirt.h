#ifndef __LIBHIRT_H__
#define __LIBHIRT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define HIRT_DEVICE_NODE      "/dev/dri/renderD128"

#if defined(__cplusplus)
extern "C" {
#endif /*__cplusplus*/

/**< Code will be compiled for running on Host */
#ifndef __R_HOST
#define __R_HOST   // something like __attribute__ or #pragma
#endif /*__R_HOST*/

/**< Code will be compiled for running on MLU */
#ifndef __R_MLU
#define __R_MLU
#endif /*__R_MLU*/

typedef uint64_t u64_t;
typedef int64_t  i64_t;
typedef uint64_t hirt_u64_t;
typedef int64_t  hirt_i64_t;
typedef uint32_t u32_t;
typedef uint16_t u16_t;
typedef uint8_t  u8_t;
typedef int32_t  i32_t;
typedef int16_t  i16_t;
typedef int8_t   i8_t;
typedef int      bool_t;
typedef u64_t    hirt_size_t;

/**< Error codes */
typedef enum 
{
  HIRT_RET_SUCCESS                 = 0,      /**< No error */
  HIRT_RET_WARNING_FAKE_DEVICE     = 1,      /**< Use fake device */
  HIRT_RET_ERR_INVALID             = 632007, /**< Invalid argument */
  HIRT_RET_ERR_NOMEM               = 632008, /**< Out of memory */
  HIRT_RET_ERR_NODEV               = 632009, /**< No such device */
  HIRT_RET_ERR_IO                  = 632010, /**< I/O error */
  HIRT_RET_ERR_SYS                 = 632011, /**< System error */
  HIRT_RET_ERR_ACCES               = 632012, /**< Permission denied */
  HIRT_RET_ERR_FAULT               = 632013, /**< Bad address */
  HIRT_RET_ERR_BUSY                = 632014, /**< Device or resource busy */
  HIRT_RET_ERR_TIMEOUT             = 632015, /**< Time expired */
  HIRT_RET_ERR_EXIST               = 632016, /**< Resource or file already exists */
  HIRT_RET_ERR_NOSYS               = 632017, /**< Function not implemenmted */
  HIRT_RET_ERR_AGAIN               = 632018, /**< try again later */
  HIRT_RET_ERR_NORES               = 632019, /**< Out of resource */
  HIRT_RET_ERR_UNSUPPORTED         = 632020, /**< Unsupported operation */
  HIRT_RET_ERR_INVALID_POINTER     = 632021, /**< Invalid pointer */
  HIRT_RET_ERR_NO_EXIST            = 632022, /**< Resource or file doesn't exist */
  HIRT_RET_ERR_BROKEN              = 632023, /**< Data transmission is broken */
  HIRT_RET_ERR_INIT                = 632024, /**< Uninitialized */
  HIRT_RET_ERR_STREAM              = 632025, /**< Failure on Stream */
  HIRT_RET_ERR_OUT_RANGE           = 632026, /**< Number out of range */
  HIRT_RET_ERR_MATH_OVERFLOW       = 632027, /**< Math result not representable */
  HIRT_RET_ERR_FUNC_CALL           = 632028, /**< Failure to call runtime functions */
  HIRT_RET_ERR_UNHANDLED           = 632029, /**< Unhandled error */
  HIRT_RET_ERR_INVALID_TYPE        = 632030, /**< Invalid type */
  HIRT_RET_ERR_INVALID_OP          = 632031, /**< Invalid operation */
  HIRT_RET_ERR_MLU                 = 632032, /**< MLU error */
  HIRT_RET_ERR_ONCHIP_CORE         = 632033, /**< Onchip core error */
  HIRT_RET_ERR_EVENT               = 632034, /**< Failure on event operation */
  HIRT_RET_ERR_RESHAPE             = 632035, /**< Failure on data reshape */
  HIRT_RET_ERR_MEMCPY              = 632036, /**< Failure on memory copy */
  HIRT_RET_ERR_ENCRYPT             = 632037, /**< Failure on encrypt */
  HIRT_RET_ERR_INVALID_DATADESC    = 632038, /**< Invalid data descriptor */
  HIRT_RET_ERR_FILEWRITEFAILED     = 632039, /**< FileWriteFailed      */
  HIRT_RET_ERR_FILEREADFAILED      = 632040, /**< FileReadFailed       */
  HIRT_RET_ERR_ENDOFFILE           = 632041, /**< EndOfFile            */
  HIRT_RET_ERR_FILEOPERATIONFAILED = 632042, /**< FileOperationFailed  */
  HIRT_RET_ERR_DIROPERATIONFAILED  = 632043, /**< DirOperationFailed   */
  HIRT_RET_ERR_ENDOFDIRLIST        = 632044, /**< EndOfDirList         */
  HIRT_RET_ERR_IOCTLFAILED         = 632045, /**< IoctlFailed          */
  HIRT_RET_ERR_PATHALREADYEXISTS   = 632046, /**< PathAlreadyExists    */
  HIRT_RET_ERR_SURFACENOTSUPPORTED = 632047, /**< SurfaceNotSupported  */
  HIRT_RET_ERR_NOTIMPLEMENTED      = 632048, /**< NotImplemented       */
  HIRT_RET_ERR_NOTINITIALIZED      = 632050, /**< NotInitialized       */
  HIRT_RET_ERR_BADPARAMETER        = 632051, /**< BadParameter         */
  HIRT_RET_ERR_INSUFFICIENTMEMORY  = 632053, /**< InsufficientMemory   */
  HIRT_RET_ERR_READONLYATTRIBUTE   = 632054, /**< ReadOnlyAttribute    */
  HIRT_RET_ERR_INVALIDSTATE        = 632055, /**< InvalidState         */
  HIRT_RET_ERR_INVALIDADDRESS      = 632056, /**< InvalidAddress       */
  HIRT_RET_ERR_INVALIDSIZE         = 632057, /**< InvalidSize          */
  HIRT_RET_ERR_BADVALUE            = 632058, /**< BadValue             */
  HIRT_RET_ERR_ALREADYALLOCATED    = 632059, /**< AlreadyAllocated     */
  HIRT_RET_ERR_MODULENOTPRESENT    = 632060, /**< ModuleNotPresent     */
  HIRT_RET_ERR_RESOURCEERROR       = 632061, /**< ResourceError        */
  HIRT_RET_ERR_COUNTMISMATCH       = 632062, /**< CountMismatch        */
  HIRT_RET_ERR_OVERFLOW            = 632063, /**< OverFlow             */
  HIRT_RET_ERR_DISCONNECTED        = 632064, /**< Disconnected         */
  HIRT_RET_ERR_FILENOTFOUND        = 632065, /**< FileNotFound         */
  HIRT_RET_ERR_TESTAPPFAILED       = 632066, /**< TestAppFailed        */
  HIRT_RET_ERR_DEVICENOTFOUND      = 632067, /**< DeviceNotFound       */
  HIRT_RET_ERR_FIFOEMPTY           = 632068, /**< fifoempty            */
  HIRT_RET_ERR_FIFOFULL            = 632069, /**< fifofull             */
  HIRT_RET_ERR_CREATETHREAD        = 632070, /**< createthread         */
  HIRT_RET_ERR_UNKNOWN             = 999991, /**< Unknown error */
  HIRT_RET_ERR_MAX,                          /**< The last one */
} hirtRet_t;

/**< Direction of data transmission */
typedef enum {
  HIRT_MEM_TRANS_DIR_HOST2DEV = 0,   /**< Host to Device*/
  HIRT_MEM_TRANS_DIR_DEV2DEV,        /**< Device to Device */
  HIRT_MEM_TRANS_DIR_DEV2HOST,       /**< Device to Host */
  HIRT_MEM_TRANS_DIR_HOST2HOST,      /**< Host to Host */
} hirtMemTransDir_t;

/**< Version and revision  */
typedef struct {
  u32_t   hirtPlatformVersion;     /**< platform version */
  u32_t   hirtPlatformRevision;    /**< platform revision */
  u32_t   hirtCoreVersion;         /**< core version */
  u32_t   hirtCoreRevision;        /**< core revision */
} hirtHardwareVersion_t;

/**< Device capabilities for compatibilities  */
typedef struct {
  u32_t   hirtCapsNumberOfCore;           /**< Number of cores */
  u32_t   hirtCapsMaxNumberOfStream;      /**< Maximum of stream that user can create */
} hirtCap_t;

/**< Data type and data order*/
typedef enum hirtDataType {
  HIRT_INVALID = 0x0,
  HIRT_FLOAT16 = 0x12,
  HIRT_FLOAT32 = 0x13,
  HIRT_FLOAT64 = 0x14,
  HIRT_INT8    = 0x21,
  HIRT_INT16   = 0x22,
  HIRT_INT32   = 0x23,
  HIRT_UINT8   = 0x31,
  HIRT_UINT32  = 0x33,
  HIRT_FIX8    = 0x41,
  HIRT_QUANT8  = 0x51,
  HIRT_BOOL    = 0x61,
} hirtDataType_t;

typedef enum hirtDimOrder {
  HIRT_NCHW = 0x0123,
  HIRT_NHWC = 0x0231,
} hirtDimOrder_t;

typedef int hirtTaskDim_t;

typedef unsigned long hirtDev_t;

typedef u64_t hirtGMemAddress_t;
#define MAKE_DEVADDR(nodenoc, low32)    ((((u64_t)nodenoc) << 32) + low32)
#define MAKE_DEVADDR_BLK(nodenoc, blk)  ((((u64_t)nodenoc) << 32) + blk*HIRT_HIPU200_MEM_BLK_SIZ)

/**< Compiler */
/**< hirt.h */

#define HIRT_CMDQUEUE_SIZMAX            (1024)
#define HIRT_PARAMBUF_MAXSIZE           (1024)
#define HIRT_HIPU200_CORENUMMAX         (13)

#define HIRT_HIPU200_MEM_CH_NUM         (2)
#define HIRT_HIPU200_MEM_CH_SIZ         (4096*1024*1024)
#define HIRT_HIPU200_MEM_BLK_SIZ        (64*1024*1024)
#define HIRT_HIPU200_MEM_BLK_NUM        (HIRT_HIPU200_MEM_CH_SIZ/HIRT_HIPU200_MEM_BLK_SIZ)

#define HIRT_HIPU200_MEM_CH0_NOCADDR    (0)
#define HIRT_HIPU200_MEM_CH1_NOCADDR    (1)

const int HIRT_HIPU200_MEM_CH_NOCADDR_TBL[HIRT_HIPU200_MEM_CH_NUM] = 
{
    HIRT_HIPU200_MEM_CH0_NOCADDR,
    HIRT_HIPU200_MEM_CH1_NOCADDR
};

/**< hirt_internal.h */
typedef struct hirtKernelParamsBuffer {
  void *pbuf_host;
  void *pbuf_dev;
  unsigned int max_param;
  unsigned int cur_param;
} hirtKernelParamsBuffer_t;

typedef struct hirtKernelBinBuffer
{
  void *pbuf_host;
  void *pbuf_dev;
  unsigned int size;
} hirtKernelBinBuffer_t;

typedef struct hirtKernelFunction {
  u64_t function_start;
  u64_t function_end;
  u64_t function_size;
} hirtKernelFunction_t;

typedef struct hirtKernelGramAllocInfo
{
  hirtGMemAddress_t base;
  size_t size;
} hirtKernelGMemAllocInfo_t;

typedef struct hirtKernelTaskParam
{
  int task_parallelism;
  int task_thread_coreid[HIRT_HIPU200_CORENUMMAX];
  hirtKernelGMemAllocInfo_t mem_input;
  hirtKernelGMemAllocInfo_t mem_output;
  hirtKernelGMemAllocInfo_t mem_fm;
  hirtKernelGMemAllocInfo_t mem_weight;
  hirtKernelGMemAllocInfo_t mem_bias;
  hirtKernelGMemAllocInfo_t mem_lut;
} hirtKernelTaskParam_t;

typedef void (*hirtThreadFunction_t)(void *arg);

#ifdef NDEBUG
#define HIRT_ASSERT(cond) do {} while (0);
#else
#define HIRT_ASSERT(cond) if (!(cond)) { \
    std::cout << "Error occur: " << __func__ << __LINE__; \
    exit(0); }
#endif

#define ERROR_CHECK(ret) if (ret != hirt_RET_SUCCESS) {\
    printf("error occur, func: %s, line: %d\n", __func__, __LINE__);\
    exit(0);\
}

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
extern __R_HOST const char *
hirtGetErrorStr(hirtRet_t errCode);

/**
 * @brief Get the error code set by any runtime calls.
 *     Its value is meaningful only when the return value indicating an error.
 *
 * @return error code of the last call of runtime functions.
 */
extern __R_HOST
hirtRet_t hirtGetLastErr(void);

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
extern __R_HOST
hirtRet_t hirtInit(unsigned int Flags);

/**
 * @brief Destroy everything that allocated by runtime API calls.
 *
 * This API should be called after any other runtime API calls.
 *
 * @return void (None).
 */
extern __R_HOST
void hirtDestroy(void);

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
extern __R_HOST
hirtRet_t hirtGetHardwareVersion(hirtHardwareVersion_t *ver);

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

extern __R_HOST
hirtRet_t hirtGetDeviceHandle(hirtDev_t *dev, int ordinal);

/**
 * @brief Set the device handle for current thread execution context.
 *
 *  It implies that any subsequent runtime API calls are for this device.
 *
 * @param  dev[in] the device handle.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtSetCurrentDevice(hirtDev_t dev);

/**
 * @brief Get the hirtDevice handle from current thread execution context.
 *
 * The handle has been set by calling hirtSetCurrentDevice().
 *
 * @param  dev[out] pointer to retrieve the device handle.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtGetCurrentDevice(hirtDev_t *dev);

/**
 * @brief Get capability of a device which is specified by device handle.
 *
 *  The function returns the device capabilities.
 *
 * @param  cap[out] pointer to retrieve the device capability.
 * @param  dev[in] the device handle to get the device capability.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtGetDeviceCapability(hirtCap_t *cap, hirtDev_t dev);

/**
 * @brief Get the number of MLU devices in the system.
 *
 * @param  devnum[out] pointer to retrieve the number of devices.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtGetDeviceCount(unsigned *devnum);

/**
 * @brief Get the total memory size in MByte.
 *
 * @param  sz[out] pointer to retrieve the memory amount on device.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtGetDeviceMemorySize(unsigned long *sz);

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
extern __R_HOST
hirtRet_t hirtGetKernelParamsBuffer(hirtKernelParamsBuffer_t **pParams);

/**
 * @brief Add a parameter to a specific parameter buffer.
 *
 * @param params[in] destination parameter buffer
 * @param data[in] pointer to host memory
 * @param nBytes[in] size in bytes
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtKernelParamsBufferAddParam(hirtKernelParamsBuffer_t
      params, void *data, size_t nBytes);

/**
 * @brief Destroy a parameter buffer returned by hirtGetKernelParamsBuffer.
 *
 * @param params[in] pointer to a param buffer
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtDestroyKernelParamsBuffer(hirtKernelParamsBuffer_t params);

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
extern __R_HOST
hirtRet_t hirtInvokeKernel(const void *function, hirtDim3_t dim,
      hirtKernelParamsBuffer_t params, hirtFunctionType_t c, hirtStream_t stream);

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
extern __R_HOST
hirtRet_t hirtMallocHost(void **pPtr, size_t nBytes, hirtMemType_t type);

/**
 * @brief Free the memory space pointed by ptr, which must be
 *        returned by a previous call of hirtMallocHost.
 *
 * @param ptr[in]  point to the address of memory to be free.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtFreeHost(void *ptr);

/**
 * @brief Allocate memory on MLU device.
 *
 * @param pPtr[out] a pointer to pointer for retrieving allocated device memory.
 * @param nBytes[in] allocate size.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtMalloc(void **pPtr, size_t nBytes);

/**
 * @brief Deallocate MLU device Memory.
 *
 * @param ptr[in] point to the memory to be free.
 * @return hirt_RET_SUCCESS if success,
 *         otherwise the error code is returned.
 */
extern __R_HOST
hirtRet_t hirtFree(void *ptr);

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
extern __R_HOST
hirtRet_t hirtMemcpy(void *dest, const void *src, size_t nBytes, hirtMemTransDir_t dir);



#if defined(__cplusplus)
}
#endif /*__cplusplus*/
#endif /*__LIBHIRT_H__*/