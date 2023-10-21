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
uint32_t *sp_blinky2 = &stack_blinky2[40];

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
    /* additional fake registers R4 - R11 */
    *(--sp_blinky1) = 0x0000000BU; /* R11 */
    *(--sp_blinky1) = 0x0000000AU; /* R10 */
    *(--sp_blinky1) = 0x00000009U; /* R9*/
    *(--sp_blinky1) = 0x00000008U; /* R8 */
    *(--sp_blinky1) = 0x00000007U; /* R7 */
    *(--sp_blinky1) = 0x00000006U; /* R6 */
    *(--sp_blinky1) = 0x00000005U; /* R5 */
    *(--sp_blinky1) = 0x00000004U; /* R4 */
    /* fabricate Cortex-M ISR stack frame for blinky 2*/
    *(--sp_blinky2) = (1U << 24); /* xPSR */
    *(--sp_blinky2) = (uint32_t)&main_blinky2;
    *(--sp_blinky2) = 0x0000000EU; /* LR */
    *(--sp_blinky2) = 0x0000000CU; /* R12 */
    *(--sp_blinky2) = 0x00000003U; /* R3 */
    *(--sp_blinky2) = 0x00000002U; /* R2 */
    *(--sp_blinky2) = 0x00000001U; /* R1 */
    *(--sp_blinky2) = 0x00000000U; /* R0 */
    /* additional fake registers R4 - R11 */
    *(--sp_blinky2) = 0x0000000BU; /* R11 */
    *(--sp_blinky2) = 0x0000000AU; /* R10 */
    *(--sp_blinky2) = 0x00000009U; /* R9*/
    *(--sp_blinky2) = 0x00000008U; /* R8 */
    *(--sp_blinky2) = 0x00000007U; /* R7 */
    *(--sp_blinky2) = 0x00000006U; /* R6 */
    *(--sp_blinky2) = 0x00000005U; /* R5 */
    *(--sp_blinky2) = 0x00000004U; /* R4 */
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

    while (1)
    {
        /* code */
        BSP_blueLedOn();
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





