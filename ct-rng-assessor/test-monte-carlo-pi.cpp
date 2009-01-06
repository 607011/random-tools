// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "ct-rng-assessor.h"

#include "tests.h"
#include "test/pi.h"

////////////////////////////////////////////////////////////
/////
///// MONTE-CARLO-PI TEST
/////
////////////////////////////////////////////////////////////
void test_pi(void)
{
    if (!quiet)
        std::cout << "MONTE CARLO PI SIMULATION" << std::endl;
    size_t n_max = std::numeric_limits<double>::digits / 8;
    for (size_t n = 1; n <= n_max; ++n)
    {
        if (!quiet)
            std::cout << " ... n = " << n << ": " << std::flush;
        double pi = ctrandom::pi_test<size_t>(r, r_min, r_max, n);
        double absErr = fabs(M_PI-pi);
        if (!quiet)
            std::cout << "Pi = " << std::setprecision(10) << std::setw(12) << std::left << pi
                      << " (Abweichung = " << std::setprecision(3) << std::setw(5)
                      << (100.0*absErr/M_PI) << "%) " << std::endl;
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << pi << "</td>";
    }
    if (!quiet)
        std::cout << std::endl << std::endl;
}
