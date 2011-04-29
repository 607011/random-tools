// $Id: gap.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __GAP_H_
#define __GAP_H_

#include <cstdlib>
#include <cassert>
#include <vector>

#include "chisq.h"

namespace randomtools {

    /// Gap-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param t
    /// @param alpha
    /// @param beta
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double gap_test(const std::vector<VariateType>& ran, const size_t t, const VariateType alpha, const VariateType beta, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(beta > alpha);
        assert(ran.size() > 100);
        long t_range = 1L + ((long) beta - (long) alpha);
        long v_range = 1L + ((long) _max - (long) _min);
        std::vector<int> histo(t+1, 0);
        int n = 0;
        // TODO: Schleife lesbarer gestalten
        std::vector<int>::size_type j = 0;
        while (j < ran.size()) 
        {
            size_t r = 0;
            while (j < ran.size())
            {
                VariateType x = ran.at(j);
                ++j;
                if (alpha <= x && x <= beta)
                    break;
                ++r;
            }
            ++histo[(r >= t)? t : r];
            ++n;
        }
        std::vector<int> expected(t+1);
        double prob = (double) t_range / (double) v_range;
        for (int i = 0; i < (int)expected.size()-1; ++i)
        {
            double e = prob * pow(1-prob, (double) i);
            expected[i] = (int) ((double) n * e);
        }
        expected[t] = (int) ((double) n * pow(1-prob, (double) t));
        double res = ChiSq<int>(histo, expected);
        double p = ChiSquareProbability(res, t);
        return p;
    };

};

#endif // __GAP_H_
