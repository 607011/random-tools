// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __SERIAL_H_
#define __SERIAL_H_

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>

#include "chisq.h"

namespace randomtools {

    /// Serial-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @param groupsize Gruppengröße
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double serial_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, const int groupsize)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        const long range = 1L + ((long) _max - (long) _min);
        const long num_buckets = (long) pow((double) range, (double) groupsize);
        std::vector<int> histo(num_buckets, 0);
        // TODO: Schleife parallelisieren
        for (int i = groupsize - 1; i < (int)ran.size(); i += groupsize)
        {
            long idx = 0;
            for (int j = 0; j < groupsize; ++j)
            {
                long m = (long) pow((double) range, (double) j);
                idx += (ran[i - j] - _min) * m;
            }
            ++histo[idx];
        }
        double expected = (double) ran.size() / num_buckets / groupsize;
        double res = ChiSq<int>(histo, expected);
        double p = ChiSquareProbability(res, (int) (pow((double) range, (int) groupsize)));
        return p;
    }

};

#endif // __SERIAL_H_
