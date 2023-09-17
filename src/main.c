#include "../include/defs.h"
#include "../include/delay.h"
#include "stdint.h"

//Base RCC register 0x4002 3800 + offset 0x30 to find RCC_AHB1ENR
#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))

//Base GPIOB register 0x4002 0400 + offset 0x00 to find GPIOB_MODER
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))

//Base GPIOB register 0x4002 0400 + offset 0x14 to find GPIOx_ODR
#define GPIOx_ODR (*((unsigned int *)(0x40020414)))
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
    

    while (1)
    {
        //bit-wise XOR the 8th bit in GPIOx_ODR with 1 - This toggles the bit and hence the led
        GPIOx_ODR ^= (0b01 << 7);
        delay(1000000);

    }
    
}


