// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/run-fips140-1.h"


////////////////////////////////////////////////////////////
/////
///// RUN TEST (FIPS 140-1)
/////
////////////////////////////////////////////////////////////
void test_runs_fips(void)
{
    std::cout << "RUN TEST (à la FIPS 140-1)" << std::endl;
    std::vector<ctrandom::RunResult> runs;
    size_t longestRun0 = 0;
    size_t longestRun1 = 0;
    std::cout << " ... " << std::flush;
    size_t passed = ctrandom::run_test_fips<size_t>(r, r_min, r_max, runs, longestRun0, longestRun1);
    if (passed == runs.size())
    {
        std::cout << "OK.";
    }
    else
    {
        std::cout << "NICHT BESTANDEN. " << (runs.size() - passed) << " von "
                  << runs.size() << " Blöcken (" 
                  << std::setprecision(3) << 100 * (float) (runs.size() - passed) / runs.size()
                  << "%)" << std::endl
                  << "     außerhalb der geforderten Bedingungen.";
    }
    std::cout << std::endl << std::endl;
}
