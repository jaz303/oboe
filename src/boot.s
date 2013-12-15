.section ".text.boot"

.globl Start

// entry point
Start:

    // setup the stack
    mov     sp, #0x8000

    // clear out bss
    ldr     r4, =_bss_start
    ldr     r9, =_bss_end
    mov     r5, #0
    mov     r6, #0
    mov     r7, #0
    mov     r8, #0
    b       2f

1:
    // store multiple at r4
    stmia   r4!, {r5-r8}

    // loop until r4 == _bss_end
2:
    cmp     r4, r9
    blo     1b

    // call kernel_main
    ldr     r3, =kernel_main
    blx     r3

    // halt
halt:
    wfe
    b       halt
