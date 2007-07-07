/**
 * Akernel - Main code
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

#include <boot/multiboot.h>
#include <core/libc.h>
#include <drivers/videomem.h>
#include <arch/gdt.h>


void kmain (unsigned long magic, unsigned long addr)
{/* main; called from boot */
	
	multiboot_info_t *bheader = (multiboot_info_t *) addr;
	
	/* basic segmentation */
	gdt k_gdt = gdt_create();
	
	k_gdt.store(&k_gdt);
	
	/* a vram object */
	videomem screen = videomem_create();
		
	screen.cls(&screen, VIDEO_BG_BLACK);
	screen.set_cursor(&screen, FALSE);
	screen.printf(&screen, 1, 0, 
					  VIDEO_FG_YELLOW | VIDEO_BG_BLUE, 
					  "Welcome to akernel");
	screen.printf(&screen, 3, 0, 
					  VIDEO_FG_WHITE,
					  "%d MiB of RAM",
					  (unsigned)bheader->mem_upper);
	/* idle loop */
	for(;;)
		continue;
	
	return;
}
