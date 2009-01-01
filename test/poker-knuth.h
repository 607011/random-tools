// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __POKERKNUTH_H_
#define __POKERKNUTH_H_

#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>
#include <map>

#include "math_functions.h"
#include "chisq.h"

namespace ctrandom {
    
    /// Poker-Test à la Knuth.
    /// @param ran Array mit Zufallszahlen
    /// @param _min kleinstmöglicher Zufallswert
    /// @param _max größtmöglicher Zufallswert
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double poker_test_knuth(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        const size_t k = 5;
        size_t d = (size_t) (_max - _min);
        std::vector<size_t> histo(5, 0);
        std::vector<size_t> expected(5);
        double chunkCount = (double) ran.size() / (double) k;
        for (size_t r = 1; r <= 5; ++r)
        {
            double nominator = (double) d;
            for (size_t j = d - r + 1; j < d; ++j)
                nominator *= (double) j;
            expected[r-1] = (size_t) (chunkCount * nominator / pow((double) d, (double) k) * stirling<double>(k, r));
        }
        for (size_t i = 0; i < ran.size() - k; i += k)
        {
            std::map<VariateType, size_t> hand;
            for (size_t j = 0; j < k; ++j)
                ++hand[ran.at(i+j)];
            ++histo[hand.size()-1];
        }
        double res = ChiSq<size_t>(histo, expected);
        double p = ChiSquareProbability(res, k-1);
        return p;
    }

};

#endif // __POKER_H_
