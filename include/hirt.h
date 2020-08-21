#ifndef __LIBHIRT_H__
#define __LIBHIRT_H__

#include "hisdk.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define HIRT_DEVICE_NODE      "/dev/dri/renderD128"

/**< Code will be compiled for running on Host */
#ifndef __R_HOST
#define __R_HOST   // something like __attribute__ or #pragma
#endif /*__R_HOST*/

/**< Code will be compiled for running on MLU */
#ifndef __R_MLU
#define __R_MLU
#endif /*__R_MLU*/

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
typedef enum {
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

typedef enum {
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

/**< hirt_internal.h */
typedef struct {
  void *pbuf_host;
  void *pbuf_dev;
  unsigned int max_param;
  unsigned int cur_param;
} hirtKernelParamsBuffer_t;

typedef struct {
  void *pbuf_host;
  void *pbuf_dev;
  unsigned int size;
} hirtKernelBinBuffer_t;

typedef struct {
  u64_t function_start;
  u64_t function_end;
  u64_t function_size;
} hirtKernelFunction_t;

typedef struct {
  hirtGMemAddress_t base;
  size_t size;
} hirtKernelGMemAllocInfo_t;

typedef struct
{
  u32_t kernelparambuf_maxsize;
  u32_t task_parallelism;
  u32_t task_thread_coreid[HIRT_HIPU200_CORENUMMAX];
} hirtKernelParamParallel_t;

typedef struct
{
  u32_t dummy;
}hirtKernelParamMemory_t;

typedef struct
{
    hirtKernelParamParallel_t parallel_info;
    hirtKernelParamMemory_t   memory_info;
}hirtKernelParamTable_t;

//typedef struct hirtKernelTaskParam
//{
//  int task_parallelism;
//  int task_thread_coreid[HIRT_HIPU200_CORENUMMAX];
//  hirtKernelGMemAllocInfo_t mem_input;
//  hirtKernelGMemAllocInfo_t mem_output;
//  hirtKernelGMemAllocInfo_t mem_fm;
//  hirtKernelGMemAllocInfo_t mem_weight;
//  hirtKernelGMemAllocInfo_t mem_bias;
//  hirtKernelGMemAllocInfo_t mem_lut;
//} hirtKernelTaskParam_t;

typedef void (*hirtThreadFunction_t)(void *arg);

#ifdef __cplusplus
}
#endif
#endif /*__LIBHIRT_H__*/