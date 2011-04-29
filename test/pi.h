// $Id: pi.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MONTE_CARLO_PI_H_
#define __MONTE_CARLO_PI_H_

#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>

#include "math_functions.h"


namespace randomtools {

    /// Pi-Simulationstest.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @param N
    /// @return Empirisch ermitteltes Pi
    template <typename VariateType>
    double pi_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, const int N)
    {
        assert(_max > _min);
        assert(N > 0);
        assert((int)ran.size() > N*2*5);
        const double range = 1 + (double) ((long) _max - (long) _min);
        const double c = pow((double) range, (int) (2*N));
        int inCount = 0;
#pragma omp parallel for reduction(+:inCount)
        for (int i = 0; i < (int)ran.size() - N*2; i += N*2)
        {
            double x = 0;
            double y = 0;
            for (int j = 0; j < N; ++j)
            {
                x = range * x + (ran[i+j+0] - _min);
                y = range * y + (ran[i+j+N] - _min);
            }
            double dist = x * x + y * y;
            if (dist <= c)
                ++inCount;
        }
        double pi = (2 * N) * 4.0 * (double) inCount / (double) ran.size();
        return pi;
    };

};

#endif // __MONTE_CARLO_PI_H_
