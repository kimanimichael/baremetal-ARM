#ifdef NO_NORETURN
#define _Noreturn [[noreturn]]
#endif

extern "C"{
#include "qpc.h"
}
#undef _Noreturn

#include "bsp.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"

#include "stdlib.h"

Q_DEFINE_THIS_FILE

#define blocking

#ifdef blocking

Shape s1(1, 2); /* static allocation */
Rectangle r1(1, 2, 15, 10);
uint32_t rectangle_area;
uint32_t upcast_rectangle_area;

Circle c1(3, 5, 10);

QXSemaphore SW1_sema;

uint32_t stack_blinky1[40];
QXThread blinky1;
void main_blinky1(QXThread * const me) {
    while (1)
    {
        BSP_send_morse_code(0xA8EEE2A0U); /* SOS */

        s1.move_by(7, 8);

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

            Q_ASSERT(s1.distance_from(&s1) == 0U);
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

int main() {
    Shape s2(3, 4); /* automatic allocation */
    // buggy
    // Shape *ps3 = malloc(sizeof(Shape));
    //
    // free(ps3);

    Shape s3(5, 6);
    // Shape const *ps1 = &s1;

    Shape* ps = &r1;

    r1.draw(); /* early binding */
    ps->draw(); /* late binding */

    upcast_rectangle_area = ps->area();

    Shape *graph[] = {&c1, &r1, &s3, (Shape *)nullptr};

    drawGraph(graph);

    s1.move_by(7, 8);
    s2.move_by(9, 10);
    s3.move_by(-1, -2);

    // Shape_move_by(ps1,-3,-4);

    r1.draw();
    rectangle_area = r1.area();

    r1.move_by(7, 8);
    Q_ASSERT(r1.distance_from(&r1) == 0U);

    Q_ASSERT(s1.distance_from(&s1) == 0U);
    Q_ASSERT(s1.distance_from(&s2) ==
             s2.distance_from(&s1));
    Q_ASSERT(s1.distance_from(&s2) <=
            s1.distance_from(&s3) +
            s3.distance_from(&s2));

    // QF_init must be called before BSP_init() which initializes a mutex(as a thread)
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
                   (QEvt const **)0, 0, /* message queue and size of queue */
                   stack_blinky1, sizeof(stack_blinky1), /* stack */
                   (QEvt *)0); /* extra unused parameter */

    /* initialize and start blinky2 thread */
    QXThread_ctor(&blinky2, &main_blinky2, 0);
    QXTHREAD_START(&blinky2,
                    2U, /* priority */
                   (QEvt const **)0, 0, /* message queue and size of queue */
                   stack_blinky2, sizeof(stack_blinky2), /* stack */
                   (QEvt *)0); /* extra unused parameter */
    /* initialize and start blinky3 thread */
    QXThread_ctor(&blinky3, &main_blinky3, 0);
    QXTHREAD_START(&blinky3,
                    1U, /* priority */
                   (QEvt const **)0, 0, /* message queue and size of queue */
                   stack_blinky3, sizeof(stack_blinky3), /* stack */
                   (QEvt *)0); /* extra unused parameter */

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





