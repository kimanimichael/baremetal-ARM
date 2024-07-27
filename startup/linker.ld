/* Highest address of the user mode stack */
_estack = 0x20030000;    /* end of 192K RAM */

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0xE00;      /* required amount of heap  */
_Min_Stack_Size = 0x1A00; /* required amount of stack */

MEMORY
{
    RAM(rw) : ORIGIN = 0x20000000, LENGTH = 192K
    ROM(rx) : ORIGIN = 0x08000000, LENGTH = 2048K
}

SECTIONS
{
    .text :
    {
        *(.vectors)
        *(.text)
        . = ALIGN(4);
        *(.rodata)
    } > ROM

    .data :
    {
        _data_start = .;
        *(.data)
        . = ALIGN(4);
        _data_end = .;
    } > RAM AT > ROM

    _data_lma = LOADADDR(.data);

    .bss :
    {
        _bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > RAM
}