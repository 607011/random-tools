// $Id: entropy.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __ENTROPY_H_
#define __ENTROPY_H_

#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#endif

#include <cstdlib>
#include <vector>
#include <cmath>
#include <cassert>

#include "math_functions.h"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace randomtools {

    /// Ermitteln der Entropie einer Zufallszahlenfolge.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return Entropie in Bits pro Bit.
    template <typename VariateType>
    double entropy_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        long range = 1L + ((long) _max - (long) _min);
        std::vector<int> histo(range, 0);
        for (int i = 0; i < (int)ran.size(); ++i)
            ++histo[ran[i]];
        double ent = 0.0;
        // TODO: Schleife parallelisieren
        for (long i = 0; i < range; ++i)
        {
            double p = (double)histo[i] / (double)ran.size();
            if (p > 0.0)
                ent += p * M_LOG2E * log(1.0 / p);
        }
        double bitsPerVariate = log((double) range) * M_LOG2E;
        return ent / bitsPerVariate;
    }

};

#endif // __ENTROPY_H_
