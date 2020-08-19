#ifndef __HIML_H__
#define __HIML_H__

typedef enum {
    HIML_RET_NODEVICE     = -1,
    HIML_RET_SUCCESS      = 0,
    HIML_RET_DOMAINERR    = 1,
    HIML_RET_INVALIDARG   = 2,
    HIML_RET_LENGTHERR    = 3,
    HIML_RET_OUTOFRANGE   = 4,
    HIML_RET_RANGEERR     = 5,
    HIML_RET_OVERFLOWERR  = 6,
    HIML_RET_UNDERFLOWERR = 7,
    HIML_RET_INVALIDPARAM = 8,
    HIML_RET_BADALLOC     = 9,
    HIML_RET_BADTYPEID    = 10,
    HIML_RET_BADCAST      = 11,
    HIML_RET_UNSUPPORT    = 12
} himlRet_t;

typedef enum
{
  DATATYPE_INT8 = 0,
  DATATYPE_FP32 = 1
} himlDataType_t;

typedef enum
{
  TS_TYPE_TENSOR = 0,
  TS_TYPE_CONST = 1,
  TS_TYPE_WEIGHT = 2
} himlTensorType_t;

typedef enum
{
  TS_FORMAT_NCHW = 0,
  TS_FORMAT_NHWC = 1
} himlDataOrder_t;

typedef enum
{
  FUNCTYPE_C1 = 0,
  FUNCTYPE_C2 = 1,
  FUNCTYPE_C4 = 2,
  FUNCTYPE_C8 = 3,
  FUNCTYPE_CA = 4
} himlFuncType_t;

typedef struct
{
  u32_t dim;
  u32_t n;
  u32_t c;
  union {
    u32_t h;
    u32_t row;
  };
  union {
    u32_t w;
    u32_t col;
  };
} shape_t;

typedef struct
{
  u8_t x_pos;
  u8_t y_pos;
  u32_t inner_addr;
} nocaddr_t;

typedef u64_t gaddr_t;

typedef struct
{
  himlTensorType_t tensor_type;
  himlDataType_t   data_type;
  himlDataOrder_t  data_order;
  shape_t shape;
  
  void *haddr;
  u32_t bufsize;
} himlCpuTensor_t;

typedef struct
{
  himlTensorType_t tensor_type;
  himlDataType_t   data_type;
  himlDataOrder_t  data_order;
  shape_t shape;
  
  gaddr_t *gmemaddr;
  u32_t   bufsize;
} himlDevTensor_t;

typedef struct
{
  u32_t stride_w;
  u32_t stride_h;
  u32_t dilation_w;
  u32_t dilation_h;
  u32_t pad_w;
  u32_t pad_h;
} himlConvOpParam_t;

#define KERNEL_SYMBAL_NAME_MAX      (128)
typedef struct 
{
    char kernel_symbal_name[KERNEL_SYMBAL_NAME_MAX];
    hirtKernelParamsBuffer_t *kernel_parambuf;
} himlBaseOp_t;


