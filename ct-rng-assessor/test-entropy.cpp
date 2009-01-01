// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>

#include "tests.h"
#include "test/entropy.h"


////////////////////////////////////////////////////////////
/////
///// ENTROPY TEST
/////
////////////////////////////////////////////////////////////
void test_entropy(void)
{
    std::cout << "ENTROPIE" << std::endl;
    std::cout << " ... " << std::flush;
    double entropy = ctrandom::entropy_test<size_t>(r, r_min, r_max);
    std::cout << std::setprecision(std::numeric_limits<double>::digits10) << (8*entropy) << " Bits pro Byte"
        << std::endl;
    std::cout << std::endl <<std::endl;
}
