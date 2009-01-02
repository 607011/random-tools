// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __GAP_H_
#define __GAP_H_

#include <cstdlib>
#include <cassert>
#include <vector>

#include "chisq.h"

namespace ctrandom {

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
        VariateType t_range = beta - alpha;
        VariateType v_range = _max - _min;
        std::vector<size_t> histo(t+1, 0);
        size_t n = 0;
        // TODO: Schleife lesbarer gestalten
        std::vector<size_t>::size_type j = 0;
        while (j < ran.size()) 
        {
            size_t r = 0;
            while (j < ran.size())
            {
                VariateType x = ran.at(j);
                ++j;
                if (alpha <= x && x < beta)
                    break;
                ++r;
            }
            ++histo[(r >= t)? t : r];
            ++n;
        }
        std::vector<size_t> expected(t+1);
        double prob = (double) t_range / (double) v_range;
        for (size_t i = 0; i < expected.size()-1; ++i)
        {
            double e = prob * pow(1-prob, (double) i);
            expected[i] = (size_t) ((double) n * e);
        }
        expected[t] = (size_t) ((double) n * pow(1-prob, (double) t));
        double res = ChiSq<size_t>(histo, expected);
        double p = ChiSquareProbability(res, t);
        return p;
    };

};

#endif // __GAP_H_
