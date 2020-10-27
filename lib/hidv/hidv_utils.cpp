#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "hidv.h"

#define MAP_SIZE (32*1024*1024UL)
#define MAP_MASK (MAP_SIZE - 1)
#define FATAL do { \
                     fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
                     __LINE__, __FILE__, errno, strerror(errno)); exit(1);    \
                 } while(0)
static int   fd_regs;
static void  *fd_regs_map_base;

static int transfer_h2c_write(const char *devname, uint64_t addr, uint64_t size, char *buffer);
static int transfer_c2h_read(const char *devname, uint64_t addr, uint64_t size, char *buffer);
void hidvDmaWriteNOCAddr(uint64_t offset,size_t size,void * src);
void hidvDmaReadNOCAddr(uint64_t offset,size_t size,void * dest);
void hidvWriteMMapReg32(uint64_t offset, uint32_t src);
uint32_t hidvReadMMapReg32(uint64_t offset);
long int filesize(FILE * fp);

void hidvDmaSetReadSize(uint32_t size)
{
    hidvWriteMMapReg32(HIPU200_REG_PCIE_NOC_RXLEN, size);//pcie bridge
}
void hidvDmaSetWriteSize(uint32_t size)
{
    hidvWriteMMapReg32(HIPU200_REG_PCIE_NOC_TXLEN, size);//pcie bridge
}

void hidvDmaWriteNOCAddr(uint64_t offset,size_t size,void * src)
{
    transfer_h2c_write(HIPU200_DEV_H2C_DEFAULT, offset, size, (char*)src);
}

void hidvDmaReadNOCAddr(uint64_t offset,size_t size,void * dest)
{
    transfer_c2h_read(HIPU200_DEV_C2H_DEFAULT, offset, size, (char*)dest);
}

void hidvWriteMMapReg32(uint64_t offset, uint32_t src)
{
	printf("hidvWriteMMapReg32, off=%lx, src=%x\n", offset, src);
	*((volatile uint32_t *)((uint64_t)fd_regs_map_base + offset + 0x104000)) = src;
}

uint32_t hidvReadMMapReg32(uint64_t offset)
{
	printf("hidvReadMMapReg32, off=%lx\n", offset);
    return *((volatile uint32_t *)((uint64_t)fd_regs_map_base + offset + 0x104000));
}

int hidvInitInternal()
{
    if ((fd_regs = open(HIPU200_DEV_USR_DEFAULT, O_RDWR | O_SYNC)) == -1)
	{
		FATAL;
	}
	   
	fd_regs_map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_regs, 0);
	//printf("fd_regs_map_base 0x%p \n",fd_regs_map_base);

	if (fd_regs_map_base == (void *)-1)
	{
		FATAL;
	}

    return 0;
}

static ssize_t read_to_buffer(const char *fname, int fd, char *buffer, uint64_t size, uint64_t base);
static ssize_t write_from_buffer(const char *fname, int fd, char *buffer, uint64_t size, uint64_t base);

static int transfer_h2c_write(const char *devname, uint64_t addr, uint64_t size, char *buffer)
{	
	ssize_t rc;

	printf("transfer_h2c_write, addr=%lx, size=%lx\n", addr, size);

	int handle_dev = open(devname, O_RDWR);

	if (handle_dev < 0)
	{
		fprintf(stderr, "unable to open device %s, %d.\n",
				devname, handle_dev);
		perror("open device");
		return -EINVAL;
	}
	/* write buffer to AXI MM address using SGDMA */
	rc = write_from_buffer(devname, handle_dev, buffer, size, addr);
	if (rc < 0)
		goto out;
out:
	close(handle_dev);
	return rc;
}

static int transfer_c2h_read(const char *devname, uint64_t addr, uint64_t size, char *buffer)
{
	ssize_t rc;

	printf("transfer_c2h_read, addr=%lx, size=%lx\n", addr, size);

	int handle_dev = open(devname, O_RDWR | O_NONBLOCK);

	if (handle_dev < 0)
	{
		fprintf(stderr, "unable to open device %s, %d.\n", devname, handle_dev);
		perror("open device");
		return -EINVAL;
	}	
	rc = read_to_buffer(devname, handle_dev, buffer, size, addr);
	if (rc < 0)
		goto out;
out:
	close(handle_dev);
	return rc;
}

/*
 * man 2 write:
 * On Linux, write() (and similar system calls) will transfer at most
 * 	0x7ffff000 (2,147,479,552) bytes, returning the number of bytes
 *	actually transferred.  (This is true on both 32-bit and 64-bit
 *	systems.)
 */

#define RW_MAX_SIZE	0x7ffff000

static ssize_t read_to_buffer(const char *fname, int fd, char *buffer, uint64_t size, uint64_t base)
{
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;

	while (count < size) 
    {
		uint64_t bytes = size - count;

		if (bytes > RW_MAX_SIZE)
			bytes = RW_MAX_SIZE;

		if (offset) 
        {
			rc = lseek(fd, offset, SEEK_SET);
			if (rc != offset) {
				fprintf(stderr, "%s, seek off 0x%lx != 0x%lx.\n",
					fname, rc, offset);
				perror("seek file");
				return -EIO;
			}
		}

		/* read data from file into memory buffer */
		rc = read(fd, buf, bytes);
		if (rc != bytes) {
			fprintf(stderr, "%s, R off 0x%lx, 0x%lx != 0x%lx.\n",
				fname, count, rc, bytes);
				perror("read file");
			return -EIO;
		}

		count += bytes;
		buf += bytes;
		offset += bytes;
	}	 

	if (count != size) {
		fprintf(stderr, "%s, R failed 0x%lx != 0x%lx.\n",
				fname, count, size);
		return -EIO;
	}
	return count;
}

static ssize_t write_from_buffer(const char *fname, int fd, char *buffer, uint64_t size, uint64_t base)
{
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;

	while (count < size) 
    {
		uint64_t bytes = size - count;

		if (bytes > RW_MAX_SIZE)
			bytes = RW_MAX_SIZE;

		if (offset) 
        {
			rc = lseek(fd, offset, SEEK_SET);
			if (rc != offset) {
				fprintf(stderr, "%s, seek off 0x%lx != 0x%lx.\n",
					fname, rc, offset);
				perror("seek file");
				return -EIO;
			}
		}

		/* write data to file from memory buffer */
		rc = write(fd, buf, bytes);
		if (rc != bytes) {
			fprintf(stderr, "%s, W off 0x%lx, 0x%lx != 0x%lx.\n",
				fname, offset, rc, bytes);
				perror("write file");
			return -EIO;
		}

		count += bytes;
		buf += bytes;
		offset += bytes;
	}	 

	if (count != size) {
		fprintf(stderr, "%s, R failed 0x%lx != 0x%lx.\n",
				fname, count, size);
		return -EIO;
	}
	return count;
}

long int filesize(FILE * fp)
{
    long size = 0;
    if(fp){
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fseek(fp,0,SEEK_SET);
    }
    return size;
}