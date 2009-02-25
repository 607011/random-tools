// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/run-fips140-1.h"


////////////////////////////////////////////////////////////
/////
///// RUN TEST (FIPS 140-1)
/////
////////////////////////////////////////////////////////////
void test_runs_fips(void)
{
    if (!quiet)
        std::cout << "RUN TEST (a la FIPS 140-1)" << std::endl;
    int runs = 0;
    int longestRun0 = 0;
    int longestRun1 = 0;
    if (!quiet)
        std::cout << " ... " << std::flush;
    size_t notPassed = randomtools::run_test_fips<variate_t>(r, r_min, r_max, runs, longestRun0, longestRun1);
    if (!quiet)
    {
        if (notPassed == 0)
        {
            std::cout << "OK.";
        }
        else
        {
            std::cout << "NICHT BESTANDEN. " << notPassed << " von "
                      << runs << " Bloecken (" 
                      << std::setprecision(3)
                      << 100 * (float)notPassed / runs
                      << "%)" << std::endl
                      << "     ausserhalb der geforderten Bedingungen.";
        }
        std::cout << std::endl << std::endl;
    }
}
