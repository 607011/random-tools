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
    int testCount = 0;
    if (!quiet)
        std::cout << " ... " << std::flush;
    int notPassed = randomtools::poker_test_fips140_1<variate_t>(r, r_min, r_max, testCount);
    if (!quiet)
    {
        if (notPassed == 0)
        {
            std::cout << "OK.";
        }
        else
        {
            std::cout << "NICHT BESTANDEN. " << notPassed << " von " << testCount
                << " Bloecken (" << std::setprecision(3) << 100 * (float)notPassed / testCount << "%)" << std::endl
                << "     nicht im geforderten p-Wert-Bereich (1,03..57,4)";
        }
        std::cout << std::endl <<std::endl;
    }
}
