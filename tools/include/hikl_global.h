#ifndef HIPU200CLIB_HIDK_GLOBAL_H__
#define HIPU200CLIB_HIDK_GLOBAL_H__

#if 0
#define __KERNEL_CODE__         __attribute__((section(".text.hikl")))
#define __KENRLE_DATA__         __attribute__((section(".data.hikl")))
#define __KERNEL_BSS__          __attribute__((section(".bss.hikl")))
#else
#define __KERNEL_CODE__
#define __KENRLE_DATA__
#define __KERNEL_BSS__
#endif

#define HIKL_ENTRY_ADDRESS      (0x80000000)

#endif /*HIPU200CLIB_HIDK_GLOBAL_H__*/
