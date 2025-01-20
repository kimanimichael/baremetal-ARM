#include "qpc.h"
#include "bsp.h"
#include "main.h"
#include "stm32f429xx.h"
#include "stm32f4xx_it.h"

extern unsigned int _data_start;
extern unsigned int _data_end;
extern unsigned int  _bss_start;
extern unsigned int  _bss_end;
extern unsigned int  _data_lma;

extern int __stack_end__;

unsigned int *vectors[] __attribute__((section(".vectors"))) = 
{
    (unsigned int *)0x20030000, //Pointer to the top of our stack memory
    (unsigned int *)start, // Pointer to our reset handler - also our startup code
    (unsigned int *)NMI_Handler, //NMI
    (unsigned int *)HardFault_Handler, //HardFault
    (unsigned int *)MemManage_Handler, //MemManage
    (unsigned int *)BusFault_Handler, //BusFault
    (unsigned int *)UsageFault_Handler, //MemManage
    0,
    0,
    0,
    0,
    (unsigned int *)SVC_Handler, //UsageFault
    (unsigned int *)DebugMon_Handler, //DebugMon_Handler
    0,
    (unsigned int *)PendSV_Handler, //PendSV_Handler
    (unsigned int *)SysTick_Handler, //SysTick_Handler
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (unsigned int *)EXTI15_10IRQHandler,//

};

void _init() {} // Provide empty _init implementation
void _fini() {} // Provide empty _fini implementation

void start()
{
    extern int __libc_init_array(void);
    extern void software_init_hook(void) __attribute__((weak));



   volatile unsigned int *src, *dest;
/**
//     * Load initialized data from ROM to RAM
//    */
   for (src = &_data_lma, dest = &_data_start; dest < &_data_end; src++, dest++)
       *dest = *src;
// Initialize all uninitialized variables (bss section) to 0
   for (dest = &_bss_start; dest < &_bss_end; dest++)
       *dest = 0;
   SystemInit();

    /* init hook provided? */
    if (&software_init_hook != (void (*)(void))(0)) {
        /* give control to the RTOS */
        software_init_hook(); /* this will also call __libc_init_array */
    }
    else {
        /* call all static constructors in C++ (harmless in C programs) */
        __libc_init_array();
        (void)main(); /* application's entry point; should never return! */
    }

   while (1);
}

// void start()
// {
//     unsigned int *src, *dest;

//     src = &_data_lma;
//     dest = &_data_start;
//     while (dest < &_data_end)
//     {
//         *dest++ = *src++;
//     }

//     dest = &_bss_start;
//     while (dest < &_bss_end)
//     {
//         *dest++ = 0;
//     }
//     // SystemInit();
//     main();
//     while (1);
// }

// void NMI_Handler (void)
// {
//     while (1)
//     {
//         /* code */
//         assert_failed("NMI_Handler", __LINE__);
//     }
//
// }
#define DEBUG
void HardFault_Handler (void) 
{
    #ifdef DEBUG
    __BKPT(0);
    #endif
    while (1)
    {
        /* code */
        assert_failed("HardFault", __LINE__);
    }
    
}

void MemManage_Handler (void) 
{
    #ifdef DEBUG
    __BKPT(0);
    #endif
    while (1)
    {
        /* code */
        assert_failed("MemManage_Handler", __LINE__);
    }
    
}

void BusFault_Handler (void) 
{
    #ifdef DEBUG
    __BKPT(0);
    #endif
    while (1)
    {
        /* code */
        assert_failed("BusFault_Handler", __LINE__);
    }
    
}

void UsageFault_Handler (void) 
{
    #ifdef DEBUG
    __BKPT(0);
    #endif
    while (1)
    {
        /* code */
        assert_failed("UsageFault_Handler", __LINE__);
    }
    
}

void Unused_Handler (void) 
{
    while (1)
    {
        /* code */
        assert_failed("Unused_Handler", __LINE__);
    }
    
}

#pragma weak SVC_Handler = Unused_Handler
#pragma weak DebugMon_Handler = Unused_Handler
// #pragma weak PendSV_Handler = Unused_Handler
// #pragma weak SysTick_Handler = Unused_Handler