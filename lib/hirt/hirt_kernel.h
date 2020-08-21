#ifndef LIBHIRT_KERNEL_H__
#define LIBHIRT_KERNEL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

__R_HOST
hisdkRet_t hirtAllocKernelParamsBuffer(hirtKernelParamsBuffer_t **pParams);
__R_HOST
hisdkRet_t hirtKernelParamsBufferAddParam(hirtKernelParamsBuffer_t *params, 
    void *data, size_t nBytes);
__R_HOST
hisdkRet_t hirtKernelParamsBufferAddPlaceHolder(hirtKernelParamsBuffer_t *params, size_t nBytes);
__R_HOST
hisdkRet_t hirtDestroyKernelParamsBuffer(hirtKernelParamsBuffer_t params);
__R_HOST
hisdkRet_t hirtInvokeKernel(const hirtKernelFunction_t *function, hirtTaskDim_t dim,
    hirtKernelParamsBuffer_t *pparams, hirtKernelBinBuffer_t **pKernelBin, hirtCmdQueue_t *pqueue);


#ifdef __cplusplus
}
#endif
#endif /*LIBHIRT_FIFO_H__*/

