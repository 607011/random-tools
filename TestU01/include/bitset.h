 
/* bitset.h  for ANSI C */
#ifndef BITSET_H
#define BITSET_H
 

extern unsigned long bitset_maskUL[];


typedef unsigned long bitset_BitSet;

 
#define bitset_SetBit(S, b) ((S) |= (bitset_maskUL[b]))
 

 
#define bitset_ClearBit(S, b) ((S) &= ~(bitset_maskUL[b]))
 

 
#define bitset_FlipBit(S, b) ((S) ^= (bitset_maskUL[b]))
 

 
#define bitset_TestBit(S, b)  ((S) & (bitset_maskUL[b]) ? 1 : 0)
 


void bitset_WriteSet (char *desc, bitset_BitSet Z, int s);

 
#endif
 

