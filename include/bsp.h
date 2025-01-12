#ifndef __BSP__H
#define __BSP__H

#include "uc_ao.h"

/**
 * @brief Initialize data and call main function
*/
void start();
/**
 * @brief Unused handler to map to other undefined handlers
*/
void Unused_Handler (void);
/**
 * @brief Defines action on failure of an assertion
*/
void assert_failed(char const* module, int id);

void Q_onError(char const* module, int id);

void EXTI15_10IRQHandler(void);

#include "stdint.h"

#define SYS_CLCK_HZ 16000000U

#define BSP_TICKS_PER_SEC 1000

//Base RCC register 0x4002 3800 + offset 0x30 to find RCC_AHB1ENR
#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))
//Base SYSCFG register 0x4001 3800 + offset 0x14 to find SYSCFG_EXTICR4
#define SYSCFG_EXTICR4 (*((unsigned int *)(0x40013814)))
// Base EXTI register 0x4001 3C00 + offset 0x08 to find EXTI_RTSR(Rising trigger selection register)
#define EXTI_RTSR ((*(unsigned int *)(0x40013c08)))
// Base EXTI register 0x4001 3C00 + offset 0x0C to find EXTI_FTSR(Falling trigger selection register)
#define EXTI_FTSR ((*(unsigned int *)(0x40013c0c)))
// Base EXTI register 0x4001 3C00 + offset 0x00 to find EXTI_IMR(Interrupt mask register)
#define EXTI_IMR ((*(unsigned int *)(0x40013C00)))
// Base EXTI register 0x4001 3C00 + offset 0x14 to find EXTI_PR(Pending register)
#define EXTI_PR ((*(unsigned int *)(0x40013C14)))

//Base RCC register 0x4002 3800 + offset 0x44 to find RCC_APB2ENR
#define RCC_APB2ENR (*((unsigned int *)(0x40023844)))

//Base GPIOB register 0x4002 0400 + offset 0x00 to find GPIOB_MODER
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))

//Base GPIOB register 0x4002 0400 + offset 0x14 to find GPIOx_ODR
#define GPIOx_ODR (*((unsigned int *)(0x40020414)))

//Base GPIOA register 0x4002 0000 + offset 0x00 to find GPIOA_MODER
#define GPIOA_MODER (*((unsigned int *)(0x40020000)))

//Base GPIOA register 0x4002 0000 + offset 0x14 to find GPIOx_ODR
#define GPIOA_ODR (*((unsigned int *)(0x40020014)))

//Base GPIOC register 0x4002 0800 + offset 0x00 to find GPIOC_MODER
#define GPIOC_MODER (*((unsigned int *)(0x40020800)))
//Base GPIOC register 0x4002 0800 + offset 0x14 to find GPIOC_ODR
#define GPIOC_ODR (*((unsigned int *)(0x40020814)))
//Base GPIOC register 0x4002 0800 + offset 0x0C to find GPIOC_PUPDR
#define GPIOC_PUPDR (*((unsigned int *)(0x4002080c)))
//Base GPIOC register 0x4002 0800 + offset 0x10 to find GPIOC_IDR
#define GPIOC_IDR (*((unsigned int *)(0x40020810)))

/**
 * @brief Implements a delay
 * @author @Mike-Kimani
 * @param ticks Number of ticks to wait for - delay of (ticks * 0.01)s
*/
void BSP_Delay(uint32_t ticks);

/**
 * @brief Get current number of ticks
 * @return number of ticks
 * @author Mike-Kimani
*/
uint32_t BSP_Tickr(void);

/**
 * @brief Initialize board peripherals
 * @author @Mike-Kimani
*/
void BSP_init();

/**
 * @brief Initialize onboard led requirements
 * @author @Mike-Kimani
*/
void BSP_ledInit();
/**
* @brief Initialize NUCLEO-F429ZI user button PC13
*/
void BSP_user_button_init();

/**
* @brief Read user button status
* @attention BSP_user_button_init() must have been called first at least once in the scope
*/
uint32_t BSP_user_button_read();

/**
 * @brief Switches on the blue LED
 * @author @Mike-Kimani
*/
void BSP_blueLedOn();

/**
 * @brief Switches off the blue LED
 * @author @Mike-Kimani
*/
void BSP_blueLedOff();

/**
 * @brief Toggles the blue LED
 * @author @Mike-Kimani
*/
void BSP_blueLedToggle();

/**
 * @brief Switches on the green LED
 * @author @Mike-Kimani
*/
void BSP_greenLedOn();

/**
 * @brief Switches off the green LED
 * @author @Mike-Kimani
*/
void BSP_greenLedOff();

/**
 * @brief Toggles the green LED
 * @author @Mike-Kimani
*/
void BSP_greenLedToggle();

/**
 * @brief Switches on the red LED
 * @author @Mike-Kimani
*/
void BSP_redLedOn();

/**
 * @brief Switches off the red LED
 * @author @Mike-Kimani
*/
void BSP_redLedOff();

/**
 * @brief Toggles the red LED
 * @author @Mike-Kimani
*/
void BSP_redLedToggle();
/**
* @brief blink green led to send morse code
* @param bitmask message to be sent in bits
*/
void BSP_send_morse_code(uint32_t bitmask);

void ledOn();

enum  EventSignals {
 BUTTON_PRESSED_SIG = USER_SIGNAL,
 BUTTON_RELEASED_SIG,
};

extern Active* AO_Button;

#endif