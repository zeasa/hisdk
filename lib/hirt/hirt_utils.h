#ifndef _HIRT_UTILS_H__
#define _HIRT_UTILS_H__

#include "hirt.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

__R_HOST
const char *hirtGetErrorStr(hisdkRet_t errCode);
__R_HOST
hisdkRet_t  hirtGetLastErr(void);
__R_HOST
hisdkRet_t  hirtInit(unsigned int Flags);
__R_HOST
void        hirtDestroy(void);

#ifdef __cplusplus
}
#endif
#endif /*_HIRT_UTILS_H__*/

