// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __MERSENNETWISTER_H_
#define __MERSENNETWISTER_H_

#include "abstract_random_number_generator.h"

namespace ctrandom {

    ///////////////////////////////////////////////////////////////////////
    //
    // Mersenne Twister 19937
    //
    ///////////////////////////////////////////////////////////////////////

    class MersenneTwister : public RandomNumberGenerator<unsigned int>
    {
    public:
        MersenneTwister(void);
        unsigned int operator()();
        unsigned int next(void);
        void seed(unsigned int _Seed);

    private:
        static const int N = 624;
        static const int M = 397;
        static const unsigned int LO = 0x7fffffff;
        static const unsigned int HI = 0x80000000;
        unsigned int A[2];
        unsigned int y[N];
        int index;
    };




    ///////////////////////////////////////////////////////////////////////
    //
    // Mersenne Twister 800
    //
    ///////////////////////////////////////////////////////////////////////

    class MT800 : public RandomNumberGenerator<unsigned int>
    {
    public:
        MT800(void);
        unsigned int operator()();
        void seed(unsigned int _Seed);

    private:
        static const int N = 25;
        static const int M = 7;
        static const unsigned int HI = 0x80000000;
        unsigned int A[2];
        unsigned int y[N];
        int index;
    };

};


#endif //  __MERSENNETWISTER_H_
