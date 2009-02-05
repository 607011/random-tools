// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RUNS_ABOVE_BELOW_H_
#define __RUNS_ABOVE_BELOW_H_

#include <cstdlib>
#include <cassert>
#include <vector>
#include <map>

#include "chisq.h"

namespace ctrandom {

    /// Run-Test à la Knuth für Werte oberhalb des Medians.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double run_above_test(std::vector<VariateType>& ran, VariateType _min, VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        VariateType median = (_max - _min) / 2;
        std::map<size_t, size_t> histo;
        size_t counted = 0;
        size_t runLength = 0;
        VariateType current = ran.at(0);
        for (size_t i = 1; i < ran.size(); ++i)
        {
            if (ran.at(i) >= median)
            {
                ++runLength;
            }
            else{
                ++histo[runLength];
                runLength = 0;
                ++counted;
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
        for (size_t i = 0; i < histo2.size(); ++i)
            expected[i] = (size_t) ((double) ran.size() / pow((double) 2, (int) (i+2)));
        double res = ChiSq<size_t>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    }


    /// Run-Test à la Knuth für Werte unterhalb des Medians.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double run_below_test(std::vector<VariateType>& ran, VariateType _min, VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        VariateType median = (_max - _min) / 2;
        std::map<size_t, size_t> histo;
        size_t counted = 0;
        size_t runLength = 0;
        VariateType current = ran.at(0);
        for (size_t i = 1; i < ran.size(); ++i)
        {
            if (ran.at(i) < median)
            {
                ++runLength;
            }
            else{
                ++histo[runLength];
                runLength = 0;
                ++counted;
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
        for (size_t i = 0; i < histo2.size(); ++i)
            expected[i] = (size_t) ((double) ran.size() / pow((double) 2, (int) (i+2)));
        double res = ChiSq<size_t>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    };


};

#endif // __RUNS_ABOVE_BELOW_H_
