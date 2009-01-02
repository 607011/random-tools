// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/permutation.h"


////////////////////////////////////////////////////////////
/////
///// PERMUTATION TEST
/////
////////////////////////////////////////////////////////////
void test_permutation(void)
{
    std::cout << "PERMUTATION TEST" << std::endl;
    for (size_t t = 3; t <= 5; ++t) 
    {
        double p = ctrandom::permutation_test<size_t>(r, t, r_min, r_max);
        std::cout << " ... t = " << std::setw(1) << std::right << t << ": " << std::flush
            << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
            << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.' << std::endl;
    }
    std::cout << std::endl;
}
