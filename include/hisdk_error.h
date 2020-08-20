#ifndef __HISDK_ERROR_H__
#define __HISDK_ERROR_H__

#include <stdint.h>
#include <stdbool.h>

/**< Error codes */
typedef enum
{
    HISDK_RET_SUCCESS                 = 0,      /**< No error */
    HISDK_RET_WARNING_FAKE_DEVICE     = 1,      /**< Use fake device */
    HISDK_RET_ERR_INVALID             = 632007, /**< Invalid argument */
    HISDK_RET_ERR_NOMEM               = 632008, /**< Out of memory */
    HISDK_RET_ERR_NODEV               = 632009, /**< No such device */
    HISDK_RET_ERR_IO                  = 632010, /**< I/O error */
    HISDK_RET_ERR_SYS                 = 632011, /**< System error */
    HISDK_RET_ERR_ACCES               = 632012, /**< Permission denied */
    HISDK_RET_ERR_FAULT               = 632013, /**< Bad address */
    HISDK_RET_ERR_BUSY                = 632014, /**< Device or resource busy */
    HISDK_RET_ERR_TIMEOUT             = 632015, /**< Time expired */
    HISDK_RET_ERR_EXIST               = 632016, /**< Resource or file already exists */
    HISDK_RET_ERR_NOSYS               = 632017, /**< Function not implemenmted */
    HISDK_RET_ERR_AGAIN               = 632018, /**< try again later */
    HISDK_RET_ERR_NORES               = 632019, /**< Out of resource */
    HISDK_RET_ERR_UNSUPPORTED         = 632020, /**< Unsupported operation */
    HISDK_RET_ERR_INVALID_POINTER     = 632021, /**< Invalid pointer */
    HISDK_RET_ERR_NO_EXIST            = 632022, /**< Resource or file doesn't exist */
    HISDK_RET_ERR_BROKEN              = 632023, /**< Data transmission is broken */
    HISDK_RET_ERR_INIT                = 632024, /**< Uninitialized */
    HISDK_RET_ERR_STREAM              = 632025, /**< Failure on Stream */
    HISDK_RET_ERR_OUT_RANGE           = 632026, /**< Number out of range */
    HISDK_RET_ERR_MATH_OVERFLOW       = 632027, /**< Math result not representable */
    HISDK_RET_ERR_FUNC_CALL           = 632028, /**< Failure to call runtime functions */
    HISDK_RET_ERR_UNHANDLED           = 632029, /**< Unhandled error */
    HISDK_RET_ERR_INVALID_TYPE        = 632030, /**< Invalid type */
    HISDK_RET_ERR_INVALID_OP          = 632031, /**< Invalid operation */
    HISDK_RET_ERR_MLU                 = 632032, /**< MLU error */
    HISDK_RET_ERR_ONCHIP_CORE         = 632033, /**< Onchip core error */
    HISDK_RET_ERR_EVENT               = 632034, /**< Failure on event operation */
    HISDK_RET_ERR_RESHAPE             = 632035, /**< Failure on data reshape */
    HISDK_RET_ERR_MEMCPY              = 632036, /**< Failure on memory copy */
    HISDK_RET_ERR_ENCRYPT             = 632037, /**< Failure on encrypt */
    HISDK_RET_ERR_INVALID_DATADESC    = 632038, /**< Invalid data descriptor */
    HISDK_RET_ERR_FILEWRITEFAILED     = 632039, /**< FileWriteFailed      */
    HISDK_RET_ERR_FILEREADFAILED      = 632040, /**< FileReadFailed       */
    HISDK_RET_ERR_ENDOFFILE           = 632041, /**< EndOfFile            */
    HISDK_RET_ERR_FILEOPERATIONFAILED = 632042, /**< FileOperationFailed  */
    HISDK_RET_ERR_DIROPERATIONFAILED  = 632043, /**< DirOperationFailed   */
    HISDK_RET_ERR_ENDOFDIRLIST        = 632044, /**< EndOfDirList         */
    HISDK_RET_ERR_IOCTLFAILED         = 632045, /**< IoctlFailed          */
    HISDK_RET_ERR_PATHALREADYEXISTS   = 632046, /**< PathAlreadyExists    */
    HISDK_RET_ERR_SURFACENOTSUPPORTED = 632047, /**< SurfaceNotSupported  */
    HISDK_RET_ERR_NOTIMPLEMENTED      = 632048, /**< NotImplemented       */
    HISDK_RET_ERR_NOTINITIALIZED      = 632050, /**< NotInitialized       */
    HISDK_RET_ERR_BADPARAMETER        = 632051, /**< BadParameter         */
    HISDK_RET_ERR_INSUFFICIENTMEMORY  = 632053, /**< InsufficientMemory   */
    HISDK_RET_ERR_READONLYATTRIBUTE   = 632054, /**< ReadOnlyAttribute    */
    HISDK_RET_ERR_INVALIDSTATE        = 632055, /**< InvalidState         */
    HISDK_RET_ERR_INVALIDADDRESS      = 632056, /**< InvalidAddress       */
    HISDK_RET_ERR_INVALIDSIZE         = 632057, /**< InvalidSize          */
    HISDK_RET_ERR_BADVALUE            = 632058, /**< BadValue             */
    HISDK_RET_ERR_ALREADYALLOCATED    = 632059, /**< AlreadyAllocated     */
    HISDK_RET_ERR_MODULENOTPRESENT    = 632060, /**< ModuleNotPresent     */
    HISDK_RET_ERR_RESOURCEERROR       = 632061, /**< ResourceError        */
    HISDK_RET_ERR_COUNTMISMATCH       = 632062, /**< CountMismatch        */
    HISDK_RET_ERR_OVERFLOW            = 632063, /**< OverFlow             */
    HISDK_RET_ERR_DISCONNECTED        = 632064, /**< Disconnected         */
    HISDK_RET_ERR_FILENOTFOUND        = 632065, /**< FileNotFound         */
    HISDK_RET_ERR_TESTAPPFAILED       = 632066, /**< TestAppFailed        */
    HISDK_RET_ERR_DEVICENOTFOUND      = 632067, /**< DeviceNotFound       */
    HISDK_RET_ERR_FIFOEMPTY           = 632068, /**< fifoempty            */
    HISDK_RET_ERR_FIFOFULL            = 632069, /**< fifofull             */
    HISDK_RET_ERR_CREATETHREAD        = 632070, /**< createthread         */
    HISDK_RET_ERR_UNKNOWN             = 999991, /**< Unknown error */
    HISDK_RET_ERR_MAX,                          /**< The last one */
} hisdkRet_t; 

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

