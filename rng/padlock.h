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

#ifndef _PADLOCK_H_
#define _PADLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif
//////////////////////////////////////////////////////////////////////////
// PADLOCK_API MACRO definition
#ifdef	_WIN32
#define PADLOCK_API _declspec(dllexport)
#else
#define PADLOCK_API 
#endif

////////////////////////////////////////////////////////////////////////////
// Parameter Type Definition


#ifndef uint_16
	typedef unsigned short uint_16;
#endif
#ifndef uint_32
	typedef unsigned int uint_32;
#endif

typedef enum _RNG_Result
{
	RNG_SUCCEEDED,
	RNG_FAILED
}RNG_RESULT;

typedef enum _ACE_AES_MODE
{
	ACE_AES_ECB=0,
	ACE_AES_CBC,
	ACE_AES_CFB128,
	ACE_AES_OFB128,
	ACE_AES_CTR
}ACE_AES_MODE;

typedef enum _AES_Result{
	AES_SUCCEEDED,
	AES_FAILED,
	AES_ADDRESS_NOT_ALIGNED,    // the addresses of plaintxt/ciphertxt/iv must be aligned by 128bits for VIA ACE hardware
	AES_NOT_BLOCKED,         // the length of plaintxt/ciphertxt must be multiples of 16bytes 
	AES_KEY_NOT_SUPPORTED,   // Invalid Key length
	AES_MODE_NOT_SUPPORTED   // Invalid cipher mode
}AES_RESULT;

typedef enum _SHA_RESULT{
	SHA_SUCCEEDED,
	SHA_FAILED  
}SHA_RESULT;

typedef enum _KEY_LENGTH
{
  KEY_128BITS,
  KEY_192BITS,
  KEY_256BITS
}KEY_LENGTH;

struct ace_aes_context;

struct aligned_memory_context;

/////////////////////////////////////////////////////////////////////////
//   VIA Padlock SDK API Declaration

//////////////////////////////////////////
//   VIA Padlock SDK RNG API

PADLOCK_API
int padlock_rng_available();
//	Function : To test whether VIA RNG hardware is available
//	Input	 :
//  Output   :
//	return   : true  -- 1 : if VIA RNG hardware is available
//	           false -- 0 : if VIA RNG hardware is unavailable

PADLOCK_API
RNG_RESULT padlock_rng_rand(unsigned char *rand, int rand_len);
//	Function : To generate random number by using VIA RNG hardware
//	Input	 : rand		 -- address to store the generated rand number
//			   rand_len	 -- how long a random number need to be generated
//  Output   : 	
//	return   : RNG_SUCCEEDED  -- successed.
//	           RNG_FAILED     -- failed.

///////////////////////////////////////////////
//    VIA Padlock SDK plain ACE aes API

PADLOCK_API
int padlock_ace_available();
//	Function : To test whether VIA ACE hardware is available
//	Input	 :
//  Output   :
//	return   : true  -- 1 : if VIA ACE hardware is available
//	           false -- 0 : if VIA ACE hardware is unavailable
 
PADLOCK_API
int padlock_ace2_available();
//	Function : To test whether VIA ACE2 hardware is available
//	Input	 :
//  Output   :
//	return   : true  -- 1 : if VIA ACE2 hardware is available
//	           false -- 0 : if VIA ACE2 hardware is unavailable

PADLOCK_API
struct ace_aes_context *padlock_aes_begin();
//	Function : To creat a ace aes context for the later cryptography
//	Input	 :
//  Output   :
//	return   : Pointer to a blank struct ace_aes_context variable
//  Note     : If it fails to create a context, the program will exit thus it will never return NULL

PADLOCK_API
AES_RESULT 
padlock_aes_setkey(struct ace_aes_context *ctx, const unsigned char *key, KEY_LENGTH key_len);
//	Function : To set key for later aes cryptography
//	Input	 : ctx		--  a pointer pointing to ace aes context for aes cryptography
//	           key		--  original cipher key for aes cryptography
//			   key_len	--  key length of the key See KEY_LENGTH Only KEY_128BITS/KEY_192BITS/KEY_256BITS are valid
//  Output   :
//	return   : See AES_RESULT

PADLOCK_API
AES_RESULT 
padlock_aes_setmodeiv(struct ace_aes_context *ctx, ACE_AES_MODE mode, unsigned char *iv);
//	Function : To set mode and iv for later aes cryptography
//	Input	 : ctx		--  a pointer pointing to ace aes context for aes cryptography
//	           mode		--  process mode for aes cryptography See ACE_AES_MODE
//                          only ACE_AES_ECB/ACE_AES_CBC/ACE_AES_CFB128/ACE_AES_OFB128 are valid
//			   iv		--  the initialization vector for aes cryptography
//  Output   :
//	return   : See AES_RESULT

