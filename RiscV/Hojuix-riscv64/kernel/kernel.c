#include <drivers/uart.h>

#define MSTATUS_MPP_SHIFT 11
#define MSTATUS_MPP_MASK  (3UL << MSTATUS_MPP_SHIFT)

void get_execution_level() {
    // Fetch the 'mstatus' register's contents
    uint32_t mstatus;
    asm volatile("csrr %0, mstatus": "=r"(mstatus));

    // Extract the MPP field (Bits 11-10)
    // Page 36
    int oper_mode = (mstatus & MSTATUS_MPP_MASK) >> MSTATUS_MPP_SHIFT;
    kprintf("Current execution level: %d\n", oper_mode);

    // NOTE: Page 17 of the RiscV Privileged Spec contains the meanings of the values
    // 0x11 --> Machine mode
    // 0x01 --> Supervisor mode
    // 0x00 --> User mode
}

void kmain(void) {
    ns16550a_init();
    kprintf("Hojuix 0.1a riscv64\n");
    get_execution_level();

    // Fetch the current HART (Hardware Thread / Core)
    //uint8_t mhartid;
    //asm volatile("csrr %0, mhartid": "=r"(mhartid));
    //kprintf("0x%X\n", mhartid);

    kprintf("after issue\n");
}