himlRet_t himlCreateCpuTensor(himlCpuTensor_t** ppCpu_tensor,
                              himlTensorType_t  tensor_type,
                              himlDataType_t    data_type,
                              himlDataOrder_t   data_order,
                              int n,
                              int c,
                              int h,
                              int w)
{
    himlRet_t ret = HIML_RET_SUCCESS;
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

himlRet_t himlDestroyCpuTensor(himlCpuTensor_t* pCpu_tensor)
{
    himlRet_t ret = HIML_RET_SUCCESS;

    if(pCpu_tensor->haddr != 0)
    {
        free(pCpu_tensor->haddr);
    }
    free(pCpu_tensor);

    return ret;
}

himlRet_t himlCreateDevTensor(himlDevTensor_t** ppDev_tensor,
                              himlTensorType_t  tensor_type,
                              himlDataType_t    data_type,
                              himlDataOrder_t   data_order,
                              int n,
                              int c,
                              int h,
                              int w)
{
    himlRet_t ret = HIML_RET_SUCCESS;
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

himlRet_t himlDestroyDevTensor(himlDevTensor_t* pDev_tensor)
{
    himlRet_t ret = HIML_RET_SUCCESS;

    hirtFree(pDev_tensor->gmemaddr);
    
    free(pDev_tensor);

    return ret;
}


himlRet_t himlCreateConvOpParam(himlConvOpParam_t **ppParam,
                int stride_h,
                int stride_w,
                int dilation_h,
                int dilation_w,
                int pad_h = 0,
                int pad_w = 0)
{
    himlRet_t ret = HIML_RET_SUCCESS;
    himlConvOpParam_t *pParam;

    pParam = (himlConvOpParam_t*)malloc(sizeof(himlConvOpParam_t));
    *ppParam = pParam;

    pParam->stride_h = stride_h;
    pParam->stride_w = stride_w;
    pParam->dilation_h = dilation_h;
    pParam->dilation_w = dilation_w;
    pParam->pad_h = pad_h;
    pParam->pad_w = pad_w;
}

himlRet_t himlDestroyConvOpParam(himlConvOpParam_t *pParam)
{
    himlRet_t ret = HIML_RET_SUCCESS;

    free(pParam);

    return ret;
}

#define HIPU300_KERNEL_NUM_PERGROUP     (8)
#define HIPU300_CHANEL_NUM_PERGROUP     (8)
#define CALC_CHANNEL_GROUP8(ch)			(((ch-1) >> 3) + 1)			
#define CALC_KERNEL_GROUP8(n)			(((n-1) >> 3) + 1)	

u32_t himlCalcShapeBufsize(shape_t *pShape)
{
    return CALC_KERNEL_GROUP8(pShape->n) * HIPU300_KERNEL_NUM_PERGROUP * 
           CALC_CHANNEL_GROUP8(pShape->c) * HIPU300_CHANEL_NUM_PERGROUP *
           pShape->h * pShape->w;
}

// malloc himl tensor
himlRet_t himlMallocBuffer(himlDevTensor_t *tensor_himl)
{
    himlRet_t ret = HIML_RET_SUCCESS;
    hirtRet_t rc;
    u32_t bufsize;
    gaddr_t gmem_addr;

    bufsize = himlCalcShapeBufsize(&tensor_himl->shape);

    rc = hirtMalloc(&gmem_addr, bufsize);
    if(rc != HIRT_RET_SUCCESS)
    {
        ret = HIML_RET_BADALLOC; 
    }

    tensor_himl->gmemaddr = gmem_addr;
    tensor_himl->bufsize = bufsize;
    return ret;
}
        
// copy input to himl tensor
himlRet_t himlMemcpyTensorToDevice(
    himlCpuTensor_t *tensor_cpu,
    himlDevTensor_t *tensor_dev)
{
    himlRet_t ret = HIML_RET_SUCCESS;
    hirtRet_t rc;
    
    rc = hirtMemcpy(tensor_dev->gmemaddr , tensor_cpu->haddr, tensor_dev->bufsize, HIRT_MEM_TRANS_DIR_HOST2DEV);
    if(rc != HIRT_RET_SUCCESS)
    {
        ret = HIML_RET_BADALLOC;
    }
    
    return ret;
}

// copy input to himl tensor
himlRet_t himlMemcpyTensorToHost(
    himlCpuTensor_t *tensor_cpu,
    himlDevTensor_t *tensor_dev)
{
    himlRet_t ret = HIML_RET_SUCCESS;
    hirtRet_t rc;
    
    rc = hirtMemcpy(tensor_cpu->haddr, tensor_dev->gmemaddr, tensor_dev->bufsize, HIRT_MEM_TRANS_DIR_DEV2HOST);
    if(rc != HIRT_RET_SUCCESS)
    {
        ret = HIML_RET_BADALLOC;
    }
    
    return ret;
}


himlRet_t himlCreateConvOp(
    himlBaseOp_t *op,
    himlConvOpParam_t *param,
    himlDevTensor_t *inputTensor,
    himlDevTensor_t *outputTensor,
    himlDevTensor_t *filterTensor,
    himlDevTensor_t *biasTensor)
{
    himlRet_t ret = HIML_RET_SUCCESS;
    hirtKernelParamsBuffer_t *pKernelParams;
    const char kernel_symbal[] = "libhikl_op_conv";
    
    /*construct the parambuf*/
    hirtAllocKernelParamsBuffer(hirtKernelParamsBuffer_t &pKernelParams);
    hirtKernelParamsBufferAddPlaceHolder(pKernelParams, sizeof(hirtKernelParamTable_t));
    hirtKernelParamsBufferAddParam(pKernelParams, inputTensor,  sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, outputTensor, sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, filterTensor, sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, biasTensor,   sizeof(himlDevTensor_t));
    hirtKernelParamsBufferAddParam(pKernelParams, param,        sizeof(himlConvOpParam_t));

    memcpy(op->kernel_symbal_name, kernel_symbal, sizeof(kernel_symbal));

    return ret;
}

himlRet_t himlComputeConvOpForward(
    himlBaseOp_t *op,
    himlDevTensor_t *pTensorInput,
    himlDevTensor_t *pTensorOutput,
    himlFuncType_t type,
    hirtCmdQueue_t *pQueue)
{
    himlRet_t ret = HIML_RET_SUCCESS;
    hirtRet_t rc;
    hirtKernelBinBuffer_t *kernel_binbuf;
    /*alloc the kernelbin gmem and load the kernelbin into the gmembin*/
    
    rc = hirtInvokeKernel(op->kernel_parambuf, type, op->kernel_parambuf, &kernel_binbuf, pQueue);
    if(rc != HIRT_RET_SUCCESS)
    {
        ret = HIML_RET_UNSUPPORT;
        goto fail;
    }
    
fail:
    return ret;
}


#endif /*__LIBHIRT_H__*/
