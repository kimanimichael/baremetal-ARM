#include "defs.h"
#include "delay.h"
#include "stdint.h"

#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))
#define GPIOB_ODR (*((unsigned int *)(0x40020414)))
/**
 * @brief main program function
 * @author @Mike-Kimani
*/
int main()
{
    RCC_AH1BEN |= 0x2;
    GPIOB_MODER |= (0b01 << 14);

    while (1)
    {
        GPIOB_ODR |= (0b01 << 7);
        delay(1000000);

        GPIOB_ODR &= ~(0b1 << 7);
        delay(2000000);
    }
    
}


