// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <set>
#include <limits>
#include <cstdlib>
#include <cmath>

#include "tests.h"
#include "test/pi.h"

////////////////////////////////////////////////////////////
/////
///// MONTE-CARLO-PI TEST
/////
////////////////////////////////////////////////////////////
void test_pi(void)
{
    std::cout << "MONTE CARLO PI SIMULATION" << std::endl;
    size_t n_max = std::numeric_limits<double>::digits / 8;
    for (size_t n = 1; n <= n_max; ++n)
    {
        std::cout << " ... n = " << n << ": " << std::flush;
        double pi = ctrandom::pi_test<size_t>(r, r_min, r_max, n);
        double absErr = fabs(M_PI-pi);
        std::cout << "Pi = " << std::setprecision(10) << std::setw(12) << std::left << pi
            << " (Abweichung = " << std::setprecision(3) << std::setw(5) << 100.0*absErr/M_PI << "%) "
            << std::endl;
    }
    std::cout << std::endl <<std::endl;
}
