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
        std::cout << "MONOBIT TEST (à la NIST)" << std::endl
                  << " ... " << std::flush;
    double p = ctrandom::monobit_test_nist<size_t>(r, r_min, r_max);
    if (!quiet)
    {
        std::cout << "p = " << p << "  ";
        if (p > alpha)
        {
            std::cout << "OK.";
        }
        else
        {
            std::cout << "NICHT BESTANDEN.";
        }
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << ((p > alpha)? "OK" : "FAIL") << "</td>";
        std::cout << std::endl << std::endl;
    }
#endif

    if (!quiet)
        std::cout << "MONOBIT TEST (à la FIPS 140-1)" << std::endl;
    std::vector<size_t> counts;
    if (!quiet)
        std::cout << " ... " << std::flush;
    size_t passed = ctrandom::monobit_test<size_t>(r, r_min, r_max, counts);
    if (!quiet) 
    {
        double pctFailed = 100.0 * (double) (counts.size() - passed) / (double) counts.size();
        if (passed == counts.size())
        {
            std::cout << "OK.";
        }
        else
        {
            size_t notPassed = counts.size() - passed;
            std::cout << "NICHT BESTANDEN. " << notPassed
                      << " von " << counts.size() << " Blöcken ("
                      << std::setprecision(3) << pctFailed << "%)" << std::endl
                      << "     " << ((notPassed == 1)? "enthält" : "enthalten")
                      << " nicht die geforderte Anzahl Bits (9654..10346)";
        }
        if (htmlReport)
            std::cout << "<td>" << pctFailed << "</td>";
        std::cout << std::endl << std::endl;
    }


}
