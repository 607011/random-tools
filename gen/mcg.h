// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MULTIPLICATIVECONGRUENTIAL_H_
#define __MULTIPLICATIVECONGRUENTIAL_H_

#include "abstract_random_number_generator.h"

namespace randomtools {

    template <unsigned long long R0>
    class MultiplicativeCongruential : public RandomNumberGenerator<unsigned char>
    {
    public:
        MultiplicativeCongruential(unsigned long long seed = R0) : _R(seed)
        { }
        unsigned char operator()()
        {
            _R = (_R * 16807) & 0x7fffffffULL;
            return (unsigned char) ((_R >> 11) & 0xff);
        }
        inline void seed(unsigned long long _Seed) { _R = _Seed; }
        inline unsigned char max(void) { return 0xff; }

    private:
        unsigned long long _R;
    };

    typedef MultiplicativeCongruential<1> MCG;
};


#endif // __MULTIPLICATIVECONGRUENTIAL_H_
