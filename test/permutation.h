// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __PERMUTATION_H_
#define __PERMUTATION_H_

#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>
#include <map>
#include <limits>

#include "math_functions.h"
#include "chisq.h"


namespace randomtools {


    namespace helper {

        template <typename T>
        static inline void _Swap(T& a, T& b)
        {
            T tmp = a;
            a = b;
            b = tmp;
        }


        template <typename VariateType>
        static int _F(std::vector<VariateType>& U)
        {
            int r = (int) U.size();
            int f = 0;
            do
            {
                int s = 0;
                VariateType _max = std::numeric_limits<VariateType>::min();
                for (int i = 0; i < r; ++i)
                {
                    if (U.at(i) > _max)
                    {
                        _max = U.at(i);
                        s = i;
                    }
                }
                f = r * f + s;
                _Swap(U[r-1], U[s]);
            }
            while (--r > 0);
            return f;
        }

    };


    /// Permutation-Test à la Knuth.
    /// @param ran Zufallszahlenfolge
    /// @param t über wie viel Elemente soll permutiert werden?
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double permutation_test(const std::vector<VariateType>& ran, const int t, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        int num_buckets = ran.size() / t;
        int n = randomtools::factorial<int>(t);
        std::vector<int> histo(n);
        // TODO: Schleife parallelisieren
        for (int i = 0; i < (int)ran.size() - t; i += t)
        {
           std::vector<VariateType> U(t);
           for (int j = 0; j < t; ++j)
               U[j] = ran[i+j];
           int f = helper::_F(U);
           ++histo[f];
        }
        double E = (double) num_buckets / (double) n;
        double res = ChiSq<int>(histo, E);
        double p = ChiSquareProbability(res, n-1);
        return p;
    }

};

#endif // __PERMUTATION_H_
