#ifndef __BSP__H
#define __BSP__H

#define SYS_CLCK_HZ 16000000U

//Base RCC register 0x4002 3800 + offset 0x30 to find RCC_AHB1ENR
#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))

//Base GPIOB register 0x4002 0400 + offset 0x00 to find GPIOB_MODER
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))

//Base GPIOB register 0x4002 0400 + offset 0x14 to find GPIOx_ODR
#define GPIOx_ODR (*((unsigned int *)(0x40020414)))

/**
 * Switches on the blue LED
*/
void ledOn();

#endif