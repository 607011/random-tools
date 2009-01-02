// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __COUPONCOLLECTOR_H_
#define __COUPONCOLLECTOR_H_

#include <cstdlib>
#include <cassert>
#include <vector>

#include "math_functions.h"
#include "chisq.h"

namespace ctrandom {

    /// Coupon-Collector-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param d
    /// @param t
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double coupon_collector_test(const std::vector<VariateType>& ran, const size_t d, const size_t t)
    {
        assert(ran.size() > 100);
        std::vector<size_t> histo(t-d+1, 0);
        size_t segmentCount = 0;
        for (size_t i = 0; i < ran.size(); /* */)
        {
            std::vector<bool> coupon(d, false);
            size_t collectCount = 0;
            size_t segmentLength = 0;
            while (collectCount < d && i < ran.size())
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
        std::vector<size_t> expected(t-d+1, 0);
        double fac_d = ctrandom::factorial<double>(d);
        for (size_t i = d; i < t; ++i)
            expected[i-d] = (size_t) (segmentCount * fac_d / pow((double) d, (double) i) * stirling<double>(i - 1, d - 1));
        expected[t-d] = (size_t) (segmentCount * (1 - fac_d / pow((double) d, (double) (t - 1)) * stirling<double>(t - 1, d)));
        double res = ChiSq<size_t>(histo, expected);
        double p = ChiSquareProbability(res, t-d+1);
        return p;
    };

};

#endif // __COUPONCOLLECTOR_H_
