#include "stdint.h"
#include "../include/bsp.h"
#include "../include/miros.h"
#include "../include/main.h"

#define blocking

#ifdef blocking

uint32_t stack_blinky1[40];
OSThread blinky1;

void main_blinky1() {
    while (1)
    {
        BSP_greenLedToggle();
        BSP_Delay(50);
    }

}

uint32_t stack_blinky2[40];
OSThread blinky2;
void main_blinky2() {
    while (1)
    {
        BSP_blueLedToggle();
        BSP_Delay(50);
    }

}
uint32_t stack_blinky3[40];
OSThread blinky3;
void main_blinky3() {
    while (1) {
        BSP_redLedToggle();
        BSP_Delay(100);
    }
}

int main() {
    BSP_init();
    OS_init();
    BSP_ledInit();

    /* fabricate Cortex-M ISR stack for blinky1 */
    OSThread_start(&blinky1,
                   &main_blinky1,
                   stack_blinky1,
                   sizeof(stack_blinky1));
    /* fabricate Cortex-M ISR stack for blinky2 */
    OSThread_start(&blinky2,
                   &main_blinky2,
                   stack_blinky2,
                   sizeof(stack_blinky2));
    /* fabricate Cortex-M ISR stack for blinky3 */
    OSThread_start(&blinky3,
        &main_blinky3,
        stack_blinky3,
        sizeof(stack_blinky3));

    OS_run();
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





