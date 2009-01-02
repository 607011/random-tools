// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/run-above-below.h"
#include "test/run-up-down.h"


////////////////////////////////////////////////////////////
/////
///// RUN TEST
/////
////////////////////////////////////////////////////////////
void test_runs(void)
{
    double p;
    std::cout << "RUN TEST" << std::endl;
    std::cout << " ... Serien oberhalb des Median : " << std::flush;
    p = ctrandom::run_above_test<size_t>(r, r_min, r_max);
    std::cout << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
        << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.' << std::endl;
    std::cout << " ... Serien unterhalb des Median: " << std::flush;
    p = ctrandom::run_below_test<size_t>(r, r_min, r_max);
    std::cout  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
        << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.' << std::endl;
    std::cout << std::endl;
}
