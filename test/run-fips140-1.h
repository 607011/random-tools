// $Id: run-fips140-1.h,v af569101d24e 2009/02/25 15:00:20 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RUN_FIPS140_1_H_
#define __RUN_FIPS140_1_H_

#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>
#include <map>

#include "bitvector.h"
#include "chisq.h"
#include "math_functions.h"

namespace randomtools {

    struct RunResult
    {
        static const int MaxRunLength = 6;
        RunResult(void)
        {
            longestRun0 = 0;
            longestRun1 = 0;
            for (int i = 0; i < MaxRunLength; ++i) 
            {
                run0[i] = 0;
                run1[i] = 0;
            }
            passed = false;
        }
        int run0[MaxRunLength];
        int run1[MaxRunLength];
        int longestRun0;
        int longestRun1;
        bool passed;
    };

    /// The Runs Test (FIPS 140-1).
    /// - A run is defined as a maximal sequence of consecutive bits of either all
    ///   ones or all zeros, which is part of the 20,000 bit sample stream. The
    ///   incidences of runs (for both consecutive zeros and consecutive ones) of
    ///    all lengths ( >= 1 ) in the sample stream should be counted and stored.
    /// - The test is passed if the number of runs that occur (of lengths 1
    ///   through 6) is each within the corresponding interval specified below.
    ///   This must hold for both the zeros and ones; that is, all 12 counts must
    ///   lie in the specified interval. For the purpose of this test, runs of
    ///   greater than 6 are considered to be of length 6.
    ///   
    /// Length of Run   Required Interval
    /// 1               2267-2733
    /// 2               1079-1421
    /// 3                502-748
    /// 4                223-402
    /// 5                 90-223
    /// 6                 90-223
    /// 
    /// The Long Run Test.
    /// - A long run is defined to be a run of length 34 or more (of either zeros
    ///   or ones).
    /// - On the sample of 20,000 bits, the test is passed if there are NO long runs.
    /// @see http://csrc.nist.gov/publications/fips/fips1401.htm
    /// @param ran [in] Zufallszahlenfolge
    /// @param _min [in] kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max [in] größtmöglicher Wert in der Zufallszahlenfolge
    /// @param runCount [out] Histogramm: Anzahl der Läufe einer bestimmten Länge
    /// @param longestRun0 [out] längster Run aus Nullen
    /// @param longestRun1 [out] längster Run aus Einsen
    /// @return Anzahl der Tests, die bestanden wurden
    template <typename VariateType>
    size_t run_test_fips(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, int& runCount, int& longestRun0, int& longestRun1)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        const int ChunkSize = 20000;
        size_t range = 1 + (size_t) ((long) _max - (long) _min);
        int bitsPerVariate = (int) ceil(M_LOG2E * log((double) range));
        int stepLen = ChunkSize / bitsPerVariate;
        longestRun0 = 0;
        longestRun1 = 0;
        size_t passedCount = 0;
        runCount = 0;
        // TODO: Schleife parallelisieren
        for (int i = 0; i < (int)ran.size() - stepLen; i += stepLen)
        {
            BitVector chunk(ChunkSize);
            for (int j = 0; j < stepLen; ++j)
            {
                VariateType r = ran.at(i + j) - _min;
                for (int k = 0; k < bitsPerVariate; ++k)
                {
                    if ((r & 1) == 1)
                        chunk.set(j * bitsPerVariate + k);
                    r >>= 1;
                }
            }
            bool isSet = chunk.at(0);
            int runLength = 0;
            RunResult run;
            for (int j = 1; j < ChunkSize; ++j)
            {
                if (isSet) 
                {
                    if (chunk.at(j))
                    {
                        ++runLength;
                    }
                    else
                    {
                        ++run.run1[(runLength > RunResult::MaxRunLength)? RunResult::MaxRunLength : runLength];
                        if (run.longestRun1 < runLength)
                            run.longestRun1 = runLength;
                        runLength = 0;
                        isSet = false;
                    }
                }
                else 
                {
                    if (!chunk.at(j))
                    {
                        ++runLength;
                    }
                    else
                    {
                        ++run.run0[(runLength > RunResult::MaxRunLength)? RunResult::MaxRunLength : runLength];
                        if (run.longestRun0 < runLength)
                            run.longestRun0 = runLength;
                        runLength = 0;
                        isSet = true;
                    }
                }
            }
            run.passed = (run.longestRun0 < 35) && (run.longestRun1 < 35)
                && (2267 < run.run0[0]) && (run.run0[0] < 2733)
                && (1079 < run.run0[1]) && (run.run0[1] < 1421)
                && ( 502 < run.run0[2]) && (run.run0[2] <  748)
                && ( 223 < run.run0[3]) && (run.run0[3] <  402)
                && (  90 < run.run0[4]) && (run.run0[4] <  223)
                && (  90 < run.run0[5]) && (run.run0[5] <  223)
                && (2267 < run.run1[0]) && (run.run1[0] < 2733)
                && (1079 < run.run1[1]) && (run.run1[1] < 1421)
                && ( 502 < run.run1[2]) && (run.run1[2] <  748)
                && ( 223 < run.run1[3]) && (run.run1[3] <  402)
                && (  90 < run.run1[4]) && (run.run1[4] <  223)
                && (  90 < run.run1[5]) && (run.run1[5] <  223);
            if (run.passed)
                ++passedCount;
            ++runCount;
        }
        return runCount - passedCount;
    };

};

#endif // __RUN_FIPS140_1_H_
