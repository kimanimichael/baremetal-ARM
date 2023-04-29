#include "main.h"
#include "stdint.h"

#define GPIO_CLK_ADDRESS (*((unsigned int *)(0x40023830)))
#define GPIO_MODER (*((unsigned int *)(0x40020400)))
#define GPIO_OUPTUT_R (*((unsigned int *)(0x40020414)))

extern unsigned int *_data_start;
extern unsigned int *_data_end;
extern unsigned int * _bss_start;
extern unsigned int * _bss_end;
extern unsigned int * _data_lma;

unsigned int *vectors[] __attribute__((section(".vectors"))) = 
{
    (unsigned int *)0x20030000, //Pointer to the top of our stack memory
    (unsigned int *)start, // Pointer to our reset handler - also our startup code
};

void start()
{
    unsigned int *src, *dest;

    src = _data_lma;
    dest = _data_start;
    while (dest < _data_end)
    {
        *dest++ = *src++;
    }

    dest = _bss_start;
    while (dest < _bss_end)
    {
        *dest++ = 0;
    }
    int k = 1;
    main();
}

void delay(unsigned int counter)
{
    while(counter-- >0){}
}

int *swap(int *x, int *y)
{
    static int temp[2];
    temp[0] = *x;
    temp[1] = *y;
    *y = temp[0];
    *x = temp[1];
    return temp;
}

uint8_t u8a, u8b;
uint16_t u16c, u16d;
uint32_t u32e, u32f;

int8_t s8;
int16_t s16;
int32_t s32;

int main()
{
    u8a = sizeof(u8a);
    u16c = sizeof(uint16_t);
    u32e = sizeof(uint32_t);

    u16c = 40000U;
    u16d = 30000U;
    u32e = (uint32_t)u16c +u16d;  // Trully portable code. Works in 16 - bit processors

    GPIO_CLK_ADDRESS |= 0x2;
    GPIO_MODER |= (0b01 << 14);

    while (1)
    {
        GPIO_OUPTUT_R |= (0b01 << 7);
        delay(1000000);

        GPIO_OUPTUT_R &= ~(0b1 << 7);
        delay(1000000);
    }
    
}


