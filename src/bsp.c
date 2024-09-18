#include "../include/cmsis/stm32f429xx.h"
#include "../include/bsp.h"
#include "../include/miros.h"

void assert_failed(char const* module, int id) {
    Q_onError(module, id);
}

void Q_onError(char const* module, int id) {
    /* TBD Damage control */
    (void)module;
    (void)id;
    NVIC_SystemReset();
}

void OS_onStartup(void) {
    SystemCoreClockUpdate();
    /* For 16MHz clock frequency. This results in BSP_TICKS_PER_SEC SysTick interrupts per sec*/
    SysTick_Config(16000000/BSP_TICKS_PER_SEC);

    NVIC_SetPriority(SysTick_IRQn, 0U);
}

void OS_on_idle(void) {
    // GPIOx_ODR |= (0b01 << 14);
    // GPIOx_ODR &= ~(0b01 << 14);
    // GPIOA_ODR |= (0b01 << 12);
    // GPIOA_ODR &= ~(0b01 << 12);

}

unsigned int volatile l_tickrCtr;

void SysTick_Handler (void) 
{
    GPIOx_ODR |= (0b01 << 1);
    OS_tick();

    __disable_irq();
    OS_sched();
    GPIOx_ODR &= ~(0b01 << 1);
    __enable_irq();
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

}

void BSP_ledInit() {
    //Bitwise OR the second bit of RCC_AHB1ENR with 1 to enable GPIOB_EN CLOCK
    RCC_AH1BEN |= (0b01 << 1) | (0b01 << 0);
    //Bitwise AND the 16th bit and 2nd bit of GPIOB_MODER with 0 - CONFIG PB7 & PB0 & PB14 & PB1 as output
    GPIOB_MODER &= ((0b00 << 15) | (0b00 << 1) | (0b00 << 29) | (0b00 << 3));
    //Bitwise OR the 15th bit and 1st of GPIOB_MODER with 1 - CONFIG PB7 & PB0 & PB14 & PB1 as output
    GPIOB_MODER |= ((0b01 << 14) | (0b01 << 0) | (0b01 << 28) | (0b01 << 2));
    /* Bitwise AND the second bit of GPIOA_MODER with 0 */
    GPIOA_MODER &= (0b00 << 13);
    /* Bite wise OR the 1st bit of GPIOA_MODER with 1*/
    GPIOA_MODER |= (0b01 << 12);
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

void BSP_blueLedToggle() {
    GPIOx_ODR ^= (0b01 << 7);
}

void BSP_blueLedOn() {
    GPIOx_ODR |= (0b01 << 7);
}

void BSP_blueLedOff() {
    GPIOx_ODR &= ~(0b01 << 7);
}

void BSP_redLedOn() {
    GPIOx_ODR |= (0b01 << 14);
}

void BSP_redLedOff() {
    GPIOx_ODR &= ~(0b01 << 14);
}

void BSP_redLedToggle() {
    GPIOx_ODR ^= (0b01 << 14);
}

