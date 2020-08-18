.globl _start
.globl _exit

.section .text
.balign 0 
_start:
    la sp, __stack_top__

#	/* Load data section */
#	la a0, __data_load_start__
#	la a1, __data_start__
#	la a2, __data_end__
#	bgeu a1, a2, 2f
#1:
#	lw t0, (a0)
#	sw t0, (a1)
#	addi a0, a0, 4
#	addi a1, a1, 4
#	bltu a1, a2, 1b
#2:
#
#	/* Load sdata section */
#	la a0, __sdata_load_start__
#	la a1, __sdata_start__
#	la a2, __sdata_end__
#	bgeu a1, a2, 2f
#1:
#	lw t0, (a0)
#	sw t0, (a1)
#	addi a0, a0, 4
#	addi a1, a1, 4
#	bltu a1, a2, 1b
#2:

	/* Zero bss section */
	la a0, __bss_start__
	la a1, __bss_end__
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:

	/* Zero sbss section */
	la a0, __sbss_start__
	la a1, __sbss_end__
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
    call kernel_wrapper

_exit:
1:
	j    1b
