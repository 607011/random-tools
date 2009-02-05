// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __BLUM_BLUM_SHUB_H_
#define __BLUM_BLUM_SHUB_H_

#include "abstract_random_number_generator.h"

#include <cstdlib>
#include <gmp.h>

namespace ctrandom {

    typedef unsigned int BBSVariateType;

    class BlumBlumShub : public RandomNumberGenerator<BBSVariateType>
    {
    public:
        BlumBlumShub(size_t keyLength = 1024);
        ~BlumBlumShub()
        {
            mpz_clear(_Blum);
            mpz_clear(_R);
        }
        BBSVariateType operator()();

    private:
        size_t _KeyLen;
        mpz_t _Blum;
        mpz_t _R;
    };


};

#endif // __BLUM_BLUM_SHUB_H_
