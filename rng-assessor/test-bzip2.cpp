// $Id$
// Copyright (c) 2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "rng-assessor.h"
#include "tests.h"
#include "bzip2-1.0.5/bzlib.h"


////////////////////////////////////////////////////////////
/////
///// COMPRESSION TEST
/////
////////////////////////////////////////////////////////////
void test_bzip2(void)
{
    if (!quiet)
        std::cout << "BZIP2 COMPRESSION TEST (1 MByte blocks)" << std::endl;
    const size_t BufSize = 1048576;
    const size_t DstBufSize = 1154034; // 1.1*BufSize+600
    char* srcBuf = new char[BufSize];
    char* dstBuf = new char[DstBufSize];
    RNGArray::const_iterator rp = r.begin();
    for (size_t i = 0; i < r.size(); i += BufSize)
    {
        if (!quiet)
            std::cout << " ... " << std::flush;
        size_t j = 0;
        while (j < BufSize && rp != r.end())
            srcBuf[j++] = *(rp++);
        unsigned int compressedSize = (unsigned int) DstBufSize;
        BZ2_bzBuffToBuffCompress(dstBuf, &compressedSize, srcBuf, j, 9, 0, 0);
        float pct = 100.0f * compressedSize / (float) j;
        if (!quiet)
            std::cout << std::setw(10) << std::setprecision(4) << std::right << pct << "% "
                      << ((pct > 100.299f)? "OK." : "NICHT BESTANDEN.") << std::endl;
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << pct << "</td>";
    }
    if (!quiet)
        std::cout << std::endl;
    delete [] dstBuf;
    delete [] srcBuf;
}
