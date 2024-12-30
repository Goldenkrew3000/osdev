#include <drivers/syscon.h>
#include <drivers/uart.h>

#define SYSCON_ADDR 0x100000

void syscon_poweroff(void) {
    kprintf("SYSCON: Power off requested.\n");
    *(uint32_t*)SYSCON_ADDR = 0x5555;
}

void syscon_reboot(void) {
    kprintf("SYSCON: Reboot requested.\n");
    *(uint32_t*)SYSCON_ADDR = 0x7777;
}
