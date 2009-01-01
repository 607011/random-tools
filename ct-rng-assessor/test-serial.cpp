// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include "tests.h"
#include "test/serial.h"


////////////////////////////////////////////////////////////
/////
///// SERIAL TEST
/////
////////////////////////////////////////////////////////////
void test_serial(void)
{
    std::cout << "SERIAL TEST" << std::endl;
    for (size_t groupsize = 2; groupsize <= 3; ++groupsize)
    {
        double p = ctrandom::serial_test<size_t>(r, r_min, r_max, groupsize);
        std::cout << " ... Gruppengröße = " << std::setw(1) << std::right << groupsize << ": " << std::flush
            << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
            << " " << (((alpha < p) && (p < (1-alpha)))? "OK" : "NICHT BESTANDEN") << '.' << std::endl;
    }
    std::cout << std::endl;
}
