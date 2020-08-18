#ifndef __LIBHIKL_STDIO_H__
#define __LIBHIKL_STDIO_H__

typedef char *va_list;

#define _INTSIZEOF(n)		( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v)		( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_end(ap)			( ap = (va_list)0 )
#define va_arg(ap,t)		( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )

signed int printk(const char *pFormat, ...);

#endif /* __LIBHIKL_STDIO_H__ */
