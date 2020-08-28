#include "himl.h"

#define HIPU300_KERNEL_NUM_PERGROUP     (8)
#define HIPU300_CHANEL_NUM_PERGROUP     (8)
#define CALC_CHANNEL_GROUP8(ch)			(((ch-1) >> 3) + 1)			
#define CALC_KERNEL_GROUP8(n)			(((n-1) >> 3) + 1)	


hisdkRet_t himlCreateCpuTensor(
    himlCpuTensor_t** ppCpu_tensor,
    himlTensorType_t  tensor_type,
    himlDataType_t    data_type,
    himlDataOrder_t   data_order,
    int n,
    int c,
    int h,
    int w)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    himlCpuTensor_t *pCpu_Tensor;

    pCpu_Tensor = (himlCpuTensor_t*)hisdkAlloc(sizeof(himlCpuTensor_t));
    if(pCpu_Tensor == NULL)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "himlCreateCpuTensor allocmem failed.");
    }

    pCpu_Tensor->tensor_type = tensor_type;
    pCpu_Tensor->data_type   = data_type;
    pCpu_Tensor->data_order  = data_order;
    pCpu_Tensor->shape.n = n;
    pCpu_Tensor->shape.c = c;
    pCpu_Tensor->shape.h = h;
    pCpu_Tensor->shape.w = w;
    pCpu_Tensor->bufsize = 0;

    *ppCpu_tensor = pCpu_Tensor;
    return e;

fail:
    *ppCpu_tensor = NULL;
    return e;
}

hisdkRet_t himlDestroyCpuTensor(himlCpuTensor_t* pCpu_tensor)
{
    hisdkFree(pCpu_tensor);

    return HISDK_RET_SUCCESS;
}

hisdkRet_t himlCreateGpuTensor(himlGpuTensor_t** ppDev_tensor,
                              himlTensorType_t  tensor_type,
                              himlDataType_t    data_type,
                              int n,
                              int c,
                              int h,
                              int w)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    himlGpuTensor_t *pDev_Tensor;

    pDev_Tensor = (himlGpuTensor_t*)hisdkAlloc(sizeof(himlGpuTensor_t));
    if(pDev_Tensor == NULL)
    {
        HISDK_ERR_RPTFAIL(HISDK_RET_ERR_NOMEM, "himlCreateGpuTensor allocmem failed.");
    }

    pDev_Tensor->tensor_type = tensor_type;
    pDev_Tensor->data_type   = data_type;
    pDev_Tensor->shape.n = n;
    pDev_Tensor->shape.c = c;
    pDev_Tensor->shape.h = h;
    pDev_Tensor->shape.w = w;
    pDev_Tensor->bufsize = 0;

    *ppDev_tensor = pDev_Tensor;
    return e;

fail:
    *ppDev_tensor = NULL;
    return e;
}

hisdkRet_t himlDestroyGpuTensor(himlGpuTensor_t* pDev_tensor)
{
    hisdkFree(pDev_tensor);

    return HISDK_RET_SUCCESS;
}

static u32_t himlCalcGpuBufsize(shape_t *pShape)
{
    return CALC_KERNEL_GROUP8(pShape->n) * HIPU300_KERNEL_NUM_PERGROUP * 
           CALC_CHANNEL_GROUP8(pShape->c) * HIPU300_CHANEL_NUM_PERGROUP *
           pShape->h * pShape->w;
}

// hisdkAlloc himl tensor
hirtGMemAddress_t himlMallocBuffer(himlGpuTensor_t *tensor_himl)
{
    hisdkRet_t e;
    u32_t bufsize;
    hirtGMemAddress_t gmem_addr;

    bufsize = himlCalcGpuBufsize(&tensor_himl->shape);

    e = hirtGpuMalloc(&gmem_addr, bufsize);
    if(e != HISDK_RET_SUCCESS)
    {
        tensor_himl->bufsize = 0;
        return 0;
    }

    tensor_himl->bufsize = bufsize;
    
    return gmem_addr;
}
        
// copy input to himl tensor
hisdkRet_t himlMemcpyTensorToDevice(
    himlCpuTensor_t  *tensor_cpu,
    void             *tensor_cpu_buf,
    himlGpuTensor_t  *tensor_dev,
    hirtGMemAddress_t tensor_dev_buf)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    
    HISDK_ERR_FCALLFAIL( hirtMemcpy((void*)&tensor_dev_buf, tensor_cpu_buf, tensor_dev->bufsize, HIRT_MEM_TRANS_DIR_HOST2GPU) );

    return e;
    
fail:
    return e;
}

// copy input to himl tensor
hisdkRet_t himlMemcpyTensorToHost(
    himlGpuTensor_t  *tensor_dev,
    hirtGMemAddress_t tensor_dev_buf,
    himlCpuTensor_t  *tensor_cpu,
    void             *tensor_cpu_buf)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    
    HISDK_ERR_FCALLFAIL( hirtMemcpy(tensor_cpu_buf, (void*)&tensor_dev_buf, tensor_dev->bufsize, HIRT_MEM_TRANS_DIR_GPU2HOST) );

    return e;
    
fail:
    return e;
}

hisdkRet_t himlDumpTensor2File(const char *filename, himlCpuTensor_t *tensor_cpu)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;

    HISDK_LOG_INFO(LOG_SYSTEM, "%s", filename);

fail:
    return e;
}


