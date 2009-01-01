// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __MONOBIT_H_
#define __MONOBIT_H_

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

#include "chisq.h"
#include "math_functions.h"

#ifdef HAVE_BOOST
#include <boost/math/special_functions/erf.hpp>
#endif


namespace ctrandom {

    /// The FIPS 140-1 Monobit Test.
    /// - Count the number of ones in the 20000 bit stream. Denote this quantity by X.
    /// - The test is passed if 9654 < X < 10346.
    /// @see http://csrc.nist.gov/publications/fips/fips1401.htm
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @param [out] Histogramm
    /// @return Anzahl der bestandenen Tests
    template <typename VariateType>
    size_t monobit_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, std::vector<size_t>& counts)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        VariateType range = _max - _min;
        size_t bitsPerVariate = (size_t) (M_LOG2E * log((double) range));
        size_t stepLen = 20000 / bitsPerVariate;
        size_t passedCount = 0;
        for (size_t i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            size_t bitCount = 0;
            for (size_t j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                for (size_t k = 0; k < bitsPerVariate; ++k)
                {
                    if ((r & 1) == 1)
                        ++bitCount;
                    r >>= 1;
                }
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            counts.push_back(bitCount);
        }
        return passedCount;
    };


#ifdef HAVE_BOOST
    /// Monobit-Test (NIST).
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert
    template <typename VariateType>
    double monobit_test_nist(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max)
    {
        VariateType range = _max - _min;
        size_t bitsPerVariate = (size_t) (M_LOG2E * log((double) range));
        double Sn = 0;
        for (size_t i = 0; i < ran.size(); ++i)
        {
            VariateType r = ran.at(i) - _min;
            for (size_t j = 0; j < bitsPerVariate; ++j)
            {
                Sn += 2 * (double) (r & 1) - 1;
                r >>= 1;
            }
        }
        double Sobs = fabs(Sn) / sqrt((double) ran.size() * bitsPerVariate);
        double p = boost::math::erfc<double>(Sobs / M_SQRT2);
        return p;
    }
#endif

};

#endif // __MONOBIT_H_
