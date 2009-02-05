// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "ct-rng-assessor.h"
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
        double p = ctrandom::permutation_test<size_t>(r, t, r_min, r_max);
        if (!quiet)
            std::cout << " ... t = " << std::setw(1) << std::right << t << ": " << std::flush
                      << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                      << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN")
                      << '.' << std::endl;
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << p << "</td>";
    }
    if (!quiet)
        std::cout << std::endl;
}
