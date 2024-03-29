#include "hirt_device.h"


static hirtDev_t hirtCurrentDev_g = 0;

/******************************************************************************
 * Version and revision
 ******************************************************************************/
/**
 * @brief Return the version of the MLU hardware.
 *
 * Higher version usually offers more features provided by this library.
 *
 * @param  ver[out] pointer to retrieve the version.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGetHardwareVersion(hirtHardwareVersion_t *ver)
{
    ver->hirtPlatformVersion = 0;
    ver->hirtPlatformRevision = 0;
    ver->hirtCoreVersion = 0;
    ver->hirtCoreRevision = 0;
    
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtGetHardwareVersion");
    return HISDK_RET_SUCCESS;
}

/******************************************************************************
 * Device managment
 ******************************************************************************/
/**
 * @brief Get the device handle by a given device ordinal.
 *
 *  The function returns the device handle given a specific device ordinal.
 *
 * @param  dev[out] pointer to retrieve the device handle.
 * @param  ordinal[in] the device ordinal to get the device handle.
 * @note   the ordinal should be in the range [0~hirtGetDeviceCount() - 1].
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGetDeviceHandle(hirtDev_t *dev, int ordinal)
{
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtGetDeviceHandle");
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Set the device handle for current thread execution context.
 *
 *  It implies that any subsequent runtime API calls are for this device.
 *
 * @param  dev[in] the device handle.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtSetCurrentDevice(hirtDev_t dev)
{
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtSetCurrentDevice");
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Get the hirtDevice handle from current thread execution context.
 *
 * The handle has been set by calling hirtSetCurrentDevice().
 *
 * @param  dev[out] pointer to retrieve the device handle.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGetCurrentDevice(hirtDev_t *dev)
{
    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtGetCurrentDevice");
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Get capability of a device which is specified by device handle.
 *
 *  The function returns the device capabilities.
 *
 * @param  cap[out] pointer to retrieve the device capability.
 * @param  dev[in] the device handle to get the device capability.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGetDeviceCapability(hirtCap_t *cap, hirtDev_t dev)
{
    cap->hirtCapsMaxNumberOfStream = 1;
    cap->hirtCapsNumberOfCore = 13;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtGetDeviceCapability : maxstream=%d, ncores=%d", 
        cap->hirtCapsMaxNumberOfStream,
        cap->hirtCapsNumberOfCore);
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Get the number of MLU devices in the system.
 *
 * @param  devnum[out] pointer to retrieve the number of devices.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGetDeviceCount(unsigned *devnum)
{
    *devnum = 0;

    HISDK_LOG_INFO(LOG_SYSTEM, "%s", "hirtGetDeviceCount");
    return HISDK_RET_SUCCESS;
}

/**
 * @brief Get the total memory size in MByte.
 *
 * @param  sz[out] pointer to retrieve the memory amount on device.
 * @return hirt_RET_SUCCESS if success, otherwise the error code is returned.
 */
__R_HOST
hisdkRet_t hirtGetDeviceMemorySizeMB(unsigned int *sz)
{
    *sz = 8 * 1024;

    HISDK_LOG_INFO(LOG_SYSTEM, "hirtGetDeviceMemorySize : size=%d", *sz);
    return HISDK_RET_SUCCESS;
}
