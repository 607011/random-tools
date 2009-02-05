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
        std::cout << "RUN TEST (à la FIPS 140-1)" << std::endl;
    std::vector<ctrandom::RunResult> runs;
    size_t longestRun0 = 0;
    size_t longestRun1 = 0;
    if (!quiet)
        std::cout << " ... " << std::flush;
    size_t passed = ctrandom::run_test_fips<size_t>(r, r_min, r_max, runs, longestRun0, longestRun1);
    if (!quiet)
    {
        if (passed == runs.size())
        {
            std::cout << "OK.";
        }
        else
        {
            std::cout << "NICHT BESTANDEN. " << (runs.size() - passed) << " von "
                      << runs.size() << " Blöcken (" 
                      << std::setprecision(3)
                      << 100 * (float) (runs.size() - passed) / runs.size()
                      << "%)" << std::endl
                      << "     außerhalb der geforderten Bedingungen.";
        }
        std::cout << std::endl << std::endl;
    }
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<float>::digits10)
                  << 100.0f * (float) (runs.size() - passed) / runs.size() << "</td>";
}
