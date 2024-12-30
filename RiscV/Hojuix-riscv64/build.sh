mkdir kernel-objs
mkdir kernel-objs/drivers

riscv64-elf-gcc \
-ffreestanding -nostartfiles -nostdlib -nodefaultlibs -g -Wl,--gc-sections -mcmodel=medany \
-I kernel \
-c kernel/kernel.c -o kernel-objs/kernel.o

riscv64-elf-gcc \
-ffreestanding -nostartfiles -nostdlib -nodefaultlibs -g -Wl,--gc-sections -mcmodel=medany \
-I kernel \
-c kernel/drivers/uart.c -o kernel-objs/drivers/uart.o

riscv64-elf-gcc \
-ffreestanding -nostartfiles -nostdlib -nodefaultlibs -g -Wl,--gc-sections -mcmodel=medany \
-I kernel \
-c kernel/drivers/syscon.c -o kernel-objs/drivers/syscon.o

riscv64-elf-gcc \
-T linker.ld \
-ffreestanding -nostartfiles -nostdlib -nodefaultlibs -g -Wl,--gc-sections -mcmodel=medany \
kernel-objs/kernel.o \
kernel-objs/drivers/uart.o \
kernel-objs/drivers/syscon.o \
kernel/asm/boot.S \
-o kernel.macho
