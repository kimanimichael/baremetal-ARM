/*****************************************************************************
* Active Object pattern implementation based on uC/OS-II (uC/AO)
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2020 Quantum Leaps, LLC. All rights reserved.
*
* SPDX-License-Identifier: APACHE-2.0
*
* This software is distributed by Quantum Leaps, LLC under the terms of
* Apache License Version 2.0, which is the same license used for uC/OS-II RTOS.
* The text of the license is available at: www.apache.org/licenses/LICENSE-2.0.
*
* Contact information:
* <www.state-machine.com>
* <info@state-machine.com>
*****************************************************************************/
#ifndef _UC_AO_H_
#define _UC_AO_H_

#include "ucos_ii.h"
/*---------------------------------------------------------------------------*/
/* Event facilities... */

typedef uint16_t Signal;

enum ReservedSignals {
    INIT_SIGNAL = 0, /* dispatched to AO before entering event-loop */
    ENTRY_SIGNAL,
    EXIT_SIGNAL,
    USER_SIGNAL, /* first signal available to the users */
};

/* Event base class */
typedef struct {
    Signal sig; /* event signal */
    /* event parameters added in subclasses of Event */
} Event;

/*---------------------------------------------------------------------------*/
/* Hierarchical State Machine facilities... */

typedef struct HSM HSM; /* forward declaration */

typedef enum{TRAN_STATUS, HANDLED_STATUS, IGNORED_STATUS, INIT_STATUS, SUPER_STATUS} State;

typedef State(*StateHandler)(HSM * const me, Event const * const e);

#define TRAN(target_) (((HSM*)me)->state = (StateHandler)(target_), TRAN_STATUS)
#define SUPER(super_) (((HSM*)me)->temp = (StateHandler)(super_), SUPER_STATUS)

struct HSM {
    StateHandler state; /* the "state" variable */
    StateHandler temp; /* top state machine variable */
};

void HSM_ctor(HSM * const me, StateHandler initial);
void HSM_init(HSM * const me, Event const * const e);
void HSM_dispatch(HSM * const me, Event const * const e);

State HSM_top(HSM * const me, Event const * const e);

/*---------------------------------------------------------------------------*/
/* Active Object facilities... */

typedef struct Active Active; /* forward declaration */

/* Active Object base class */
struct Active {
    HSM super;
    INT8U thread; /* private thread (the unique uC/OS-II task priority) */
    OS_EVENT *queue; /* private message queue */

    /* active object data added in subclasses of Active */
};

void Active_ctor(Active * const me, StateHandler initial);
void Active_start(Active * const me,
                  uint8_t prio,             /* priority (1-based) */
                  Event **queueSto,
                  uint32_t queueLen,
                  void *stackSto,
                  uint32_t stackSize,
                  uint16_t opt);
void Active_post(Active * const me, Event const * const e);

typedef struct {
    Event super;
    Active *act;
    uint32_t timeout;
    uint32_t interval;
} TimeEvent;

void TimeEvent_ctor(TimeEvent * const me, Signal sig, Active *act);
void TimeEvent_arm(TimeEvent * const me, uint32_t timeout, uint32_t interval);
void TimeEvent_disarm(TimeEvent * const me);

/* static operation */
void TimeEvent_tick(void);

#endif
