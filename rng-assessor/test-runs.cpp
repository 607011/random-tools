// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "ct-rng-assessor.h"
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

    if (!quiet)
        std::cout << "RUN TEST" << std::endl
                  << " ... Serien oberhalb des Median : " << std::flush;
    p = ctrandom::run_above_test<size_t>(r, r_min, r_max);
    if (!quiet)
        std::cout << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN")
                  << '.' << std::endl;

    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";

    if (!quiet)
        std::cout << " ... Serien unterhalb des Median: " << std::flush;
    p = ctrandom::run_below_test<size_t>(r, r_min, r_max);
    if (!quiet)
        std::cout  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
        << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.' << std::endl;

    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";

    if (!quiet)
        std::cout << std::endl;
}
