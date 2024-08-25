#ifndef MIROS_H
#define MIROS_H

#endif /*MIROS_H*/

/* Thread Control Block (TCB)*/
typedef struct 
{
    void *sp; /*Stack pointer*/
    /*... Other thread attributes*/
} OSThread;

/* Function pointer to function run in thread */
typedef void (*OSThreadHandler)();

/**
 * @brief Initialize thread handling setting
 */
void OS_init(void);
/**
 * @brief Set the bit for PendSV to be triggered if next thread is different with current thread
 * @attention Function must be called with interrupts disabled
 */
void OS_sched(void);
/**
 * @brief Fabricate Cortex-M ISR stack
 * @param me thread whose stack frame is to be created
 * @param threadHandler function handled by the thread
 * @param stkSto allocated stack
 * @param stkSize size of allocated stack
 */
void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void* stkSto, uint32_t stkSize);