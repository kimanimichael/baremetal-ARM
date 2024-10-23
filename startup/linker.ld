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
        *(.init)
        *(.fini)
        . = ALIGN(4);
        *(.rodata)
    } > ROM

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } > ROM

    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } > ROM

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
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

    /* Define 'end' symbol for dynamic memory allocation */
    end = .;  /* End of BSS */

    /* Section for Exception Handling */
    .ARM.exidx : {
        PROVIDE(__exidx_start = .);
        *(.ARM.exidx.*)
        PROVIDE(__exidx_end = .);
    } > ROM


}