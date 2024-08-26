#ifndef __BSP__H
#define __BSP__H

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

#include "stdint.h"

#define SYS_CLCK_HZ 16000000U

#define BSP_TICKS_PER_SEC 100

//Base RCC register 0x4002 3800 + offset 0x30 to find RCC_AHB1ENR
#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))

//Base GPIOB register 0x4002 0400 + offset 0x00 to find GPIOB_MODER
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))

//Base GPIOB register 0x4002 0400 + offset 0x14 to find GPIOx_ODR
#define GPIOx_ODR (*((unsigned int *)(0x40020414)))

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

void ledOn();

#endif