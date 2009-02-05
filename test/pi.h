// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MONTE_CARLO_PI_H_
#define __MONTE_CARLO_PI_H_

#include <cstdlib>
#include <vector>
#include <cassert>
#include <cmath>

#include "math_functions.h"


namespace ctrandom {

    /// Pi-Simulationstest.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstm�glicher Wert in der Zufallszahlenfolge
    /// @param _max gr��tm�glicher Wert in der Zufallszahlenfolge
    /// @param N
    /// @return Empirisch ermitteltes Pi
    template <typename VariateType>
    double pi_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, const size_t N)
    {
        assert(_max > _min);
        assert(N > 0);
        assert(ran.size() > N*2*5);
        size_t inCount = 0;
        const double r = (double) (_max - _min);
        const double c = pow((double) r, (int) (2*N));
        for (size_t i = 0; i < ran.size() - N*2; i += N*2)
        {
            double x = 0;
            double y = 0;
            for (size_t j = 0; j < N; ++j)
            {
                x = r * x + (ran[i+j+0] - _min);
                y = r * y + (ran[i+j+N] - _min);
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