PADLOCK_API
AES_RESULT 
padlock_aes_encrypt(struct ace_aes_context *ctx, unsigned char * plaintxt, unsigned char * ciphertxt, int nbytes);
//	Function : To encrypt the data in plaintxt and store the result in ciphertxt by VIA ACE hardware
//	Input	 : ctx			--  a pointer pointing to ace aes context for aes cryptography
//             plaintxt		--  address where the data to be encrypted is stored
//			   ciphertxt	--  address where the encryption result to be stored
//			   nbytes		--  how many data need to be encrypted
//  Output   :
//	return   : See AES_RESULT
//	Note	 : iv will be updated after calling this function in cbc/cfb/ofb mode cipher
//             remember to backup the original iv value before calling this function.

PADLOCK_API
AES_RESULT 
padlock_aes_decrypt(struct ace_aes_context *ctx, unsigned char * ciphertxt, unsigned char * plaintxt, int nbytes);
//	Function : To encrypt the data in plaintxt and store the result in ciphertxt by VIA ACE hardware
//	Input	 : ctx			--  a pointer pointing to ace aes context for aes cryptography
//			   ciphertxt	--  address where the data to be decrypted is stored
//             plaintxt		--  address where the decryption result to be stored
//			   nbytes		--  how many data need to be encrypted
//  Output   :
//	return   : See AES_RESULT
//	Note	 :  iv will be updated after calling this function in cbc/cfb/ofb mode cipher
//              remember to backup the original iv value before calling this function.

PADLOCK_API
void padlock_aes_close(struct ace_aes_context *ctx);
//	Function : To destroy ace aes context
//	Input	 : ctx	--  a pointer pointing ro ace aes context for aes cryptography


/////////////////////////////////////////////////////////////////////////////////
// VIA Padlock SDK aligned AES API
// to allocate and free nbytes memory whose address is aligned with 16 bytes

PADLOCK_API 
struct aligned_memory_context *padlock_aligned_malloc(int size);
//	Function : To create an aligned memory context
//	Input	 : size  -- the size of aligned memory
//  Output   :
//	return   : a pointer pointing to an aligned memory context
//  Note     :  (old version 2.0.1)If it fails to create a context, the program will exit thus it will never return NULL
//              (now version 2.0.2)If it fails to create a context, the program will return NULL

PADLOCK_API
void padlock_aligned_mfree(struct aligned_memory_context *aligned_mctx);
//	Function : To destroy an aligned memory context
//	Input	 : aligned_mctx -- an aligned memory context pointer

PADLOCK_API
AES_RESULT
padlock_aligned_memcpy_to(struct aligned_memory_context *aligned_mctx, unsigned char *src, int nbytes);
//	Function : To copy data from src to aligned memory
//	Input	 : aligned_mctx				-- the dst aligned memory context
//             src						-- the address of src data
//             aligned_offset_bytes		-- the destination offset in aligned memory in bytes
//             nbytes					-- the total number of the data to be moved
//  Output   :
//	return   : See ALIGNED_MEMORY_RESULT
//  Note     : aligned memory context is stateless so user must keep its state such as the offset etc by himself

PADLOCK_API
AES_RESULT
padlock_aligned_memcpy_from(struct aligned_memory_context *aligned_mctx, unsigned char *dst, int nbytes);
//	Function : To copy data from aligned memory to dst
//	Input	 : aligned_mctx				-- the src aligned memory context
//             src						-- the address of dst data
//             aligned_offset_bytes		-- the source offset in aligned memory in bytes
//             nbytes					-- the total number of the data to be moved
//  Output   :
//	return   : See ALIGNED_MEMORY_RESULT
//  Note     : aligned memory context is stateless so user must keep its state such as the offset etc by himself

PADLOCK_API
AES_RESULT 
padlock_aes_aligned_encrypt(unsigned char *key, KEY_LENGTH key_len, 
                                 ACE_AES_MODE mode, 
					             struct aligned_memory_context *buf_aligned_mctx, 
					             unsigned char *iv);
//	Function :  To encrypt plaintxt to ciphertxt by VIA ACE hardware
//	Input	 :  key					-- address of the key
//				key_len				-- length of the key
//				mode				-- cipher mode : ecb/cbc/cfb/ofb
//				buf_aligned_mctx	-- aligned cipher buffer context
//				iv					-- address of initialization vector. 
//  Output   : 	
//	return   :  See AES_RESULT  
//	Note	 :  iv will be updated after calling this function in cbc/cfb/ofb mode cipher
//              remember to backup the original iv value before calling this function.

PADLOCK_API
AES_RESULT 
padlock_aes_aligned_decrypt(unsigned char *key, 
                                 KEY_LENGTH key_len, 
                                 ACE_AES_MODE mode, 
					             struct aligned_memory_context *buf_aligned_mctx, 
					             unsigned char *iv);
//	Function :  To decrypt ciphertxt to plaintxt by VIA ACE hardware
//	Input	 :  key					-- address of the key
//				key_len				-- length of the key
//				mode				-- cipher mode : ecb/cbc/cfb/ofb
//				buf_aligned_mctx	-- aligned cipher buffer context
//				iv					-- address of initialization vector. 
//  Output   : 	
//	return   :  AES_SUCCEEDED	-- successed
//	            AES_FAILED	-- failed   
//	Note	 :  iv will be updated after calling this function in cbc/cfb/ofb mode cipher
//              remember to backup the original iv value before calling this function.


