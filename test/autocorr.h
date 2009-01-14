// $Id$
// Copyright (c) 2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __AUTOCORR_H_
#define __AUTOCORR_H_

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

#include "math_functions.h"


namespace ctrandom {

    /// Autokorrelationstest nach AIS31/AIS20.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @param [out] Histogramm
    /// @return Anzahl der bestandenen Tests
    template <typename VariateType>
    size_t autocorrelation_test(const std::vector<VariateType>& ran, size_t _min, size_t _max, std::vector<size_t>& counts)
    {
        assert(_max > _min);
        const VariateType range = _max - _min;
        const size_t bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
        assert(ran.size() > 5000 / bitsPerVariate);
        const size_t stepLen = 5000 / bitsPerVariate;
        size_t passedCount = 0;
        for (size_t i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            size_t sum = 0;
            for (size_t j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j);
                for (size_t k = 0; k < bitsPerVariate; ++k)
                {
                    sum += ((r & 1) ^ ((r >> 1) & 1));
                    r >>= 1;
                }
            }
            if ((2326 < sum) && (sum < 2674))
                ++passedCount;
            counts.push_back(sum);
        }
        return passedCount;
    };

};

#endif // __AUTOCORR_H_
