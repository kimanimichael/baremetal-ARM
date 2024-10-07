/****************************************************************************
* MInimal Real-time Operating System (MiROS), ARM-CLANG port.
* version 1.26 (matching lesson 26, see https://youtu.be/kLxxXNCrY60)
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
* SPDX-License-Identifier: GPL-3.0-or-later
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
* Git repo:
* https://github.com/QuantumLeaps/MiROS
****************************************************************************/
#include <stdint.h>
#include "../../include/cmsis/stm32f429xx.h"
#include "../../include/miros.h"

#include <stdbool.h>

#include "../../qpc/include/qsafe.h"

Q_DEFINE_THIS_FILE

OSThread * volatile OS_curr; /* pointer to the current thread */
OSThread * volatile OS_next; /* pointer to the next thread to run */

OSThread *OS_thread[32 + 1]; /* array of threads */
uint32_t OS_ready_set; /* bit mask of threads that are ready to run*/
uint32_t OS_delayed_set; /* bitmask of threads that are delayed */

#define LOG2(x) (32 - __CLZ(x))

OSThread idle_thread;

void main_idle() {
    while(1) {
        OS_on_idle();
    }
}



void OS_init(void *stkSto, uint32_t stkSize) {
    /* set the PendSV interrupt priority to the lowest level 0xFF */
    *(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);

    OSThread_start(&idle_thread,
        0,
    &main_idle,
    stkSto,
    stkSize
    );
}

void OS_sched(void) {
    if (OS_ready_set == 0U) {
        OS_next = OS_thread[0];
    }
    else {
            OS_next = OS_thread[LOG2(OS_ready_set)];
            Q_ASSERT(OS_next != (OSThread*)0);
    }

        /* OS_next = ... */
        OSThread const *next = OS_next; /* volatile to temporary */
        if (next != OS_curr) {
            *(uint32_t volatile *)0xE000ED04 = (1U << 28);
        }
}

void OS_run(void) {
    OS_onStartup();

    __disable_irq();
    OS_sched();
    __enable_irq();
    /* code should never get here as the PendSV exception occurs immediately after enabling interrupts*/
    Q_ERROR();
}

void OS_tick(void) {
    uint32_t working_set = OS_delayed_set;
    while (working_set != 0U) {
        OSThread *t = OS_thread[LOG2(working_set)];
        Q_ASSERT(t != (OSThread*)0 && t->timeout != 0);

        uint32_t bit = (1U << (t->priority - 1U));
        --t->timeout;
        if (t->timeout == 0U) {
            OS_ready_set |= bit; /* insert thread into ready set*/
            OS_delayed_set &= ~bit; /* remove thread from delayed set*/
        }
        working_set &= ~bit; /* remove from working set as it's already processed */
    }
}

void OS_delay(uint32_t ticks) {
    __disable_irq();
    /* OS_delay should never be called from the idle thread */
    Q_REQUIRE(OS_curr != OS_thread[0]);

    OS_curr->timeout = ticks;

    uint32_t bit = (1U << (OS_curr->priority - 1U));
    OS_ready_set &= ~bit;
    OS_delayed_set |= bit;
    OS_sched();
    /* This switches the context immediately away from this thread as PendSV exception occurs immediately after enabling interrupts */
    __enable_irq();
}

void OSThread_start(
        OSThread *me,
        uint8_t priority,
        OSThreadHandler threadHandler,
        void *stkSto, uint32_t stkSize)
{
    /* round down the stack top to the 8-byte boundary
    * NOTE: ARM Cortex-M stack grows down from hi -> low memory
    */
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize) / 8) * 8);
    uint32_t *stk_limit;

    /*
    * priority must be in range && must not have been used before
    */
    Q_REQUIRE((priority < Q_DIM(OS_thread))
        && (OS_thread[priority] == (OSThread *)0));

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

    /* save the top of the stack in the thread's attribute */
    me->sp = sp;

    /* round up the bottom of the stack to the 8-byte boundary */
    stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);

    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }

    OS_thread[priority] = me;
    me->priority = priority;
    /* Make thread ready to run. Except for the idle thread of course */
    if (priority > 0) {
        OS_ready_set |= (1 << (priority - 1));
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