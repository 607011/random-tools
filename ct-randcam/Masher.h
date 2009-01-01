// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __MASHER_H_
#define __MASHER_H_

#include <cstdlib>
#include "md5/md5.h"

class Masher {
public:
    Masher(const unsigned char* src, const size_t sz);
    ~Masher();
    void mash(const size_t nWays);
    inline const unsigned char* rng(void) const { return _Rng; }
    inline size_t size(void) const { return _RngSize; }

    static const size_t HashSize = 16; // MD5 hash size (fixed)

private:
    void fold(const unsigned char* r);
    void rotate(void);
    unsigned char* _Src;
    size_t _Sz;
    unsigned char* _Rng;
    unsigned char _Num;
    unsigned char* _Dst;
    size_t _Nways;
    size_t _MaxRngSize;
    size_t _PartSize;
    size_t _RngSize;
    unsigned char buffer[HashSize];
    int _BitIdx;
    static unsigned char bitMul[8];
};


#endif // __MASHER_H_
