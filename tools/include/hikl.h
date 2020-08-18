#ifndef HIPU200CLIB_HIKL_H__
#define HIPU200CLIB_HIKL_H__

#include "hikl_global.h"
#include "libhikl_stdlib.h"

typedef int (*hikl_api_func_t)(void* pParam);

typedef struct __hikl_api_entrytable_t
{
    int api_code;
    hikl_api_func_t func_ptr;
} hikl_api_entrytable_t;

#define HIKL_API_BEGIN      (0)
#define HIKL_API_CONV1      (0)
#define HIKL_API_CONV2      (1)
#define HIKL_API_CONV3      (2)
#define HIKL_API_END        (10000)

typedef int (*libhikl_func_call_t)(int code, void *pParam, void *pResult);

#define LIBHIKL_FUNC_CALL(code, pParam, pResult)        (*((libhikl_func_call_t)(HIKL_ENTRY_ADDRESS)))(code, pParam, pResult)

#define LIBHIKL_ASSERT(p)   if(!(p)){printk("Assertion failed/n"); abort();} 

#endif /*HIPU200CLIB_HIKL_H__*/
