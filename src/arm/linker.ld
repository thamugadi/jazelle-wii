ENTRY(_start)
OUTPUT_FORMAT("elf32-bigarm")
OUTPUT_ARCH(arm)
SECTIONS
{
    . = 0x12000000;
    .text : {
        _start = .;
    }

    .data : {
        *(.data)
        *(.data.*)
    }

    .bss : {
        *(.bss)
        *(.bss.*)
    } 
}
