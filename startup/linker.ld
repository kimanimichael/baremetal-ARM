MEMORY
{
    RAM(rw) : ORIGIN = 0x20000000, LENGTH = 192K
    ROM(rx) : ORIGIN = 0x08000000, LENGTH = 2048K
}

_Min_Stack_Size = 16384;
_Min_Heap_Size = 0x2000; /* required amount of heap */

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        *(.vectors)
        . = ALIGN(4);
        *(.text)
        *(.text*)          /* .text* sections (code) */
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
    } > ROM

     .preinit_array :
     {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
     } >ROM

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } >ROM

    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(.fini_array*))
        KEEP (*(SORT(.fini_array.*)))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } >ROM

    .ARM.exidx   : {
        __exidx_start = .;
         *(.ARM.exidx* .gnu.linkonce.armexidx.*)
         __exidx_end = .;
    }>ROM

    _etext = .;            /* global symbols at end of code */

    .data :
    {
        . = ALIGN(4);
        _data_start = .;
        *(.data*)
        . = ALIGN(4);
        _data_end = .;
    } > RAM AT > ROM
    _data_lma = LOADADDR(.data);

    . = ALIGN(4);
    .bss :
    {
        _bss_start = .;
        *(.bss*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > RAM

    .stack :
     {
        . = ALIGN(8);
        _sstack = .;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
        _estack = .;
    } >RAM

    /* User_heap_stack section, used to check that there is enough "RAM" Ram  type memory left */
      ._user_heap_stack :
      {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
      } >RAM

    /* ARM Standard Symbols */
      PROVIDE(__initial_sp = _estack);
}
