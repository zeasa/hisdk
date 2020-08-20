#include "ErrorMacros.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

const char* hisdkUtilsGetErrorString(hisdkRet_t e)
{
    return NULL;
}

/**
 * The default error logging function to use when HISDK_UTILS_ERROR_TAG is defined.
 */
#if defined (HISDK_UTILS_ERROR_TAG)
void hisdkUtilsLogError(const char* tag, const char* path, hisdkRet_t e, const char* file, const char* func,
                        uint32_t line, bool propagating, const char* format, ...)
{
    static const uint32_t MAX_LENGTH = 1024;

    char msg[MAX_LENGTH];
    char *cur = msg;

    // Remove the leading file path.
    if (strstr(file, path))
        file = strstr(file, path) + strlen(path);

    // Use the error string if found, otherwise just use the raw error code.
    const char* errorString = hisdkUtilsGetErrorString(e);
    if (errorString)
        cur += snprintf(cur, msg + sizeof(msg) - cur, "(%s) Error %s:", tag, errorString);
    else
        cur += snprintf(cur, msg + sizeof(msg) - cur, "(%s) Error 0x%08x:", tag, e);

    // Append the error message.
    if (format)
    {
        cur += snprintf(cur, msg + sizeof(msg) - cur, " ");

        va_list args;
        va_start(args, format);
        cur += vsnprintf(cur, msg + sizeof(msg) - cur, format, args);
        va_end(args);
    }

    // Append the error location.
    if (propagating)
        cur += snprintf(cur, msg + sizeof(msg) - cur, " (propagating from ");
    else
        cur += snprintf(cur, msg + sizeof(msg) - cur, " (in ");
    snprintf(cur, msg + sizeof(msg) - cur, "%s, function %s(), line %d)", file, func, line);

    // Output the error.
    hirtPortOsDebugPrintf("%s\n", msg);
}
#endif // defined(HISDK_UTILS_ERROR_TAG)
