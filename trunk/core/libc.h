/**
 * Alibc - Simple libc
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
#ifndef _LIBC_H_
#define _LIBC_H_

#include <core/types.h>

/* string.h functions */

void *memcpy(void *dest, const void *src, unsigned int len);
void *memset(void *dest, int c, unsigned int len);
int memcmp(const void *s1, const void *s2, size_t len);

unsigned int strlen(const char *str);
unsigned int strnlen(const char *str, size_t maxlen);


/* same as strncpy/strncat but last char is '\0' */
char *strzcpy(char *dest, const char *src, int len);
char *strzcat(char *dest, const char *src, size_t len);
 
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int len);

/* some stdarg.h macros for va_list */
#define __GNUC_VA_LIST 
typedef void *__gnuc_va_list;
typedef __gnuc_va_list va_list;
#define __va_rounded_size(TYPE) \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))
#define va_start(AP, LASTARG) \
  (AP = ((__gnuc_va_list) __builtin_next_arg (LASTARG)))
#define va_end(AP) \
  ((void)0)
#define va_arg(AP, TYPE) \
  (AP = (__gnuc_va_list) ((char *) (AP) + __va_rounded_size (TYPE)),  \
   *((TYPE *) (void *) ((char *) (AP) - __va_rounded_size (TYPE))))
#define __va_copy(dest, src) \
  (dest) = (src)

/* stdarg.h functions (always ended by '\0') */
int vsnprintf(char *str, size_t len, const char *format, va_list ap);
int snprintf(char *str, size_t len, const char *format, /*args*/ ...)
  __attribute__((format(printf, 3, 4)));

#endif /* _LIBC_H_ */
