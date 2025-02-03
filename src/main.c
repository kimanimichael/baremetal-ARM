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

void TimeBomb_init(TimeBomb * const  me, Event const * const e) {
    BSP_greenLedOn();
    me->state = WAIT_FOR_BUTTON;
}

void TimeBomb_wait4button_pressed(TimeBomb * const  me, Event const * const e) {
    BSP_greenLedOff();
    BSP_redLedOn();
    TimeEvent_arm(&me->te, (blink_time), 0U);
    me->blink_ctr = 3;
    me->state = BLINK;
}

void TimeBomb_ignore(TimeBomb * const  me, Event const * const e) {

}

void TimeBomb_blink_timeout(TimeBomb * const  me, Event const * const e) {
    BSP_redLedOff();
    TimeEvent_arm(&me->te, (blink_time), 0U);
    me->state = PAUSE;
}

void TimeBomb_pause_timeout(TimeBomb * const  me, Event const * const e) {
    if (--me->blink_ctr > 0) {
        BSP_redLedOn();
        TimeEvent_arm(&me->te, (blink_time), 0U);
        me->state = BLINK;
    } else {
        BSP_redLedOn();
        BSP_greenLedOn();
        BSP_blueLedOn();
        me->state = BOOM;
    }
}

typedef void(*TimeBombAction)(TimeBomb * const  me, Event const * const e);

TimeBombAction const TimeBomb_table[MAX_STATE][MAX_SIG] = {
                        /* INIT_SIGNAL       | BUTTON_PRESSED_SIG           |BUTTON_RELEASED_SIG |   TIMEOUT_SIG */
    /*wait_for_button*/{&TimeBomb_init,   &TimeBomb_wait4button_pressed, &TimeBomb_ignore,    &TimeBomb_ignore},
    /*blink*/          {&TimeBomb_ignore, &TimeBomb_ignore,              &TimeBomb_ignore,    &TimeBomb_blink_timeout},
    /*pause*/          {&TimeBomb_ignore, &TimeBomb_ignore,              &TimeBomb_ignore,    &TimeBomb_pause_timeout},
    /*boom*/           {&TimeBomb_ignore, &TimeBomb_ignore,              &TimeBomb_ignore,    &TimeBomb_ignore}
};

static void TimeBomb_dispatch(TimeBomb * const me, Event const * const e) {
    (*TimeBomb_table[me->state][e->sig])(me, e);
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
