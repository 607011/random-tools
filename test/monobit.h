// $Id$
// Copyright (c) 2008-2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __MONOBIT_H_
#define __MONOBIT_H_

#ifdef _WIN32
#include "../config-win32.h"
#endif

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <vector>
#include <limits>

#include "chisq.h"
#include "math_functions.h"
#include "popcount.h"

#ifdef HAVE_BOOST
#include <boost/math/special_functions/erf.hpp>
#include <boost/type_traits.hpp>
#endif


namespace ctrandom {

#ifdef HAVE_BOOST
    template <typename T> 
    struct is_64bit : public boost::false_type { };

    template <> 
    struct is_64bit<__int64> : public boost::true_type { };

    template <> 
    struct is_64bit<unsigned __int64> : public boost::true_type { };

    template <typename VariateType>
    size_t __monobit_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, std::vector<size_t>& counts, const boost::true_type&)
    {
        const VariateType range = _max - _min;
        const size_t bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
        const size_t stepLen = 20000 / bitsPerVariate;
        size_t passedCount = 0;
        for (size_t i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            size_t bitCount = 0;
            for (size_t j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                bitCount += ctrandom::PopCount[r & 0xffff] + ctrandom::PopCount[(r >> 16) & 0xffff] +
                    ctrandom::PopCount[(r >> 32) & 0xffff] + ctrandom::PopCount[(r >> 48) & 0xffff];
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            counts.push_back(bitCount);
        }
        return passedCount;
    }


    template <typename VariateType>
    size_t __monobit_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, std::vector<size_t>& counts, const boost::false_type&)
    {
        const VariateType range = _max - _min;
        const size_t bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
        const size_t stepLen = 20000 / bitsPerVariate;
        size_t passedCount = 0;
        for (size_t i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            size_t bitCount = 0;
            for (size_t j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                bitCount += ctrandom::PopCount[r & 0xffff] + ctrandom::PopCount[(r >> 16) & 0xffff];
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            counts.push_back(bitCount);
        }
        return passedCount;
    }
#endif // HAVE_BOOST


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
#ifdef HAVE_BOOST
        return __monobit_test(ran, _min, _max, counts, is_64bit<VariateType>());
#else
        const VariateType range = _max - _min;
        const size_t bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
        const size_t stepLen = 20000 / bitsPerVariate;
        size_t passedCount = 0;
        for (size_t i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            size_t bitCount = 0;
            for (size_t j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                bitCount += ctrandom::PopCount[r & 0xffff] + ctrandom::PopCount[(r >> 16) & 0xffff] +
                    ctrandom::PopCount[(r >> 32) & 0xffff] + ctrandom::PopCount[(r >> 48) & 0xffff];
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            counts.push_back(bitCount);
        }
        return passedCount;
#endif // HAVE_BOOST
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
        const VariateType range = _max - _min;
        const size_t bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
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
