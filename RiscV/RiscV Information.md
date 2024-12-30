RiscV stuff:

Traps and Interrupts:
The register 'mtvec' stands for Machine Trap Vector. (A vector is a function pointer)
When a trap/interrupt happens, the cpu will call the function given by the 'mtvec' register
Page 46 of the RiscV priviliged ISA

So apparently (Reference: https://osblog.stephenmarz.com/ch4.html), if entering kernel entry in supervised mode, it seems advisable
to have virtual memory already set up (Sv39)

Page 48 of the RiscV ISA. Machine Interrupt registers ('mip' and 'mie')
They disable interrupts by 'csrw mie, zero'

So it seems they start boot, disable interrupts, enter rust with 'kinit', setup virtual memory, enter kmain

According to (https://traxys.me/riscv_os_setup.html), Machine mode does not support virtual memory
Both the operating system and SBI can transition from Machine mode to Supervisor mode.
P.S. When loading OpenSBI via QEMU (with -bios default), it loads at 0x80000000, thus loading a kernel has to happen
at least at 0x80100000 or higher

OpenSBI can continue execution in 3 different ways (to the Supervisor stage):
--> FW_PAYLOAD
--> FW_JUMP
--> FW_DYANMIC
Loading a kernel in QEMU (-kernel <elf file>), OpenSBI uses the FW_JUMP method. Aka put the kernel at the right memory
address and it _should_ work

P.S. OpenSBI shows where the next place it's going to jump: "Domain0 Next Address      : 0x0000000080100000"

Okay new day time to look at this
So I found a fork of XV6 for RiscV that works under OpenSBI
(But the person who did this also really improved it)
Located at: https://github.com/jrmenzel/vimix_os

So apparently 'csrr a0, mhartid' only works in Machine mode
OpenSBI proves the hartid in 'a0' and a pointer to the device tree in 'a1'
Oh wait so that means it already practically does that 'csrr' line automatically

So this OS keeps the CPU hartid in 'tp' something about SMP
Then they check if t0 (I guess CPU id) is above the Max CPU count, then it infinite
loops it, but when would that even be a possibility...
Anyway, then they setup the stack for C, and then jump to start() in start.c

Oh shit they clear BSS and everything in C. Really a good idea tbh.
Drop the fucking assembly lmfao
