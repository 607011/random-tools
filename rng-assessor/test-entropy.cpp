// $Id$
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/entropy.h"

////////////////////////////////////////////////////////////
/////
///// ENTROPY TEST
/////
////////////////////////////////////////////////////////////
void test_entropy(void)
{
#ifdef TESTTIMING
    TIMER stopwatch;
    LONGLONG duration;
#endif // TESTTIMING
    if (!quiet)
        std::cout << "ENTROPIE" << std::endl
        << " ... " << std::flush;

    START();
    double entropy = randomtools::entropy_test<variate_t>(r, r_min, r_max);
    STOP(duration);

    if (!quiet) 
    {
        std::cout << std::setprecision(std::numeric_limits<double>::digits10)
            << (8*entropy) << " Bits pro Byte" << std::endl;
#ifdef TESTTIMING
        if (timeIt)
            std::cout << " Laufzeit: " << duration << "ms" << std::endl;
#endif // TESTTIMING
        std::cout << std::endl;
    }
}
