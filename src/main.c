/* TimeBomb/Button with uC/AO active-object framework */
#include "qpc.h"
#include "bsp.h"
#include "uc_ao.h"

/* The TimeBomb AO =======================================================*/
enum { blink_time = OS_TICKS_PER_SEC * 3U };

typedef struct {
    Active super;
    TimeEvent te;

    enum {
        WAIT_FOR_BUTTON,
        BLINK,
        PAUSE,
        BOOM,
        /* */
        MAX_STATE
    }state;
    uint32_t blink_ctr;
} TimeBomb;

typedef enum {TRAN_STATUS, HANDLED_STATUS, IGNORED_STATUS, INIT_STATUS} Status;

Status TimeBomb_init(TimeBomb * const  me, Event const * const e) {
    me->state = WAIT_FOR_BUTTON;
    return INIT_STATUS;
}

Status TimeBomb_wait4button_entry(TimeBomb * const  me, Event const * const e) {
    BSP_greenLedOn();
    return HANDLED_STATUS;
}
Status TimeBomb_wait4button_exit(TimeBomb * const  me, Event const * const e) {
    BSP_greenLedOff();
    return HANDLED_STATUS;
}

Status TimeBomb_wait4button_pressed(TimeBomb * const  me, Event const * const e) {
    me->blink_ctr = 3;
    me->state = BLINK;
    return TRAN_STATUS;
}

Status TimeBomb_ignore(TimeBomb * const  me, Event const * const e) {
    return IGNORED_STATUS;
}

Status TimeBomb_blink_entry(TimeBomb * const  me, Event const * const e) {
    BSP_redLedOn();
    TimeEvent_arm(&me->te, (blink_time), 0U);
    return HANDLED_STATUS;
}
Status TimeBomb_blink_exit(TimeBomb * const  me, Event const * const e) {
    BSP_redLedOff();
    return HANDLED_STATUS;
}

Status TimeBomb_blink_timeout(TimeBomb * const  me, Event const * const e) {
    me->state = PAUSE;
    return TRAN_STATUS;
}

Status TimeBomb_pause_entry(TimeBomb * const  me, Event const * const e) {
    TimeEvent_arm(&me->te, (blink_time), 0U);
    return HANDLED_STATUS;
}

Status TimeBomb_pause_timeout(TimeBomb * const  me, Event const * const e) {
    if (--me->blink_ctr > 0) {
        me->state = BLINK;
    } else {
        me->state = BOOM;
    }
    return TRAN_STATUS;
}

Status TimeBomb_boom_entry(TimeBomb * const  me, Event const * const e) {
    BSP_redLedOn();
    BSP_greenLedOn();
    BSP_blueLedOn();
    return HANDLED_STATUS;
}

typedef Status(*TimeBombAction)(TimeBomb * const  me, Event const * const e);

TimeBombAction const TimeBomb_table[MAX_STATE][MAX_SIG] = {
                        /* INIT_SIGNAL       | ENTRY SIGNAL               | EXIT SIGNAL               | BUTTON_PRESSED_SIG           |BUTTON_RELEASED_SIG |   TIMEOUT_SIG */
    /*wait_for_button*/{&TimeBomb_init,   &TimeBomb_wait4button_entry, &TimeBomb_wait4button_exit, &TimeBomb_wait4button_pressed, &TimeBomb_ignore,    &TimeBomb_ignore},
    /*blink*/          {&TimeBomb_ignore, &TimeBomb_blink_entry,       &TimeBomb_blink_exit,       &TimeBomb_ignore,              &TimeBomb_ignore,    &TimeBomb_blink_timeout},
    /*pause*/          {&TimeBomb_ignore, &TimeBomb_pause_entry,       &TimeBomb_ignore,           &TimeBomb_ignore,              &TimeBomb_ignore,    &TimeBomb_pause_timeout},
    /*boom*/           {&TimeBomb_ignore, &TimeBomb_boom_entry,        &TimeBomb_ignore,           &TimeBomb_ignore,              &TimeBomb_ignore,    &TimeBomb_ignore}
};

static void TimeBomb_dispatch(TimeBomb * const me, Event const * const e) {
    Status stat;
    int prev_state = me->state;
    stat = (*TimeBomb_table[me->state][e->sig])(me, e);
    if (stat == TRAN_STATUS) {
        (*TimeBomb_table[prev_state][EXIT_SIGNAL])(me, (Event *)0);
        (*TimeBomb_table[me->state][ENTRY_SIGNAL])(me, (Event *)0);
    } else if (stat == INIT_STATUS) {
        (*TimeBomb_table[me->state][ENTRY_SIGNAL])(me, (Event *)0);
    }
}

void TimeBomb_ctor(TimeBomb * const me) {
    Active_ctor(&me->super, (DispatchHandler)&TimeBomb_dispatch);
    TimeEvent_ctor(&me->te, TIMEOUT_SIG, &me->super);
    me->state = WAIT_FOR_BUTTON;
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
