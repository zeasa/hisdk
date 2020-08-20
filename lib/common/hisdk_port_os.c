#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#include "hisdk_port.h"

static hisdkRet_t getOpenMode(u32_t flags, int *mode)
{
    switch( flags )
    {
        case HISDK_OPEN_READ:
            *mode = O_RDONLY /*| O_LARGEFILE*/;
            break;
        case HISDK_OPEN_WRITE:
        case HISDK_OPEN_CREATE | HISDK_OPEN_WRITE:
            *mode = O_CREAT | O_WRONLY | O_TRUNC /*| O_LARGEFILE*/;
            break;
        case HISDK_OPEN_READ | HISDK_OPEN_WRITE:
            *mode = O_RDWR /*| O_LARGEFILE*/;
            break;
        case HISDK_OPEN_CREATE | HISDK_OPEN_READ | HISDK_OPEN_WRITE:
            *mode = O_CREAT | O_RDWR | O_TRUNC /*| O_LARGEFILE*/;
            break;
        case HISDK_OPEN_APPEND:
        case HISDK_OPEN_CREATE | HISDK_OPEN_APPEND:
        case HISDK_OPEN_WRITE | HISDK_OPEN_APPEND:
        case HISDK_OPEN_CREATE | HISDK_OPEN_WRITE | HISDK_OPEN_APPEND:
            *mode = O_CREAT | O_WRONLY | O_APPEND /*| O_LARGEFILE*/;
            break;
        case HISDK_OPEN_READ | HISDK_OPEN_APPEND:
        case HISDK_OPEN_CREATE | HISDK_OPEN_READ | HISDK_OPEN_APPEND:
        case HISDK_OPEN_READ | HISDK_OPEN_WRITE | HISDK_OPEN_APPEND:
        case HISDK_OPEN_CREATE | HISDK_OPEN_READ | HISDK_OPEN_WRITE | HISDK_OPEN_APPEND:
            *mode = O_CREAT | O_RDWR | O_APPEND /*| O_LARGEFILE*/;
            break;
        default:
            return HISDK_RET_ERR_BADPARAMETER;
    }

    return HISDK_RET_SUCCESS;
}

void *hisdkAlloc(size_t size)
{
    return malloc(size);
}

void hisdkFree(void *ptr)
{
    if (ptr != NULL)
        free(ptr);
}

hisdkRet_t hisdkPortOsStat(const char *filename, hisdkStatType_t *stat)
{
    hisdkFileHandle_t file = 0;
    hisdkDirHandle_t dir = 0;
    hisdkRet_t err = HISDK_RET_SUCCESS;

    if (!filename || !stat) {
        err = HISDK_RET_ERR_BADPARAMETER;
        goto fail;
    }

    stat->size = 0;
    stat->type = HisdkFileType_Unknown;

    if (hisdkPortOsFopen(filename, HISDK_OPEN_READ, &file) == HISDK_RET_SUCCESS) {
        err = hisdkPortOsFstat(file, stat);
        goto close_file;
    }

    if (hisdkPortOsOpenDir(filename, &dir) == HISDK_RET_SUCCESS) {
        stat->type = HisdkFileType_Directory;
        stat->size = 0;
    }

    hisdkPortOsCloseDir(dir);
close_file:
    hisdkPortOsFclose(file);
fail:
    return err;
}

