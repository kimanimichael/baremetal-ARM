#include "../include/cmsis/stm32f429xx.h"
#include "../include/bsp.h"

void assert_failed(char const* file, int line) {
    /**
     * @brief Resets the system,
    */
    NVIC_SystemReset();
}

unsigned int volatile l_tickrCtr;

void SysTick_Handler (void) 
{
    ++l_tickrCtr;
} 

void ledOn() {
    //Bitwise OR the 8th bit of GPIOx_ODR with 1
    GPIOx_ODR |= (0b01 << 7);
}

void BSP_Delay(uint32_t ticks) {
    uint32_t start = BSP_Tickr();
    while ((BSP_Tickr() - start) < ticks)
    {
        /* code */

    }
    

}

uint32_t BSP_Tickr(void) {
    uint32_t tickrCtr;

    __disable_irq();
    tickrCtr = l_tickrCtr;
    __enable_irq();

    return tickrCtr;
}

void BSP_init() {
    // SystemCoreClockUpdate();
    SysTick_Config(16000000/BSP_TICKS_PER_SEC);

    //Find out why this isn't necessary
    __enable_irq();
}

void BSP_ledInit() {
    //Bitwise OR the second bit of RCC_AHB1ENR with 1 to enable GPIOB_EN CLOCK
    RCC_AH1BEN |= (0b01 << 1);
    //Bitwise AND the 16th bit and 2nd bit of GPIOB_MODER with 0 - CONFIG PB7 & PB0 as output
    GPIOB_MODER &= ((0b00 << 15) | (0b00 << 1));
    //Bitwise OR the 15th bit and 1st of GPIOB_MODER with 1 - CONFIG PB7 & PB0 as output
    GPIOB_MODER |= ((0b01 << 14) | (0b01 << 0));
}

void BSP_greenLedToggle() {
    GPIOx_ODR ^= (0b01 << 0);
}

void BSP_greenLedOn() {
    GPIOx_ODR |= (0b01 << 0);
}

void BSP_greenLedOff() {
    GPIOx_ODR &= ~(0b01 << 0);
}

