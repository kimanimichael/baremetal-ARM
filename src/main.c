#include "stdint.h"

#include "../include/bsp.h"
#include "../include/miros.h"


/**
 * @brief main program function
 * @author @Mike-Kimani
*/

#define blocking

#ifdef blocking

void main_blinky1();
void main_blinky2();

uint32_t stack_blinky1[40];
OSThread blinky1;
// uint32_t *sp_blinky1 = &stack_blinky1[40];

void main_blinky1() {
    while (1)
    {
        BSP_greenLedToggle();
        BSP_Delay(50);
    }

}
uint32_t stack_blinky2[40];
//uint32_t *sp_blinky2 = &stack_blinky2[40];

OSThread blinky2;
// uint32_t *sp_blinky2 = &stack_blinky2[40];
void main_blinky2() {
    while (1)
    {
        BSP_blueLedToggle();
        BSP_Delay(50);
    }

}

int main() {
    BSP_init();
    OS_init();
    BSP_ledInit();
    BSP_blueLedOn();
    BSP_blueLedOff();
//    main_blinky2();
//
    while (1) {

        // Set the thumb state
        OSThread_start(&blinky1,
                       &main_blinky1,
                       stack_blinky1,
                       sizeof(stack_blinky1));

        OSThread_start(&blinky2,
                       &main_blinky2,
                       stack_blinky2,
                       sizeof(stack_blinky2));

        while (1) {
            /* code */
            BSP_blueLedOff();
        }
    }
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





