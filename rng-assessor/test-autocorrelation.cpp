// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <set>
#include <cstdlib>
#include <cmath>

#include "rng-assessor.h"
#include "test/autocorr.h"


////////////////////////////////////////////////////////////
/////
///// AUTOCORRELATION TEST
/////
////////////////////////////////////////////////////////////
void test_autocorrelation(void)
{
#ifdef TESTTIMING
    TIMER stopwatch;
    LONGLONG duration;
#endif // TESTTIMING
    int counts;
    if (!quiet)
        std::cout << "AUTOCORRELATION TEST" << std::endl << " ... " << std::flush;

    START();
    int notPassed = randomtools::autocorrelation_test<variate_t>(r, r_min, r_max, counts);
    STOP(duration);

    if (!quiet)
    {
        if (notPassed == 0)
        {
            std::cout << "OK." << std::endl;
        }
        else
        {
            std::cout << "NICHT BESTANDEN. " << notPassed << " von " << counts
                << " Blöcken (" << std::setprecision(3) << 100 * (float)notPassed / counts << "%)" << std::endl
                << "     " << ((notPassed == 1)? "enthaelt" : "enthalten")
                << " nicht die geforderte Anzahl Bits (2326..2674)" << std::endl;
        }
#ifdef TESTTIMING
        if (timeIt)
            std::cout << " Laufzeit: " << duration << " ms" << std::endl;
#endif // TESTTIMING
        std::cout << std::endl;
    }
}
