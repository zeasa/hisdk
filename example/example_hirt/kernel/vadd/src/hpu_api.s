## ---------------------------------------------------------------------------------------------------------------------
## Copyright (c) 1986 - 2020, CAG team, Institute of AI and Robotics, Xi'an Jiaotong University. Proprietary and
## Confidential All Rights Reserved.
## ---------------------------------------------------------------------------------------------------------------------
## NOTICE: All information contained herein is, and remains the property of CAG team, Institute of AI and Robotics,
## Xi'an Jiaotong University. The intellectual and technical concepts contained herein are proprietary to CAG team, and
## may be covered by P.R.C. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
##
## This work may not be copied, modified, re-published, uploaded, executed, or distributed in any way, in any time, in
## any medium, whether in whole or in part, without prior written permission from CAG team, Institute of AI and
## Robotics, Xi'an Jiaotong University.
##
## The copyright notice above does not evidence any actual or intended publication or disclosure of this source code,
## which includes information that is confidential and/or proprietary, and is a trade secret, of CAG team.
## ---------------------------------------------------------------------------------------------------------------------
## FILE NAME  : hpu_api.s
## DEPARTMENT : Architecture
## AUTHOR     : wenzhe
## AUTHOR'S EMAIL : wenzhe@xjtu.edu.cn
## ---------------------------------------------------------------------------------------------------------------------
## Ver 1.0  2020--08--01 initial version.
## ---------------------------------------------------------------------------------------------------------------------

.include "inc/hpu_def.inc"

.section .text
.align 2

.globl get_hpuid
.globl init_intr
.globl enter_wfi
.globl _wfi_handle
.globl cfg_timer
.globl enable_timer_intr
.globl disable_timer_intr
.globl _timer_handle
.globl enable_ndma_intr
.globl disable_ndma_intr
.globl _ndma_handle
.globl ndma_save_data
.globl ndma_load_data
.globl ndma_atomic_swap
.globl ndma_wait
.globl claim_sim_complete
.weak ndma_callback

# Function: int get_hpuid(void)
get_hpuid:
    csrr a0, CSR_HPU_ID             # Read HPU_ID from CSR_HPU_ID
    ret

# Function: void init_intr(void)
init_intr:
    addi sp, sp, -4                 # Allocate the stack frame
    sw ra, 4(sp)                    # Save the return address on the stack frame

    li t0, 0x80020001               # Intr_mode = Vectored, Intr_addr = 0x80020000
    csrw mtvec, t0                  # CSR_mtvec = 0x80020001
    csrw mie, zero                  # CSR_mie = 0x0000
    li t0, STACK_BASE_ADDR 
    csrw mscratch, t0               # set the interrupt stack base address
    csrsi mstatus, 8                # CSR_mstatus[MIE] = 1'b1;

    lw ra, 4(sp)                    # Restore the return address on the stack frame
    addi sp, sp, 4                  # Deallocate the stack frame
    ret

# Function: void cfg_timer(int timecmp, int timecmph)
cfg_timer:
    li t0, LCM_TIME_ADDR
    li t1, LCM_TIMECMP_ADDR
    li t2, 0xffffffff
    sw zero, 0(t0)                  # mem[LCM_TIMER_ADDR] = 0
    sw zero, 4(t0)                  # mem[LCM_TIMERH_ADDR] = 0
    sw t2, 4(t1)                    # protect the timercmph
    sw a0, 0(t1)                    # mem[LCM_TIMERCMP_ADDR] = timecmp
    sw a1, 4(t1)                    # mem[LCM_TIMERCMPH_ADDR] = timecmph
    ret

# Function: void enable_timer_intr(void)
enable_timer_intr:
    li t0, 0x80
    csrs mie, t0                    # csr_mie[MT] = 1'b1;
    ret

# Function: void disable_timer_intr(void)
disable_timer_intr:
    li t0, 0x80
    csrc mie, t0                    # csr_mie[MT] = 1'b0;
    ret

# Function: void intr::_timer_handle(void)
_timer_handle:
    csrrw a0, mscratch, a0          # swap a0 <=> csr_mscratch
    sw a1, 4(a0)                    # Save context
    sw a2, 8(a0)
    sw a3, 12(a0)
    sw a4, 16(a0)
    sw a5, 20(a0)
    sw a6, 24(a0)
    sw a7, 28(a0)
    sw t0, 32(a0)
    sw t1, 36(a0)
    sw t2, 40(a0)
    sw t3, 44(a0)
    sw t4, 48(a0)
    sw t5, 52(a0)
    sw t6, 56(a0)
    li t0, LCM_TIME_ADDR
    sw zero, 0(t0)                  # mem[LCM_TIMER_ADDR] = 0
    sw zero, 4(t0)                  # mem[LCM_TIMERH_ADDR] = 0
    call timer_callback             # call back function in C code
    lw a1, 4(a0)                    # restore context
    lw a2, 8(a0)
    lw a3, 12(a0)
    lw a4, 16(a0)
    lw a5, 20(a0)
    lw a6, 24(a0)
    lw a7, 28(a0)
    lw t0, 32(a0)
    lw t1, 36(a0)
    lw t2, 40(a0)
    lw t3, 44(a0)
    lw t4, 48(a0)
    lw t5, 52(a0)
    lw t6, 56(a0)
    csrrw a0, mscratch, a0          # swap csr_mscratch <=> a0
    mret

