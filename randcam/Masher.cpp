// $Id$
// Copyright (c) 2008-2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "Masher.h"
#include <cstring>


#ifndef _MAX
#define _MAX(a, b) ((a<b)?(b):(a))
#endif


Masher::Masher(const unsigned char* src, const size_t sz)
{
    _Src = const_cast<unsigned char*>(src);
    _Sz = sz;
    _MaxRngSize = _MAX(_Sz / sizeof(unsigned char) / 4, _Sz / HashSize);
    _Rng = new unsigned char[_MaxRngSize];
    _Dst = new unsigned char[_Sz];
    memset(buffer, 0, HashSize);
}


Masher::~Masher()
{
    delete [] _Dst;
    delete [] _Rng;
}


void Masher::mash(const size_t nWays)
{
    _Nways = (nWays > 0)? nWays : 1;
    _RngSize = HashSize * _Nways;
    _PartSize = _Sz / _Nways;
    size_t k = 0;
    for (size_t i = 0; i < _PartSize; ++i) 
        for (size_t j = 0; j < _Nways; ++j) 
            _Dst[j * _PartSize + i] = _Src[k++];
    for (size_t i = 0; i < _Nways; ++i)
    {
        const unsigned char* buf = const_cast<const unsigned char*>(_Src + i * _PartSize); 
        for (size_t j = 0; j < _PartSize; j += 20) 
            fold(buf + j);
        unsigned char* hash = _Rng + i * HashSize;
        md5_buffer((const char*) buf, _PartSize, hash);
        for (size_t j = 0; j < HashSize; ++j)
            hash[j] ^= buffer[j];
    }
}


void Masher::fold(const unsigned char* r)
{
    rotate();
    for (size_t i = 0; i < HashSize; ++i)
        buffer[i] ^= r[i];
}



static inline void _rotate(unsigned int& w, const int s = 1)
{
    const int b = 8 * sizeof(unsigned int);
    w = (w << s) | (w >> (b - s));
}


void Masher::rotate(void)
{
    unsigned int* b = reinterpret_cast<unsigned int*>(buffer);
    const size_t sz = HashSize / sizeof(unsigned int);
    for (size_t i = 0; i < sz; ++i)
        _rotate(b[i]);
}


unsigned char Masher::bitMul[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
