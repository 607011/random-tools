// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <cassert>

#include "mcg.h"
#include "mersenne_twister.h"


namespace randomtools {

    ///////////////////////////////////////////////////////////////////////
    //
    // Mersenne Twister 19937
    //
    ///////////////////////////////////////////////////////////////////////

    MersenneTwister::MersenneTwister(void)
    {
        index = N + 1;
        A[0] = 0;
        A[1] = 0x9908b0df;
    }


    void MersenneTwister::seed(unsigned int _Seed)
    {
        MCG rng(_Seed);
        for (unsigned int i = 0; i < N; ++i)
            y[i] = rng();
        index = 0;
    }


    void MersenneTwister::warmup(int rounds)
    {
        assert(rounds > N);
        seed(makeSeed());
        for (int i = 0; i < rounds; ++i)
            (*this)();
    }


    unsigned int MersenneTwister::operator()()
    {
        if (index >= N)
        {
            if (index > N)
            {
                unsigned int r = 9;
                unsigned int s = 3402;
                for (int i = 0 ; i < N ; ++i)
                {
                    r = 509845221 * r + 3;
                    s *= s + 1;
                    y[i] = s + (r >> 10);
                }
            }
            unsigned int h;
            for (int k = 0 ; k < N-M ; ++k)
            {
                h = (y[k] & HI) | (y[k+1] & LO);
                y[k] = y[k+M] ^ (h >> 1) ^ A[h & 1];
            }
            for (int k = N-M ; k < N-1 ; ++k)
            {
                h = (y[k] & HI) | (y[k+1] & LO);
                y[k] = y[k+(M-N)] ^ (h >> 1) ^ A[h & 1];
            }
            h = (y[N-1] & HI) | (y[0] & LO);
            y[N-1] = y[M-1] ^ (h >> 1) ^ A[h & 1];
            index = 0;
        }

        unsigned int e = y[index++];
        e ^= (e >> 11);
        e ^= (e << 7) & 0x9d2c5680;
        e ^= (e << 15) & 0xefc60000;
        e ^= (e >> 18);
        return e;
    }


    ///////////////////////////////////////////////////////////////////////
    //
    // Mersenne Twister 800
    //
    ///////////////////////////////////////////////////////////////////////

    MT800::MT800(void)
    {
        index = N + 1;
        A[0] = 0;
        A[1] = 0x8ebfd028;
    }


    void MT800::seed(unsigned int _Seed)
    {
        (void) (_Seed);
        index = N + 1;
    }


    unsigned int MT800::operator()()
    {
        if (index >= N) {
            if (index > N) {
                unsigned int r = 9;
                unsigned int s = 3402;
                for (int i = 0 ; i < N ; ++i) {
                    r = 509845221 * r + 3;
                    s *= s + 1;
                    y[i] = s + (r >> 10);
                }
            }
            for (int k=0 ; k<N-M ; ++k)
                y[k] = y[k+M] ^ (y[k] >> 1) ^ A[y[k] & 1];
            for (int k=N-M ; k<N ; ++k)
                y[k] = y[k+(M-N)] ^ (y[k] >> 1) ^ A[y[k] & 1];
            index = 0;
        }
        unsigned int e = y[index++];
        e ^= (e << 7) & 0x2b5b2500;
        e ^= (e << 15) & 0xdb8b0000;
        e ^= (e >> 16);
        return e;
    }

};
