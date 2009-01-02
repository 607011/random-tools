// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/poker-fips140-1.h"


////////////////////////////////////////////////////////////
/////
///// POKER TEST
/////
////////////////////////////////////////////////////////////
void test_poker_fips(void)
{
    std::cout << "POKER TEST (à la FIPS 140-1)" << std::endl;
    std::vector<double> p;
    size_t passed = ctrandom::poker_test_fips140_1<size_t>(r, r_min, r_max, p);
    std::cout << " ... " << std::flush;
    if (passed == p.size())
    {
        std::cout << "OK.";
    }
    else
    {
        std::cout << "NICHT BESTANDEN. " << (p.size() - passed) << " von " << p.size() << " Blöcken (" << std::setprecision(3) << 100 * (float) (p.size() - passed) / p.size() << "%)" << std::endl
            << "     nicht im geforderten p-Wert-Bereich (1,03..57,4)";
    }
    std::cout << std::endl <<std::endl;
}
