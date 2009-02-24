// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <set>
#include <cstdlib>
#include <cmath>

#include "rng-assessor.h"
#include "tests.h"
#include "test/autocorr.h"


////////////////////////////////////////////////////////////
/////
///// AUTOCORRELATION TEST
/////
////////////////////////////////////////////////////////////
void test_autocorrelation(void)
{
    std::vector<size_t> counts;
    if (!quiet)
        std::cout << "AUTOCORRELATION TEST" << std::endl << " ... " << std::flush;
    size_t passed = ctrandom::autocorrelation_test<variate_t>(r, r_min, r_max, counts);
    if (passed == counts.size())
    {
        if (!quiet)
            std::cout << "OK.";
    }
    else
    {
        size_t notPassed = counts.size() - passed;
        if (!quiet)
            std::cout << "NICHT BESTANDEN. " << notPassed << " von " << counts.size()
                << " Blöcken (" << std::setprecision(3) << 100 * (float) (counts.size() - passed) / counts.size() << "%)" << std::endl
                << "     " << ((notPassed == 1)? "enthält" : "enthalten")
                << " nicht die geforderte Anzahl Bits (2326..2674)";
    }
    if (!quiet)
        std::cout << std::endl <<std::endl;
}
