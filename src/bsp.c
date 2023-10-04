#include "../include/defs.h"
#include "../include/cmsis/stm32f429xx.h"
#include "../include/bsp.h"

void assert_failed(char const* file, int line) {
    /**
     * @brief Resets the syste,
    */
    NVIC_SystemReset();
}

void SysTick_Handler (void) 
{
    GPIOx_ODR ^= (0b01 << 7);
} 

void ledOn() {
    GPIOx_ODR = (0b01 << 7);
}