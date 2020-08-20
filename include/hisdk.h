#ifndef __HISDK_H__
#define __HISDK_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "hisdk_config.h"
#include "hisdk_log.h"
#include "hisdk_error.h"

typedef uint64_t u64_t;
typedef int64_t  i64_t;
typedef uint64_t hirt_u64_t;
typedef int64_t  hirt_i64_t;
typedef uint32_t u32_t;
typedef uint16_t u16_t;
typedef uint8_t  u8_t;
typedef int32_t  i32_t;
typedef int16_t  i16_t;
typedef int8_t   i8_t;
typedef int      bool_t;
typedef u64_t    hirt_size_t;

#define HISDK_UNUSED(expr) do { (void)(expr); } while (0)

#ifdef HISDK_DEBUG
#define HISDK_ASSERT(expr) do {} while (0);
#else
#define HISDK_ASSERT(expr)                                  \
    do{                                                     \
        if (!(expr)) { \
            printf("\nASSERT failed at:\n  >File name: %s\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n", \
                __FILE__, __FUNCTION__, __LINE__, #expr);   \
        } \
    }while(0);\
    exit(0);
#endif

#define HISDK_ERRCHECK(ret) if (ret != hirt_RET_SUCCESS) {\
    printf("error occur, func: %s, line: %d\n", __func__, __LINE__);\
    exit(0);\
    }

#endif /*__HISDK_H__*/

