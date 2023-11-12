#include "../include/defs.h"
#include "../include/cmsis/stm32f429xx.h"
#include "../include/bsp.h"

void assert_failed(char const* file, int line) {
    /**
     * @brief Resets the system,
    */
    NVIC_SystemReset();
}

void SysTick_Handler (void) 
{
    //Bitwise XOR the 8th bit of GPIOx_ODR with 1
    GPIOx_ODR ^= (0b01 << 7);
} 

void ledOn() {
    //Bitwise OR the 8th bit of GPIOx_ODR with 1
    GPIOx_ODR |= (0b01 << 7);
}