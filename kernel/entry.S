	# qemu -kernel loads the kernel at 0x80000000
        # and causes each CPU to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.
.section .text
_entry:
	# set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)
        # 将地址 stack0 载入 sp 寄存器
        la sp, stack0
        # 将 4096 载入 a0 寄存器
        li a0, 1024*4
        # 将 hartid 载入 a1
	csrr a1, mhartid
        # a1 = hartid + 1
        addi a1, a1, 1
        # a0 = (hartid+1) * 4096
        mul a0, a0, a1
        # sp = stack0 + (hartid+1)*4096
        add sp, sp, a0
	# jump to start() in start.c
        call start
spin:
        j spin
