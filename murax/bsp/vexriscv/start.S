  .section .init
  .global _start
  .type _start,@function
_start:
  nop
  nop
  nop
#ifndef VEXRISCV_RWMTVEC
  j   
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  j trap_entry
_crtInit:
#endif
  .cfi_startproc
  .cfi_undefined ra
.option push
.option norelax
  la gp, __global_pointer$
.option pop
  la sp, _sp

#ifndef VEXRISCV_VOLATILE
  /* Load data section */
  la a0, _data_lma
  la a1, _data
  la a2, _edata
  bgeu a1, a2, 2f
1:
  lw t0, (a0)
  sw t0, (a1)
  addi a0, a0, 4
  addi a1, a1, 4
  bltu a1, a2, 1b
2:
#endif

  /* Clear bss section */
  la a0, __bss_start
  la a1, _end
  bgeu a0, a1, 2f
1:
  sw zero, (a0)
  addi a0, a0, 4
  bltu a0, a1, 1b
2:

  /* Call global constructors */
#if 0
  /* [2019-12-12 Daan] Ha, ha!  Nope!  The stack machine seems to get
   * corrupted in one of these calls.  Afaik we don't need any of these
   * constructors.  Just skip it, and get on with calling main. */
  la a0, __libc_fini_array
  call atexit
  call __libc_init_array
#endif

  auipc ra, 0
  addi sp, sp, -16
  sw ra, 8(sp)

  /* Enable Interrupts and set trap vector */
#ifndef VEXRISCV_RWMTVEC
  la a0, trap_entry
  csrw mtvec, a0
#endif
  li a0, 0x880     //880 enable timer + external interrupts
  csrw mie, a0
  li a0, 0x1808     //1808 enable interrupts
  csrw mstatus, a0

  /* argc = argv = 0 */
  li a0, 0
  li a1, 0
  call main
  tail exit
1:
  j 1b
  .cfi_endproc

  .align 4
  .weak trap_entry
  .global trap_entry
trap_entry:
  addi sp, sp, -32*4

  sw x1, 1*4(sp)
  sw x2, 2*4(sp)
  sw x3, 3*4(sp)
  sw x4, 4*4(sp)
  sw x5, 5*4(sp)
  sw x6, 6*4(sp)
  sw x7, 7*4(sp)
  sw x8, 8*4(sp)
  sw x9, 9*4(sp)
  sw x10, 10*4(sp)
  sw x11, 11*4(sp)
  sw x12, 12*4(sp)
  sw x13, 13*4(sp)
  sw x14, 14*4(sp)
  sw x15, 15*4(sp)
  sw x16, 16*4(sp)
  sw x17, 17*4(sp)
  sw x18, 18*4(sp)
  sw x19, 19*4(sp)
  sw x20, 20*4(sp)
  sw x21, 21*4(sp)
  sw x22, 22*4(sp)
  sw x23, 23*4(sp)
  sw x24, 24*4(sp)
  sw x25, 25*4(sp)
  sw x26, 26*4(sp)
  sw x27, 27*4(sp)
  sw x28, 28*4(sp)
  sw x29, 29*4(sp)
  sw x30, 30*4(sp)
  sw x31, 31*4(sp)

  call irqCallback

  lw x1, 1*4(sp)
  lw x2, 2*4(sp)
  lw x3, 3*4(sp)
  lw x4, 4*4(sp)
  lw x5, 5*4(sp)
  lw x6, 6*4(sp)
  lw x7, 7*4(sp)
  lw x8, 8*4(sp)
  lw x9, 9*4(sp)
  lw x10, 10*4(sp)
  lw x11, 11*4(sp)
  lw x12, 12*4(sp)
  lw x13, 13*4(sp)
  lw x14, 14*4(sp)
  lw x15, 15*4(sp)
  lw x16, 16*4(sp)
  lw x17, 17*4(sp)
  lw x18, 18*4(sp)
  lw x19, 19*4(sp)
  lw x20, 20*4(sp)
  lw x21, 21*4(sp)
  lw x22, 22*4(sp)
  lw x23, 23*4(sp)
  lw x24, 24*4(sp)
  lw x25, 25*4(sp)
  lw x26, 26*4(sp)
  lw x27, 27*4(sp)
  lw x28, 28*4(sp)
  lw x29, 29*4(sp)
  lw x30, 30*4(sp)
  lw x31, 31*4(sp)

  addi sp, sp, 32*4
  mret

  .weak irqCallback
irqCallback:
1:
  j 1b
