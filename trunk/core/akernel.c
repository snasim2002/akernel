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
#include <core/alibc.h>
#include <drivers/videomem.h>


void kmain (unsigned long magic, multiboot_header_t bheader)
{/* main; called from boot */
	
	videomem_init();
	videomem_cls(VIDEO_FG_WHITE | VIDEO_BG_BLACK);
	videomem_printf(1, 0, VIDEO_FG_YELLOW | VIDEO_BG_BLUE, "Welcome to akernel");
	
	/* idle loop */
	for(;;)
		asm("hlt;");
	
	return;
}
