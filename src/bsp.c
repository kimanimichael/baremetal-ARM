#include "stm32f429xx.h"

#include "qpc.h"
#include "bsp.h"
#include "ucos_ii.h"

// static QXSemaphore morse_sema;

// Mutex
static QXMutex morse_mutex;

void assert_failed(char const* module, int id) {
    Q_onError(module, id);
}

void Q_onError(char const* module, int id) {
    /* TBD Damage control */
    (void)module;
    (void)id;
    NVIC_SystemReset();
}

void QF_onStartup(void) {
    SystemCoreClockUpdate();
    /* For 16MHz clock frequency. This results in BSP_TICKS_PER_SEC SysTick interrupts per sec*/
    SysTick_Config(16000000/BSP_TICKS_PER_SEC);
    /* set systick priority to be "kernel aware" */
    // NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI);
    NVIC_SetPriority(SysTick_IRQn, CPU_CFG_KA_IPL_BOUNDARY + 1);
    // Enable IRQ for EXTI lines 10-15
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void QF_onCleanup(void) {
}

void QXK_onIdle(void) {
    /* @TODO Investigate why this causes irregular thread switching */
    // GPIOx_ODR |= (0b01 << 14);
    // GPIOx_ODR &= ~(0b01 << 14);

    GPIOA_ODR |= (0b01 << 12);
    GPIOA_ODR &= ~(0b01 << 12);
    // __WFI();

}

unsigned int volatile l_tickrCtr;

// void SysTick_Handler (void)
// {
//     GPIOx_ODR |= (0b01 << 1);
//     QXK_ISR_ENTRY(); /* inform qxk about entering an ISR */
//     QF_TICK_X(0, (void *)0); /* process timeouts at a specific clock tick rate */
//
//     QXK_ISR_EXIT(); /* inform qxk about exiting an ISR */
//     GPIOx_ODR &= ~(0b01 << 1);
// }

void EXTI15_10IRQHandler (void)
{
    QXK_ISR_ENTRY(); /* inform qxk about entering an ISR */
    /* check that the interrupt is actually from EXTI 13*/
    if (EXTI_PR & 0b01 << 13) {
        // QXSemaphore_signal(&SW1_sema);
    }
    //clear the pending interrupt
    EXTI_PR |= 0b01 << 13;
    QXK_ISR_EXIT(); /* inform qxk about exiting an ISR */
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
    __disable_irq();
    uint32_t tickrCtr = l_tickrCtr;
    __enable_irq();

    return tickrCtr;
}

void BSP_init() {
    // QXSemaphore_init(&morse_sema,
    //     1U,
    //     1U);
    SystemCoreClockUpdate();
    BSP_ledInit();
    BSP_user_button_init();

    QXMutex_init(&morse_mutex, 6U);
}

void BSP_ledInit() {
    //Bitwise OR the second & first bit of RCC_AHB1ENR with 1 to enable GPIOB_EN CLOCK and GPIOA_EN CLOCK
    RCC_AH1BEN |= (0b01 << 1) | (0b01 << 0);
    //Bitwise AND the 16th bit and 2nd bit of GPIOB_MODER with 0 - CONFIG PB7 & PB0 & PB14 & PB1 as output
    GPIOB_MODER &= ((0b00 << 15) | (0b00 << 1) | (0b00 << 29) | (0b00 << 3));
    //Bitwise OR the 15th bit and 1st of GPIOB_MODER with 1 - CONFIG PB7 & PB0 & PB14 & PB1 as output
    GPIOB_MODER |= ((0b01 << 14) | (0b01 << 0) | (0b01 << 28) | (0b01 << 2));
    /* Bitwise AND the 15th of GPIOA_MODER with 0 */
    /* @TODO Investigate why this bricks flashing with stlink */
    // GPIOA_MODER &= (0b00 << 25);
    /* Bite wise OR the 14th bit of GPIOA_MODER with 1*/
    GPIOA_MODER |= (0b01 << 24);
}

void BSP_user_button_init() {
    //Bitwise OR the third bit of RCC_AHB1ENR with 1 to enable GPIOC_EN CLOCK
    RCC_AH1BEN |= (0b01 << 2);

    //Bitwise AND the 27th bit of GPIOC_MODER with 0 - CONFIG PC13 as input
    GPIOC_MODER &= (0b00 << 27);
    //Bitwise AND the 26th bit of GPIOC_MODER with 0 - CONFIG PC13 as input
    GPIOC_MODER &= (0b00 << 26);
    //Bitwise AND the 27th bit of GPIOC_PUPDR with 0 - CONFIG PC13 as input pull-down
    GPIOC_PUPDR &= (0b00 << 27);
    //Bitwise AND the 26th bit of GPIOC_MODER with 0 - CONFIG PC13 as input pull-down
    GPIOC_PUPDR &= (0b00 << 26);

    //Bitwise OR the 14th bit of RCC_APB2ENR with 1 to enable SYSCFGEN for EXTI
    RCC_APB2ENR |= (0b01 << 14); // Enable SYSCFG clock
    //Bitwise OR the 4th bit of SYSCFG_EXTICR4 with 0b0010 to configure EXTI line for PC13
    SYSCFG_EXTICR4 |= (0b0010 << 4);
    // Bitwise OR the 13th bit of EXTI_RTSR with 1 to enable the rising edge trigger for EXTI13
    EXTI_RTSR |= (1 << 13);
    // Bitwise OR the 13th bit of EXTI_IMR to unmask interrupt requests for line 13
    EXTI_IMR |= (1 << 13);
    // Enable IRQ for EXTI lines 10-15
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

uint32_t BSP_user_button_read() {
    const uint32_t button_status = (GPIOC_IDR & (0b01 << 13));
    return button_status;
}

void BSP_greenLedToggle() {
    QF_CRIT_ENTRY();
    GPIOx_ODR ^= (0b01 << 0);
    QF_CRIT_EXIT();
}

void BSP_greenLedOn() {
    GPIOx_ODR |= (0b01 << 0);
}

void BSP_greenLedOff() {
    GPIOx_ODR &= ~(0b01 << 0);
}

void BSP_blueLedToggle() {
    QF_CRIT_ENTRY();
    GPIOx_ODR ^= (0b01 << 7);
    QF_CRIT_EXIT();
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
    QF_CRIT_ENTRY();
    GPIOx_ODR ^= (0b01 << 14);
    QF_CRIT_EXIT();
}

void BSP_send_morse_code(uint32_t bitmask) {
    uint32_t volatile delay_ctr;
    enum {DOT_DELAY = 75 };
    // SEMA
    // QXSemaphore_wait(&morse_sema,
    //     QXTHREAD_NO_TIMEOUT);

    // Scheduler lock
    // const QSchedStatus lock_status = QXK_schedLock(5U);

    // Mutex
    QXMutex_lock(&morse_mutex, QXTHREAD_NO_TIMEOUT);

    for (; bitmask != 0U; bitmask <<= 1U) {
        if ((bitmask & (1U << 31U)) != 0U) {
            BSP_greenLedOn();
        } else {
            BSP_greenLedOff();
        }
        for (delay_ctr = DOT_DELAY; delay_ctr != 0U; --delay_ctr) {

        }
    }
    BSP_greenLedOff();
    for(delay_ctr = 7 * DOT_DELAY; delay_ctr != 0U; --delay_ctr) {

    }
    // SEMA
    // QXSemaphore_signal(&morse_sema);

    // Scheduler lock
    // QXK_schedUnlock(lock_status);

    // Mutex
    QXMutex_unlock(&morse_mutex);
}

void App_TimeTickHook(void) {
    /* state of button. static to persist between func calls */
    static struct ButtonDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } button = {0U, 0U};

    const uint32_t current = BSP_user_button_read();
    uint32_t tmp     = button.depressed;

    button.depressed |= (button.previous & current); /* set depressed */
    button.depressed &= (button.previous | current); /* set released */
    button.previous = current; /* update history for next function call */

    tmp ^= button.depressed; /* change of button depressed state */

    if ((tmp & (0b01 << 13)) != 0U) { /* check change of button depressed state */
        if ((current & (0b01 << 13)) != 0U) { /* button pressed */
            static const Event buttonPressedEvt = {BUTTON_PRESSED_SIG};
            Active_post(AO_Button, &buttonPressedEvt);
        } else { /* button released */
            static const Event buttonReleasedEvt = {BUTTON_RELEASED_SIG};
            Active_post(AO_Button, &buttonReleasedEvt);
        }
    }
}
/*..........................................................................*/
void App_TaskIdleHook(void) {
    #ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    __WFI(); /* Wait-For-Interrupt */
    #endif
}
/*..........................................................................*/

void App_TaskCreateHook (OS_TCB *ptcb) { (void)ptcb; }
void App_TaskDelHook    (OS_TCB *ptcb) { (void)ptcb; }
void App_TaskReturnHook (OS_TCB *ptcb) { (void)ptcb; }
void App_TaskStatHook   (void)         {}
void App_TaskSwHook     (void)         {}
void App_TCBInitHook    (OS_TCB *ptcb) { (void)ptcb; }

