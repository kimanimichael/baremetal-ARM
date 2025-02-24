/* TimeBomb/Button with QPC framework */
#include "qpc.h"
#include "bsp.h"

// Q_DEFINE_THIS_MODULE("main") /* this module name for Q_ASSERT() */

enum { blink_time = BSP_TICKS_PER_SEC / 4U };

/* The TimeBomb AO =======================================================*/
typedef struct TimeBomb TimeBomb;

struct TimeBomb {
    QActive super;
    QTimeEvt te;

    uint32_t blink_ctr;
} ;

QState TimeBomb_armed(TimeBomb *  me, QEvt const * e);
QState TimeBomb_wait_for_button(TimeBomb *  me, QEvt const * e);
QState TimeBomb_blink(TimeBomb *  me, QEvt const * e);
QState TimeBomb_pause(TimeBomb *  me, QEvt const * e);
QState TimeBomb_boom(TimeBomb *  me, QEvt const * e);
QState TimeBomb_defused(TimeBomb *  me, QEvt const * e);
QState TimeBomb_initial(TimeBomb *  me, QEvt const * e);


QState TimeBomb_initial(TimeBomb * const  me, QEvt const * const e) {
    return  Q_TRAN(TimeBomb_wait_for_button);
}

QState TimeBomb_armed(TimeBomb * const  me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_EXIT_SIG: {
                BSP_redLedOff();
                BSP_greenLedOff();
                BSP_blueLedOff();
                status = Q_HANDLED();
                break;
        }
        case Q_INIT_SIG:{
                status = Q_TRAN(TimeBomb_wait_for_button);
                break;
            }
        case BUTTON2_PRESSED_SIG: {
                status = Q_TRAN(TimeBomb_defused);
                break;
        }
        default: {
                status = Q_SUPER(QHsm_top);
                break;
        }
    }
    return status;
}

QState TimeBomb_wait_for_button(TimeBomb * const  me, QEvt const * const e) {
    QState status;
    switch (e->sig) {

        case Q_ENTRY_SIG: {
                BSP_greenLedOn();
                status = Q_HANDLED();
                break;
            }
        case Q_EXIT_SIG: {
                BSP_greenLedOff();
                status = Q_HANDLED();
                break;
            }
        case BUTTON_PRESSED_SIG: {
                me->blink_ctr = 5;
                status = Q_TRAN(TimeBomb_blink);
                break;
            }
        default: {
                status = Q_SUPER(TimeBomb_armed);
                break;
            }
    }
    return status;
}

QState TimeBomb_blink(TimeBomb * const  me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
                BSP_redLedOn();
                QTimeEvt_armX(&me->te, (blink_time), 0U);
                status = Q_HANDLED();
                break;
        }
        case Q_EXIT_SIG: {
                BSP_redLedOff();
                status = Q_HANDLED();
                break;
        }
        case TIMEOUT_SIG: {
                status = Q_TRAN(TimeBomb_pause);
                break;
        }
        default: {
                status = Q_SUPER(TimeBomb_armed);
                break;
        }
    }
    return status;
}

QState TimeBomb_pause(TimeBomb * const  me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
                QTimeEvt_armX(&me->te, (blink_time), 0U);
                status = Q_HANDLED();
                break;
        }

        case TIMEOUT_SIG: {
                if (--me->blink_ctr > 0) {
                    status = Q_TRAN(TimeBomb_blink);
                } else {
                    status = Q_TRAN(TimeBomb_boom);
                }
                break;
        }
        default: {
                status = Q_SUPER(TimeBomb_armed);
                break;
        }
    }
    return status;
}

QState TimeBomb_boom(TimeBomb * const  me, QEvt const * const e) {
    QState status;
    switch (e->sig) {

        case Q_ENTRY_SIG: {
                BSP_redLedOn();
                BSP_greenLedOn();
                BSP_blueLedOn();
                status = Q_HANDLED();
                break;
        }

        default: {
                status = Q_SUPER(TimeBomb_armed);
                break;
        }
    }
    return status;
}

QState TimeBomb_defused(TimeBomb * const  me, QEvt const * const e) {
    QState status;
    switch (e->sig) {

        case Q_ENTRY_SIG: {
                BSP_blueLedOn();
                status = Q_HANDLED();
                break;
        }
        case Q_EXIT_SIG: {
                BSP_blueLedOff();
                status = Q_HANDLED();
                break;
        }
        case BUTTON2_PRESSED_SIG: {
                status = Q_TRAN(TimeBomb_armed);
                break;
        }
        default: {
                status = Q_SUPER(QHsm_top);
                break;
        }
    }
    return status;
}

void TimeBomb_ctor(TimeBomb * const me) {
    QActive_ctor(&me->super, (QStateHandler)&TimeBomb_initial);
    QTimeEvt_ctorX(&me->te, &me->super, TIMEOUT_SIG, 0U);
}

static QEvt const *timeBomb_queue[10];
static TimeBomb timeBomb;
QActive *AO_TimeBomb = &timeBomb.super;


/* the main function =========================================================*/
int main() {
    BSP_init(); /* initialize the BSP */
    QF_init();   /* initialize QP/C */

    /* create AO and start it */
    TimeBomb_ctor(&timeBomb);
    QACTIVE_START(AO_TimeBomb,
        2U,
        timeBomb_queue,
        sizeof(timeBomb_queue)/ sizeof(timeBomb_queue[0]),
        (void*)0, 0U,
        (void*)0
        );

    QF_run(); /* run QP/C */
    return 0; /* NOTE: the scheduler does NOT return */
}
