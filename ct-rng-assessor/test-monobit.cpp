// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ct-rng-assessor.h"
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
        std::cout << "MONOBIT TEST (� la NIST)" << std::endl
                  << " ... " << std::flush;
    double p = ctrandom::monobit_test_nist<size_t>(r, r_min, r_max);
    if (!quiet) {
        std::cout << "p = " << p << "  ";
        if (p > alpha) {
            std::cout << "OK.";
        }
        else {
            std::cout << "NICHT BESTANDEN.";
        }
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << ((p > alpha)? "OK" : "FAIL") << "</td>";
        std::cout << std::endl;
    }
#endif

    if (!quiet)
        std::cout << "MONOBIT TEST (� la FIPS 140-1)" << std::endl;
    std::vector<size_t> counts;
    if (!quiet)
        std::cout << " ... " << std::flush;
    size_t passed = ctrandom::monobit_test<size_t>(r, r_min, r_max, counts);
    if (!quiet) {
        double pctFailed = 100.0 * (double) (counts.size() - passed) / (double) counts.size();
        if (passed == counts.size()) {
            std::cout << "OK.";
        }
        else {
            std::cout << "NICHT BESTANDEN. " << (counts.size() - passed)
                      << " von " << counts.size() << " Bl�cken ("
                      << std::setprecision(3) << pctFailed << "%)" << std::endl
                      << "     enth�lt nicht die geforderte Anzahl Bits (9654..10346)";
        }
        if (htmlReport)
            std::cout << "<td>" << pctFailed << "</td>";
        std::cout << std::endl <<std::endl;
    }


}
