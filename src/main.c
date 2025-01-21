/* BlinkyButton/Button with uC/AO active-object framework */
#include "qpc.h"
#include "bsp.h"
#include "uc_ao.h"

// Q_DEFINE_THIS_FILE

/* The BlinkyButton AO =======================================================*/
enum { INITIAL_BLINK_TIME = (OS_TICKS_PER_SEC) };

typedef struct {
    Active super;

    TimeEvent te;
    enum {
        OFF_STATE,
        ON_STATE,
    }state;
    uint32_t blink_time;
} BlinkyButton;

static void BlinkyButton_dispatch(BlinkyButton * const me, Event const * const e) {
    if (e->sig == INIT_SIGNAL) {
        BSP_blueLedOff();
        TimeEvent_arm(&me->te, (me->blink_time)*3U, 0U);
        me->state = OFF_STATE;
    }
    switch (me->state) {
        case OFF_STATE:
            {
                switch (e->sig) {
                    case TIMEOUT_SIG:
                        {
                            BSP_greenLedOn();
                            TimeEvent_arm(&me->te, (me->blink_time), 0U);
                            me->state = ON_STATE;
                            break;
                        }
                    case BUTTON_PRESSED_SIG:
                        {
                            BSP_blueLedOn();
                            me->blink_time >>= 1; /* shorten the blink time by factor of 2 */
                            if (me->blink_time == 0U) {
                                me->blink_time = INITIAL_BLINK_TIME;
                            }
                            break;
                        }
                    case BUTTON_RELEASED_SIG:
                        {
                            BSP_blueLedOff();
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
                break;
            }
        case ON_STATE:
            {
                switch (e->sig) {
                    case TIMEOUT_SIG:
                        {
                            BSP_greenLedOff();
                            TimeEvent_arm(&me->te, (me->blink_time) * 3U, 0U);
                            me->state = OFF_STATE;
                            break;
                        }
                    case BUTTON_PRESSED_SIG:
                        {
                            BSP_blueLedOn();
                            me->blink_time >>= 1; /* shorten the blink time by factor of 2 */
                            if (me->blink_time == 0U) {
                                me->blink_time = INITIAL_BLINK_TIME;
                            }
                            break;
                        }
                    case BUTTON_RELEASED_SIG:
                        {
                            BSP_blueLedOff();
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
                break;
            }
        default:
            {
                // Q_ASSERT(0);
                break;
            }
    }

}

void BlinkyButton_ctor(BlinkyButton * const me) {
    Active_ctor(&me->super, (DispatchHandler)&BlinkyButton_dispatch);
    TimeEvent_ctor(&me->te, TIMEOUT_SIG, &me->super);
    me->state = OFF_STATE;
    me->blink_time = INITIAL_BLINK_TIME;
}

/* The BlinkyButton thread =========================================================*/
OS_STK stack_blinky_button[100]; /* task stack */
static Event *BlinkyButton_queue[10];
static BlinkyButton blinkyButton;
Active *AO_BlinkyButton = &blinkyButton.super;


/* the main function =========================================================*/
int main() {
    BSP_init(); /* initialize the BSP */
    BSP_redLedOn();
    OSInit();   /* initialize uC/OS-II */

    /* create AO and start it */
    BlinkyButton_ctor(&blinkyButton);
    Active_start(AO_BlinkyButton,
        1U,
        BlinkyButton_queue,
        sizeof(BlinkyButton_queue)/ sizeof(BlinkyButton_queue[0]),
        stack_blinky_button,
        sizeof(stack_blinky_button),
        0U
        );

    QF_onStartup(); /* configure and start the interrupts */

    OSStart(); /* start the uC/OS-II scheduler... */
    return 0; /* NOTE: the scheduler does NOT return */
}
