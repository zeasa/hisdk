#include "libhirt.h"


/************************************************************************
 * Error handling
 ************************************************************************/
/**
 * @brief Return string pointer that describes
 *     the error code passed in the argument errCode.
 *
 * The function returns a read only string that is corresponding
 * to the argument @p errcode.
 *
 * @param  errCode[in] the error code was returned by previous function call.
 * @return a pointer that points to a constant string.
 */
__R_HOST
const char *hirtGetErrorStr(hirtRet_t errCode)
{
    return "hirtGetErrorStr";
}

/**
 * @brief Get the error code set by any runtime calls.
 *     Its value is meaningful only when the return value indicating an error.
 *
 * @return error code of the last call of runtime functions.
 */
__R_HOST
hirtRet_t hirtGetLastErr(void)
{
    return HIRT_RET_SUCCESS;
}

/*************************************************************************
 * Initialization and destroy
 *************************************************************************/
/**
 * @brief Initialize runtime environment in current process space.
 *
 * Initializes this API must be called before any other runtime API calls.
 *
 * @param  Flags[in] reserved for further use, pass 0 as well.
 * @return hirt_RET_SUCCESS if success, otherwise with the error code.
 */
__R_HOST
hirtRet_t hirtInit(unsigned int Flags)
{
    return HIRT_RET_SUCCESS;
}

/**
 * @brief Destroy everything that allocated by runtime API calls.
 *
 * This API should be called after any other runtime API calls.
 *
 * @return void (None).
 */
__R_HOST
void hirtDestroy(void)
{

}