hisdkRet_t hisdkPortOsMkDir(char *dirname)
{
    int err;

    err = mkdir(dirname, S_IRWXU);
    if (err != 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;

    return HISDK_RET_SUCCESS;
}

hisdkRet_t hisdkPortOsRm(const char *filename)
{
    int err;
    if (!filename)
        return HISDK_RET_ERR_BADPARAMETER;

    err = unlink(filename);
    if(err != 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hisdkPortOsFopen(const char *path, u32_t flags,
    hisdkFileHandle_t *file)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hisdkFile_t *f = NULL;
    int fd = -1;
    int permissionFlags = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int mode;

    if (!path || !file)
        return HISDK_RET_ERR_BADPARAMETER;

    e = getOpenMode(flags, &mode);
    if (e != HISDK_RET_SUCCESS)
        return HISDK_RET_ERR_BADPARAMETER;

    f = malloc(sizeof(hisdkFile_t));
    if (!f)
        return HISDK_RET_ERR_INSUFFICIENTMEMORY;

    fd = open(path, mode, permissionFlags);
    if (fd < 0) {
        e = HISDK_RET_ERR_FILEOPERATIONFAILED;
        goto fail;
    }
    f->fd = fd;

    *file = f;
    return HISDK_RET_SUCCESS;

fail:
    free(f);
    return e;
}

void hisdkPortOsFclose(hisdkFileHandle_t stream)
{
    if (!stream)
        return;

    // TODO: what if close fails. Insert assertions??
    (void)close(stream->fd);
    stream->fd = -1;
    free(stream);
}

// TODO: Should the FIFO device be considered?
hisdkRet_t hisdkPortOsFwrite(hisdkFileHandle_t stream, const void *ptr, size_t size)
{
    ssize_t len;
    size_t s;
    char *p;

    if (!stream || !ptr)
        return HISDK_RET_ERR_BADPARAMETER;

    if (!size)
        return HISDK_RET_SUCCESS;

    s = size;
    p = (char *)ptr;
    do
    {
        len = write(stream->fd, p, s);
        if (len > 0)
        {
            p += len;
            s -= len;
        }
    } while ((len < 0 && (errno == EINTR)) || (s > 0 && len > 0));

    if (len < 0)
        return HISDK_RET_ERR_FILEWRITEFAILED;

    return HISDK_RET_SUCCESS;
}

// TODO: Should the FIFO device be considered?
hisdkRet_t hisdkPortOsFread(hisdkFileHandle_t stream, void *ptr,
                size_t size, size_t *bytes)
{
    ssize_t len;
    size_t s;
    char *p;

    if(!stream || !ptr)
        return HISDK_RET_ERR_BADPARAMETER;

    if (!size) {
        if (bytes) *bytes = 0;
        return HISDK_RET_SUCCESS;
    }
    if (size > SSIZE_MAX)
        return HISDK_RET_ERR_BADVALUE;

    s = size;
    p = (char *)ptr;

    do
    {
        len = read(stream->fd, p, s);
        if (len > 0)
        {
            p += len;
            s -= len;
        }
    } while ((len < 0 && (errno == EINTR)) || (s > 0 && len > 0));

    if (len < 0)
        return HISDK_RET_ERR_FILEREADFAILED;

    if (bytes)
        *bytes = size - s;

    if (!len)
        return HISDK_RET_ERR_ENDOFFILE;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hisdkPortOsFseek(hisdkFileHandle_t file, i64_t offset, hisdkSeekEnum_t whence)
{
    loff_t off;
    int seekMode;

    if(!file)
        return HISDK_RET_ERR_BADPARAMETER;

    switch(whence) {
    case HisdkSeek_Set: seekMode = SEEK_SET; break;
    case HisdkSeek_Cur: seekMode = SEEK_CUR; break;
    case HisdkSeek_End: seekMode = SEEK_END; break;
    default:
        return HISDK_RET_ERR_BADPARAMETER;
    }

    off = lseek(file->fd, (loff_t)offset, seekMode);
    if (off < 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hisdkPortOsFstat(hisdkFileHandle_t file, hisdkStatType_t *stat)
{
    struct stat fs;
    int err;

    if (!stat || !file)
        return HISDK_RET_ERR_BADPARAMETER;

    err = fstat(file->fd, &fs);
    if (err != 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;

    stat->size = (u64_t)fs.st_size;
    stat->mtime = (u64_t)fs.st_mtime;

    if( S_ISREG( fs.st_mode ) ) {
        stat->type = HisdkFileType_File;
    }
    else if( S_ISDIR( fs.st_mode ) ) {
        stat->type = HisdkFileType_Directory;
    }
    else if( S_ISFIFO( fs.st_mode ) ) {
        stat->type = HisdkFileType_Fifo;
    }
    else if( S_ISCHR( fs.st_mode ) ) {
        stat->type = HisdkFileType_CharacterDevice;
    }
    else if( S_ISBLK( fs.st_mode ) ) {
        stat->type = HisdkFileType_BlockDevice;
    }
    else {
        stat->type = HisdkFileType_Unknown;
    }

    return HISDK_RET_SUCCESS;
}

u64_t hisdkPortOsStatGetSize(hisdkStatType_t *stat)
{
    return stat->size;
}

hisdkRet_t hisdkPortOsFgetc(hisdkFileHandle_t stream, u8_t *c)
{
    return hisdkPortOsFread(stream, c, 1, NULL);
}

void hisdkPortOsMemSet( void *s, u8_t c, size_t size )
{
    // s should not be NULL!! Assert if required
    if (!s)
        return;

    (void)memset(s, (int)c, size);
}

hisdkRet_t hisdkPortOsOpenDir(const char *path, hisdkDirHandle_t *dirHandle)
{
    hisdkDir_t *d;
    if (!path || !dirHandle)
        return HISDK_RET_ERR_BADPARAMETER;

    d = (hisdkDir_t *)malloc(sizeof(hisdkDir_t));
    if (!d)
        return HISDK_RET_ERR_INSUFFICIENTMEMORY;

    d->dir = opendir(path);
    if (d->dir == NULL)
        return HISDK_RET_ERR_DIROPERATIONFAILED;

    *dirHandle = d;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hisdkPortOsReadDir(hisdkDirHandle_t dirHandle, char *name, size_t size)
{
    struct dirent *d;

    if(!dirHandle || !name)
        return HISDK_RET_ERR_BADPARAMETER;

    if(!dirHandle->dir)
        return HISDK_RET_ERR_BADVALUE;

    d = readdir(dirHandle->dir);
    if (!d)
        return HISDK_RET_ERR_ENDOFDIRLIST;

    (void) strncpy(name, d->d_name, size);
    name[size-1] = '\0';

    return HISDK_RET_SUCCESS;
}

void hisdkPortOsCloseDir(hisdkDirHandle_t dirHandle)
{
    if (!dirHandle)
        return;

    if (dirHandle->dir)
        (void) closedir(dirHandle->dir);

    free(dirHandle);
    return;
}

void hisdkPortOsDebugPrintf(const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    vprintf(format, ap);
    va_end( ap );
}