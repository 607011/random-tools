// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
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
    p = ctrandom::gap_test<variate_t>(r, 7,   0, 127, r_min, r_max);
    if (!quiet)
        std::cout << " ... untere Haelfte : " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;

    p = ctrandom::gap_test<variate_t>(r, 7, 128, 255, r_min, r_max);
    if (!quiet)
        std::cout << " ... obere Haelfte  : " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;

    p = ctrandom::gap_test<variate_t>(r, 7,   0,  63, r_min, r_max);
    if (!quiet)
        std::cout << " ... erstes Viertel : " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;
    
    p = ctrandom::gap_test<variate_t>(r, 7,  64, 127, r_min, r_max);
    if (!quiet)
        std::cout << " ... zweites Viertel: " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;
    
    p = ctrandom::gap_test<variate_t>(r, 7, 128, 191, r_min, r_max);
    if (!quiet)
        std::cout << " ... drittes Viertel: " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl;

    p = ctrandom::gap_test<variate_t>(r, 7, 192, 255, r_min, r_max);
    if (!quiet)
        std::cout << " ... viertes Viertel: " << std::flush
                  << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                  << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.'
                  << std::endl
                  << std::endl;
}
