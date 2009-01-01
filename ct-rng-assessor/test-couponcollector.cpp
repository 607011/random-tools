// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/couponcollector.h"


////////////////////////////////////////////////////////////
/////
///// COUPON COLLECTOR'S TEST
/////
////////////////////////////////////////////////////////////
void test_couponcollector(void)
{
    std::cout << "COUPON COLLECTOR'S TEST" << std::endl;
    for (size_t d = 2; d <= 16; ++d) 
    {
        double p = ctrandom::coupon_collector_test<size_t>(r, d, 20);
        std::cout << " ... " << std::flush
                  << "d = " << std::setw(2) << std::right << d << ": " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN")
                  << '.' << std::endl;
    }
    std::cout << std::endl;
}
