#ifndef LIBHIRT_KERNEL_H__
#define LIBHIRT_KERNEL_H__

#include "hirt.h"
#include "hirt_cqueue.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

__R_HOST
hisdkRet_t hirtKernelParamsBufferCreate(hirtKernelParamsBuffer_t **ppParams);
__R_HOST
hisdkRet_t hirtKernelParamsBufferDestroy(hirtKernelParamsBuffer_t *pParams);
__R_HOST
hisdkRet_t hirtKernelParamsBufferAddParam(hirtKernelParamsBuffer_t *pParams, 
    void *data, size_t nBytes);
__R_HOST
hisdkRet_t hirtKernelParamsBufferAddPlaceHolder(hirtKernelParamsBuffer_t *pParams, size_t nBytes);
__R_HOST
hisdkRet_t hirtInvokeKernel(const char* function, 
      hirtKernelParamsBuffer_t *pParams, hirtKernelBinBuffer_t **ppKernelBin, 
      hirtTaskDim_t dim, hirtCmdQueue_t *pQueue);
__R_HOST
hisdkRet_t hirtKernelBinBufferDestroy(hirtKernelBinBuffer_t *pKernelBin);

#ifdef __cplusplus
}
#endif
#endif /*LIBHIRT_FIFO_H__*/

