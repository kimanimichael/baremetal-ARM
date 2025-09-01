#ifndef __MK_PRINTF_H
#define __MK_PRINTF_H
#include <stdio.h>
#ifdef SPY
#define mk_printf(format_, ...) printf(format_ "\r\n", ##__VA_ARGS__)
#define mk_printf_init() printf_init()
#else
#define mk_printf(format_, ...) (0)
#define mk_printf_init() ((void)0)
#endif


#endif // __MK_PRINTF_H