/* TimeBomb/Button with uC/AO active-object framework */

#include "bsp.h"
#include "uc_ao.h"
#include "qpc.h"

// Q_DEFINE_THIS_MODULE("main") /* this module name for Q_ASSERT() */

enum { blink_time = OS_TICKS_PER_SEC * 27U };

/* The TimeBomb AO =======================================================*/
typedef struct TimeBomb TimeBomb;

struct TimeBomb {
    Active super;
    TimeEvent te;

    uint32_t blink_ctr;
} ;

State TimeBomb_armed(TimeBomb *  me, Event const * e);
State TimeBomb_wait_for_button(TimeBomb *  me, Event const * e);
State TimeBomb_blink(TimeBomb *  me, Event const * e);
State TimeBomb_pause(TimeBomb *  me, Event const * e);
State TimeBomb_boom(TimeBomb *  me, Event const * e);
State TimeBomb_defused(TimeBomb *  me, Event const * e);
State TimeBomb_initial(TimeBomb *  me, Event const * e);


State TimeBomb_initial(TimeBomb * const  me, Event const * const e) {
    return  TRAN(TimeBomb_wait_for_button);
}

State TimeBomb_armed(TimeBomb * const  me, Event const * const e) {
    State status;
    switch (e->sig) {
        case BUTTON2_PRESSED_SIG: {
                status = TRAN(TimeBomb_defused);
                break;
        }
        case ENTRY_SIGNAL: {
                status = TRAN(TimeBomb_initial);
                break;
            }
        default: {
                status = SUPER(HSM_top);
                break;
        }
    }
    return status;
}

State TimeBomb_wait_for_button(TimeBomb * const  me, Event const * const e) {
    State status;
    switch (e->sig) {

        case ENTRY_SIGNAL: {
                BSP_greenLedOn();
                status = HANDLED_STATUS;
                break;
            }
        case EXIT_SIGNAL: {
                BSP_greenLedOff();
                status = HANDLED_STATUS;
                break;
            }
        case BUTTON_PRESSED_SIG: {
                me->blink_ctr = 3;
                status = TRAN(TimeBomb_blink);
                break;
            }
        default: {
                status = SUPER(TimeBomb_armed);
                break;
            }
    }
    return status;
}

State TimeBomb_blink(TimeBomb * const  me, Event const * const e) {
    State status;
    switch (e->sig) {
        case ENTRY_SIGNAL: {
                BSP_redLedOn();
                TimeEvent_arm(&me->te, (blink_time), 0U);
                status = HANDLED_STATUS;
                break;
        }
        case EXIT_SIGNAL: {
                BSP_redLedOff();
                status = HANDLED_STATUS;
                break;
        }
        case TIMEOUT_SIG: {
                status = TRAN(TimeBomb_pause);
                break;
        }
        default: {
                status = SUPER(TimeBomb_armed);
                break;
        }
    }
    return status;
}

State TimeBomb_pause(TimeBomb * const  me, Event const * const e) {
    State status;
    switch (e->sig) {
        case ENTRY_SIGNAL: {
                TimeEvent_arm(&me->te, (blink_time), 0U);
                status = HANDLED_STATUS;
                break;
        }

        case TIMEOUT_SIG: {
                if (--me->blink_ctr > 0) {
                    status = TRAN(TimeBomb_blink);
                } else {
                    status = TRAN(TimeBomb_boom);
                }
                break;
        }
        default: {
                status = SUPER(TimeBomb_armed);
                break;
        }
    }
    return status;
}

State TimeBomb_boom(TimeBomb * const  me, Event const * const e) {
    State status;
    switch (e->sig) {

        case ENTRY_SIGNAL: {
                BSP_redLedOn();
                BSP_greenLedOn();
                BSP_blueLedOn();
                status = HANDLED_STATUS;
                break;
        }

        case EXIT_SIGNAL: {
                BSP_redLedOff();
                BSP_greenLedOff();
                BSP_blueLedOff();
                status = HANDLED_STATUS;
                break;
        }
        default: {
                status = SUPER(TimeBomb_armed);
                break;
        }
    }
    return status;
}

State TimeBomb_defused(TimeBomb * const  me, Event const * const e) {
    State status;
    switch (e->sig) {

        case ENTRY_SIGNAL: {
                BSP_blueLedOn();
                status = HANDLED_STATUS;
                break;
        }
        case EXIT_SIGNAL: {
                BSP_blueLedOff();
                status = HANDLED_STATUS;
                break;
        }
        case BUTTON2_PRESSED_SIG: {
                status = TRAN(TimeBomb_armed);
                break;
            }
        default: {
                status = SUPER(HSM_top);
                break;
        }
    }
    return status;
}


void TimeBomb_ctor(TimeBomb * const me) {
    Active_ctor(&me->super, (StateHandler)TimeBomb_initial);
    TimeEvent_ctor(&me->te, TIMEOUT_SIG, &me->super);
}

/* The TimeBomb thread =========================================================*/
OS_STK stack_timeBomb[100]; /* task stack */
static Event *timeBomb_queue[10];
static TimeBomb timeBomb;
Active *AO_TimeBomb = &timeBomb.super;


/* the main function =========================================================*/
int main() {
    BSP_init(); /* initialize the BSP */
    OSInit();   /* initialize uC/OS-II */

    /* create AO and start it */
    TimeBomb_ctor(&timeBomb);
    Active_start(AO_TimeBomb,
        1U,
        timeBomb_queue,
        sizeof(timeBomb_queue)/ sizeof(timeBomb_queue[0]),
        stack_timeBomb,
        sizeof(stack_timeBomb),
        0U
        );

    QF_onStartup(); /* configure and start the interrupts */

    OSStart(); /* start the uC/OS-II scheduler... */
    return 0; /* NOTE: the scheduler does NOT return */
}
