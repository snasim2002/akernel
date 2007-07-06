/* Boot sector - Multiboot standard */
	
#define ASM 1

#include "multiboot.h"

	/* The multiboot header itself. It must come first. */
.section ".multiboot"
	/* Multiboot header must be aligned on a 4-byte boundary */
	.align 4
multiboot_header:
  /* magic=        */ .long MULTIBOOT_HEADER_MAGIC
  /* flags=        */ .long MULTIBOOT_HEADER_FLAGS
  /* checksum=     */ .long -(MULTIBOOT_HEADER_MAGIC \
                              +MULTIBOOT_HEADER_FLAGS)
  /* header_addr=  */ .long multiboot_header
  /* load_addr=    */ .long __b_kernel
  /* load_end_addr=*/ .long __e_load
  /* bss_end_addr= */ .long __e_kernel
  /* entry_addr=   */ .long multiboot_entry
	
/* Here is the beginning of the code of our operating system */
.text

.globl start, _start
start:
_start:
multiboot_entry:
	/* Set up a stack */
	movl $(stack + MULTIBOOT_STACK_SIZE), %ebp
	movl %ebp, %esp

	/* Set EFLAGS to 0 */
	pushl $0
	/* pop stack into the EFLAGS register */
	popf

	/* Push the magic and the address on the stack, so that they
	will be the parameters of the cmain function */
	pushl %ebx
	pushl %eax

	/* Call the main function */
	call EXT_C(kmain)

	/* Should never get there */
loop:
	hlt
	jmp loop

	/* Here is the stack */
.comm	stack, MULTIBOOT_STACK_SIZE
