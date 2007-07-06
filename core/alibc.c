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

#include "alibc.h"


void *memcpy(void *dest, const void *src, unsigned int len)
{/* copy memory area */
	
	char *tmp = (char *)dest, *stmp = (char *)src;
	
	while(len--)
		*tmp++ = *stmp++;
	
	return dest;
}


void *memset(void *dest, int c, unsigned int len)
{/* fill memory with a constant byte */
	
	char *tmp = dest;
	
	while(len--)
		*tmp++ = c;
	
	return dest;
}


int memcmp(const void *s1, const void *s2, size_t len)
{/* compare memory areas */
	
	const unsigned char *tmp1, *tmp2;
	
	for(tmp1 = s1, tmp2 = s2; len; len--, tmp1++, tmp2++)
		if(*tmp1 != *tmp2) {
			return *tmp1-*tmp2;
		}
	
	return 0;
}


unsigned int strlen(const char *str)
{/* calculate the length of a string */
	
	unsigned int ret = 0;
	
	while(*str++ != '\0')
		ret++;
	
	return ret;
}


unsigned int strnlen(const char *str, size_t maxlen)
{/*  determine the length of a fixed-size string */
	
	unsigned int ret = 0;
	
	while(*str++ != '\0' && maxlen--)
		ret++;
	
	return ret;
}


char *strzcpy(char *dest, const char *src, int len)
{/* copy n bytes of a string; null-terminated */
	
	char *tmp = dest;
	
	while(*src != '\0' && len--)
		*dest++ = *src++;
		
	*dest = '\0';
	
	return tmp;
}


char *strzcat(char *dest, const char *src, size_t len)
{/* concatenate n char of src to dest; null-terminated */
	
	char *tmp = dest;
	
	while(*dest != '\0')
		dest++;
	
	while(*src != '\0' && len--)
		*dest++ = *src++;
	
	*dest = '\0';
	
	return tmp;	
}


int strcmp(const char *s1, const char *s2)
{/* compare two string */
	
	while(*s1 != '\0') {
		if(*s1 != *s2)
			return *s1-*s2;
		s1++; s2++;
	}
	
	return 0;
}


int strncmp(const char *s1, const char *s2, int len)
{/* compare the n first char of two string */
	
	while(*s1 != '\0' && len--) {
		if(*s1 != *s2)
			return *s1-*s2;
		s1++; s2++;
	}
	
	return 0;
}


int vsnprintf(char *str, size_t len, const char *format, va_list ap)
{/* produce output according to a format (%idcs) from va_list */
	
	size_t i, ret = 0;
	
	if(!str || !format || len < 0)
		return -1;
	
#define PUTCHAR(achar)\
	({\
		if(ret < len-1)\
			*str++= achar;\
		ret++;\
	})
	
	for(i = 0; format[i] != '\0'; i++) {
		switch(format[i])
		{
			case '%':
			{
				i++;
				
				switch(format[i])
				{
					case '%':
					{
						PUTCHAR('%');
						break;
					}
					case 'i':;
					case 'd':
					{
						/* 32 bits int = [-2^31, 2^31-1] */
						int integer = va_arg(ap, int);
						char buff[10];
						int j;
						
						if(integer < 0)
							PUTCHAR('-');
						
						for(j = 0; integer; j++) {
							buff[j] = (integer > 0)? integer%10: integer%(-10);
							integer = (integer-buff[j])/10;
							buff[j] = (buff[j] > 0)? buff[j]: -buff[j];
						}
						
						while(j--)
							PUTCHAR('0'+buff[j]);
						
						break;
					}
					case 'c':
					{
						int character = va_arg(ap, int);
						PUTCHAR((char)character);
						break;
					}
					case 's':
					{
						char *string = va_arg(ap, char *);
						
						if(string == NULL)
							string = "(null)";
						
						while(*string != '\0') {
							PUTCHAR(*string);
							string++;
						}
						
						break;
					}
					case 'x':
					{
						/* unsigned int hexa = va_arg(ap, int);*/
						
						/* TODO: hexa */
						
						break;
					}

					default:
					{
						PUTCHAR('%');
						PUTCHAR(format[i]);
					}
				}
				break;
			}			
			default:
				PUTCHAR(format[i]);
		}
	}
	
	*str = '\0';
	
	return ret;
}


int snprintf(char *str, size_t len, const char *format, ...)
{/* produce output according to a format from args */
	va_list ap;
	
	va_start(ap, format);
	len = vsnprintf(str, len, format, ap);
	va_end(ap);
	
	return len;
}
