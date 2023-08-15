#include "defs.h"
#include "delay.h"
#include "stdint.h"

#include "include/stm32f429xx.h"

#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))
#define GPIOB_ODR (*((unsigned int *)(0x40020414)))


extern unsigned int *_data_start;
extern unsigned int *_data_end;
extern unsigned int * _bss_start;
extern unsigned int * _bss_end;
extern unsigned int * _data_lma;

unsigned int *vectors[] __attribute__((section(".vectors"))) = 
{
    (unsigned int *)0x20030000, //Pointer to the top of our stack memory
    (unsigned int *)start, // Pointer to our reset handler - also our startup code
    (unsigned int *)NMI, //NMI
};
void NMI () 
{
    while (1)
    {
        /* code */
    }
    
}
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

typedef struct 
{
    long x;
    long y;
} point;

point p1, p2;

typedef struct
{
    /* data */
    point p, q;
} player;

player mk;

// typedef struct 
// {
//     /* data */
//     point corners[4];
// } rectangle;

// rectangle rm;

int main()
{
    mk.p.x = 10; //mk position p
    mk.p.y = 20; //mk position p

    mk.q.x = 30; //mk position q
    mk.q.y = 45; //mk position q

    // rm.corners[0].x = 0;
    // rm.corners[0].y = 0;
    // rm.corners[1].x = 10;
    // rm.corners[1].y = 0;
    // rm.corners[2].x = 10;
    // rm.corners[2].y = 5;
    // rm.corners[3].x = 0;
    // rm.corners[3].y = 5;


    
    p1.x = 1000000;
    p1.y = 1000000;
    p2 = p1;

    point *pp;
    player *pl;

    pp = &p1;
    pl = &mk;

    RCC_AH1BEN |= 0x2;
    GPIOB_MODER |= (0b01 << 14);

    while (1)
    {
        GPIOB_ODR |= (0b01 << 7);
        delay((*pp).x);

        GPIOB_ODR &= ~(0b1 << 7);
        delay((*pp).y);
    }
    
}


