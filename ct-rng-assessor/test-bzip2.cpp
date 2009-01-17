// $Id$
// Copyright (c) 2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <set>
#include <cstdlib>
#include <cmath>

#include "ct-rng-assessor.h"
#include "tests.h"
#include "bzip2-1.0.5/bzlib.h"


////////////////////////////////////////////////////////////
/////
///// COMPRESSION TEST
/////
////////////////////////////////////////////////////////////
void test_compression(void)
{
    if (!quiet)
        std::cout << "COMPRESSION TEST (1 MByte blocks)" << std::endl;
    const size_t BufSize = 1024*1024;
    char* srcBuf = new char[BufSize];
    unsigned int dstBufSize = (unsigned int) (1.1*(float)BufSize+600);
    char* dstBuf = new char[dstBufSize];
    std::vector<size_t>::const_iterator rp = r.begin();
    for (size_t i = 0; i < r.size(); i += BufSize)
    {
        if (!quiet)
            std::cout << " ... " << std::flush;
        size_t j = 0;
        while (j < BufSize && rp != r.end())
            srcBuf[j++] = *(rp++);
        unsigned int compressedSize = dstBufSize;
        BZ2_bzBuffToBuffCompress(dstBuf, &compressedSize, srcBuf, j, 9, 0, 0);
        float pct = 100.0f * compressedSize / (float) j;
        if (!quiet)
        {
            std::cout << std::setw(10) << std::setprecision(4) << std::right << pct << "%    ";
            if (pct > 99.999f)
            {
                std::cout << "OK.";
            }
            else
            {
                std::cout << "NICHT BESTANDEN.";
            }
            std::cout<< std::endl;
        }
        if (htmlReport)
            std::cout << "<td>" << std::setprecision(std::numeric_limits<double>::digits10)
                      << pct << "</td>";
    }
    if (!quiet)
        std::cout << std::endl <<std::endl;
    delete [] dstBuf;
    delete [] srcBuf;
}
