#ifndef _LIBHIRT_PORT_H__
#define _LIBHIRT_PORT_H__

#include <stddef.h>
#include <dirent.h>
#include "libhirt.h"

#define HIRT_OPEN_WRITE     (0x1)
#define HIRT_OPEN_READ      (0x2)
#define HIRT_OPEN_CREATE    (0x4)
#define HIRT_OPEN_APPEND    (0x8)

/*
 * Thread structures
 */
struct hirtThreadRec 
{
    void *handle;
};
typedef struct hirtThreadRec    hirtThread_t;
typedef struct hirtThreadRec*   hirtThreadHandle_t;
typedef void (*hirtThreadFunction_t)(void *args);

/*
 * Files and directory structures.
 */
typedef enum 
{
    HirtFileType_Unknown = 0,
    HirtFileType_File,
    HirtFileType_Directory,
    HirtFileType_Fifo,
    HirtFileType_CharacterDevice,
    HirtFileType_BlockDevice,

    HirtFileType_Force32 = 0x7FFFFFFF
} hirtFileType_t;

struct hirtStatTypeRec
{
    u64_t size;
    hirtFileType_t type;
    u64_t mtime;
};
typedef struct hirtStatTypeRec hirtStatType_t;

enum hirtSeek 
{
    HirtSeek_Set,
    HirtSeek_Cur,
    HirtSeek_End
};
typedef enum hirtSeek hirtSeekEnum_t;

struct hirtFileRec 
{
    int fd;
};
typedef struct hirtFileRec  hirtFile_t;
typedef struct hirtFileRec* hirtFileHandle_t;

struct hirtDirRec 
{
    DIR *dir;
};
typedef struct hirtDirRec   hirtDir_t;
typedef struct hirtDirRec*  hirtDirHandle_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * File and directory operations
 */
hirtRet_t hirtPortOsStat(const char *filename, hirtStatType_t *stat);
hirtRet_t hirtPortOsMkdir(char *dirname);
hirtRet_t hirtPortOsRm(const char *filename);
hirtRet_t hirtPortOsFopen(const char *path, u32_t flags, hirtFileHandle_t *file);
void      hirtPortOsFclose(hirtFileHandle_t stream);
hirtRet_t hirtPortOsFwrite(hirtFileHandle_t stream, const void *ptr, size_t size);
hirtRet_t hirtPortOsFread(hirtFileHandle_t stream, void *ptr, size_t size, size_t *bytes);
hirtRet_t hirtPortOsFseek(hirtFileHandle_t file, i64_t offset, hirtSeekEnum_t whence);
hirtRet_t hirtPortOsFstat(hirtFileHandle_t file, hirtStatType_t *stat);
i64_t     hirtPortOsStatGetSize(hirtStatType_t *stat);
hirtRet_t hirtPortOsFgetc(hirtFileHandle_t stream, u8_t *c);
void      hirtPortOsMemset(void *s, u8_t c, size_t size);
hirtRet_t hirtPortOsOpenDir(const char *path, hirtDirHandle_t *dir);
hirtRet_t hirtPortOsReadDir(hirtDirHandle_t dir, char *name, size_t size);
void      hirtPortOsCloseDir(hirtDirHandle_t dir);

#ifdef __cplusplus
}
#endif
#endif /*_LIBHIRT_PORT_H__*/