#include <stdint.h>
#include "../../include/cmsis/stm32f429xx.h"
#include "../../include/miros.h"

/****************************************************************************
* MInimal Real-time Operating System (MIROS)
* version 0.23 (matching lesson 23)
*
* This software is a teaching aid to illustrate the concepts underlying
* a Real-Time Operating System (RTOS). The main goal of the software is
* simplicity and clear presentation of the concepts, but without dealing
* with various corner cases, portability, or error handling. For these
* reasons, the software is generally NOT intended or recommended for use
* in commercial applications.
*
* Copyright (C) 2018 Miro Samek. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*
* Contact Information:
* https://www.state-machine.com
****************************************************************************/
#include <stdint.h>
//#include "miros.h"

OSThread * volatile OS_curr; /* pointer to the current thread */
OSThread * volatile OS_next; /* pointer to the next thread to run */

void OS_init(void) {
    /* set the PendSV interrupt priority to the lowest level 0xFF */
    *(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);
}

void OS_sched(void) {
    /* OS_next = ... */
    OSThread const *next = OS_next; /* volatile to temporary */
    if (next != OS_curr) {
        *(uint32_t volatile *)0xE000ED04 = (1U << 28);
    }
}

void OSThread_start(
        OSThread *me,
        OSThreadHandler threadHandler,
        void *stkSto, uint32_t stkSize)
{
    /* round down the stack top to the 8-byte boundary
    * NOTE: ARM Cortex-M stack grows down from hi -> low memory
    */
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8);
    uint32_t *stk_limit;

    *(--sp) = (1U << 24);  /* xPSR */
    *(--sp) = (uint32_t)threadHandler; /* PC */
    *(--sp) = 0x0000000EU; /* LR  */
    *(--sp) = 0x0000000CU; /* R12 */
    *(--sp) = 0x00000003U; /* R3  */
    *(--sp) = 0x00000002U; /* R2  */
    *(--sp) = 0x00000001U; /* R1  */
    *(--sp) = 0x00000000U; /* R0  */
    /* additionally, fake registers R4-R11 */
    *(--sp) = 0x0000000BU; /* R11 */
    *(--sp) = 0x0000000AU; /* R10 */
    *(--sp) = 0x00000009U; /* R9 */
    *(--sp) = 0x00000008U; /* R8 */
    *(--sp) = 0x00000007U; /* R7 */
    *(--sp) = 0x00000006U; /* R6 */
    *(--sp) = 0x00000005U; /* R5 */
    *(--sp) = 0x00000004U; /* R4 */

    /* save the top of the stack in the thread's attibute */
    me->sp = sp;

    /* round up the bottom of the stack to the 8-byte boundary */
    stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);

    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }
}

/* inline assembly syntax for Compiler 6 (ARMCLANG) */
__attribute__ ((naked))
void PendSV_Handler(void) {
    __asm volatile (
        /* __disable_irq(); */
            "  CPSID         I                 \n"

            /* if (OS_curr != (OSThread *)0) { */
            "  LDR           r1,=OS_curr       \n"
            "  LDR           r1,[r1,#0x00]     \n"
            "  CBZ           r1,PendSV_restore \n"

            /*     push registers r4-r11 on the stack */
            "  PUSH          {r4-r11}          \n"

            /*     OS_curr->sp = sp; */
            "  LDR           r1,=OS_curr       \n"
            "  LDR           r1,[r1,#0x00]     \n"
            "  STR           sp,[r1,#0x00]     \n"
            /* } */

            "PendSV_restore:                   \n"
            /* sp = OS_next->sp; */
            "  LDR           r1,=OS_next       \n"
            "  LDR           r1,[r1,#0x00]     \n"
            "  LDR           sp,[r1,#0x00]     \n"

            /* OS_curr = OS_next; */
            "  LDR           r1,=OS_next       \n"
            "  LDR           r1,[r1,#0x00]     \n"
            "  LDR           r2,=OS_curr       \n"
            "  STR           r1,[r2,#0x00]     \n"

            /* pop registers r4-r11 */
            "  POP           {r4-r11}          \n"

            /* __enable_irq(); */
            "  CPSIE         I                 \n"

            /* return to the next thread */
            "  BX            lr                \n"
            );
}

