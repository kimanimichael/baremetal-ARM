#include "defs.h"
#include "delay.h"
#include "stdint.h"

#define RCC_AH1BEN (*((unsigned int *)(0x40023830)))
#define GPIOB_MODER (*((unsigned int *)(0x40020400)))
#define GPIOB_ODR (*((unsigned int *)(0x40020414)))
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
    p1.y = 2000000;
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


