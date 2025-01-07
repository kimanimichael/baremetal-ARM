#include "qpc.h"
#include "bsp.h"
#include "ucos_ii.h"

Q_DEFINE_THIS_FILE

/* The Blinky thread =========================================================*/
OS_STK stack_blinky[100]; /* task stack */

enum { INITIAL_BLINK_TIME = (OS_TICKS_PER_SEC / 4) };

/* data shared between tasks */
INT32U volatile shared_blink_time = INITIAL_BLINK_TIME;
OS_EVENT *shared_blink_time_mutex;

void main_blinky(void *pdata) { /* task function */
    (void)pdata; /* unused parameter(s) */

    while (1) { /* endless "superloop" */
        INT8U err;
        INT32U bt; /* local copy of shared_blink_time */

        OSMutexPend(shared_blink_time_mutex, 0, &err); /* mutual exclusion */
        Q_ASSERT(err == 0);
        bt = shared_blink_time;
        OSMutexPost(shared_blink_time_mutex); /* mutual exclusion */

        BSP_greenLedOn();
        OSTimeDly(bt);       /* BLOCKING! */
        BSP_greenLedOff();
        OSTimeDly(bt * 3U);  /* BLOCKING! */
    }
}

/* The Button thread =========================================================*/
OS_STK stack_button[100]; /* task stack */

void main_button(void *pdata) { /* task function */
    (void)pdata; /* unused parameter(s) */

    while (1) { /* endless "superloop" */
        INT8U err; /* uC/OS-II error status */

        /* wait on the button-press semaphore (BLOCK indefinitely) */
        OSSemPend(BSP_semaPress, 0, &err); /* BLOCKING! */
        Q_ASSERT(err == 0);
        BSP_blueLedOn();

        /* update the blink time for the 'blink' thread */
        OSMutexPend(shared_blink_time_mutex, 0, &err); /* mutual exclusion */
        Q_ASSERT(err == 0);
        shared_blink_time >>= 1; /* shorten the blink time by factor of 2 */
        if (shared_blink_time == 0U) {
            shared_blink_time = INITIAL_BLINK_TIME;
        }
        OSMutexPost(shared_blink_time_mutex); /* mutual exclusion */

        /* wait on the button-release semaphore (BLOCK indefinitely) */
        OSSemPend(BSP_semaRelease, 0, &err); /* BLOCKING! */
        BSP_blueLedOff();
    }
}

OS_EVENT *BSP_semaPress;   /* global semaphore handle */
OS_EVENT *BSP_semaRelease; /* global semaphore handle */

/* the main function =========================================================*/
int main() {
    INT8U err;

    BSP_init(); /* initialize the BSP */
    BSP_redLedOn();
    OSInit();   /* initialize uC/OS-II */

    /* initialize the RTOS objects before using them */
    BSP_semaPress   = OSSemCreate(0);
    Q_ASSERT(BSP_semaPress);
    BSP_semaRelease = OSSemCreate(0);
    Q_ASSERT(BSP_semaRelease);
    shared_blink_time_mutex = OSMutexCreate(OS_LOWEST_PRIO - 5, &err);
    Q_ASSERT(err == 0);

    /* create uC/OS-II task, see NOTE1 */
    err = OSTaskCreateExt(&main_blinky, /* the task function */
          (void *)0,      /* the 'pdata' parameter (not used) */
          &stack_blinky[(sizeof(stack_blinky) / sizeof(OS_STK)) - 1], /* ptos */
          OS_LOWEST_PRIO - 4, /* uC/OS-II task priority */
          OS_LOWEST_PRIO - 4, /* unique priority is used as the task ID */
          stack_blinky,   /* pbos */
          (INT32U)(sizeof(stack_blinky)/sizeof(OS_STK)),/* stack depth */
          (void *)0,      /* pext */
          (INT16U)0);     /* task options */
    Q_ASSERT(err == 0);

    /* create uC/OS-II task, see NOTE1 */
    err = OSTaskCreateExt(&main_button, /* the task function */
          (void *)0,      /* the 'pdata' parameter (not used) */
          &stack_button[(sizeof(stack_button) / sizeof(OS_STK)) - 1], /* ptos */
          OS_LOWEST_PRIO - 3, /* uC/OS-II task priority */
          OS_LOWEST_PRIO - 3, /* unique priority is used as the task ID */
          stack_button,   /* pbos */
          (INT32U)(sizeof(stack_button)/sizeof(OS_STK)),/* stack depth */
          (void *)0,      /* pext */
          (INT16U)0);     /* task options */
    Q_ASSERT(err == 0);

    QF_onStartup(); /* configure and start the interrupts */

    OSStart(); /* start the uC/OS-II scheduler... */
    return 0; /* NOTE: the scheduler does NOT return */
}

/*******************************************************************************
* NOTE1:
* The call to uC/OS-II API OSTaskCreateExt() assumes that the pointer to the
* top-of-stack (ptos) is at the end of the provided stack memory. This is
* correct only for CPUs with downward-growing stack, but must be changed for
* CPUs with upward-growing stack.
*/

