// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/gap.h"


////////////////////////////////////////////////////////////
/////
///// GAP TEST
/////
////////////////////////////////////////////////////////////
void test_gap(void)
{
    if (!quiet)
        std::cout << "GAP TEST" << std::endl;
    double p;
    p = ctrandom::gap_test<size_t>(r, 7,   0, 128, r_min, r_max);
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << " ... untere Hälfte  : " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;

    p = ctrandom::gap_test<size_t>(r, 7, 128, 256, r_min, r_max);
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << " ... obere Hälfte   : " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;

    p = ctrandom::gap_test<size_t>(r, 7,   0,  64, r_min, r_max);
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << " ... erstes Viertel : " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;
    
    p = ctrandom::gap_test<size_t>(r, 7,  64, 128, r_min, r_max);
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << " ... zweites Viertel: " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;
    
    p = ctrandom::gap_test<size_t>(r, 7, 128, 192, r_min, r_max);
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << " ... drittes Viertel: " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;

    p = ctrandom::gap_test<size_t>(r, 7, 192, 256, r_min, r_max);
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << p << "</td>";
    if (!quiet)
        std::cout << " ... viertes Viertel: " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl
                  << std::endl;
}
