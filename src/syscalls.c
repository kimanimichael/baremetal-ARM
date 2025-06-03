#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "stm32f4xx.h"

int _write( int handle, char* data, int size) {
    int count = size;
    while( count-- ) {
        while( !( USART3->SR & USART_SR_TXE ) ) {};
        USART3->DR = *data++;
    }
    return size;
}

void* _sbrk(int incr) {
    extern char _end;         // Defined by the linker - start of heap
    extern char _estack; // Defined in our linker script - bottom of stack area

    static char *heap_end = &_end;
    char *prev_heap_end = heap_end;

    // Calculate safe stack limit - stack grows down from _stack_top towards _stack_bottom
    char *stack_limit = &_estack;

    // Check if heap would grow too close to stack
    if (heap_end + incr > stack_limit) {
        errno = ENOMEM;
        return (void*) -1; // Return error
    }

    heap_end += incr;
    return (void*) prev_heap_end;
}

void _exit(int status) { while(1); }
int _close(int fd)           { return -1; }
int _fstat(int fd, struct stat *st) {
    st->st_mode = S_IFCHR;    // character device
    return 0;
}
int _lseek(int fd, int ptr, int dir) { return 0; }
int _isatty(int fd)          { return 1; }
int _read(int fd, char *buf, int len) { return 0; }
