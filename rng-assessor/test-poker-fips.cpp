// $Id$
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/poker-fips140-1.h"


////////////////////////////////////////////////////////////
/////
///// POKER TEST
/////
////////////////////////////////////////////////////////////
void test_poker_fips(void)
{
    if (!quiet)
        std::cout << "POKER TEST (a la FIPS 140-1)" << std::endl;
    std::vector<double> p;
    if (!quiet)
        std::cout << " ... " << std::flush;
    size_t passed = ctrandom::poker_test_fips140_1<variate_t>(r, r_min, r_max, p);
    if (!quiet)
    {
        if (passed == p.size())
        {
            std::cout << "OK.";
        }
        else
        {
            size_t notPassed = p.size() - passed;
            std::cout << "NICHT BESTANDEN. " << notPassed << " von " << p.size()
                << " Bloecken (" << std::setprecision(3) << 100 * (float) (p.size() - passed) / p.size() << "%)" << std::endl
                << "     nicht im geforderten p-Wert-Bereich (1,03..57,4)";
        }
        std::cout << std::endl <<std::endl;
    }
}