const char* hisdkUtilsGetErrorString(hisdkRet_t e);

/**
 * There are two ways for a client to customize the error logging macros, both of which
 * are performed by defining a macro value from the client before including this file:
 *  1) Define HISDK_UTILS_ERROR_TAG to be a string with the client name. This string will
 *     be appended to any error logs before being output by the default hisdkUtilsLogError.
 *     In addition, HISDK_UTILS_ERROR_PATH may be optionally defined so that this path will
 *     be stripped from any generated errors (if not set, the 'hisdk' path
 *     will be stripped by default).
 *  2) Define HISDK_UTILS_LOG_ERROR to point to a custom error logging function within the
 *     client. The signature for this macro should be the same as the default, below.
 */
#if defined (HISDK_UTILS_ERROR_TAG)
// Use the default hisdkUtilsLogError with the client tag appended.
void hisdkUtilsLogError(const char* tag, const char* path, hisdkRet_t e, const char* file, const char* func,
                        uint32_t line, bool propagating, const char* format, ...);
#if !defined (HISDK_UTILS_ERROR_PATH)
#define HISDK_UTILS_ERROR_PATH "hisdk"
#endif

#define HISDK_UTILS_LOG_ERROR(_err, _file, _func, _line, _propagating, _format, ...) \
    do { \
        hisdkUtilsLogError(HISDK_UTILS_ERROR_TAG, HISDK_UTILS_ERROR_PATH, \
                                (_err), (_file), (_func), (_line), \
                                (_propagating), (_format), ##__VA_ARGS__); \
    } \
    while (0)
#elif defined (HISDK_UTILS_LOG_ERROR)
// Use the client's custom error logging function.
#else
#error "One of HISDK_UTILS_ERROR_TAG or HISDK_UTILS_LOG_ERROR must be defined"
#endif

/**
 * Argument counting macros.
 */
#define HISDK_UTILS_CAT(A, B) A##B
#define HISDK_UTILS_SELECT(NAME, NUM) HISDK_UTILS_CAT(NAME##_, NUM)
#define HISDK_UTILS_GET_COUNT(_1, _2, _3, _4, _5, COUNT, ...) COUNT
#define HISDK_UTILS_VA_SIZE(...) HISDK_UTILS_GET_COUNT(__VA_ARGS__, 5, 4, 3, 2, 1)
#define HISDK_UTILS_VA_SELECT(NAME, ...) HISDK_UTILS_SELECT(NAME, HISDK_UTILS_VA_SIZE(__VA_ARGS__))(__VA_ARGS__)

/**
 * Simply report an error.
 */
#define REPORT_ERROR(...) HISDK_UTILS_VA_SELECT(REPORT_ERROR_IMPL, __VA_ARGS__)
#define REPORT_ERROR_IMPL_1(_err) \
    do { HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, 0); } while (0)
