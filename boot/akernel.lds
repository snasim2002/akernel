/**
 * akernel ld script
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

OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386");

ENTRY(_start) /* entry point is start */

OUTPUT_ARCH("i386")

SECTIONS
{
	. = 0x200000; /* kernel loaded at 2 MiB */
	__b_load   = .;

	.multiboot : /* multiboot header */
	{
		*(.multiboot); /* multiboot section */
	
		/* multiboot header within the first 8192 bytes, 32 bits aligned */
		LONG(0);
	}

	. = ALIGN(4096); /* start of the kernel code/data */
	__s_kernel = .;
  
	.text ALIGN(4096) : /* text section */
	{   
		*(.text*)
		PROVIDE(etext = .);
		PROVIDE(_etext = .);
	}

	.data . : /* data section */
	{
		*(.data*) 
		PROVIDE(edata = .);
		PROVIDE(_edata = .);
	}

	.rodata . : /* RO data section */
	{
		*(.rodata*)
		*(.eh_frame*)

		/* "files" 4kB boundary better for sharing program pagesZ */
		. = ALIGN(4096);
		*(.userprogs)

		PROVIDE(erodata = .);
		PROVIDE(_erodata = .);
	}

	__e_kdata = .; /* end of the data */

	.bss SIZEOF(.rodata) + ADDR(.rodata) : /* bss start */
	{   *(.bss)
		*(COMMON)

		/* stack of the bootstrap thread on a page boundary; can be un-allocated */
		. = ALIGN(4096);
		*(.init_stack)

		PROVIDE(ebss = .);
		PROVIDE(_ebss = .);
	}

    __e_kernel = .; /* end of kernel */
}
