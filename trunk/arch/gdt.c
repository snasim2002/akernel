/**
 * gdt - Basic segmentation on Basic Flat Model with 3 segments (null,code,data)
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

#include "gdt.h"
#include <core/types.h>


ret_t gdt_store(gdt *this)
{/* store the map (virtual adress space to the linear space) */
				
	__asm__ volatile("lgdt %0\n\t"
						  "ljmp %1, $1f\n\t"
						  "1:\n\t"
						  "movw %2, %%ax\n\t"
						  "movw %%ax,  %%ss\n\t"
						  "movw %%ax,  %%ds\n\t"
						  "movw %%ax,  %%es\n\t"
						  "movw %%ax,  %%fs\n\t"
						  "movw %%ax,  %%gs"
						  :
						  :"m"(this->gdtr),
						  "i"(8), /* code segment */
						  "i"(16) /* data segment */
						  :"memory","eax");
						 
	return OK;	
}


/* gdt constructor */
gdt gdt_create(void)
{
	gdt this;
	
	this.table[0] = (struct segment_descriptor){ 0, };
	this.table[1] = MAKE_DESCRIPTOR(0, SEG_TYPE_CODE);
	this.table[2] = MAKE_DESCRIPTOR(0, SEG_TYPE_DATA);
	
	this.gdtr.base_address = (ui32_t) this.table;
	
	this.gdtr.table_limit = sizeof(this.table) - 1;
	
	this.store = gdt_store;
	
	return this;
}
