// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/permutation.h"


////////////////////////////////////////////////////////////
/////
///// PERMUTATION TEST
/////
////////////////////////////////////////////////////////////
void test_permutation(void)
{
    if (!quiet)
        std::cout << "PERMUTATION TEST" << std::endl;
    for (size_t t = 3; t <= 5; ++t) 
    {
        double p = randomtools::permutation_test<variate_t>(r, t, r_min, r_max);
        if (!quiet)
            std::cout << " ... t = " << std::setw(1) << std::right << t << ": " << std::flush
                      << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                      << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN")
                      << '.' << std::endl;
    }
    if (!quiet)
        std::cout << std::endl;
}
