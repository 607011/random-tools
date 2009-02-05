// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/max-t.h"


////////////////////////////////////////////////////////////
/////
///// MAX-OF-T TEST
/////
////////////////////////////////////////////////////////////
void test_max_of_t(void)
{
    if (!quiet)
        std::cout << "MAXIMUM-OF-T TEST" << std::endl;
    size_t _T[] = { 2, 3, 4, 5, 7, 10, 11, 13, 16, 24, 0 };
    for (size_t i = 0; _T[i] > 0; ++i)
    {
        size_t t = _T[i];
        double p = ctrandom::max_t_test<size_t>(r, t, r_min, r_max);
        if (!quiet)
            std::cout << " ... t = " << std::setw(2) << std::right << t << ": " << std::flush
                      << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                      << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN")
                      << '.' << std::endl;
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << p << "</td>";
    }
    if (!quiet)
        std::cout << std::endl;
}
