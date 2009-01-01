// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/monobit.h"


////////////////////////////////////////////////////////////
/////
///// MONOBIT TEST
/////
////////////////////////////////////////////////////////////
void test_monobit(void)
{
#ifdef HAVE_BOOST
    std::cout << "MONOBIT TEST (à la NIST)" << std::endl;
    std::cout << " ... " << std::flush;
    double p = ctrandom::monobit_test_nist<size_t>(r, r_min, r_max);
    std::cout << "p = " << p << "  ";
    if (p > alpha)
    {
        std::cout << "OK.";
    }
    else
    {
        std::cout << "NICHT BESTANDEN.";
    }
    std::cout << std::endl <<std::endl;
#endif

    std::cout << "MONOBIT TEST (à la FIPS 140-1)" << std::endl;
    std::vector<size_t> counts;
    std::cout << " ... " << std::flush;
    size_t passed = ctrandom::monobit_test<size_t>(r, r_min, r_max, counts);
    if (passed == counts.size())
    {
        std::cout << "OK.";
    }
    else
    {
        std::cout << "NICHT BESTANDEN. " << (counts.size() - passed) << " von " << counts.size() << " Blöcken (" << std::setprecision(3) << 100 * (float) (counts.size() - passed) / counts.size() << "%)" << std::endl
            << "     enthält nicht die geforderte Anzahl Bits (9654..10346)";
    }
    std::cout << std::endl <<std::endl;
}
