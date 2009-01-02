// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __MAX_T_H_
#define __MAX_T_H_

#include <cstdlib>
#include <cmath>
#include <vector>
#include <vector>
#include <limits>

#include "chisq.h"


namespace ctrandom {

    template <typename VariateType>
    static inline double _P(VariateType k, size_t d, size_t t)
    {
        double res = (pow((double) (k+1), (double) t) - pow((double) k,  (double) t)) / pow((double) d, (double) t);
        return res;
    }

    /// Maximum-of-t-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double max_t_test(const std::vector<VariateType>& ran, const size_t t, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        VariateType d = _max - _min;
        std::vector<size_t> histo(d, 0);
        for (size_t i = 0; i < ran.size() - t; i += t)
        {
            VariateType t_max = std::numeric_limits<VariateType>::min();
            for (size_t j = 0; j < t; ++j)
                if (ran.at(i+j) > t_max)
                    t_max = ran.at(i+j);
            ++histo[t_max];
        }
        std::vector<size_t> expected;
        std::vector<size_t> histo2;
        double t_count = (double) (ran.size() - t) / (double) t;
        for (VariateType i = _min; i < _max; ++i) {
            size_t k = (size_t) (i - _min);
            size_t p = (size_t) (t_count * _P(k, d, t));
            if (p > 5 && histo.at(k) > 5)
            {
                expected.push_back(p);
                histo2.push_back(histo.at(k));
            }
        }
        double res = ChiSq<size_t>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    }

};

#endif // __MAX_T_H_
