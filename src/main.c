#include "qpc.h"
#include "bsp.h"
#include "shape.h"

#include "stdlib.h"

Q_DEFINE_THIS_FILE

#define blocking

#ifdef blocking

QXSemaphore SW1_sema;

uint32_t stack_blinky1[40];
QXThread blinky1;

void main_blinky1(QXThread * const me) {
    while (1)
    {
        BSP_send_morse_code(0xA8EEE2A0U); /* SOS */
        QXThread_delay(1U);
    }

}

uint32_t stack_blinky2[40];
QXThread blinky2;
void main_blinky2(QXThread * const me) {
    while (1)
    {
        QXSemaphore_wait(&SW1_sema,
            QXTHREAD_NO_TIMEOUT);
        for (uint32_t volatile i = 3 * 320U;i != 0U;i--) {
            // BSP_blueLedOn();
            // BSP_blueLedOff();
            BSP_blueLedToggle();
        }
        // QXThread_delay(50U);
    }

}
uint32_t stack_blinky3[40];
QXThread blinky3;
void main_blinky3(QXThread * const me) {
    while (1) {
        BSP_send_morse_code(0xE22A3800U); /* TEST */
        BSP_send_morse_code(0xE22A3800U); /* TEST */
        QXThread_delay(5U);
    }
}

Shape s1; /* static allocation */

int main() {
    // QF_init must be called before BSP_init() which initializes a mutex(as a thread)
    Shape s2; /* automatic allocation */
    // buggy
    // Shape *ps3 = malloc(sizeof(Shape));
    //
    // free(ps3);

    Shape s3;
    // Shape const *ps1 = &s1;

    Shape_ctor(&s1, 1, 2);
    Shape_ctor(&s2, 3, 4);
    Shape_ctor(&s3, 5, 6);

    Shape_move_by(&s1, 7,8);
    Shape_move_by(&s2, 9,10);
    Shape_move_by(&s3, -1,-2);
    // Shape_move_by(ps1,-3,-4);

    Q_ASSERT(Shape_distance_from(&s1, &s1) == 0U);
    Q_ASSERT(Shape_distance_from(&s1, &s2) ==
             Shape_distance_from(&s2, &s1));
    Q_ASSERT(Shape_distance_from(&s1, &s2) <=
            Shape_distance_from(&s1, &s3) +
            Shape_distance_from(&s3, &s3));

    BSP_init();
    QF_init();
    BSP_init();
    BSP_ledInit();
    BSP_user_button_init();
    /* Initialize SW1_sema semaphore as a binary, signalling semaphore */
    QXSemaphore_init(&SW1_sema, /* pointer to the semaphore you wish to initialize */
        0U,
        1U);

    /* initialize and start blinky1 thread */
    QXThread_ctor(&blinky1, &main_blinky1, 0);
    QXTHREAD_START(&blinky1,
                    5U, /* priority */
                   (void *)0, 0, /* message queue and size of queue */
                   stack_blinky1, sizeof(stack_blinky1), /* stack */
                   (void *)0); /* extra unused parameter */

    /* initialize and start blinky2 thread */
    QXThread_ctor(&blinky2, &main_blinky2, 0);
    QXTHREAD_START(&blinky2,
                    2U, /* priority */
                   (void *)0, 0, /* message queue and size of queue */
                   stack_blinky2, sizeof(stack_blinky2), /* stack */
                   (void *)0); /* extra unused parameter */
    /* initialize and start blinky3 thread */
    QXThread_ctor(&blinky3, &main_blinky3, 0);
    QXTHREAD_START(&blinky3,
                    1U, /* priority */
                   (void *)0, 0, /* message queue and size of queue */
                   stack_blinky3, sizeof(stack_blinky3), /* stack */
                   (void *)0); /* extra unused parameter */

    QF_run();
}


#else
int main()
{
    BSP_init();
    BSP_ledInit();
    BSP_greenLedOn();

    enum {
        INIT_STATE,
        ON_STATE,
        OFF_STATE
    }state = INIT_STATE;

    uint32_t start;

    while (1)
    {
        /* code */

        switch (state)
        {
        case INIT_STATE:
            
            start = BSP_Tickr();
            state = OFF_STATE;
            break;
        case OFF_STATE:
            if ((BSP_Tickr() - start) > (BSP_TICKS_PER_SEC/2)) {
                BSP_greenLedOn();
                state = ON_STATE;
                start = BSP_Tickr();
            }
            break;
        case ON_STATE:
            if ((BSP_Tickr() - start) > (BSP_TICKS_PER_SEC/2)) {
                BSP_greenLedOff();
                state = OFF_STATE;
                start = BSP_Tickr();
            }
        break;
        
        default:
            break;
        }
    }
    
}
#endif





