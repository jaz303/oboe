/* declares entry point; defined in boot.s */
ENTRY(Start)

SECTIONS
{
    /* '.' means 'current address' */
    . = 0x8000;
    
    _start = .;
    
    /* text */

    _text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096);
    _text_end = .;

    /* read-only data */

    _rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    .= ALIGN(4096);
    _rodata_end = .;

    /* data */

    _data_start = .;
    .data :
    {
        *(.data)
    }
    .= ALIGN(4096);
    _data_end = .;

    /* bss - zero initialised memory */

    _bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    .= ALIGN(4096);
    _bss_end = .;

    _end = .;

    /* Font */

    . = 0x00100000;
    _font_start = .;
    .text :
    {
        INCLUDE "font/font_8x16.ld";
    }
    .= ALIGN(4096);
    ._font_end = .;
    
}