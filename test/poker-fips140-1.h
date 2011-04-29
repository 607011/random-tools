// $Id: poker-fips140-1.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __POKERFIPS1401_H_
#define __POKERFIPS1401_H_

#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>

#include "bitvector.h"
#include "chisq.h"
#include "math_functions.h"

namespace randomtools {

    /// Poker Test à la FIPS 140-1.
    /// - Divide the 20000 bit stream into 5000 contiguous 4 bit segments.
    /// - Count and store the number of occurrences of each of the 16 possible 4 bit values.
    /// - Denote f(i) as the number of each 4 bit value i where 0 <= i <= 15.
    /// - Evaluate the following: X = (16/5000)·[ sum{i=0}{15}(f(i)^2) ] - 5000
    /// - The test is passed if 1.03 < X < 57.4
    /// @see http://csrc.nist.gov/publications/fips/fips1401.htm
    /// @param ran [in] Zufallszahlenfolge
    /// @param _min [in] kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max [in] größtmöglicher Wert in der Zufallszahlenfolge
    /// @param p [out] Histogramm: Anzahl der Läufer einer bestimmten Länge
    /// @return Anzahl der Tests, die bestanden wurden
    template <typename VariateType>
    int poker_test_fips140_1(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, int& testCount)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        const int ChunkSize = 20000;
        long range = 1L + ((long) _max - (long) _min);
        int bitsPerVariate = (int) (log((double) range) / log(2.0));
        int stepLen = ChunkSize / bitsPerVariate;
        int passedCount = 0;
        // TODO: Schleife parallelisieren
        for (int i = 0; i < (int)ran.size() - stepLen; i += stepLen)
        {
            BitVector chunk(ChunkSize);
            for (int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                for (int k = 0; k < bitsPerVariate; ++k)
                {
                    if ((r & 1) == 1)
                        chunk.set(j * bitsPerVariate + k);
                    r >>= 1;
                }
            }
            std::vector<int> histo(16, 0);
            for (int j = 0; j < ChunkSize - 4; j += 4)
            {
                int v = 0;
                for (int k = 0; k < 4; ++k)
                    v |= chunk.at(j + k) << k;
                ++histo[v];
            }
            double X = 0;
            for (int j = 0; j < 16; ++j)
                X += histo[j] * histo[j];
            X = (16/((double)ChunkSize/4)) * X - (double)ChunkSize/4;
            ++testCount;
            if ((1.03 < X) && (X < 57.4))
                ++passedCount;
        }
        return testCount - passedCount;
    }

};

#endif // __POKERFIPS1401_H_