#define REPORT_ERROR_IMPL_2(_err, _format) \
    do { HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, (_format)); } while (0)
#define REPORT_ERROR_IMPL_3(_err, _format, ...) REPORT_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define REPORT_ERROR_IMPL_4(_err, _format, ...) REPORT_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define REPORT_ERROR_IMPL_5(_err, _format, ...) REPORT_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define REPORT_ERROR_IMPL_N(_err, _format, ...) \
    do { HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, (_format), __VA_ARGS__); } while (0)

/**
 * Report and return an error that was first detected in the current method.
 */
#define ORIGINATE_ERROR(...) HISDK_UTILS_VA_SELECT(ORIGINATE_ERROR_IMPL, __VA_ARGS__)
#define ORIGINATE_ERROR_IMPL_1(_err) \
    do { \
        HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, 0); \
        return (_err); \
    } while (0)
#define ORIGINATE_ERROR_IMPL_2(_err, _format) \
    do { \
        HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, (_format)); \
        return (_err); \
    } while (0)
#define ORIGINATE_ERROR_IMPL_3(_err, _format, ...) ORIGINATE_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define ORIGINATE_ERROR_IMPL_4(_err, _format, ...) ORIGINATE_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define ORIGINATE_ERROR_IMPL_5(_err, _format, ...) ORIGINATE_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define ORIGINATE_ERROR_IMPL_N(_err, _format, ...) \
    do { \
        HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, (_format), __VA_ARGS__); \
        return (_err); \
    } while (0)

/**
 * Report an error that was first detected in the current method, then jumps to the "fail:" label.
 * The variable "hisdkRet_t e" must have been previously declared.
 */
#define ORIGINATE_ERROR_FAIL(...) HISDK_UTILS_VA_SELECT(ORIGINATE_ERROR_FAIL_IMPL, __VA_ARGS__)
#define ORIGINATE_ERROR_FAIL_IMPL_1(_err) \
    do { \
        e = (_err); \
        HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, 0); \
        goto fail; \
    } while (0)
#define ORIGINATE_ERROR_FAIL_IMPL_2(_err, _format) \
    do { \
        e = (_err); \
        HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, (_format)); \
        goto fail; \
    } while (0)
#define ORIGINATE_ERROR_FAIL_IMPL_3(_err, _format, ...) ORIGINATE_ERROR_FAIL_IMPL_N((_err), (_format), __VA_ARGS__)
#define ORIGINATE_ERROR_FAIL_IMPL_4(_err, _format, ...) ORIGINATE_ERROR_FAIL_IMPL_N((_err), (_format), __VA_ARGS__)
#define ORIGINATE_ERROR_FAIL_IMPL_5(_err, _format, ...) ORIGINATE_ERROR_FAIL_IMPL_N((_err), (_format), __VA_ARGS__)
#define ORIGINATE_ERROR_FAIL_IMPL_N(_err, _format, ...) \
    do { \
        e = (_err); \
        HISDK_UTILS_LOG_ERROR((_err), __FILE__, __FUNCTION__, __LINE__, false, (_format), __VA_ARGS__); \
        goto fail; \
    } while (0)

/**
 * Calls another function, and if an error was returned it is reported and returned.
 */
#define PROPAGATE_ERROR(...) HISDK_UTILS_VA_SELECT(PROPAGATE_ERROR_IMPL, __VA_ARGS__)
#define PROPAGATE_ERROR_IMPL_1(_err) \
    do { \
        hisdkRet_t peResult = (_err); \
        if (peResult != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(peResult, __FILE__, __FUNCTION__, __LINE__, true, 0); \
            return peResult; \
        } \
    } while (0)
