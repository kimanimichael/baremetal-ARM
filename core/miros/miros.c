#include <stdint.h>
#include "../../include/cmsis/stm32f429xx.h"
#include "../../include/miros.h"

OSThread * volatile OS_curr;/* pointer to current thread */
OSThread * volatile OS_next;/* pointer to next thread */

void OS_init(void) {
    /* Set PendSV priority to the lowest value */
    *(uint32_t volatile *) 0xE000ED20 |= (0xFFu << 16);
}

/* only call this when interrupts are disabled*/
void OS_sched(void) {
    /* trigger a PendSV exception*/
    if (OS_next != OS_curr) {
        *(uint32_t *) 0xE000ED04 = (1 << 28);
    }
    // *(uint32_t *) 0xE000ED04 = (1 << 28);
}
void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void* stkSto, uint32_t stkSize)
{
    /* Round down the stack top to the 8-byte boundary
    * NOTE: Stack grows from high to low memory in ARM Cortex-M
    */
    uint32_t *sp = (uint32_t *)((((uint32_t)stkSto + stkSize)/8)*8);
    uint32_t *stk_limit;

    *(--sp) = (1U << 24); /* xPSR */
    *(--sp) = (uint32_t)&threadHandler; /*PC*/
    *(--sp) = 0x0000000EU;
    *(--sp) = 0x0000000CU;
    *(--sp) = 0x00000003U;
    *(--sp) = 0x00000002U;
    *(--sp) = 0x00000001U;
    *(--sp) = 0x00000000U;

    // Save additional registers to prevent cloberring
    *(--sp) = 0x0000000BU;
    *(--sp) = 0x0000000AU;
    *(--sp) = 0x00000009U;
    *(--sp) = 0x00000008U;
    *(--sp) = 0x00000007U;
    *(--sp) = 0x00000006U;
    *(--sp) = 0x00000005U;
    *(--sp) = 0x00000004U;

    /* Save the current top of the stack in the thread's attribute*/
    me->sp = sp;

    /* Round down the bottom of stack to the 8-byte boundary*/
    stk_limit = (uint32_t *)((((uint32_t)stkSto -1U )/8 +1U)*8);

    /*Prefill unused  part of the stack with 0xDEADBEEF*/
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    } 
}

void PendSV_Handler(void) {
    // /* fake sp  as we cannot access the real sp*/
    // void *sp;
    // __disable_irq();
    // /* save stack content of the current thread*/
    // if(OS_curr != (OSThread *)0) {
    //     /* push registers r4-r11 to the stack */

    //     OS_curr->sp = sp;
    // }
    // sp = OS_next->sp;
    // OS_curr = OS_next;

    /* pop registers r4-r11 from the stack */

#define PendSV_restore

__asm__ (

        // "IMPORT OS_curr;"
        // "IMPORT OS_next;"

    // (0x8000548 <PendSV_Handler+56>) -> Should repeat later in the code

        /* disable IRQ*/
     	"CPSID	I;"

        /* if(OS_curr != (OSThread *)0) */
   	    "LDR	r3,=OS_curr;"
        "LDR	r3, [r3, #0x00];"
        "CBZ    r3,PendSV_restore;"
    	// "CMP	r3, #0;"
    	// "BEQ.N	PendSV_restore;"

        /* save stack content of the current thread */
        /* push registers r4-r11 to the stack*/
        "PUSH {r4-r11};"
        /* OS_curr->sp = sp */
    	"LDR	r3,=OS_curr;"
    	"LDR	r3, [r3, #0x00];"
    	"LDR	r2, [r7, #4];"
    	"STR	sp, [r3, #0x00];"
    
    "PendSV_restore:"
        /* sp = OS_next->sp */
    	"LDR	r3,=OS_next;"
    	"LDR	r3, [r3, #0x00];"
        "LDR	sp, [r3, #0x00];"

    	"STR	r3, [r7, #4];"
        /* OS_curr = OS_next */
    	"LDR	r3,=OS_next;"
    	"LDR	r3, [r3, #0x00];"
    	"LDR	r2,=OS_curr;"
    	"STR	r3, [r2, #0x00f];"
        /* pop registers r4-r11 from the stack*/
        "POP {r4-r11};"
        /* enable IRQ */
    	"CPSIE	I;"
        /* return to the next thread */
    	"BX      lr;"
    	"NOP;"
);
}