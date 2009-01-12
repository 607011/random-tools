// $Id$
// Copyright (c) 2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <set>
#include <cstdlib>
#include <cmath>

#include "tests.h"
#include "test/autocorr.h"



////////////////////////////////////////////////////////////
/////
///// AUTOCORRELATION TEST
/////
////////////////////////////////////////////////////////////
void test_autocorrelation(void)
{
    std::cout << "AUTOCORRELATION TEST" << std::endl;
    std::vector<size_t> counts;
    std::cout << " ... " << std::flush;
    size_t passed = ctrandom::autocorrelation_test<size_t>(r, r_min, r_max, counts);
    if (passed == counts.size())
    {
        std::cout << "OK.";
    }
    else
    {
        std::cout << "NICHT BESTANDEN. " << (counts.size() - passed) << " von " << counts.size() << " Blöcken (" << std::setprecision(3) << 100 * (float) (counts.size() - passed) / counts.size() << "%)" << std::endl
            << "     enthält nicht die geforderte Anzahl Bits (2326..2674)";
    }
    std::cout << std::endl <<std::endl;

}
