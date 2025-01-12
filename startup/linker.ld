MEMORY
{
    RAM(rw) : ORIGIN = 0x20000000, LENGTH = 192K
    ROM(rx) : ORIGIN = 0x08000000, LENGTH = 2048K
}

STACK_SIZE = 2048;

HEAP_SIZE = 1024;

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

    _etext = .;            /* global symbols at end of code */

     .stack :
     {
        __stack_start__ = .;
        . = . + STACK_SIZE;
        . = ALIGN(4);
        __stack_end__ = .;
    } >RAM

    .data :
    {
        . = ALIGN(4);
        _data_start = .;
        *(.data)
        . = ALIGN(4);
        _data_end = .;
    } > RAM AT > ROM

    _data_lma = LOADADDR(.data);
    . = ALIGN(4);
    .bss :
    {
        _bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > RAM

     __exidx_start = .;
    .ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } >RAM
    __exidx_end = .;

    .heap :
    {
        . = ALIGN(8);
        __heap_start__ = .;
        . = . + HEAP_SIZE;
        . = ALIGN(8);
        __heap_end__ = .;
    } >RAM
    PROVIDE(end = __heap_end__);

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}