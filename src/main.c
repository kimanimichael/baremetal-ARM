#include "stdint.h"

#include "../include/bsp.h"


/**
 * @brief main program function
 * @author @Mike-Kimani
*/

#define blocking

#ifdef blocking

void main_blinky1();
void main_blinky2();

uint32_t stack_blinky1[40];
uint32_t *sp_blinky1 = &stack_blinky1[40];

void main_blinky1() {
    while (1)
    {
        BSP_greenLedToggle();
        BSP_Delay(50);
    }

}
uint32_t stack_blinky2[40];
uint32_t *sp_blinky2 = &stack_blinky2[40];

void main_blinky2() {
    while (1)
    {
        BSP_blueLedToggle();
        BSP_Delay(50);
    }

}

int main() {
    BSP_init();
    BSP_ledInit();
    BSP_blueLedOn();
//    main_blinky2();

    /* fabricate Cortex-M ISR stack frame for blinky 1*/
    *(--sp_blinky1) = (1U << 24); /* xPSR */ // Stack pointer decremented as the arm cpu uses the full-stack i.e. stack pointer points to last used  stack entry
    *(--sp_blinky1) = (uint32_t)&main_blinky1;
    *(--sp_blinky1) = 0x0000000EU; /* LR */
    *(--sp_blinky1) = 0x0000000CU; /* R12 */
    *(--sp_blinky1) = 0x00000003U; /* R3 */
    *(--sp_blinky1) = 0x00000002U; /* R2 */
    *(--sp_blinky1) = 0x00000001U; /* R1 */
    *(--sp_blinky1) = 0x00000000U; /* R0 */
//
    /* fabricate Cortex-M ISR stack frame for blinky 2*/
    *(--sp_blinky2) = (1U << 24); /* xPSR */
    *(--sp_blinky2) = (uint32_t)&main_blinky2;
    *(--sp_blinky2) = 0x0000000EU; /* LR */
    *(--sp_blinky2) = 0x0000000CU; /* R12 */
    *(--sp_blinky2) = 0x00000003U; /* R3 */
    *(--sp_blinky2) = 0x00000002U; /* R2 */
    *(--sp_blinky2) = 0x00000001U; /* R1 */
    *(--sp_blinky2) = 0x00000000U; /* R0 */
//
    while (1) {

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





