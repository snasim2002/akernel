/**
 * videomem - Driver for the x86 video buffer
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

#include <core/alibc.h>
#include <arch/ioports.h>

#include "videomem.h"


ret_t videomem_init(void)
{/* screen initialisation */

#define CRT_REG_INDEX 0x3d4
#define CRT_REG_DATA  0x3d5

  /* selection of the CRT register (cursor start) */
  outb(0x0a, CRT_REG_INDEX);

  /* stops the cursor (bit 5: 0/1 E/D) */
  outb(1 << 5, CRT_REG_DATA);

  return OK;
}


ret_t videomem_cls(unsigned char attribute)
{/* clears the screen */

  int i;
  for(i = 0 ; i < LINES*COLUMNS ; i++)
    {
      (*video)[i].character = 0;
      (*video)[i].attribute = attribute;
    }

  return OK;  
}


ret_t videomem_putstring(unsigned char row, unsigned char col,
				     unsigned char attribute,
				     const char *str)
{/* prints a stringn on the screen */
	
  unsigned video_offs = row*COLUMNS + col;

  if (video_offs >= LINES*COLUMNS)
    return EINVAL;
  
  for ( ; str && *str && (video_offs < LINES*COLUMNS) ; str++, video_offs++)
    {
      (*video)[video_offs].character = (unsigned char)*str;
      (*video)[video_offs].attribute = attribute;
    }

  return OK;
}


ret_t videomem_putchar(unsigned char row, unsigned char col,
				   unsigned char attribute,
				   unsigned char c)
{/* prints a character on the screen */
	
  unsigned video_offs = row*COLUMNS + col;

  if (video_offs >= LINES*COLUMNS)
    return EINVAL;
  
  (*video)[video_offs].character = c;
  (*video)[video_offs].attribute = attribute;

  return OK;
}


ret_t videomem_printf(unsigned char row, unsigned char col,
				 					unsigned char attribute,
				 					const char *format, ...)
{/* prints a formated text on the screen */
	
  char buff[256];
  va_list ap;
  
  va_start(ap, format);
  vsnprintf(buff, sizeof(buff), format, ap);
  va_end(ap);
  
  return videomem_putstring(row, col, attribute, buff);
}
