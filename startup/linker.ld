MEMORY
{
    RAM(rw) : ORIGIN = 0x20000000, LENGTH = 200K
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