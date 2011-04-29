// $Id: max-t.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __MAX_T_H_
#define __MAX_T_H_

#include <cstdlib>
#include <cmath>
#include <vector>
#include <vector>
#include <limits>

#include "chisq.h"


namespace randomtools {

    template <typename VariateType>
    static inline double _P(VariateType k, size_t d, size_t t)
    {
        double res = (pow((double) (k+1), (double) t) - pow((double) k,  (double) t)) / pow((double) d, (double) t);
        return res;
    }

    /// Maximum-of-t-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param t t
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double max_t_test(const std::vector<VariateType>& ran, const int t, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        long d = 1L + ((long) _max - (long) _min);
        std::vector<int> histo(d, 0);
        // TODO: Schleife parallelisieren
        for (int i = 0; i < (int)ran.size() - t; i += t)
        {
            VariateType t_max = std::numeric_limits<VariateType>::min();
            for (int j = 0; j < t; ++j)
                if (ran.at(i+j) > t_max)
                    t_max = ran.at(i+j);
            ++histo[t_max];
        }
        std::vector<int> expected;
        std::vector<int> histo2;
        double t_count = (double) (ran.size() - t) / (double) t;
        // TODO: Schleife ggf. parallelisieren
        for (long i = _min; i <= (long) _max; ++i)
        {
            long k = (long) (i - (long) _min);
            long p = (long) (t_count * _P(k, d, t));
            if (p > 5 && histo.at(k) > 5)
            {
                expected.push_back(p);
                histo2.push_back(histo.at(k));
            }
        }
        double res = ChiSq<int>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    }

};

#endif // __MAX_T_H_
