#ifndef _HISDK_PORT_H__
#define _HISDK_PORT_H__

#include <stddef.h>
#include <dirent.h>

#include "hisdk.h"

#define HISDK_OPEN_WRITE     (0x1)
#define HISDK_OPEN_READ      (0x2)
#define HISDK_OPEN_CREATE    (0x4)
#define HISDK_OPEN_APPEND    (0x8)

/*
 * Thread structures
 */
struct hisdkThreadRec 
{
    void *handle;
};
typedef struct hisdkThreadRec    hisdkThread_t;
typedef struct hisdkThreadRec*   hisdkThreadHandle_t;
typedef void (*hisdkThreadFunction_t)(void *args);

/*
 * Files and directory structures.
 */
typedef enum 
{
    HisdkFileType_Unknown = 0,
    HisdkFileType_File,
    HisdkFileType_Directory,
    HisdkFileType_Fifo,
    HisdkFileType_CharacterDevice,
    HisdkFileType_BlockDevice,

    HisdkFileType_Force32 = 0x7FFFFFFF
} hisdkFileType_t;

typedef struct
{
    u64_t size;
    hisdkFileType_t type;
    u64_t mtime;
} hisdkStatType_t;

typedef enum hisdkSeek 
{
    HisdkSeek_Set,
    HisdkSeek_Cur,
    HisdkSeek_End
} hisdkSeekEnum_t;

struct hisdkFileRec 
{
    int fd;
};
typedef struct hisdkFileRec  hisdkFile_t;
typedef struct hisdkFileRec* hisdkFileHandle_t;

struct hisdkDirRec 
{
    DIR *dir;
};
typedef struct hisdkDirRec   hisdkDir_t;
typedef struct hisdkDirRec*  hisdkDirHandle_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * File and directory operations
 */
hisdkRet_t hisdkPortOsStat(const char *filename, hisdkStatType_t *stat);
hisdkRet_t hisdkPortOsMkdir(char *dirname);
hisdkRet_t hisdkPortOsRm(const char *filename);
hisdkRet_t hisdkPortOsFopen(const char *path, u32_t flags, hisdkFileHandle_t *file);
void       hisdkPortOsFclose(hisdkFileHandle_t stream);
hisdkRet_t hisdkPortOsFwrite(hisdkFileHandle_t stream, const void *ptr, size_t size);
hisdkRet_t hisdkPortOsFread(hisdkFileHandle_t stream, void *ptr, size_t size, size_t *bytes);
hisdkRet_t hisdkPortOsFseek(hisdkFileHandle_t file, i64_t offset, hisdkSeekEnum_t whence);
hisdkRet_t hisdkPortOsFstat(hisdkFileHandle_t file, hisdkStatType_t *stat);
u64_t      hisdkPortOsStatGetSize(hisdkStatType_t *stat);
hisdkRet_t hisdkPortOsFgetc(hisdkFileHandle_t stream, u8_t *c);
void       hisdkPortOsMemset(void *s, u8_t c, size_t size);
hisdkRet_t hisdkPortOsOpenDir(const char *path, hisdkDirHandle_t *dir);
hisdkRet_t hisdkPortOsReadDir(hisdkDirHandle_t dir, char *name, size_t size);
void       hisdkPortOsCloseDir(hisdkDirHandle_t dir);
void       hisdkPortOsDebugPrintf( const char *format, ... );

#ifdef __cplusplus
}
#endif

#endif /*_HISDK_PORT_H__*/