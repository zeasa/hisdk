#ifndef LIBHIRT_KERNEL_H__
#define LIBHIRT_KERNEL_H__

__R_HOST
hirtRet_t hirtAllocKernelParamsBuffer(hirtKernelParamsBuffer_t **pParams);
__R_HOST
hirtRet_t hirtKernelParamsBufferAddParam(hirtKernelParamsBuffer_t *params, 
      void *data, size_t nBytes);
__R_HOST
hirtRet_t hirtKernelParamsBufferAddPlaceHolder(hirtKernelParamsBuffer_t *params, size_t nBytes);
__R_HOST
hirtRet_t hirtDestroyKernelParamsBuffer(hirtKernelParamsBuffer_t params);
__R_HOST
hirtRet_t hirtInvokeKernel(const hirtKernelFunction_t *function, hirtTaskDim_t dim,
      hirtKernelParamsBuffer_t *pparams, hirtKernelBinBuffer_t **pKernelBin, hirtCmdQueue_t *pqueue);

#endif /*LIBHIRT_FIFO_H__*/

