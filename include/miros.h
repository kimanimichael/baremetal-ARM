#ifndef MIROS_H
#define MIROS_H

#endif /*MIROS_H*/

/* Thread Control Block (TCB)*/
typedef struct 
{
    void *sp; /*Stack pointer*/
    /*... Other thread attributes*/
} OSThread;

typedef void (*OSThreadHandler)();

void OS_init(void);

void OS_sched(void);

void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void* stkSto, uint32_t stkSize);