# Function: void enter_wfi(void)
enter_wfi:
    addi sp, sp, -4                 # Allocate the stack frame
    sw ra, 4(sp)                    # Save the return address on the stack frame
    # set external interrupt
    li t0, 0x800
    csrs mie, t0                    # csr_mie[ME] = 1'b1;
    # Enter WFI
    wfi                             # enter sleep mode.
    # exit this function
    lw ra, 4(sp)                    # Restore the return address on the stack frame
    addi sp, sp, 4                  # Deallocate the stack frame
    ret

# Function: void intr::_wfi_handle(void)
_wfi_handle:
    csrrw a0, mscratch, a0          # swap a0 <=> csr_mscratch
    sw t0, 4(a0)
    # clear external interrupt
    li t0, 0x800
    csrc mie, t0                    # csr_mie[ME] = 1'b0;
    csrrw a0, mscratch, a0          # swap csr_mscratch <=> a0
    mret

# Function: void enable_ndma_intr(void)
enable_ndma_intr:
    li t0, 0x8000
    csrs mie, t0                    # csr_mie[NDMA] = 1'b1;
    ret

# Function: void disable_ndma_intr(void)
disable_ndma_intr:
    li t0, 0x8000
    csrc mie, t0                    # csr_mie[NDMA] = 1'b0;
    ret

# Function: void intr::_ndma_handle(void)
_ndma_handle:
    csrrw a0, mscratch, a0          # swap a0 <=> csr_mscratch
    sw a1, 4(a0)                    # Save context
    sw a2, 8(a0)
    sw a3, 12(a0)
    sw a4, 16(a0)
    sw a5, 20(a0)
    sw a6, 24(a0)
    sw a7, 28(a0)
    sw t0, 32(a0)
    sw t1, 36(a0)
    sw t2, 40(a0)
    sw t3, 44(a0)
    sw t4, 48(a0)
    sw t5, 52(a0)
    sw t6, 56(a0)
    csrci CSR_NDMA_STATUS, 1        # clear the insterrupt signal
    call ndma_callback              # call back function in C code
    lw a1, 4(a0)                    # restore context
    lw a2, 8(a0)
    lw a3, 12(a0)
    lw a4, 16(a0)
    lw a5, 20(a0)
    lw a6, 24(a0)
    lw a7, 28(a0)
    lw t0, 32(a0)
    lw t1, 36(a0)
    lw t2, 40(a0)
    lw t3, 44(a0)
    lw t4, 48(a0)
    lw t5, 52(a0)
    lw t6, 56(a0)
    csrrw a0, mscratch, a0          # swap csr_mscratch <=> a0
    mret

# Function: void ndma_save_data(int loc_addr, int rmt_addr, int len, int node_x, int node_y)
ndma_save_data:
    csrw CSR_NDMA_LCADDR, a0        # set local address of ndma
    csrw CSR_NDMA_RTADDR, a1        # set remote address of ndma
    csrw CSR_NDMA_SIZE,   a2        # set transfer length of ndma
    csrw CSR_NDMA_DESTXY, a3        # set node.x location of ndma
    fence
    csrwi CSR_NDMA_CTRL, 0          # set read command to ndma
    ret
 
# Function: void ndma_load_data(int loc_addr, int rmt_addr, int len, int node_x, int node_y)
ndma_load_data:
    csrw CSR_NDMA_LCADDR, a0        # set local address of ndma
    csrw CSR_NDMA_RTADDR, a1        # set remote address of ndma
    csrw CSR_NDMA_SIZE,   a2        # set transfer length of ndma
    csrw CSR_NDMA_DESTXY, a3        # set node.x location of ndma
    fence
    csrwi CSR_NDMA_CTRL, 1          # set read command to ndma
    ret
 
# Function: void ndma_atomic_swap(int loc_addr, int rmt_addr, int node_x, int node_y)
ndma_atomic_swap:
    csrw CSR_NDMA_LCADDR, a0        # set local address of ndma
    csrw CSR_NDMA_RTADDR, a1        # set remote address of ndma
    csrwi CSR_NDMA_SIZE,   4        # set transfer length of ndma
    csrw CSR_NDMA_DESTXY, a2        # set node.x location of ndma
    fence
    csrwi CSR_NDMA_CTRL, 2          # set read command to ndma
    ret
 
# function: void ndma_wait(void)
ndma_wait:
0:  csrr t0, CSR_NDMA_STATUS        # read NDMA status
    lui	t1,0xff000
    and t0, t0, t1                  # only pay attention to bit[0]
    beqz t0, 0b                     # do-while(s0 != 0)
    li   t0, 0xFFFFFFFF
    csrw CSR_NDMA_CTRL, t0
    ret

# function: void claim_sim_complete(int fail)
claim_sim_complete:
    csrw mie, zero                  # close all interrupt source.
    csrw mstatus, zero              # clean mstatus
    beqz a0, mk                     # Judge whether it is sucess
    li t0, 0xfff
    csrw mstatus, t0                # mstatus = 0xfff
mk: csrwi mie, 1                    # Just for simulation, current simulation is finished.
0:  j 0b

