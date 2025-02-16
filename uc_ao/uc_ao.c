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
#include "uc_ao.h"
#include "qpc.h"

Q_DEFINE_THIS_FILE

/* Finite State Machine facilities... */

static Event const entryEvt = {ENTRY_SIGNAL};
static Event const exitEvt = {EXIT_SIGNAL};

void FSM_ctor(FSM * const me, StateHandler initial) {
    me->state = initial;
}
void FSM_init(FSM * const me, Event const * const e) {
    Q_ASSERT(me->state != (StateHandler)0);
    (*me->state)(me, e);
    Q_ASSERT(me->state != (StateHandler)0);
    (*me->state)(me, &entryEvt);
}
void FSM_dispatch(FSM * const me, Event const * const e){
    StateHandler prev_state = me->state;

    Q_ASSERT(me->state != (StateHandler)0);
    State stat = (*me->state)(me, e); /* Updates me->state if transition is needed */
    if (stat == TRAN_STATUS) { /* Transition taken? */
        Q_ASSERT(me->state != (StateHandler)0);
        (*prev_state)(me, &exitEvt);
        (*me->state)(me, &entryEvt);
    }
}

/*..........................................................................*/
void Active_ctor(Active * const me, StateHandler initial) {
    FSM_ctor(&me->super, initial);
}

/*..........................................................................*/
/* Thread function for all Active Objects (uC/OS-II task signature) */
static  void Active_event_loop(void *pdata) {
    Active *me = (Active *)pdata; /* the AO instance "me" */

    /* initialize the AO */
    FSM_init(&me->super, (Event*)0);

    /* event loop ("message pump") */
    while (1) {
        Event *e;
        INT8U err;

        /* wait for any event and receive it into object 'e' */
        e = OSQPend(me->queue, 0U, &err);
        Q_ASSERT(err == 0);

        /* dispatch event to the active object 'me' */
        FSM_dispatch(&me->super, e); /* NO BLOCKING ' */
    }
}

/*..........................................................................*/
void Active_start(Active * const me,
                  uint8_t prio,             /* priority (1-based) */
                  Event **queueSto,
                  uint32_t queueLen,
                  void *stackSto,
                  uint32_t stackSize,
                  uint16_t opt)
{
    INT8U err;
    OS_STK *stk_sto = stackSto;
    INT32U stk_depth = (stackSize / sizeof(OS_STK));

    Q_ASSERT(me /* AO instance must be provided (cannot be NULL) */
        && (0 < prio) && (prio < OS_LOWEST_PRIO - 2U));

    me->queue = OSQCreate((void **)queueSto, queueLen);
    Q_ASSERT(me->queue); /* queue must be created */

    me->thread = OS_LOWEST_PRIO - 2U - prio;

    err = OSTaskCreateExt(
        &Active_event_loop,
        me,
#if OS_STK_GROWTH
        &stk_sto[stk_depth - 1],  /* ptos */
#else
            stk_sto,                  /* ptos */
#endif
        me->thread,
        prio,
        stk_sto,
        stk_depth,
        (void *)0,
        opt);
    Q_ASSERT(err == 0); /* thread must be created */
}


void Active_post(Active * const me, Event const * const e) {
    OSQPost(me->queue, (void *)e);
}

static TimeEvent *l_tevt[10];
static uint_fast8_t l_tevtNum;

void TimeEvent_ctor(TimeEvent * const me, Signal sig, Active *act) {
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
    #endif
    me->super.sig = sig;
    me->act = act;
    me->timeout = 0U;
    me->interval = 0U;

    OS_ENTER_CRITICAL();
    // Q_ASSERT(l_tevtNum < sizeof(l_tevt)/sizeof(l_tevt[0]));
    l_tevt[l_tevtNum] = me;
    l_tevtNum++;
    OS_EXIT_CRITICAL();
}

void TimeEvent_arm(TimeEvent * const me, uint32_t timeout, uint32_t interval) {
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
    #endif
    OS_ENTER_CRITICAL();
    me->timeout = timeout;
    me->interval = interval;
    OS_EXIT_CRITICAL();
}

void TimeEvent_disarm(TimeEvent * const me) {
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
    #endif

    OS_ENTER_CRITICAL();
    me->timeout = 0U;
    OS_EXIT_CRITICAL();
}

void TimeEvent_tick(void) {
    for (uint_fast8_t i = 0; i < l_tevtNum; i++) {
        Q_ASSERT(l_tevt[i]);
        if (l_tevt[i]->timeout > 0U) {
            l_tevt[i]->timeout--;
            if (l_tevt[i]->timeout == 0U) {
                Active_post(l_tevt[i]->act, &l_tevt[i]->super);
                l_tevt[i] -> timeout = l_tevt[i]->interval;
            }
        }
    }
}
