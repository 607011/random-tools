// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __RUNS_UP_DOWN_H_
#define __RUNS_UP_DOWN_H_

#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>
#include <map>

#include "chisq.h"
#include "math_functions.h"

namespace ctrandom {


    static inline double _P(size_t r)
    {
        return (double) r / (double) ctrandom::factorial<size_t>(r+1);
    }


    template <typename VariateType>
    double run_up_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        std::map<size_t, size_t> histo;
        size_t runLength = 0;
        VariateType current = ran.at(0);
        size_t counted = 0;
        for (size_t i = 1; i < ran.size()-1; ++i)
        {
            if (ran.at(i) >= current)
            {
                ++runLength;
            }
            else
            {
                ++i;
                ++counted;
                ++histo[runLength];
                runLength = 0;
            }
            current = ran.at(i);
        }
        size_t maxIdx = 0;
        for (std::map<size_t,size_t>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            if ((*i).first > maxIdx)
                maxIdx = (*i).first;
        std::vector<size_t> histo2(maxIdx+1);
        for (std::map<size_t, size_t>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            histo2[(*i).first] = (*i).second;
        std::vector<size_t> expected(histo2.size());
        for (size_t i = 1; i <= expected.size(); ++i)
            expected[i-1] = (size_t) ((double) counted * _P(i));
        double res = ChiSq<size_t>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    };


    template <typename VariateType>
    double run_down_test(const std::vector<VariateType>& ran)
    {
        std::map<size_t, size_t> histo;
        size_t runLength = 0;
        VariateType current = ran.at(0);
        size_t counted = 0;
        for (size_t i = 1; i < ran.size()-1; ++i)
        {
            if (ran.at(i) <= current)
            {
                ++runLength;
            }
            else
            {
                ++i;
                ++counted;
                ++histo[runLength];
                runLength = 0;
            }
            current = ran.at(i);
        }
        size_t maxIdx = 0;
        for (std::map<size_t,size_t>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            if ((*i).first > maxIdx)
                maxIdx = (*i).first;
        std::vector<size_t> histo2(maxIdx+1);
        for (std::map<size_t, size_t>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            histo2[(*i).first] = (*i).second;
        std::vector<size_t> expected(histo2.size());
        for (size_t i = 1; i <= expected.size(); ++i)
            expected[i-1] = (size_t) ((double) counted * _P(i));
        double res = ChiSq<size_t>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    };

};

#endif // __RUNS_UP_DOWN_H_
