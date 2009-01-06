// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "ct-rng-assessor.h"
#include "test/entropy.h"

////////////////////////////////////////////////////////////
/////
///// ENTROPY TEST
/////
////////////////////////////////////////////////////////////
void test_entropy(void)
{
    if (!quiet)
        std::cout << "ENTROPIE" << std::endl
                  << " ... " << std::flush;
    double entropy = ctrandom::entropy_test<size_t>(r, r_min, r_max);
    if (!quiet)
        std::cout << std::setprecision(std::numeric_limits<double>::digits10)
                  << (8*entropy) << " Bits pro Byte" << std::endl
                  << std::endl << std::endl;
    if (htmlReport)
        std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                  << (8*entropy) << "</td>";
}
