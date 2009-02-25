// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _WIN32
#include "../config-win32.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rng-assessor.h"
#include "test/monobit.h"


////////////////////////////////////////////////////////////
/////
///// MONOBIT TEST
/////
////////////////////////////////////////////////////////////
void test_monobit(void)
{
#ifdef HAVE_BOOST
    if (!quiet)
        std::cout << "MONOBIT TEST (a la NIST)" << std::endl
                  << " ... " << std::flush;
    double p = randomtools::monobit_test_nist<variate_t>(r, r_min, r_max);
    if (!quiet)
    {
        std::cout << "p = " << p << "  ";
        if (p > alpha)
            std::cout << "OK.";
        else
            std::cout << "NICHT BESTANDEN.";
        std::cout << std::endl << std::endl;
    }
#endif

    if (!quiet)
        std::cout << "MONOBIT TEST (a la FIPS 140-1)" << std::endl;
    int counts = 0;
    if (!quiet)
        std::cout << " ... " << std::flush;
    int notPassed = randomtools::monobit_test<variate_t>(r, r_min, r_max, counts);
    if (!quiet) 
    {
        if (notPassed == 0)
        {
            std::cout << "OK.";
        }
        else
        {
            double pctFailed = 100 * (float)notPassed / (float)counts;
            std::cout << "NICHT BESTANDEN. " << notPassed
                      << " von " << counts << " Blöcken ("
                      << std::setprecision(3) << pctFailed << "%)" << std::endl
                      << "     " << ((notPassed == 1)? "enthält" : "enthalten")
                      << " nicht die geforderte Anzahl Bits (9654..10346)";
        }
        std::cout << std::endl << std::endl;
    }


}
