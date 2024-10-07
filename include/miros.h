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

#ifndef MIROS_H
#define MIROS_H

#endif /*MIROS_H*/

/* Thread Control Block (TCB)*/
typedef struct 
{
    void *sp; /*Stack pointer*/
    /*... Other thread attributes*/
    uint32_t timeout;
    uint8_t priority; /* thread priority */
} OSThread;

/* Function pointer to function run in thread */
typedef void (*OSThreadHandler)();

/**
 * @brief Initialize thread handling setting
 */
void OS_init(void *stkSto, uint32_t stkSize);
/**
 * @brief Callback to run idle thread
 */
void OS_on_idle();

/**
 * @brief Set the bit for PendSV to be triggered if next thread is different with current thread
 * @attention Function must be called with interrupts disabled
 */
void OS_sched(void);
/**
 * @brief Callback to configure  and start interrupts
 */
void OS_onStartup(void);
/**
 * @brief Set number of clock ticks needed to unblock the calling thread
 * @param ticks
 */
void OS_delay(uint32_t ticks);
/**
 * @brief Process timout for all threads
 */
void OS_tick(void);

/**
 * @brief Transfer control to the RTOS to run threads
 */
void OS_run(void);

/**
 * @brief Fabricate Cortex-M ISR stack
 * @param me thread whose stack frame is to be created
 * @param priority priority of the thread
 * @param threadHandler function handled by the thread
 * @param stkSto allocated stack
 * @param stkSize size of allocated stack
 */
void OSThread_start(
    OSThread *me,
    uint8_t priority,
    OSThreadHandler threadHandler,
    void* stkSto, uint32_t stkSize);