/*
 * Copyright 1998-2005 VIA Technologies, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * VIA, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "padlock.h"
#include "via_ace.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


////////////////////////////////////////////////////////////////
// VIA Padlock SDK RNG API
int padlock_rng_available()
{
	int result = 0;
#ifndef _WIN32
	__asm__ __volatile__ ("pushl %%eax\n pushl %%ebx\n pushl %%ecx\n \
	      	pushl %%edx\n pushl %%esi\n pushl %%edi\n	\
		    \
   	    	movl $0xC0000000, %%eax\n \
	    	cpuid\n \
	    	cmpl $0xC0000001, %%eax\n \
	    	jb  local_label\n \
	    	movl $0xC0000001, %%eax\n \
	    	cpuid\n \
	    	andl $0x0C, %%edx\n \
	    	cmpl $0x0C, %%edx\n \
	    	jne  local_label \n \
	     	movl $1, %0 \n \
	    	\
local_label:\n \
             	popl %%edi\n popl %%esi\n popl %%edx\n \
		popl %%ecx\n popl %%ebx\n popl %%eax\n "
		:"=m"(result));
#else
	__asm {
		mov eax,  0xC0000000;
		cpuid;

	    cmp eax, 0xC0000001;  
		jb  local_label;

		mov eax,  0xC0000001;
		cpuid;

	    and edx, RNG_MASK
		cmp edx, RNG_MASK
	    jne local_label

	    mov result, 1;

local_label:
	};
#endif
	return result;
}
// Ted Zhou:The BCNT is Bits[4:0] in EAX
// So "0x0000001f and eax" is correct 
static RNG_RESULT padlock_rng(unsigned char *rand, int rand_len)
{
	while(rand_len)
	{
#ifndef _WIN32
		asm("pushl %edi\n"
		    "pushl %edx\n"
		   );
		asm("movl %0, %%edi\n"
		    "movl $3, %%edx\n"
		    :
		    :"m"(rand)
			);
		XSTORERNG
		asm("movl %%edi, %0\n"
		    "andl $0x0000001F, %%eax\n"
		    "subl %%eax, %1\n" 
		    :"=m"(rand), "=m"(rand_len)
		    :
			);
		asm("popl %edx\n"
		    "popl %edi\n"
		   );
#else
	__asm{
			mov edi, rand;
			mov edx, 3;
			XSTORE;
			mov rand, edi;
			and eax, 0x0000001F;
			sub rand_len, eax;
		}
#endif
	}

	return RNG_SUCCEEDED;
}

RNG_RESULT 
padlock_rng_rand(unsigned char *rand,int rand_len)
{

	int processing_len = 0;
	int processed_len = 0;
	unsigned char *temp_buffer;
    unsigned char *buffer;
	RNG_RESULT ret= RNG_SUCCEEDED;	
	
	if(NULL == rand)
	{
		printf("Random number storage address null pointer error!\n");
		return RNG_FAILED;
	}

	if (0 == rand_len)
	{
		return RNG_SUCCEEDED;
	}

	// first 16 bytes because RNG hardware may change the value after allocated memory
	// boundary second 16 bytes because RNG hardware need the destination memory address
	// is 16 bytes aligned
	temp_buffer = (unsigned char *)malloc(MAX_CIPHER_BUFFER_SIZE+16+16);
	if(temp_buffer == NULL)
	{
		printf("Fail to malloc buffer for RNG!\n");
		return RNG_FAILED;
	}

	buffer = ALIGN16(temp_buffer);

	processing_len = rand_len > MAX_CIPHER_BUFFER_SIZE ? MAX_CIPHER_BUFFER_SIZE : rand_len;
		
	do{
			
		ret = padlock_rng(buffer,processing_len);

		if(ret != RNG_SUCCEEDED)
		{
			break;
		}
		
		memcpy(rand + processed_len, buffer, processing_len);
			
		processed_len = processed_len + processing_len;
			
		if((rand_len - processed_len) < MAX_CIPHER_BUFFER_SIZE)
		{
			processing_len = rand_len - processed_len;
		}
		
	}while(processed_len < rand_len);

	free( temp_buffer);

	return ret;
}



