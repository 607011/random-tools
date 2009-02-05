// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
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

namespace ctrandom {

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
    size_t poker_test_fips140_1(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, std::vector<double>& p)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        const size_t ChunkSize = 20000;
        VariateType range = _max - _min;
        unsigned int bitsPerVariate = (unsigned int) (log((double) range) / log(2.0));
        unsigned int stepLen = ChunkSize / bitsPerVariate;
        size_t passedCount = 0;
        for (unsigned int i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            BitVector chunk(ChunkSize);
            for (unsigned int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                for (unsigned int k = 0; k < bitsPerVariate; ++k)
                {
                    if ((r & 1) == 1)
                        chunk.set(j * bitsPerVariate + k);
                    r >>= 1;
                }
            }
            std::vector<size_t> histo(16, 0);
            for (unsigned int j = 0; j < ChunkSize - 4; j += 4)
            {
                int v = 0;
                for (unsigned int k = 0; k < 4; ++k)
                    v |= chunk.at(j + k) << k;
                ++histo[v];
            }
            double X = 0;
            for (size_t j = 0; j < 16; ++j)
                X += histo[j] * histo[j];
            X = (16/((double)ChunkSize/4)) * X - (double)ChunkSize/4;
            p.push_back(X);
            if ((1.03 < X) && (X < 57.4))
                ++passedCount;
        }
        return passedCount;
    }

};

#endif // __POKERFIPS1401_H_
