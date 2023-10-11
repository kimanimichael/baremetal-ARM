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
    //Bitwise OR the second bit of RCC_AHB1ENR with 1 to enable GPIOB_EN CLOCK
    RCC_AH1BEN |= (0b01 << 1);
    //Bitwise AND the 16th bit and 2nd bit of GPIOB_MODER with 0 - CONFIG PB7 & PB0 as output
    GPIOB_MODER &= ((0b00 << 15) | (0b00 << 1));
    //Bitwise OR the 15th bit and 1st of GPIOB_MODER with 1 - CONFIG PB7 & PB0 as output
    GPIOB_MODER |= ((0b01 << 14) | (0b01 << 0));
    
    SysTick_Config(SYS_CLCK_HZ/2);

    //Find out why this isn't necessary
    __enable_irq();

    // ledOn();

    while (1)
    {
        __disable_irq();
        GPIOx_ODR ^= (0b01 << 0);
        delay(1);
        __enable_irq();
        
    }
    
}


