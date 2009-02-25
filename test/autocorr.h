// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __AUTOCORR_H_
#define __AUTOCORR_H_

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

#include "math_functions.h"


namespace randomtools {

    /// Autokorrelationstest nach AIS31/AIS20.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @param counts [out] Anzahl der durchgeführten Tests
    /// @return Anzahl der bestandenen Tests
    template <typename VariateType>
    int autocorrelation_test(const std::vector<VariateType>& ran, size_t _min, size_t _max, int& testCount)
    {
        assert(_max > _min);
        const long range = 1 + (size_t) ((long) _max - (long) _min);
        const int bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
        assert((int) ran.size() > 5000 / bitsPerVariate);
        const int stepLen = 5000 / bitsPerVariate;
        int passedCount = 0;
        int _testCount = 0;
#pragma omp parallel for reduction(+:_testCount, passedCount)
        for (int i = 0; i < (int) (ran.size() - stepLen); i += stepLen)
        {
            int sum = 0;
            for (int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j);
                for (int k = 0; k < bitsPerVariate; ++k)
                {
                    sum += ((r & 1) ^ ((r >> 1) & 1));
                    r >>= 1;
                }
            }
            ++_testCount;
            if ((2326 < sum) && (sum < 2674))
                ++passedCount;
        }
        testCount = _testCount;
        return testCount - passedCount;
    };

};

#endif // __AUTOCORR_H_