// #define blocking
//
// #ifdef blocking
//
// QXSemaphore SW1_sema;
//
// uint32_t stack_blinky1[40];
// QXThread blinky1;
//
// void main_blinky1(QXThread * const me) {
//     while (1)
//     {
//         BSP_send_morse_code(0xA8EEE2A0U); /* SOS */
//         QXThread_delay(1U);
//     }
//
// }
//
// uint32_t stack_blinky2[40];
// QXThread blinky2;
// void main_blinky2(QXThread * const me) {
//     while (1)
//     {
//         QXSemaphore_wait(&SW1_sema,
//             QXTHREAD_NO_TIMEOUT);
//         for (uint32_t volatile i = 3 * 320U;i != 0U;i--) {
//             // BSP_blueLedOn();
//             // BSP_blueLedOff();
//             BSP_blueLedToggle();
//         }
//         // QXThread_delay(50U);
//     }
//
// }
// uint32_t stack_blinky3[40];
// QXThread blinky3;
// void main_blinky3(QXThread * const me) {
//     while (1) {
//         BSP_send_morse_code(0xE22A3800U); /* TEST */
//         BSP_send_morse_code(0xE22A3800U); /* TEST */
//         QXThread_delay(5U);
//     }
// }
//
// Shape s1; /* static allocation */
// Rectangle r1;
// uint32_t rectangle_area;
//
// int main() {
//     Shape s2; /* automatic allocation */
//     // buggy
//     // Shape *ps3 = malloc(sizeof(Shape));
//     //
//     // free(ps3);
//
//     Shape s3;
//     // Shape const *ps1 = &s1;
//
//     Shape_ctor(&s1, 1, 2);
//     Shape_ctor(&s2, 3, 4);
//     Shape_ctor(&s3, 5, 6);
//     Rectangle_ctor(&r1, 1, 2, 15, 10);
//
//     Shape const *graph[] = {
//         &s1, (Shape const *)&r1, 0
//     };
//     drawGraph(graph);
//
//     Shape_move_by(&s1, 7,8);
//     Shape_move_by(&s2, 9,10);
//     Shape_move_by(&s3, -1,-2);
//     // Shape_move_by(ps1,-3,-4);
//
//     Rectangle_draw(&r1);
//     rectangle_area = Rectangle_area(&r1);
//     Shape_move_by((Shape *)&r1, 7, 8);
//     Q_ASSERT(Shape_distance_from((Shape *)&r1, (Shape *)&r1) == 0U);
//
//     Q_ASSERT(Shape_distance_from(&s1, &s1) == 0U);
//     Q_ASSERT(Shape_distance_from(&s1, &s2) ==
//              Shape_distance_from(&s2, &s1));
//     Q_ASSERT(Shape_distance_from(&s1, &s2) <=
//             Shape_distance_from(&s1, &s3) +
//             Shape_distance_from(&s3, &s3));
//
//     // QF_init must be called before BSP_init() which initializes a mutex(as a thread)
//     QF_init();
//     BSP_init();
//     BSP_ledInit();
//     BSP_user_button_init();
//     /* Initialize SW1_sema semaphore as a binary, signalling semaphore */
//     QXSemaphore_init(&SW1_sema, /* pointer to the semaphore you wish to initialize */
//         0U,
//         1U);
//
//     /* initialize and start blinky1 thread */
//     QXThread_ctor(&blinky1, &main_blinky1, 0);
//     QXTHREAD_START(&blinky1,
//                     5U, /* priority */
//                    (void *)0, 0, /* message queue and size of queue */
//                    stack_blinky1, sizeof(stack_blinky1), /* stack */
//                    (void *)0); /* extra unused parameter */
//
//     /* initialize and start blinky2 thread */
//     QXThread_ctor(&blinky2, &main_blinky2, 0);
//     QXTHREAD_START(&blinky2,
//                     2U, /* priority */
//                    (void *)0, 0, /* message queue and size of queue */
//                    stack_blinky2, sizeof(stack_blinky2), /* stack */
//                    (void *)0); /* extra unused parameter */
//     /* initialize and start blinky3 thread */
//     QXThread_ctor(&blinky3, &main_blinky3, 0);
//     QXTHREAD_START(&blinky3,
//                     1U, /* priority */
//                    (void *)0, 0, /* message queue and size of queue */
//                    stack_blinky3, sizeof(stack_blinky3), /* stack */
//                    (void *)0); /* extra unused parameter */
//
//     QF_run();
// }
//
//
// #else
// int main()
// {
//     BSP_init();
//     BSP_ledInit();
//     BSP_greenLedOn();
//
//     enum {
//         INIT_STATE,
//         ON_STATE,
//         OFF_STATE
//     }state = INIT_STATE;
//
//     uint32_t start;
//
//     while (1)
//     {
//         /* code */
//
//         switch (state)
//         {
//         case INIT_STATE:
//
//             start = BSP_Tickr();
//             state = OFF_STATE;
//             break;
//         case OFF_STATE:
//             if ((BSP_Tickr() - start) > (BSP_TICKS_PER_SEC/2)) {
//                 BSP_greenLedOn();
//                 state = ON_STATE;
//                 start = BSP_Tickr();
//             }
//             break;
//         case ON_STATE:
//             if ((BSP_Tickr() - start) > (BSP_TICKS_PER_SEC/2)) {
//                 BSP_greenLedOff();
//                 state = OFF_STATE;
//                 start = BSP_Tickr();
//             }
//         break;
//
//         default:
//             break;
//         }
//     }
//
// }
// #endif





