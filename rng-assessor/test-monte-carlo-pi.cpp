// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"

#include "tests.h"
#include "test/pi.h"

////////////////////////////////////////////////////////////
/////
///// MONTE-CARLO-PI TEST
/////
////////////////////////////////////////////////////////////
void test_pi(void)
{
#ifdef TESTTIMING
    TIMER stopwatch;
    LONGLONG duration;
#endif // TESTTIMING

    if (!quiet)
        std::cout << "MONTE CARLO PI SIMULATION" << std::endl;
    int n_max = std::numeric_limits<double>::digits / 8;
    for (int n = 1; n <= n_max; ++n)
    {
        if (!quiet)
            std::cout << " ... n = " << n << ": " << std::flush;
        START();
        double pi = randomtools::pi_test<variate_t>(r, r_min, r_max, n);
        STOP(duration)
        double absErr = fabs(M_PI-pi);
        if (!quiet)
            std::cout << "Pi = " << std::setprecision(10) << std::setw(12) << std::left << pi
                      << " (Abweichung = " << std::setprecision(3) << std::setw(5)
                      << (100.0*absErr/M_PI) << "%) " << std::endl;
#ifdef TESTTIMING
        if (timeIt)
            std::cout << " Laufzeit: " << duration << " ms" << std::endl;
#endif // TESTTIMING
    }
    if (!quiet)
        std::cout << std::endl;
}
