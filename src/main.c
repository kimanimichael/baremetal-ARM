#include "stdint.h"

#include "../include/bsp.h"


/**
 * @brief main program function
 * @author @Mike-Kimani
*/

#define blocking

#ifdef blocking

uint32_t stack_blinky1[40]__attribute__ ((aligned (8)));
uint32_t *sp_blinky1 = &stack_blinky1[40];

void main_blinky1() {
    while (1)
    {
        BSP_greenLedToggle();
        BSP_Delay(50);
    }

}
uint32_t stack_blinky2[40]__attribute__ ((aligned (8)));
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

    /* fabricate Cortex-M ISR stack frame for blinky 1*/
    *(--sp_blinky1) = (1U << 24); /* xPSR */
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

// int main()
// {
//     BSP_init();
//     BSP_ledInit();

//     // Set the thumb state
//     *(--sp_blinky1) = (1U << 24); /* xPSR */
//     *(--sp_blinky1) = (uint32_t)&main_blinky1; /*PC*/
//     *(--sp_blinky1) = 0x0000000EU;
//     *(--sp_blinky1) = 0x0000000CU;
//     *(--sp_blinky1) = 0x00000003U;
//     *(--sp_blinky1) = 0x00000002U;
//     *(--sp_blinky1) = 0x00000001U;
//     *(--sp_blinky1) = 0x00000000U;

//     // Save additional registers to prevent cloberring
//     *(--sp_blinky1) = 0x0000000BU;
//     *(--sp_blinky1) = 0x0000000AU;
//     *(--sp_blinky1) = 0x00000009U;
//     *(--sp_blinky1) = 0x00000008U;
//     *(--sp_blinky1) = 0x00000007U;
//     *(--sp_blinky1) = 0x00000006U;
//     *(--sp_blinky1) = 0x00000005U;
//     *(--sp_blinky1) = 0x00000004U;

//     // Set the thumb state
//     *(--sp_blinky2) = (1U << 24); /* xPSR */
//     *(--sp_blinky2) = (uint32_t)&main_blinky2; /*PC*/
//     *(--sp_blinky2) = 0x0000000EU;
//     *(--sp_blinky2) = 0x0000000CU;
//     *(--sp_blinky2) = 0x00000003U;
//     *(--sp_blinky2) = 0x00000002U;
//     *(--sp_blinky2) = 0x00000001U;
//     *(--sp_blinky2) = 0x00000000U;

//     //Save additional registers to prevent cloberring
//     *(--sp_blinky2) = 0x0000000BU;
//     *(--sp_blinky2) = 0x0000000AU;
//     *(--sp_blinky2) = 0x00000009U;
//     *(--sp_blinky2) = 0x00000008U;
//     *(--sp_blinky2) = 0x00000007U;
//     *(--sp_blinky2) = 0x00000006U;
//     *(--sp_blinky2) = 0x00000005U;
//     *(--sp_blinky2) = 0x00000004U;

//     while (1)
//     {
//         /* code */
//     }
    
//     // uint32_t volatile run = 0;

//     // if(run) {
//     //     main_blinky1();
//     // }
//     // else {
//     //     main_blinky2();
//     // }
// }

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





