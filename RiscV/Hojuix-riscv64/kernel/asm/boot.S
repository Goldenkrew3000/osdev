    .section .text
    .global _start
_start:
    /* Keep each CPU's hartid in it's TP register, idfk why tho */
    mv tp, a0

    /* Lock up other CPUs here */

    /* Setup stack for C */
    la sp, stack_top    /* sp --> Address of the top of the stack */
    li t0, 0x0

    /* Jump to kmain */
    call kmain

spin:
    wfi /* Wait for interrupt */
    j spin
