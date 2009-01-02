// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __BITVECTOR_H_
#define __BITVECTOR_H_

#include <cstdlib>
#include <vector>


class BitVector {

    typedef unsigned int BitVectorElementType;

public:
    BitVector(void) { }

    BitVector(size_t n)
    {
        resize(n);
    }

    inline bool operator[](size_t i) const
    {
        size_t j = i / BitsPerElement;
        size_t k = 1 << (i % BitsPerElement);
        return (v.at(j) & k) == k;
    }

    inline bool at(size_t i) const
    {
        size_t j = i / BitsPerElement;
        size_t k = 1 << (i % BitsPerElement);
        return (v.at(j) & k) == k;
    }

    inline void set(size_t i)
    {
        size_t j = i / BitsPerElement;
        size_t k = 1 << (i % BitsPerElement);
        v[j] |= k;
    }

    void resize(size_t n)
    {
        v.resize(1 + n / BitsPerElement);
    }

private:
    enum _bits_per_element { BitsPerElement = 8 * sizeof(BitVectorElementType) };
    std::vector<BitVectorElementType> v;

};


#endif // __BITVECTOR_H_
