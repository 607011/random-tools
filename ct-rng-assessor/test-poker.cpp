// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"

#include "test/poker-knuth.h"


////////////////////////////////////////////////////////////
/////
///// POKER TEST
/////
////////////////////////////////////////////////////////////
void test_poker_knuth(void)
{
    std::cout << "POKER TEST (à la Knuth)" << std::endl;
    double p = ctrandom::poker_test_knuth<size_t>(r, r_min, r_max);
    std::cout << " ... " << std::flush
        << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
        << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.' << std::endl;
    std::cout << std::endl;
}
