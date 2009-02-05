// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __LINEARCONGRUENTIAL_H_
#define __LINEARCONGRUENTIAL_H_

#include "abstract_random_number_generator.h"

namespace ctrandom {

    template <unsigned long long A, unsigned long long R0, unsigned long long C, unsigned long long M>
    class LinearCongruential : public RandomNumberGenerator<unsigned int>
    {
    public:
        LinearCongruential(unsigned long long _R = R0)
        { }
        unsigned int operator()()
        {
            _R = (A * _R + C) % M;
            return (unsigned int) (_R & 0xffffffffULL);
        }
        inline void seed(unsigned long long _Seed) { _R = _Seed; }
        inline unsigned int max(void) { return M; }

    private:
        unsigned long long _R;
    };

    typedef LinearCongruential<1103515245ULL, 12345ULL, 12345ULL, 2147483648ULL> LCG_ANSIC;
    typedef LinearCongruential<16807ULL, 1ULL, 0ULL, 2147483647ULL> LCG_MINSTD;
    typedef LinearCongruential<1078318381ULL, 1ULL, 0ULL, 2147483647ULL> LCG_HOAGLIN1;
    typedef LinearCongruential<1203248318ULL, 1ULL, 0ULL, 2147483647ULL> LCG_HOAGLIN2;
    typedef LinearCongruential<397204094ULL, 1ULL, 0ULL, 2147483647ULL> LCG_HOAGLIN3;
    typedef LinearCongruential<2027812808ULL, 1ULL, 0ULL, 2147483647ULL> LCG_HOAGLIN4;
    typedef LinearCongruential<1323257245ULL, 1ULL, 0ULL, 2147483647ULL> LCG_HOAGLIN5;
    typedef LinearCongruential<764261123ULL, 1ULL, 0ULL, 2147483647ULL> LCG_HOAGLIN6;
    typedef LinearCongruential<452807053ULL, 1ULL, 0ULL, 2147483648ULL> LCG_URN12;
    typedef LinearCongruential<630360016ULL, 1ULL, 0ULL, 2147483647ULL> LCG_SIMSCRIPT;
    typedef LinearCongruential<65539ULL, 1ULL, 0ULL, 2147483648ULL> LCG_RANDU;
    typedef LinearCongruential<950706376ULL, 1ULL, 0ULL, 2147483647ULL> LCG_FISHMAN;
    typedef LinearCongruential<2147001325ULL, 715136305ULL, 0ULL, 4294967296ULL> LCG_BCPL;
    typedef LinearCongruential<30517578125ULL, 7261067085ULL, 0ULL, 34359738368ULL> LCG_BCSLIB;
    typedef LinearCongruential<69069ULL, 1ULL, 0ULL, 4294967296ULL> LCG_SUPERDUPER;
    typedef LinearCongruential<1220703125ULL, 1ULL, 0ULL, 34359738368ULL> LCG_APPLE; 

};


#endif // __LINEARCONGRUENTIAL_H_
