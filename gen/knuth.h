// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __KNUTH_RAND_H_
#define __KNUTH_RAND_H_

#include <cstdlib>
#include "abstract_random_number_generator.h"

#include "lcg.h"

namespace ctrandom {

    template <size_t _M, size_t _N>
    class KnuthAdditive : public RandomNumberGenerator<unsigned int>
    {
    public:
        KnuthAdditive(void)
        {
            j = _M - 1;
            k = _N - 1;
            seed(19937);
        }
        unsigned int operator()()
        {
            _R[k] = _R[j] + _R[k];
            unsigned int r = _R[k]& 0xffffffff;
            if (--j < 0)
                j = _M - 1;
            if (--k < 0)
                k = _N - 1;
            return r;
        }
        inline void seed(unsigned long long _Seed)
        {
            LCG_HOAGLIN1 gen(_Seed);
            for (size_t i = 0; i < _N; ++i)
                _R[i] = (unsigned long long) gen() *  (unsigned long long) gen();
        }

    private:
        unsigned long long _R[_N];
        int j;
        int k;
    };

    typedef KnuthAdditive<24, 55>         KnuthRand1;
    typedef KnuthAdditive<30, 127>        KnuthRand2;
    typedef KnuthAdditive<37, 100>        KnuthRand3;
    typedef KnuthAdditive<38, 89>         KnuthRand4;
    typedef KnuthAdditive<83, 258>        KnuthRand5;
    typedef KnuthAdditive<273, 607>       KnuthRand6;
    typedef KnuthAdditive<576, 3217>      KnuthRand7;
    typedef KnuthAdditive<1029, 2281>     KnuthRand8;
    typedef KnuthAdditive<4187, 9689>     KnuthRand9;
    typedef KnuthAdditive<7083, 19937>    KnuthRand10;
    typedef KnuthAdditive<9739, 232097>   KnuthRand11;
};


#endif // __KNUTH_RAND_H_
