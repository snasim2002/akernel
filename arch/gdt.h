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

#ifndef _GDT_H_
#define _GDT_H_

#include <core/errno.h>
#include <core/types.h>

/* 2* 32 bits segment descriptor */
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

/* the GDT register */
struct gdt_register
{
	ui16_t table_limit; /* table size */
	ui32_t base_address; /* linear address of byte 0 */
	
} __attribute__((packed));


/* gdt 'object' structure */
typedef struct gdt gdt;
struct gdt
{
	/* the GDT */
	struct segment_descriptor table[3];
	/* gdt register */
	struct gdt_register gdtr;
		
	/* store the map (virtual adress space to the linear space)  */
	ret_t (*store) (gdt *this);
};

/* gdt constructor */
gdt gdt_create(void);


#endif /* _GDT_H_ */
