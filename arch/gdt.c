/**
 * gdt - Basic segmentation on Basic Flat Model with 2 segments (code/data) 
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

struct segment_descriptor
{
	ui16_t segment_limit; /* segment size; 0-15 */
	ui16_t base_address; /* location of the 0 byte of the segment; 0-15 */
	
	ui8_t base_addressx; /* base address 16-23 */
	ui8_t segment_type:4; /* code/data */
	ui8_t descriptor_type:1; /* sys/code-data */
	ui8_t descriptor_privilege:2; /* ring 0-3 */
	ui8_t segment_present:1; /* seg absent/present */
	ui8_t segment_limitx:4; /* segment size 16-19 */
	ui8_t use_by_soft:1;
	ui8_t zero:1;
	ui8_t operation_size:1; /* 16bits code-data / 32 bits */
	ui8_t granularity:1; /* size 1B->1MB by 1B / 4kB->4GB by 4kB */
	ui8_t base_addressxx; /* base address 23-31 */
	
} __attribute__((packed));

struct gdt_register
{
	ui16_t table_limit; /* table size */
	ui32_t base_address; /* linear address of byte 0 */
	
} __attribute__((packed));

/* creates a segment descriptor from a privilege and a type (code/data) */
#define SEG_TYPE_CODE 0xb
#define SEG_TYPE_DATA 0x3
#define MAKE_DESCRIPTOR(ring,seg_type) \
 ((struct segment_descriptor) { \
	.segment_limit = 0xffff, \
	.base_address = 0, \
	.base_addressx = 0, \
	.segment_type = seg_type, /* code:Access|Read|0 data:Access|Write|0 */ \
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

/* the gdt (global) */
static struct segment_descriptor gdt[] = {
  (struct segment_descriptor){ 0, },
  MAKE_DESCRIPTOR(0, SEG_TYPE_CODE),
  MAKE_DESCRIPTOR(0, SEG_TYPE_DATA)
};

ret_t gdt_init(void)
{/* maps the virtual adress space to the linear space */
	
	struct gdt_register gdtr;

	gdtr.base_address = (ui32_t) gdt;
	
	gdtr.table_limit = sizeof(gdt) - 1;
		
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
						  :"m"(gdtr),
						  "i"(8), /* code segment */
						  "i"(16) /* data segment */
						  :"memory","eax");
						 
	return OK;	
}
