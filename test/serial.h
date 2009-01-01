// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __SERIAL_H_
#define __SERIAL_H_

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>

#include "chisq.h"

namespace ctrandom {

    /// Serial-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @param groupsize Gruppengröße
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double serial_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, const size_t groupsize)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        VariateType range = _max - _min;
        size_t num_buckets = (size_t) pow((double) range, (double) groupsize);
        std::vector<size_t> histo(num_buckets, 0);
        for (size_t i = groupsize - 1; i < ran.size(); i += groupsize)
        {
            size_t idx = 0;
            for (size_t j = 0; j < groupsize; ++j)
            {
                size_t m = (size_t) pow((double) range, (double) j);
                idx += (ran[i - j] - _min) * m;
            }
            ++histo[idx];
        }
        double expected = (double) ran.size() / num_buckets / groupsize;
        double res = ChiSq<size_t>(histo, expected);
        double p = ChiSquareProbability(res, (size_t) (pow((double) range, (int) groupsize)));
        return p;
    }

};

#endif // __SERIAL_H_
