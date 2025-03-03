//$file${AOs::../src::main.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: time_bomb.qm
// File:  ${AOs::../src::main.c}
//
// This code has been generated by QM 7.0.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// Copyright (c) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                 ____________________________________
//                /                                   /
//               /    GGGGGGG    PPPPPPPP   LL       /
//              /   GG     GG   PP     PP  LL       /
//             /   GG          PP     PP  LL       /
//            /   GG   GGGGG  PPPPPPPP   LL       /
//           /   GG      GG  PP         LL       /
//          /     GGGGGGG   PP         LLLLLLL  /
//         /___________________________________/
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open-source software licensed under the GNU
// General Public License (GPL) as published by the Free Software Foundation
// (see <https://www.gnu.org/licenses>).
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for
// closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${AOs::../src::main.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/* TimeBomb/Button with QPC framework */
#include "qpc.h"
#include "bsp.h"

// Q_DEFINE_THIS_MODULE("main") /* this module name for Q_ASSERT() */

enum { blink_time = BSP_TICKS_PER_SEC / 4U };

/* The TimeBomb AO =======================================================*/
//$declare${AOs::TimeBomb} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::TimeBomb} ...........................................................
typedef struct TimeBomb {
// protected:
    QActive super;

// private:
    QTimeEvt te;

// public:

// private:
    int32_t blink_ctr;
} TimeBomb;

extern TimeBomb TimeBomb_inst;

// public:
static void TimeBomb_ctor(TimeBomb * const me);

// protected:
static QState TimeBomb_initial(TimeBomb * const me, void const * const par);
static QState TimeBomb_armed(TimeBomb * const me, QEvt const * const e);
static QState TimeBomb_wait4Button(TimeBomb * const me, QEvt const * const e);
static QState TimeBomb_blink(TimeBomb * const me, QEvt const * const e);
static QState TimeBomb_pause(TimeBomb * const me, QEvt const * const e);
static QState TimeBomb_boom(TimeBomb * const me, QEvt const * const e);
static QState TimeBomb_defused(TimeBomb * const me, QEvt const * const e);
//$enddecl${AOs::TimeBomb} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::TimeBomb} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::TimeBomb} ...........................................................
TimeBomb TimeBomb_inst;

//${AOs::TimeBomb::ctor} .....................................................
static void TimeBomb_ctor(TimeBomb * const me) {
    QActive_ctor(&me->super, (QStateHandler)&TimeBomb_initial);
    QTimeEvt_ctorX(&me->te, &me->super, TIMEOUT_SIG, 0U);
}

//${AOs::TimeBomb::SM} .......................................................
static QState TimeBomb_initial(TimeBomb * const me, void const * const par) {
    //${AOs::TimeBomb::SM::initial}

    QS_FUN_DICTIONARY(&TimeBomb_armed);
    QS_FUN_DICTIONARY(&TimeBomb_wait4Button);
    QS_FUN_DICTIONARY(&TimeBomb_blink);
    QS_FUN_DICTIONARY(&TimeBomb_pause);
    QS_FUN_DICTIONARY(&TimeBomb_boom);
    QS_FUN_DICTIONARY(&TimeBomb_defused);

    return Q_TRAN(&TimeBomb_wait4Button);
}

//${AOs::TimeBomb::SM::armed} ................................................
static QState TimeBomb_armed(TimeBomb * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::TimeBomb::SM::armed}
        case Q_EXIT_SIG: {
            BSP_redLedOff();
            BSP_greenLedOff();
            BSP_blueLedOff();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::armed::initial}
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TimeBomb_wait4Button);
            break;
        }
        //${AOs::TimeBomb::SM::armed::BUTTON2_PRESSED}
        case BUTTON2_PRESSED_SIG: {
            status_ = Q_TRAN(&TimeBomb_defused);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${AOs::TimeBomb::SM::armed::wait4Button} ...................................
static QState TimeBomb_wait4Button(TimeBomb * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::TimeBomb::SM::armed::wait4Button}
        case Q_ENTRY_SIG: {
            BSP_greenLedOn();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::armed::wait4Button}
        case Q_EXIT_SIG: {
            BSP_greenLedOff();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::armed::wait4Button::BUTTON_PRESSED}
        case BUTTON_PRESSED_SIG: {
            me->blink_ctr = 5;
            status_ = Q_TRAN(&TimeBomb_blink);
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}

//${AOs::TimeBomb::SM::armed::blink} .........................................
static QState TimeBomb_blink(TimeBomb * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::TimeBomb::SM::armed::blink}
        case Q_ENTRY_SIG: {
            BSP_redLedOn();
            QTimeEvt_armX(&me->te, (blink_time), 0U);
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::armed::blink}
        case Q_EXIT_SIG: {
            BSP_redLedOff();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::armed::blink::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&TimeBomb_pause);
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}

//${AOs::TimeBomb::SM::armed::pause} .........................................
static QState TimeBomb_pause(TimeBomb * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::TimeBomb::SM::armed::pause}
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->te, (blink_time), 0U);
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::armed::pause::TIMEOUT}
        case TIMEOUT_SIG: {
            --me->blink_ctr;
            //${AOs::TimeBomb::SM::armed::pause::TIMEOUT::[blink_ctr>0]}
            if (--me->blink_ctr > 0) {
                status_ = Q_TRAN(&TimeBomb_blink);
            }
            //${AOs::TimeBomb::SM::armed::pause::TIMEOUT::[else]}
            else {
                status_ = Q_TRAN(&TimeBomb_boom);
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}

//${AOs::TimeBomb::SM::armed::boom} ..........................................
static QState TimeBomb_boom(TimeBomb * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::TimeBomb::SM::armed::boom}
        case Q_ENTRY_SIG: {
            BSP_redLedOn();
            BSP_greenLedOn();
            BSP_blueLedOn();
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&TimeBomb_armed);
            break;
        }
    }
    return status_;
}

//${AOs::TimeBomb::SM::defused} ..............................................
static QState TimeBomb_defused(TimeBomb * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::TimeBomb::SM::defused}
        case Q_ENTRY_SIG: {
            BSP_blueLedOn();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::defused}
        case Q_EXIT_SIG: {
            BSP_blueLedOff();
            status_ = Q_HANDLED();
            break;
        }
        //${AOs::TimeBomb::SM::defused::BUTTON2_PRESSED}
        case BUTTON2_PRESSED_SIG: {
            status_ = Q_TRAN(&TimeBomb_armed);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
//$enddef${AOs::TimeBomb} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

