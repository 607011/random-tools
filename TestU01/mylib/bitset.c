/*************************************************************************\
 *
 * Package:        MyLib
 * File:           bitset.c
 * Environment:    ANSI C
 *
 * Copyright (c) 2002 Pierre L'Ecuyer, DIRO, Université de Montréal.
 * e-mail: lecuyer@iro.umontreal.ca
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted without a fee for private, research,
 * academic, or other non-commercial purposes.
 * Any use of this software in a commercial environment requires a
 * written licence from the copyright owner.
 *
 * Any changes made to this package must be clearly identified as such.
 *
 * In scientific publications which used this software, a reference to it
 * would be appreciated.
 *
 * Redistributions of source code must retain this copyright notice
 * and the following disclaimer.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
\*************************************************************************/


#include "bitset.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>


unsigned long bitset_maskUL[] = {
   1,
   2,
   4,
   8,
   16,
   32,
   64,
   128,
   256,
   512,
   1024,
   2048,
   4096,
   8192,
   16384,
   32768,
   65536,
   131072,
   262144,
   524288,
   1048576,
   2097152,
   4194304,
   8388608,
   16777216,
   33554432,
   67108864,
   134217728,
   268435456,
   536870912,
   1073741824,
   2147483648U
#if ULONG_MAX > 4294967295UL
   ,
   4294967296,
   8589934592,
   17179869184,
   34359738368,
   68719476736,
   137438953472,
   274877906944,
   549755813888,
   1099511627776,
   2199023255552,
   4398046511104,
   8796093022208,
   17592186044416,
   35184372088832,
   70368744177664,
   140737488355328,
   281474976710656,
   562949953421312,
   1125899906842624,
   2251799813685248,
   4503599627370496,
   9007199254740992,
   18014398509481984,
   36028797018963968,
   72057594037927936,
   144115188075855872,
   288230376151711744,
   576460752303423488,
   1152921504606846976,
   2305843009213693952,
   4611686018427387904,
   9223372036854775808UL
#endif
   };



void bitset_WriteSet (char *desc, bitset_BitSet S, int n)
{
   int i;
   bitset_BitSet mask;
   
   util_Assert (n > 0, "bitset_WriteSet:   s <= 0");
   if ((unsigned) n > CHAR_BIT * sizeof (bitset_BitSet)) {
      n = CHAR_BIT * sizeof (bitset_BitSet);
      printf ("********** bitset_WriteSet:   only %d bits in a BitSet\n\n", n);
   }
   if (desc != NULL && strlen (desc) > 0)
      printf ("%s", desc);
   mask = (bitset_BitSet) 1 << (n - 1);
   for (i = 0; i < n; i++) {
      if (S & mask)
         printf ("1");
      else
         printf ("0");
      mask >>= 1;
   }
}