#define PROPAGATE_ERROR_IMPL_2(_err, _format) \
    do { \
        hisdkRet_t peResult = (_err); \
        if (peResult != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(peResult, __FILE__, __FUNCTION__, __LINE__, true, (_format)); \
            return peResult; \
        } \
    } while (0)
#define PROPAGATE_ERROR_IMPL_3(_err, _format, ...) PROPAGATE_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_IMPL_4(_err, _format, ...) PROPAGATE_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_IMPL_5(_err, _format, ...) PROPAGATE_ERROR_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_IMPL_N(_err, _format, ...) \
    do { \
        hisdkRet_t peResult = (_err); \
        if (peResult != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(peResult, __FILE__, __FUNCTION__, __LINE__, true, (_format), __VA_ARGS__); \
            return peResult; \
        } \
    } while (0)

/**
 * Calls another function, and if an error was returned it is reported before jumping to the
 * "fail:" label. The variable "hisdkRet_t e" must have been previously declared.
 */
#define PROPAGATE_ERROR_FAIL(...) HISDK_UTILS_VA_SELECT(PROPAGATE_ERROR_FAIL_IMPL, __VA_ARGS__)
#define PROPAGATE_ERROR_FAIL_IMPL_1(_err) \
    do { \
        e = (_err); \
        if (e != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(e, __FILE__, __FUNCTION__, __LINE__, true, 0); \
            goto fail; \
        } \
    } while (0)
#define PROPAGATE_ERROR_FAIL_IMPL_2(_err, _format) \
    do { \
        e = (_err); \
        if (e != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(e, __FILE__, __FUNCTION__, __LINE__, true, (_format)); \
            goto fail; \
        } \
    } while (0)
#define PROPAGATE_ERROR_FAIL_IMPL_3(_err, _format, ...) PROPAGATE_ERROR_FAIL_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_FAIL_IMPL_4(_err, _format, ...) PROPAGATE_ERROR_FAIL_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_FAIL_IMPL_5(_err, _format, ...) PROPAGATE_ERROR_FAIL_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_FAIL_IMPL_N(_err, _format, ...) \
    do { \
        e = (_err); \
        if (e != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(e, __FILE__, __FUNCTION__, __LINE__, true, (_format), __VA_ARGS__); \
            goto fail; \
        } \
    } while (0)

/**
 * Calls another function, and if an error was returned it is reported and assigned to the
 * variable 'e' (which must have been previously declared). The caller does not return.
 */
#define PROPAGATE_ERROR_CONTINUE(...) HISDK_UTILS_VA_SELECT(PROPAGATE_ERROR_CONTINUE_IMPL, __VA_ARGS__)
#define PROPAGATE_ERROR_CONTINUE_IMPL_1(_err) \
    do { \
        hisdkRet_t peResult = (_err); \
        if (peResult != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(peResult, __FILE__, __FUNCTION__, __LINE__, true, 0); \
            if (e == HISDK_RET_SUCCESS) \
                e = peResult; \
        } \
    } while (0)
#define PROPAGATE_ERROR_CONTINUE_IMPL_2(_err, _format) \
    do { \
        hisdkRet_t peResult = (_err); \
        if (peResult != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(peResult, __FILE__, __FUNCTION__, __LINE__, true, (_format)); \
            if (e == HISDK_RET_SUCCESS) \
                e = peResult; \
        } \
    } while (0)
#define PROPAGATE_ERROR_CONTINUE_IMPL_3(_err, _format, ...) PROPAGATE_ERROR_CONTINUE_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_CONTINUE_IMPL_4(_err, _format, ...) PROPAGATE_ERROR_CONTINUE_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_CONTINUE_IMPL_5(_err, _format, ...) PROPAGATE_ERROR_CONTINUE_IMPL_N((_err), (_format), __VA_ARGS__)
#define PROPAGATE_ERROR_CONTINUE_IMPL_N(_err, _format, ...) \
    do { \
        hisdkRet_t peResult = (_err); \
        if (peResult != HISDK_RET_SUCCESS) \
        { \
            HISDK_UTILS_LOG_ERROR(peResult, __FILE__, __FUNCTION__, __LINE__, true, (_format), __VA_ARGS__); \
            if (e == HISDK_RET_SUCCESS) \
                e = peResult; \
        } \
    } while (0)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif /*__HISDK_ERROR_H__*/
