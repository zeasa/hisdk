#include "himl.h"


hisdkRet_t himlCreateConvOpParam(himlConvOpParam_t **ppParam,
                int stride_h,
                int stride_w,
                int dilation_h,
                int dilation_w,
                int pad_h = 0,
                int pad_w = 0)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;
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

hisdkRet_t himlDestroyConvOpParam(himlConvOpParam_t *pParam)
{
    hisdkRet_t ret = HISDK_RET_SUCCESS;

    free(pParam);

    return ret;
}

