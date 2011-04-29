// $Id: couponcollector.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __COUPONCOLLECTOR_H_
#define __COUPONCOLLECTOR_H_

#include <cstdlib>
#include <cassert>
#include <vector>

#include "math_functions.h"
#include "chisq.h"

namespace randomtools {

    /// Coupon-Collector-Test a la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param d
    /// @param t
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double coupon_collector_test(const std::vector<VariateType>& ran, const int d, const int t)
    {
        assert(ran.size() > 100);
        std::vector<int> histo(t-d+1, 0);
        int segmentCount = 0;
        // TODO: Schleife parallelisieren
        for (int i = 0; i < (int)ran.size(); /* */)
        {
            std::vector<bool> coupon(d, false);
            int collectCount = 0;
            int segmentLength = 0;
            while (collectCount < d && i < (int)ran.size())
            {
                VariateType nibble = ran.at(i) % d;
                if (coupon[nibble] == false)
                {
                    coupon[nibble] = true;
                    ++collectCount;
                }
                ++segmentLength;
                ++i;
            }
            if (segmentLength >= d)
            {
                ++histo[(segmentLength >= t)? t-d : segmentLength-d];
                ++segmentCount;
            }
        }
        std::vector<int> expected(t-d+1, 0);
        double fac_d = randomtools::factorial<double>(d);
        for (int i = d; i < t; ++i)
            expected[i-d] = (int) (segmentCount * fac_d / pow((double) d, (double) i) * stirling<double>(i - 1, d - 1));
        expected[t-d] = (int) (segmentCount * (1 - fac_d / pow((double) d, (double) (t - 1)) * stirling<double>(t - 1, d)));
        double res = ChiSq<int>(histo, expected);
        double p = ChiSquareProbability(res, t-d+1);
        return p;
    };

};

#endif // __COUPONCOLLECTOR_H_