////////////////////////////////////////////////////////////////////////////////
// VIA Padlock SDK fast ACE AES API for aligned data cryptography
// the addresses of plaintxt and ciphertxt should be aligned with 16 bytes
// or nothing will be done by VIA ACE hardware.

PADLOCK_API
AES_RESULT
padlock_aes_fast_encrypt(unsigned char *key, KEY_LENGTH key_len, ACE_AES_MODE mode, 
					          unsigned char *aligned_plaintxt, unsigned char *aligned_ciphertxt, int nbytes, 
					          unsigned char *iv);
//	Function :  To encrypt plaintxt to ciphertxt by VIA ACE hardware
//	Input	 :  key					-- address of the key
//				key_len				-- length of the key
//				mode				-- cipher mode : ecb/cbc/cfb/ofb
//				aligned_plaintxt	-- address of plaintxt to be encrypted which should be aligned with 16 bytes
//				aligned_ciphertxt	-- address of ciphertxt to store the result which should be aligned with 16 bytes
//				nbytes				-- length of plaintxt (in bytes)
//				iv					-- address of initialization vector. 
//  Output   : 	
//	return   :  See AES_RESULT  
//	Note	 :  iv will be updated after calling this function in cbc/cfb/ofb mode cipher
//              remember to backup the original iv value before calling this function.

PADLOCK_API
AES_RESULT
padlock_aes_fast_decrypt(unsigned char *key, KEY_LENGTH key_len, ACE_AES_MODE mode, 
					          unsigned char *aligned_ciphertxt, unsigned char *aligned_plaintxt, int nbytes, 
					          unsigned char *iv);
//	Function :  To decrypt ciphertxt to plaintxt by VIA ACE hardware
//	Input	 :  key					-- address of the key
//				key_len				-- length of the key
//				mode				-- cipher mode : ecb/cbc/cfb/ofb
//				aligned_ciphertxt	-- address of ciphertxt to be decrypted which should be aligned with 16 bytes
//				aligned_plaintxt	-- address of plaintxt to store the result which should be aligned with 16 bytes
//				nbytes				-- length of plaintxt (in bytes)
//				iv					-- address of initialization vector. 
//  Output   : 	
//	return   :  AES_SUCCEEDED	-- successed
//	            AES_FAILED	-- failed   
//	Note	 :  iv will be updated after calling this function in cbc/cfb/ofb mode cipher
//              remember to backup the original iv value before calling this function.


//////////////////////////////////////////
//   VIA Padlock SDK PHE API

PADLOCK_API
int padlock_phe_available();
//	Function : To test whether VIA SHA1 hardware is available
//	Input	 :
//  Output   :
//	return   : true  -- 1 : if VIA SHA1 hardware is available
//	           false -- 0 : if VIA SHA1 hardware is unavailable

PADLOCK_API
SHA_RESULT padlock_phe_sha1(unsigned char *src, int nbytes,unsigned char* dst);
//	Function :  To encrypt string to encrypted by VIA SHA hardware
//	Input	 :  src					-- address of  source text to be hashed
//				nbytes				-- length of source string (in bytes)
//	Output		dst					-- address of digest buffer to store the result 

//	return   :  SHA_SUCCEEDED	-- successed
//	            SHA_FAILED	-- failed   


PADLOCK_API
SHA_RESULT padlock_phe_sha256(unsigned char *src, int nbytes,unsigned char* dst);
//	Function :  To encrypt string to encrypted by VIA SHA hardware
//	Input	 :  src					-- address of  source text to be hashed
//				nbytes				-- length of source string (in bytes)
//	Output		dst					-- address of digest buffer to store the result 
//	return   :  SHA_SUCCEEDED	-- successed
//	            SHA_FAILED	-- failed   


//////////////////////////////////////////
//   VIA Padlock SDK PMM API

PADLOCK_API
int padlock_pmm_available();
//	Function : To test whether VIA Mont hardware is available
//	Input	 :
//  Output   :
//	return   : true  -- 1 : if VIA Mont hardware is available
//	           false -- 0 : if VIA Mont hardware is unavailable

PADLOCK_API
void padlock_pmm( uint_32* A, uint_32* B, uint_32* M, uint_32* dst, uint_32 ndigits);
//	Function : To implement Montgomery Multiplication algorithm
//	Input	 :	A		-- pointer of big integer A
//				B		-- pointer of big integer B
//				M    	-- pointer of big integer M, modulus
//			    T		-- pointer of big integer T, result buffer
//				ndigits	-- the length of big integer A, B, M, T, in digits(32bit). 
//	Output	 :
//	return	 :


#ifdef __cplusplus
}
#endif

#endif /* _PADLOCK_H_ */
