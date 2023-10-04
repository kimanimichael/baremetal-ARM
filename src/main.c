#include "../include/defs.h"
#include "../include/delay.h"
#include "stdint.h"
#include "../include/cmsis/stm32f429xx.h"
#include "../include/bsp.h"


/**
 * @brief main program function
 * @author @Mike-Kimani
*/

int main()
{
    //Bitwise OR the second bit of RCC_AHB1ENR with 1 to enable GPIOB_EN
    RCC_AH1BEN |= (0b01 << 1);
    //Bitwise AND the 16th bit of GPIOB_MODER with 0
    GPIOB_MODER &= (0b00 << 15);
    //Bitwise OR the 15th bit of GPIOB_MODER with 1
    GPIOB_MODER |= (0b01 << 14);
    
    SysTick_Config(SYS_CLCK_HZ/2);

    //Find out why this isn't necessary
    __enable_irq();

    ledOn();

    while (1)
    {
        delay(500000);
    }
    
}


