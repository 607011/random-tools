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

#ifndef VIA_ACE_H
#define VIA_ACE_H
#include "padlock.h"
#include <malloc.h>
#include <memory.h>
#ifdef __cplusplus
extern "C" {
#endif

// The Maximum Size for padlock_ace_alloc_buf is 64kb 
#define MAX_CIPHER_BUFFER_SIZE            (64 * 1024)

#define NEH_GEN_KEY 	0x00000000	// generate key schedule
#define NEH_LOAD_KEY 	0x00000080	// load key schedule from memory

#define NEH_ENCRYPT 	0x00000000	// encryption
#define NEH_DECRYPT 	0x00000200	// decryption

#define NEH_KEY128 	0x000 + 0x00a	// 128 bit key
#define NEH_KEY192 	0x400 + 0x00c	// 192 bit key
#define NEH_KEY256 	0x800 + 0x00e	// 256 bit key

#define RNG_MASK    0x0C		// CPUID EDX code for RNG
#define ACE_MASK    0xC0		// CPUID EDX code for ACE
#define ACE2_MASK   0x300		// CPUID EDX code for ACE2

struct ace_aes_context
{
	ACE_AES_MODE mode;
	KEY_LENGTH key_length;
	unsigned char *iv;
	unsigned char last_iv[16];
	unsigned int encrypt_extended_key[60];
	unsigned int decrypt_extended_key[60];
};

struct aligned_memory_context
{
	int size;
	int offset;                     // the length of buf used
	unsigned char *temp_buf;
	unsigned char *p_alignedbuf;
};

#define ALIGN16(x) (unsigned char *)((((unsigned long)x) + 15 )&(~15UL))
#ifndef UNALIGNED
#define UNALIGNED(addr) ((unsigned int)(addr) & 0x0000000F)
#endif


#ifndef _WIN32 // Linux Platform related

#define PUSHREG	asm("pushl %eax\n pushl %ebx\n pushl %ecx\n pushl %edx\n pushl %edx\n pushl %esi\n pushl %edi\n");
#define POPREG  asm("popl %edi\n popl %esi\n popl %edx\n popl %ecx\n popl %ebx\n popl %eax\n");
#define XSTORERNG	asm(".byte 0x0F, 0xA7, 0xC0\n");

#else	//Windows Platform related

#define XFUNC  __asm _emit 0x0F __asm _emit 0xA7
#define XCRYPT __asm _emit 0xF3 XFUNC
#define XCRYPT_ECB XCRYPT __asm _emit 0xC8
#define XCRYPT_CBC XCRYPT __asm _emit 0xD0
#define XCRYPT_CTR XCRYPT __asm _emit 0xD8
#define XCRYPT_CFB XCRYPT __asm _emit 0xE0
#define XCRYPT_OFB XCRYPT __asm _emit 0xE8	
#define XCRYPT_F0 XFUNC __asm _emit 0xF0
#define XCRYPT_F8 XFUNC __asm _emit 0xF8
#define NEW_KEYS   __asm pushfd  __asm popfd
#define XSTORE XFUNC __asm _emit 0xC0
#define REPXSTORE __asm _emit 0xF3 XSTORE
#define PUSHREG __asm push eax __asm push ebx __asm push ecx __asm push edx __asm push esi __asm push edi
#define POPREG __asm pop edi __asm pop esi __asm pop edx __asm pop ecx __asm pop ebx __asm pop eax
#define  ENPREPAREREG_ECB \
		{\
			__asm	mov ecx, count  \
			__asm	mov edx, ctrl  \
			__asm	mov ebx, key  \
			__asm	mov esi, src  \
			__asm	mov edi, dst  \
		}
#define  ENPREPAREREG_OTHER \
		{\
			__asm	mov ecx, count  \
			__asm	mov edx, ctrl  \
			__asm	mov ebx, key  \
			__asm	mov eax, iv  \
			__asm	mov esi, src  \
			__asm	mov edi, dst  \
		}
#endif

static __inline volatile void ace_ecb_op5(
				const void *key, 
				const void *ctrl, 
				const void *src, 
				void *dst, 
				int count) 
{
#ifndef _WIN32
 __asm__ __volatile__ ("pushl %%eax\n pushl %%ebx\n pushl %%ecx\n \
	    pushl %%edx\n pushl %%esi\n pushl %%edi\n \
	    \
	    pushfl\n popfl\n\t \
		\
		movl %0, %%ebx\n\t \
		movl %1, %%ecx\n\t \
		movl %2, %%edx\n\t \
		movl %3, %%esi\n\t \
		movl %4, %%edi\n\t \
		\
		rep\n  \
		.byte 0x0F, 0xA7, 0xC8\n \
        	\
	        popl %%edi\n popl %%esi\n popl %%edx\n \
		popl %%ecx\n popl %%ebx\n popl %%eax\n" \
		: \
		:"m"(key), "m"(count), "m"(ctrl), "m"(src), "m"(dst) \
		:"ecx", "edx", "esi", "edi");
		// The clobbered list does not include ebx:
		// High version of gcc uses ebx register as the PIC register 
                // when compiling with the -fPIC flag, 
		// and does not allow this register to be clobbered in asm.
#else
	PUSHREG
	NEW_KEYS;
	ENPREPAREREG_ECB;
	XCRYPT_ECB;
	POPREG
#endif
}

static __inline volatile void ace_cbc_op6(
				const void *key, 
				const void *ctrl, 
				const void *src, 
				void *dst, 
				int count,
				void *iv) 
{
#ifndef _WIN32
	__asm__ __volatile__ ("pushl %%eax\n pushl %%ebx\n pushl %%ecx\n \
	    pushl %%edx\n pushl %%esi\n pushl %%edi\n \
	    \
		pushfl\n popfl\n\t \
		\
		movl %0, %%eax\n\t \
		movl %1, %%ebx\n\t \
		movl %2, %%ecx\n\t \
		movl %3, %%edx\n\t \
		movl %4, %%esi\n\t \
		movl %5, %%edi\n\t \
		\
		rep\n  \
		.byte 0x0F, 0xA7, 0xD0\n \
		\
		popl %%edi\n popl %%esi\n popl %%edx\n \
		popl %%ecx\n popl %%ebx\n popl %%eax\n" \
		: \
		:"m"(iv),"m"(key), "m"(count), "m"(ctrl), \
		 "m"(src), "m"(dst) \
		:"eax", "ecx", "edx", "esi", "edi");
		// The clobbered list does not include ebx:
		// High version of gcc uses ebx register as the PIC register 
                // when compiling with the -fPIC flag, 
		// and does not allow this register to be clobbered in asm.
#else
	PUSHREG
	NEW_KEYS;
	ENPREPAREREG_OTHER;
	XCRYPT_CBC;
	POPREG
#endif
}

static __inline volatile void ace_ctr_op6(const void *key, 
			const void *ctrl, 
			 const void *src, 
			 void *dst, 
			 int count,
			 void *iv) 
{
#ifndef _WIN32
	__asm__ __volatile__ ("pushl %%eax\n pushl %%ebx\n pushl %%ecx\n \
	pushl %%edx\n pushl %%esi\n pushl %%edi\n \
	\
	pushfl\n popfl\n\t \
	\
	movl %0, %%eax\n\t \
	movl %1, %%ebx\n\t \
	movl %2, %%ecx\n\t \
	movl %3, %%edx\n\t \
	movl %4, %%esi\n\t \
	movl %5, %%edi\n\t \
	\
	rep\n  \
	.byte 0x0F, 0xA7, 0xD8\n \
	\
	popl %%edi\n popl %%esi\n popl %%edx\n \
	popl %%ecx\n popl %%ebx\n popl %%eax\n" \
	: \
	:"m"(iv),"m"(key), "m"(count), "m"(ctrl), \
	 "m"(src), "m"(dst) \
	:"eax", "ecx", "edx", "esi", "edi"); 
#else
	PUSHREG
	NEW_KEYS;
	ENPREPAREREG_OTHER;
	XCRYPT_CTR;
	POPREG
#endif
}

static __inline volatile void ace_cfb_op6(
				const void *key, 
				const void *ctrl, 
				void *src, 
				void *dst, 
				int count,
				void *iv) 
{
#ifndef _WIN32
	__asm__ __volatile__ ("pushl %%eax\n pushl %%ebx\n pushl %%ecx\n \
	    pushl %%edx\n pushl %%esi\n pushl %%edi\n \
	    \
	    pushfl\n popfl\n\t \
		\
		movl %0, %%eax\n\t \
		movl %1, %%ebx\n\t \
		movl %2, %%ecx\n\t \
		movl %3, %%edx\n\t \
		movl %4, %%esi\n\t \
		movl %5, %%edi\n\t \
		\
		rep\n  \
		.byte 0x0F, 0xA7, 0xE0\n \
		\
		popl %%edi\n popl %%esi\n popl %%edx\n \
		popl %%ecx\n popl %%ebx\n popl %%eax\n" \
		: \
		:"m"(iv),"m"(key), "m"(count), "m"(ctrl), \
		 "m"(src), "m"(dst) \
		:"eax", "ecx", "edx", "esi", "edi");
		// The clobbered list does not include ebx:
		// High version of gcc uses ebx register as the PIC register 
                // when compiling with the -fPIC flag, 
		// and does not allow this register to be clobbered in asm.
#else
	PUSHREG
	NEW_KEYS;
	ENPREPAREREG_OTHER;
	XCRYPT_CFB;
	POPREG
#endif
}

static __inline volatile void ace_ofb_op6(
				const void *key, 
				const void *ctrl, 
				void *src, 
				void *dst, 
				int count,
				void *iv) 
{
#ifndef _WIN32
	__asm__ __volatile__ ("pushl %%eax\n pushl %%ebx\n pushl %%ecx\n \
	    pushl %%edx\n pushl %%esi\n pushl %%edi\n \
	    \
	    pushfl\n popfl\n\t \
		\
		movl %0, %%eax\n\t \
		movl %1, %%ebx\n\t \
		movl %2, %%ecx\n\t \
		movl %3, %%edx\n\t \
		movl %4, %%esi\n\t \
		movl %5, %%edi\n\t \
		\
		rep\n  \
		.byte 0x0F, 0xA7, 0xE8\n \
		\
		popl %%edi\n popl %%esi\n popl %%edx\n \
		popl %%ecx\n popl %%ebx\n popl %%eax\n" \
		: \
		:"m"(iv),"m"(key), "m"(count), "m"(ctrl), \
		 "m"(src), "m"(dst) \
		:"eax", "ecx", "edx", "esi", "edi"); 
		// The clobbered list does not include ebx:
		// High version of gcc uses ebx register as the PIC register 
                // when compiling with the -fPIC flag, 
		// and does not allow this register to be clobbered in asm.
#else
	PUSHREG
	NEW_KEYS;
	ENPREPAREREG_OTHER;
	XCRYPT_OFB;
	POPREG
#endif
}

#ifdef _WIN32
int ace_ctr(
		const void *key, 
		const void *ctrl, 
		const void *src, 
		void *dst, 
		int count,
		void *iv);
#endif


AES_RESULT
ace_aes_atomic_crypt(struct ace_aes_context *ctx, 
                          int enc,
                          unsigned char *src, 
						  unsigned char *dst, 
						  int nbytes);


		
#if defined(__cplusplus)
}
#endif

#endif /* VIA_ACE_H */
