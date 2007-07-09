/**
 * multiboot header - multiboot header within the first 8196 bytes
 *
 * Copyright (c) 2007 Chabertf
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions :
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * 
 */

/* The operating system is booted by Grub, so we almost have nothing
   to do to boot it. We only have to conform to the Multiboot
   standard, as defined by the Grub documentation */
	
#define ASM 1

#include "multiboot.h"

.section ".multiboot" /* multiboot header; come first */
	.align 4 /* 32 bits aligned */
multiboot_header:
	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	.long multiboot_header
	.long __s_kernel /* code-data-bss */
	.long __e_kdata
	.long __e_kernel
	.long multiboot_entry
	
.text

.globl start, _start
start:
_start:
multiboot_entry:
	movl $(stack + MULTIBOOT_STACK_SIZE), %ebp /* set the stack */
	movl %ebp, %esp

	pushl $0 /* reset eflags */
	popf

	/* arguments on the stack (magic and adress) for kmain */
	pushl %ebx
	pushl %eax

	call EXT_C(kmain) /* starting the OS */

loop:	/* halt */
	hlt
	jmp loop


.comm	stack, MULTIBOOT_STACK_SIZE
