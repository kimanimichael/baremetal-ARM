// /* TimeBomb/Button with uC/AO active-object framework */
// #include "qpc.h"
#include "bsp.h"
// #include "uc_ao.h"
//
// /* The TimeBomb AO =======================================================*/
// enum { INITIAL_BLINK_TIME = (OS_TICKS_PER_SEC) };
//
// typedef struct {
//     Active super;
//
//     TimeEvent te;
//     enum {
//         WAIT_FOR_BUTTON,
//         BLINK,
//         PAUSE,
//         BOOM
//     }state;
//     uint32_t blink_time;
//     uint32_t blink_ctr;
// } TimeBomb;
//
// static void TimeBomb_dispatch(TimeBomb * const me, Event const * const e) {
//     if (e->sig == INIT_SIGNAL) {
//         BSP_greenLedOn();
//         TimeEvent_arm(&me->te, (me->blink_time)*3U, 0U);
//         me->state = WAIT_FOR_BUTTON;
//     }
//     switch (me->state) {
//         case WAIT_FOR_BUTTON:
//             {
//                 switch (e->sig) {
//                     case BUTTON_PRESSED_SIG:
//                         {
//                             BSP_greenLedOff();
//                             BSP_redLedOn();
//                             TimeEvent_arm(&me->te, (me->blink_time)*3U, 0U);
//                             me->blink_ctr = 3;
//                             me->state = BLINK;
//                             break;
//                         }
//                     default:
//                         {
//                             break;
//                         }
//                 }
//                 break;
//             }
//         case BLINK:
//             {
//                 switch (e->sig) {
//                     case TIMEOUT_SIG:
//                         {
//                             BSP_redLedOff();
//                             TimeEvent_arm(&me->te, (me->blink_time)*3U, 0U);
//                             me->state = PAUSE;
//                             break;
//                         }
//                     default:
//                         {
//                             break;
//                         }
//                 }
//                 break;
//             }
//         case PAUSE:
//             {
//                 switch (e->sig) {
//                     case TIMEOUT_SIG:
//                         {
//                             if (--me->blink_ctr > 0) {
//                                 BSP_redLedOn();
//                                 TimeEvent_arm(&me->te, (me->blink_time)*3U, 0U);
//                                 me->state = BLINK;
//                             } else {
//                                 BSP_redLedOn();
//                                 BSP_greenLedOn();
//                                 BSP_blueLedOn();
//                                 me->state = BOOM;
//                             }
//                             break;
//                         }
//                     default:
//                         {
//                             break;
//                         }
//                 }
//                 break;
//             }
//         case  BOOM:
//             {
//                 BSP_blueLedOn();
//                 break;
//             }
//         default:
//             {
//                 // Q_ASSERT(0);
//                 break;
//             }
//     }
//
// }
//
// void TimeBomb_ctor(TimeBomb * const me) {
//     Active_ctor(&me->super, (DispatchHandler)&TimeBomb_dispatch);
//     TimeEvent_ctor(&me->te, TIMEOUT_SIG, &me->super);
//     me->state = BLINK;
//     me->blink_time = INITIAL_BLINK_TIME;
// }
//
// /* The TimeBomb thread =========================================================*/
// OS_STK stack_timeBomb[100]; /* task stack */
// static Event *timeBomb_queue[10];
// static TimeBomb timeBomb;
// Active *AO_TimeBomb = &timeBomb.super;
//
//
// /* the main function =========================================================*/
// int main() {
//     BSP_init(); /* initialize the BSP */
//     OSInit();   /* initialize uC/OS-II */
//
//     /* create AO and start it */
//     TimeBomb_ctor(&timeBomb);
//     Active_start(AO_TimeBomb,
//         1U,
//         timeBomb_queue,
//         sizeof(timeBomb_queue)/ sizeof(timeBomb_queue[0]),
//         stack_timeBomb,
//         sizeof(stack_timeBomb),
//         0U
//         );
//
//     QF_onStartup(); /* configure and start the interrupts */
//
//     OSStart(); /* start the uC/OS-II scheduler... */
//     return 0; /* NOTE: the scheduler does NOT return */
// }

int main() {
    BSP_init();

    enum {
        INIT_STATE,
        ON_STATE,
        OFF_STATE
    } state = INIT_STATE;

    static uint32_t start;
    uint32_t prev_button_state = 0U;

    struct {
        uint32_t now;
        uint32_t button_state;
    } evt, *e;

    e = &evt;

    while (1) {
        /* code */
        // Buffer now variable
        e->now = BSP_Tickr();
        e->button_state = BSP_user_button_read();

        switch (state) {
            case INIT_STATE:

                start = e->now;
                state = OFF_STATE;
                break;
            case OFF_STATE:
                if ((e->now - start) > (BSP_TICKS_PER_SEC / 2)) {
                    BSP_greenLedOn();
                    state = ON_STATE;
                    start = e->now;
                } else if (e->button_state != 0U && prev_button_state == 0U) {
                    BSP_blueLedOn();
                    prev_button_state = e->button_state;
                } else if (e->button_state == 0U && prev_button_state != 0U) {
                    BSP_blueLedOff();
                    prev_button_state = e->button_state;
                }
                break;
            case ON_STATE:
                if ((e->now - start) > (BSP_TICKS_PER_SEC / 2)) {
                    BSP_greenLedOff();
                    state = OFF_STATE;
                    start = e->now;
                } else if (e->button_state != 0U && prev_button_state == 0U) {
                    BSP_blueLedOn();
                    prev_button_state = e->button_state;
                } else if (e->button_state == 0U && prev_button_state != 0U) {
                    BSP_blueLedOff();
                    prev_button_state = e->button_state;
                }
                break;

            default:
                break;
        }
    }
}
