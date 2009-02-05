// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "rng-assessor.h"
#include "test/serial.h"

////////////////////////////////////////////////////////////
/////
///// SERIAL TEST
/////
////////////////////////////////////////////////////////////
void test_serial(void)
{
    if (!quiet)
        std::cout << "SERIAL TEST" << std::endl;
    for (size_t groupsize = 2; groupsize <= 3; ++groupsize)
    {
        double p = ctrandom::serial_test<size_t>(r, r_min, r_max, groupsize);
        if (!quiet)
            std::cout << " ... " << std::flush;
        if (!quiet)
            std::cout << "Gruppengröße = " << std::setw(1) << std::right << groupsize << ": "
                      << std::flush
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
