#include "hirt_utils.h"
#include "hirt_mm.h"

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
const char *hirtGetErrorStr(hisdkRet_t errCode)
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
hisdkRet_t hirtGetLastErr(void)
{
    return HISDK_RET_SUCCESS;
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
 * @return HISDK_RET_SUCCESS if success, otherwise with the error code.
 */
__R_HOST
hisdkRet_t hirtInit(unsigned int Flags)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;

    HISDK_ERR_FCALLFAIL( hidvInit() );

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtInit done.");
    return e;
fail:
    HISDK_LOG_INFO(LOG_SYSTEM, "hirtInit failed.");
    return e;
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
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirt destroy.");
    
    hidvDestroy();
}

__R_HOST
uint32_t hirtGetCorePC(uint8_t node_xy)
{
    unsigned int buf[32/sizeof(unsigned int)];
    uint64_t addr = HIPU200_NOC_MAKEADDR(node_xy, HIPU200_MEM_APC_START);
    hirtMemcpy(buf, (void*)addr, 32, HIRT_MEM_TRANS_DIR_GPU2HOST);
    return buf[0];
}