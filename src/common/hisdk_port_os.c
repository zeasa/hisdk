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

hisdkRet_t hirtPortOsStat(const char *filename, hirtStatType_t *stat)
{
    hirtFileHandle_t file = 0;
    hirtDirHandle_t dir = 0;
    hisdkRet_t err = HISDK_RET_SUCCESS;

    if (!filename || !stat) {
        err = HISDK_RET_ERR_BADPARAMETER;
        goto fail;
    }

    stat->size = 0;
    stat->type = HirtFileType_Unknown;

    if (hirtPortOsFopen(filename, HIRT_OPEN_READ, &file) == HISDK_RET_SUCCESS) {
        err = hirtPortOsFstat(file, stat);
        goto close_file;
    }

    if (hirtPortOsOpenDir(filename, &dir) == HISDK_RET_SUCCESS) {
        stat->type = HirtFileType_Directory;
        stat->size = 0;
    }

    hirtPortOsCloseDir(dir);
close_file:
    hirtPortOsFclose(file);
fail:
    return err;
}

hisdkRet_t hirtPortOsMkDir(char *dirname)
{
    int err;

    err = mkdir(dirname, S_IRWXU);
    if (err != 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;

    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtPortOsRm(const char *filename)
{
    int err;
    if (!filename)
        return HISDK_RET_ERR_BADPARAMETER;

    err = unlink(filename);
    if(err != 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtPortOsFopen(const char *path, hirtU32 flags,
    hirtFileHandle_t *file)
{
    hisdkRet_t e = HISDK_RET_SUCCESS;
    hirtFile_t *f = NULL;
    int fd = -1;
    int permissionFlags = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int mode;

    if (!path || !file)
        return HISDK_RET_ERR_BADPARAMETER;

    e = getOpenMode(flags, &mode);
    if (e != HISDK_RET_SUCCESS)
        return HISDK_RET_ERR_BADPARAMETER;

    f = hirtPortOsmalloc(sizeof(hirtFile_t));
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
    hirtPortOsfree(f);
    return e;
}

void hirtPortOsFclose(hirtFileHandle_t stream)
{
    if (!stream)
        return;

    // TODO: what if close fails. Insert assertions??
    (void)close(stream->fd);
    stream->fd = -1;
    hirtPortOsfree(stream);
}

// TODO: Should the FIFO device be considered?
hisdkRet_t hirtPortOsFwrite(hirtFileHandle_t stream, const void *ptr, size_t size)
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
hisdkRet_t hirtPortOsFread(hirtFileHandle_t stream, void *ptr,
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

hisdkRet_t hirtPortOsFseek(hirtFileHandle_t file, i64_t offset, hirtSeekEnum whence)
{
    loff_t off;
    int seekMode;

    if(!file)
        return HISDK_RET_ERR_BADPARAMETER;

    switch(whence) {
    case HirtSeek_Set: seekMode = SEEK_SET; break;
    case HirtSeek_Cur: seekMode = SEEK_CUR; break;
    case HirtSeek_End: seekMode = SEEK_END; break;
    default:
        return HISDK_RET_ERR_BADPARAMETER;
    }

    off = lseek64(file->fd, (loff_t)offset, seekMode);
    if (off < 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtPortOsFstat(hirtFileHandle_t file, hirtStatType *stat)
{
    struct stat64 fs;
    int err;

    if (!stat || !file)
        return HISDK_RET_ERR_BADPARAMETER;

    err = fstat64(file->fd, &fs);
    if (err != 0)
        return HISDK_RET_ERR_FILEOPERATIONFAILED;

    stat->size = (u64_t)fs.st_size;
    stat->mtime = (u64_t)fs.st_mtime;

    if( S_ISREG( fs.st_mode ) ) {
        stat->type = HirtFileType_File;
    }
    else if( S_ISDIR( fs.st_mode ) ) {
        stat->type = HirtFileType_Directory;
    }
    else if( S_ISFIFO( fs.st_mode ) ) {
        stat->type = HirtFileType_Fifo;
    }
    else if( S_ISCHR( fs.st_mode ) ) {
        stat->type = HirtFileType_CharacterDevice;
    }
    else if( S_ISBLK( fs.st_mode ) ) {
        stat->type = HirtFileType_BlockDevice;
    }
    else {
        stat->type = HirtFileType_Unknown;
    }

    return HISDK_RET_SUCCESS;
}

u64_t hirtPortOsStatGetSize(hirtStatType_t *stat)
{
    return stat->size;
}

hisdkRet_t hirtPortOsFgetc(hirtFileHandle_t stream, u8_t *c)
{
    return hirtPortOsFread(stream, c, 1, NULL);
}

void hirtPortOsMemSet( void *s, u8_t c, size_t size )
{
    // s should not be NULL!! Assert if required
    if (!s)
        return;

    (void)memset(s, (int)c, size);
}

hisdkRet_t hirtPortOsOpenDir(const char *path, hirtDirHandle_t *dirHandle)
{
    hirtDir_t *d;
    if (!path || !dirHandle)
        return HISDK_RET_ERR_BADPARAMETER;

    d = (hirtDir_t *)hirtPortOsmalloc(sizeof(hirtDir_t));
    if (!d)
        return HISDK_RET_ERR_INSUFFICIENTMEMORY;

    d->dir = opendir(path);
    if (d->dir == NULL)
        return HISDK_RET_ERR_DIROPERATIONFAILED;

    *dirHandle = d;
    return HISDK_RET_SUCCESS;
}

hisdkRet_t hirtPortOsReadDir(hirtDirHandle_t dirHandle, char *name, size_t size)
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

void hirtPortOsCloseDir(hirtDirHandle_t dirHandle)
{
    if (!dirHandle)
        return;

    if (dirHandle->dir)
        (void) closedir(dirHandle->dir);

    hirtPortOsfree(dirHandle);
    return;
}

void hirtPortOsDebugPrintf(const char *format, ... )
{
    va_list ap;

    va_start( ap, format );
    vprintf(format, ap);
    va_end( ap );
}