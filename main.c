#include "main.h"

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

int main()
{
    int p = 1000000;
    int q = 1000000/2;
    int *z = swap(&p, &q);

    GPIO_CLK_ADDRESS |= 0x2;
    GPIO_MODER |= (0b01 << 14);

    while (1)
    {
        GPIO_OUPTUT_R |= (0b01 << 7);
        delay(z[0]);

        GPIO_OUPTUT_R &= ~(0b1 << 7);
        delay(z[1]);
    }
    
}