//OSThread * volatile OS_curr;/* pointer to current thread */
//OSThread * volatile OS_next;/* pointer to next thread */
//
//void OS_init(void) {
//    /* Set PendSV priority to the lowest value */
//    *(uint32_t volatile *) 0xE000ED20 |= (0xFFu << 16);
//}
//
///* only call this when interrupts are disabled*/
//void OS_sched(void) {
//    /* trigger a PendSV exception*/
//    if (OS_next != OS_curr) {
//        *(uint32_t *) 0xE000ED04 = (1 << 28);
//    }
//    // *(uint32_t *) 0xE000ED04 = (1 << 28);
//}
//void OSThread_start(
//    OSThread *me,
//    OSThreadHandler threadHandler,
//    void* stkSto, uint32_t stkSize)
//{
//    /* Round down the stack top to the 8-byte boundary
//    * NOTE: Stack grows from high to low memory in ARM Cortex-M
//    */
//    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize)/8)*8);
//    uint32_t *stk_limit;
//
//    *(--sp) = (1U << 24); /* xPSR */
//    *(--sp) = (uint32_t)&threadHandler; /*PC*/
//    *(--sp) = 0x0000000EU;
//    *(--sp) = 0x0000000CU;
//    *(--sp) = 0x00000003U;
//    *(--sp) = 0x00000002U;
//    *(--sp) = 0x00000001U;
//    *(--sp) = 0x00000000U;
//
//    // Save additional registers to prevent clobbering
//    *(--sp) = 0x0000000BU;
//    *(--sp) = 0x0000000AU;
//    *(--sp) = 0x00000009U;
//    *(--sp) = 0x00000008U;
//    *(--sp) = 0x00000007U;
//    *(--sp) = 0x00000006U;
//    *(--sp) = 0x00000005U;
//    *(--sp) = 0x00000004U;
//
//    /* Save the current top of the stack in the thread's attribute*/
//    me->sp = sp;
//
//    /* Round down the bottom of stack to the 8-byte boundary*/
//    stk_limit = (uint32_t *)((((uint32_t)stkSto -1U )/8 +1U)*8);
//
//    /*Prefill unused  part of the stack with 0xDEADBEEF*/
//    for (sp = sp - 1U; sp >= stk_limit; --sp) {
//        *sp = 0xDEADBEEFU;
//    }
//}
//
////void PendSV_Handler(void ) {
//////    __disable_irq();
//////    void *sp;
//////    if (OS_curr != (OSThread*)0) {
//////        /* push r4 - r11 onto the stack*/
//////        OS_curr -> sp = sp;
//////    }
//////    sp = OS_next -> sp;
//////    OS_curr = OS_next;
//////    /* pop r4 - r11 onto the stack*/
//////    __enable_irq();
////
////#define PendSV_restore
////    __asm__(
////        "IMPORT OS_curr;"
////        "IMPORT OS_next;"
//////    	"push	{r7};"
//////        "sub	sp, #12;"
//////        "add	r7, sp, #0;"
////        /* disable interrupts */
////        "cpsid	i;"
////
////        "ldr	r3, = OS_curr;"
////        "ldr	r3, [r3, #0];"
////        "cmp	r3, #0;"
////        "beq.n	PendSV_restore;"
////
////        "push {r4 - r11};"
////        "ldr	r3, = OS_curr;"
////        "ldr	r3, [r3, #0];"
//////        ldr	r2, [r7, #4]
////        /* OS_curr -> sp = sp */
////    	"str	sp, [r3, #0];"
////    "PendSV_restore:"
////        /* sp = OS_next -> sp; */
////    	"ldr	r3, = OS_next;"
////        "ldr	r3, [r3, #0];"
//////    	ldr	r3, [r3, #0]
////    	"ldr	sp, [r3, #0];"
////
////        /* OS_curr = OS_next */
////    	"ldr	r3, = OS_curr;"
////        "ldr	r3, [r3, #0];"
////        "ldr	r2, = OS_next;"
////        "str	r3, [r2, #0];"
////
////        /* pop r4 - r11 */
////        "pop {r4 - r11};"
////        /* enable irq */
////        "cpsie	i;"
////
////        "nop;"
////        "nop;"
////        "adds	r7, #12;"
////        "mov	sp, r7;"
////        "pop	{r7};"
////
////        /* return to the next thead*/
////        "bx	lr;"
////
////        "movs	r4, r0;"
////        "movs	r0, #0;"
////        "movs	r0, r1;"
////        "movs	r0, #0;"
////    );
////}
//
//void PendSV_Handler(void) {
//    // /* fake sp  as we cannot access the real sp*/
//    // void *sp;
//    // __disable_irq();
//    // /* save stack content of the current thread*/
//    // if(OS_curr != (OSThread *)0) {
//    //     /* push registers r4-r11 to the stack */
//
//    //     OS_curr->sp = sp;
//    // }
//    // sp = OS_next->sp;
//    // OS_curr = OS_next;
//
//    /* pop registers r4-r11 from the stack */
//
//#define PendSV_restore
//
//__asm__ (
//
//         "IMPORT OS_curr;"
//         "IMPORT OS_next;"
//
//    // (0x8000548 <PendSV_Handler+56>) -> Should repeat later in the code
//
//        /* disable IRQ*/
//     	"CPSID	I;"
//
//        /* if(OS_curr != (OSThread *)0) */
//   	    "LDR	r3,=OS_curr;"
//        "LDR	r3, [r3, #0x00];"
//        "CBZ    r3,PendSV_restore;"
//    	// "CMP	r3, #0;"
//    	// "BEQ.N	PendSV_restore;"
//
//        /* save stack content of the current thread */
//        /* push registers r4-r11 to the stack*/
//        "PUSH {r4-r11};"
//        /* OS_curr->sp = sp */
//    	"LDR	r3,=OS_curr;"
//    	"LDR	r3, [r3, #0x00];"
//    	"LDR	r2, [r7, #4];"
//    	"STR	sp, [r3, #0x00];"
//
//    "PendSV_restore:"
//        /* sp = OS_next->sp */
//    	"LDR	r3,=OS_next;"
//    	"LDR	r3, [r3, #0x00];"
//        "LDR	sp, [r3, #0x00];"
//
//    	"STR	r3, [r7, #4];"
//        /* OS_curr = OS_next */
//    	"LDR	r3,=OS_next;"
//    	"LDR	r3, [r3, #0x00];"
//    	"LDR	r2,=OS_curr;"
//    	"STR	r3, [r2, #0x00f];"
//        /* pop registers r4-r11 from the stack*/
//        "POP {r4-r11};"
//        /* enable IRQ */
//    	"CPSIE	I;"
//        /* return to the next thread */
//    	"BX      lr;"
//    	"NOP;"
//);
//}