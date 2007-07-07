/**
 * gdt - Basic segmentation on Basic Flat Model (2 segments code&data mapped to
 *       the entire linear adress space.
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


/* creates a segment descriptor from a privilege and a type (code/data) */
#define SEG_TYPE_CODE 0xb
#define SEG_TYPE_DATA 0x3
#define MAKE_DESCRIPTOR(ring,seg_type) \
 ((struct segment_descriptor) { \
	.segment_limit = 0xffff, \
	.base_address = 0, \
	.base_addressx = 0, \
	.segment_type = seg_type, /* code:Access-Read-0 data:Access-Write-0 */ \
	.descriptor_type = 1, \
	.descriptor_privilege = ((ring) & 0x3), \
	.segment_present = 1, \
	.segment_limitx = 0xf, \
	.use_by_soft = 0, \
	.zero = 0, \
	.operation_size = 1, \
	.granularity = 1, \
	.base_addressxx = 0 \
 })


ret_t gdt_store(gdt *this)
{/* store the map (virtual adress space to the linear space) */
				
	__asm__ volatile("lgdt %0\n\t" /* put the gdt address in the gdt register */
						  "ljmp %1, $1f\n\t" /* reload cs */
						  "1:\n\t"
						  "movw %2, %%ax\n\t" /* reload data segments selector */
						  "movw %%ax,  %%ss\n\t"
						  "movw %%ax,  %%ds\n\t"
						  "movw %%ax,  %%es\n\t"
						  "movw %%ax,  %%fs\n\t"
						  "movw %%ax,  %%gs"
						  :
						  :"m"(this->gdtr),
						  /* segment selector [index 13b][gdt/ldt bit][privilege 2b] */
						  "i"(8), /* code segment selector [0000 0000 0000 1][0][00] */
						  "i"(16) /* data segment selector [0000 0000 0001 0][0][00] */
						  :"memory","eax");
						 
	return OK;	
}


/* gdt constructor */
gdt gdt_create(void)
{
	gdt this;
	
	/* first entry of the GDT: null segment selector */
	this.table[0] = (struct segment_descriptor){ 0, };
	/* code segment mapped to the entire linear adress space */
	this.table[1] = MAKE_DESCRIPTOR(0, SEG_TYPE_CODE);
	/* data segment mapped to the entire linear adress space */
	this.table[2] = MAKE_DESCRIPTOR(0, SEG_TYPE_DATA);
	
	this.gdtr.base_address = (ui32_t) this.table;
	
	this.gdtr.table_limit = sizeof(this.table) - 1;
	
	this.store = gdt_store;
	
	return this;
}
