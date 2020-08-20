#ifndef __HIML_H__
#define __HIML_H__

#include "hisdk.h"

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


    union
    {
        u32_t h;
        u32_t row;
    };

    union
    {
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
    himlDataType_t data_type;
    himlDataOrder_t data_order;
    shape_t shape;

    void *haddr;
    u32_t bufsize;
} himlCpuTensor_t;

typedef struct
{
    himlTensorType_t tensor_type;
    himlDataType_t data_type;
    himlDataOrder_t data_order;
    shape_t shape;

    gaddr_t *gmemaddr;
    u32_t bufsize;
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

#define KERNEL_SYMBAL_NAME_MAX	(128)
typedef struct
{
    char kernel_symbal_name[KERNEL_SYMBAL_NAME_MAX];
    hirtKernelParamsBuffer_t *kernel_parambuf;
} himlBaseOp_t;

#endif /*__LIBHIRT_H__*/



