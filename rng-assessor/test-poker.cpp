// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/poker-knuth.h"

////////////////////////////////////////////////////////////
/////
///// POKER TEST
/////
////////////////////////////////////////////////////////////
void test_poker_knuth(void)
{
    if (!quiet)
        std::cout << "POKER TEST ( la Knuth)" << std::endl;
    double p = ctrandom::poker_test_knuth<size_t>(r, r_min, r_max);
    if (!quiet)
        std::cout << " ... " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN")
                  << '.' << std::endl;
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << std::endl;
}
