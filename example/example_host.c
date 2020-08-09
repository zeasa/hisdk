#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libhirt.h"
#include <kernel.h>

extern const char _example_kernel_start[];
extern const char _example_kernel_end[];
extern const char _example_kernel_size[];

const hirtKernelFunction_t kernel =
{
    _example_kernel_start,
    _example_kernel_end,
    _example_kernel_size
};

int main(void)
{
    const hirtKernelFunction_t kernel =
    {
        _example_kernel_start,
        _example_kernel_end,
        _example_kernel_size
    };

    ERROR_CHECK(hirtInit(0));
    hirtDev_t dev;
    hirtGetDeviceHandle(&dev, 0);
    hirtSetCurrentDevice(dev);

    int a, b, out;
    srand((unsigned)time(NULL));
    a = (int)rand();
    b = (int)rand();

    hirtQueue_t queue;
    hirtCreateQueue(&queue);

    int *k_a, *k_b;
    hirtMalloc((void **)&k_a, sizeof(int));
    hirtMalloc((void **)&k_b, sizeof(int));
    hirtMemcpy(k_a, &a, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2DEV);
    hirtMemcpy(k_b, &b, sizeof(int), HIRT_MEM_TRANS_DIR_HOST2DEV);

    hirtKernelParamsBuffer_t *pParams;
    ERROR_CHECK(hirtGetKernelParamsBuffer(&pParams));
    hirtKernelParamsBufferAddParams(pParams, &k_a, sizeof(int *));
    //hirtKernelParamsBufferAddParams(pParams, &k_b, sizeof(int *));


    hirtInvokeKernel_V2(&kernel, 3, pParams, queue);
    hirtSyncQueue(queue);

    hirtMemcpy(&out, k_a, sizeof(int), HIRT_MEM_TRANS_DIR_DEV2HOST);
    hirtFree(k_a);
    hirtFree(k_b);
    hirtDestroyKernelParamsBuffer(pParams);
    hirtDestroyQueue(queue);
    hirtDestroy();
}