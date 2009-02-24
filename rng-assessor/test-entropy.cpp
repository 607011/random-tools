// $Id$
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/entropy.h"

////////////////////////////////////////////////////////////
/////
///// ENTROPY TEST
/////
////////////////////////////////////////////////////////////
void test_entropy(void)
{
    if (!quiet)
        std::cout << "ENTROPIE" << std::endl
                  << " ... " << std::flush;
    double entropy = ctrandom::entropy_test<variate_t>(r, r_min, r_max);
    if (!quiet)
        std::cout << std::setprecision(std::numeric_limits<double>::digits10)
                  << (8*entropy) << " Bits pro Byte" << std::endl
                  << std::endl;
}
