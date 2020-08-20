#include "himl.h"

#define HIPU300_KERNEL_NUM_PERGROUP     (8)
#define HIPU300_CHANEL_NUM_PERGROUP     (8)
#define CALC_CHANNEL_GROUP8(ch)			(((ch-1) >> 3) + 1)			
#define CALC_KERNEL_GROUP8(n)			(((n-1) >> 3) + 1)	


hisdkRet_t himlCreateCpuTensor(himlCpuTensor_t** ppCpu_tensor,
                              himlTensorType_t  tensor_type,
                              himlDataType_t    data_type,
                              himlDataOrder_t   data_order,
                              int n,
                              int c,
                              int h,
                              int w)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    himlCpuTensor_t *pCpu_Tensor;

    pCpu_Tensor = (himlCpuTensor_t*)malloc(sizeof(himlCpuTensor_t));
    *ppCpu_tensor = pCpu_Tensor;

    pCpu_Tensor->tensor_type = tensor_type;
    pCpu_Tensor->data_type   = data_type;
    pCpu_Tensor->data_order  = data_order;
    pCpu_Tensor->shape.n = n;
    pCpu_Tensor->shape.c = c;
    pCpu_Tensor->shape.h = h;
    pCpu_Tensor->shape.w = w;
    pCpu_Tensor->haddr = (void*)0;
    pCpu_Tensor->bufsize = 0;
    
    return ret;
}

hisdkRet_t himlDestroyCpuTensor(himlCpuTensor_t* pCpu_tensor)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;

    if(pCpu_tensor->haddr != 0)
    {
        free(pCpu_tensor->haddr);
    }
    free(pCpu_tensor);

    return ret;
}

hisdkRet_t himlCreateDevTensor(himlDevTensor_t** ppDev_tensor,
                              himlTensorType_t  tensor_type,
                              himlDataType_t    data_type,
                              himlDataOrder_t   data_order,
                              int n,
                              int c,
                              int h,
                              int w)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    himlDevTensor_t *pDev_Tensor;

    pDev_Tensor = (himlDevTensor_t*)malloc(sizeof(himlDevTensor_t));
    *ppDev_tensor = pDev_Tensor;

    pDev_Tensor->tensor_type = tensor_type;
    pDev_Tensor->data_type   = data_type;
    pDev_Tensor->data_order  = data_order;
    pDev_Tensor->shape.n = n;
    pDev_Tensor->shape.c = c;
    pDev_Tensor->shape.h = h;
    pDev_Tensor->shape.w = w;
    pDev_Tensor->gmemaddr = (gaddr_t)0;
    pDev_Tensor->bufsize = 0;

    return ret;
}

hisdkRet_t himlDestroyDevTensor(himlDevTensor_t* pDev_tensor)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;

    hirtFree(pDev_tensor->gmemaddr);
    
    free(pDev_tensor);

    return ret;
}

u32_t himlCalcShapeBufsize(shape_t *pShape)
{
    return CALC_KERNEL_GROUP8(pShape->n) * HIPU300_KERNEL_NUM_PERGROUP * 
           CALC_CHANNEL_GROUP8(pShape->c) * HIPU300_CHANEL_NUM_PERGROUP *
           pShape->h * pShape->w;
}

// malloc himl tensor
hisdkRet_t himlMallocBuffer(himlDevTensor_t *tensor_himl)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    u32_t bufsize;
    gaddr_t gmem_addr;

    bufsize = himlCalcShapeBufsize(&tensor_himl->shape);

    e = hirtMalloc(&gmem_addr, bufsize);
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_NOMEM; 
    }

    tensor_himl->gmemaddr = gmem_addr;
    tensor_himl->bufsize = bufsize;
    return ret;
}
        
// copy input to himl tensor
hisdkRet_t himlMemcpyTensorToDevice(
    himlCpuTensor_t *tensor_cpu,
    himlDevTensor_t *tensor_dev)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t rc;
    
    rc = hirtMemcpy(tensor_dev->gmemaddr , tensor_cpu->haddr, tensor_dev->bufsize, HIRT_MEM_TRANS_DIR_HOST2DEV);
    if(rc != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_MEMCPY;
    }
    
    return ret;
}

// copy input to himl tensor
hisdkRet_t himlMemcpyTensorToHost(
    himlCpuTensor_t *tensor_cpu,
    himlDevTensor_t *tensor_dev)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
    hisdkRet_t e;
    
    e = hirtMemcpy(tensor_cpu->haddr, tensor_dev->gmemaddr, tensor_dev->bufsize, HIRT_MEM_TRANS_DIR_DEV2HOST);
    if(e != HISDK_RET_SUCCESS)
    {
        ret = HISDK_RET_ERR_MEMCPY;
    }
    
    return ret;
}

hisdkRet_t himlDumpTensor2File(const char *filename, himlCpuTensor_t *tensor_cpu)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;

    HISDK_LOG_INFO(LOG_SYSTEM, "%s", filename);

fail:
    return e;
}


