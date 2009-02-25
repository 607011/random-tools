// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __CIRC_H_
#define __CIRC_H_

#include "abstract_random_number_generator.h"


namespace randomtools {

    template <unsigned int M, unsigned int X0>
    class Circular : public RandomNumberGenerator<unsigned int>
    {
    public:
        Circular(unsigned int m = M, unsigned int seed = X0)
            : _M(m), _R(seed)
        { }
        unsigned int operator()() { return _R++ % _M; }
        inline void seed(unsigned int _Seed) { _R = _Seed; }
        inline unsigned int max(void) { return _M; }

    private:
        unsigned int _M;
        unsigned int _R;
    };

    typedef Circular<256U, 0U> CircularBytes;

};


#endif // __CIRC_H_
