// $Id$
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
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


namespace randomtools {

#ifdef HAVE_BOOST

#if defined _MSC_VER || defined __BORLANDC__
    typedef __int64 int64;
    typedef unsigned __int64 uint64;
#else
    typedef long long int64;
    typedef unsigned long long uint64;
#endif

    template <typename T>
    struct is_64bit : public boost::false_type { };

    template <> 
    struct is_64bit<int64> : public boost::true_type { };

    template <> 
    struct is_64bit<uint64> : public boost::true_type { };

    template <typename VariateType>
    size_t __monobit_test(const std::vector<VariateType>& ran,
                          const VariateType _min, const VariateType _max,
                          int& testCount,
                          const boost::true_type&)
    {
        const long range = 1L + (size_t) ((long) _max - (long) _min);
        const size_t bitsPerVariate = (size_t) ceil(M_LOG2E * log((double) range));
        const size_t stepLen = 20000 / bitsPerVariate;
        size_t passedCount = 0;
        testCount = 0;
        for (int i = 0; i < (int)ran.size() - stepLen; i += stepLen)
        {
            int bitCount = 0;
            for (int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
#ifdef _MSC_VER
#pragma warning(push)
// prevent warning about right shift by too large amount, possibly resulting in data loss
// TODO: distinguish between 8, 16, 32 and 64 bit variate types
#pragma warning(disable:4333)
#endif
                bitCount += randomtools::PopCount[r & 0xffff] + randomtools::PopCount[(r >> 16) & 0xffff] +
                    randomtools::PopCount[(r >> 32) & 0xffff] + randomtools::PopCount[(r >> 48) & 0xffff];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            ++testCount;
        }
        return testCount - passedCount;
    }


    template <typename VariateType>
    size_t __monobit_test(const std::vector<VariateType>& ran,
                          const VariateType _min, const VariateType _max,
                          int& testCount,
                          const boost::false_type&)
    {
        const long range = 1L + (size_t) ((long) _max - (long) _min);
        const int bitsPerVariate = (int) ceil(M_LOG2E * log((double) range));
        const int stepLen = 20000 / bitsPerVariate;
        int passedCount = 0;
        testCount = 0;
        for (size_t i = 0; i < ran.size() - stepLen; i += stepLen)
        {
            int bitCount = 0;
            for (int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
#ifdef _MSC_VER
#pragma warning(push)
// prevent warning about right shift by too large amount, possibly resulting in data loss
// TODO: distinguish between 8, 16, 32 and 64 bit variate types
#pragma warning(disable:4333)
#endif
                bitCount += randomtools::PopCount[r & 0xffff] + randomtools::PopCount[(r >> 16) & 0xffff];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            ++testCount;
        }
        return testCount - passedCount;
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
    size_t monobit_test(const std::vector<VariateType>& ran,
                        const VariateType _min, const VariateType _max,
                        int& testCount)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
#ifdef HAVE_BOOST
        return __monobit_test(ran, _min, _max, testCount, is_64bit<VariateType>());
#else
        const long range = 1L + ((long) _max - (long) _min);
        const int bitsPerVariate = (int) ceil(M_LOG2E * log((double) range));
        const int stepLen = 20000 / bitsPerVariate;
        int passedCount = 0;
        for (int i = 0; i < (int)ran.size() - stepLen; i += stepLen)
        {
            int bitCount = 0;
            for (int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
#ifdef _MSC_VER
#pragma warning(push)
// prevent warning about right shift by too large amount, possibly resulting in data loss
// TODO: distinguish between 8, 16, 32 and 64 bit variate types
#pragma warning(disable:4333)
#endif
                bitCount += randomtools::PopCount[r & 0xffff] + randomtools::PopCount[(r >> 16) & 0xffff] +
                    randomtools::PopCount[(r >> 32) & 0xffff] + randomtools::PopCount[(r >> 48) & 0xffff];
#ifdef _MSC_VER
#pragma warning(pop)
#endif
            }
            if ((9654 < bitCount) && (bitCount < 10346))
                ++passedCount;
            ++testCount;
        }
        return testCount - passedCount;
#endif // HAVE_BOOST
    };


#ifdef HAVE_BOOST
    /// Monobit-Test (NIST).
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert
    template <typename VariateType>
    double monobit_test_nist(const std::vector<VariateType>& ran,
                             const VariateType _min, const VariateType _max)
    {
        const long range = 1L + ((long) _max - (long) _min);
        const int bitsPerVariate = (int) ceil(M_LOG2E * log((double) range));
        long Sn = 0;
#pragma omp parallel for reduction(+:Sn)
        for (int i = 0; i < (int)ran.size(); ++i)
        {
            VariateType r = ran.at(i) - _min;
            for (int j = 0; j < bitsPerVariate; ++j)
            {
                Sn += 2 * (r & 1) - 1;
                r >>= 1;
            }
        }
        double Sobs = (double) abs(Sn) / sqrt((double) ran.size() * bitsPerVariate);
        double p = boost::math::erfc<double>(Sobs / M_SQRT2);
        return p;
    }
#endif

};

#endif // __MONOBIT_H_
