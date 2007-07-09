/**
 * idt - Interruption vectors
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

#include "idt.h"
#include <core/types.h>
#include <core/errno.h>


/* 2* 32 bits gate descriptor */
struct gate_descriptor
{
	ui16_t offset; /* offset 0-15 */
	ui16_t segment_selector; /* which segment */
	
	ui8_t reserved:5; 
	ui8_t zero:3;
	
	ui8_t gate_type:3; /* trap=111, interrupt=110, task=101 */
	ui8_t gate_size:1; /* 0/1 for size of gate 16/32 bits */
	ui8_t zerox:1;
	ui8_t descriptor_privilege:2; /* ring 0-3 */
	ui8_t segment_present:1; /* seg absent/present */
	
	ui16_t offsetx; /* offset 23-31 */
	
} __attribute__((packed));

/* the IDT register */
struct idt_register
{
	ui16_t table_limit; /* table size */
	ui32_t base_address; /* linear address of byte 0 */
	
} __attribute__((packed, aligned(8)));

/* the IDT */
static struct gate_descriptor idt[GATES_MAX];

ret_t idt_setup(void)
{/* setup the idt */
	
	struct idt_register idtr;
	int i;
	
	/* empty descriptor and disable entry for each vector */
	for(i = 0; i < GATES_MAX; i++)
	{
		struct gate_descriptor *gdesc = idt + i;
		
		gdesc->segment_selector = 8; /* code is entry 1 of the gdt [..01][0][00] */
      gdesc->reserved = 0;
      gdesc->zero = 0;
      gdesc->gate_type = 6; /* interrupt 110 */
      gdesc->gate_size = 1; /* 32 bits gate */
      gdesc->zerox = 0;

      idt_set_handler(i, (vaddr_t)NULL, 0);
	}
		
	idtr.base_address = (ui32_t) idt;
	
	idtr.table_limit = sizeof(idt) - 1;
	
	__asm__ volatile("lidt %0" /* put the idtr address in the idt register */
						  :
						  :"m" (idtr)
						  :"memory");
	
	return OK;
}


ret_t idt_set_handler(int index, vaddr_t handler_address, int ring)
{/* set the handler of an idt entry (if NULL, disable it) */
	
	struct gate_descriptor *gdesc;
	
	if ((index < 0) || (index >= GATES_MAX))
		return EINVAL;
	if ((ring < 0) || (ring > 3))
		return EINVAL;
	
	gdesc = idt + index;
	
	if (handler_address != (vaddr_t)NULL)
	{
		gdesc->offset = handler_address & 0xffff;
		gdesc->offsetx = (handler_address >> 16) & 0xffff;
		gdesc->descriptor_privilege = ring;
		gdesc->segment_present = 1;   /* handler is present */
	}
	else /* disable the entry */
	{
		gdesc->offset = 0;
		gdesc->offsetx = 0;
		gdesc->descriptor_privilege = 0;
		gdesc->segment_present = 0;   /* no handler */
    }

  return OK;
}


ret_t idt_get_handler(int index, vaddr_t *handler_address, int *ring)
{/* get the handler of an idt entry and its ring */
	
  if ((index < 0) || (index >= GATES_MAX))
    return EINVAL;

  if (handler_address != NULL)
    *handler_address = idt[index].offset | (idt[index].offsetx << 16);
  if (ring != NULL)
    *ring = idt[index].descriptor_privilege;

  return OK;
}
