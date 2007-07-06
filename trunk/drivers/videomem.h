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

#ifndef _VIDEOMEM_H_
#define _VIDEOMEM_H_

#include <core/errno.h>


/* video memory starts at 0xB8000.*/
#define VIDEO   0xb8000


/* console size */
#define LINES   25
#define COLUMNS 80

/* 8 bits attributes */

/* foreground color */
#define VIDEO_FG_BLACK     0
#define VIDEO_FG_BLUE      1
#define VIDEO_FG_GREEN     2
#define VIDEO_FG_CYAN      3
#define VIDEO_FG_RED       4
#define VIDEO_FG_MAGENTA   5
#define VIDEO_FG_BROWN     6
#define VIDEO_FG_LTGRAY    7
#define VIDEO_FG_DKGRAY    8
#define VIDEO_FG_LTBLUE    9
#define VIDEO_FG_LTGREEN   10
#define VIDEO_FG_LTCYAN    11
#define VIDEO_FG_LTRED     12
#define VIDEO_FG_LTMAGENTA 13
#define VIDEO_FG_YELLOW    14
#define VIDEO_FG_WHITE     15

/* background color */
#define VIDEO_BG_BLACK     (0 << 4)
#define VIDEO_BG_BLUE      (1 << 4)
#define VIDEO_BG_GREEN     (2 << 4)
#define VIDEO_BG_CYAN      (3 << 4)
#define VIDEO_BG_RED       (4 << 4)
#define VIDEO_BG_MAGENTA   (5 << 4)
#define VIDEO_BG_BROWN     (6 << 4)
#define VIDEO_BG_LTGRAY    (7 << 4)

/* blinking */
#define VIDEO_FG_BLINK  (1 << 7)


/* 2 bytes screen block:  [char 8b][blink 1b][bg color 3b][fg color 4b] */
typedef struct {
  unsigned char character;
  unsigned char attribute;
} __attribute__ ((packed)) video_mem[LINES*COLUMNS];

/* video mem base pointer */
static volatile video_mem *video = (volatile video_mem*)VIDEO;


/* screen initialisation */
ret_t videomem_init(void);

/* clears the screen */
ret_t videomem_cls(unsigned char attribute);

/* prints a stringn on the screen */
ret_t videomem_putstring(unsigned char row, unsigned char col,
										unsigned char attribute,
				    					const char *str);

/* prints a character on the screen */
ret_t videomem_putchar(unsigned char row, unsigned char col,
				   					unsigned char attribute,
				  						unsigned char c);

/* prints a formated text on the screen */
ret_t videomem_printf(unsigned char row, unsigned char col,
							unsigned char attribute,
				 			const char *format, ...)
     __attribute__ ((format (printf, 4, 5)));

#endif /* _VIDEOMEM_H_ */
