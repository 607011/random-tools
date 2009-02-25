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

namespace randomtools {

    /// Run-Test a la Knuth für Werte oberhalb des Medians.
    /// @param ran Zufallszahlenfolge
    /// @param _min kleinstmöglicher Wert in der Zufallszahlenfolge
    /// @param _max größtmöglicher Wert in der Zufallszahlenfolge
    /// @return p-Wert des Chi-Quadrat-Anpassungstests
    template <typename VariateType>
    double run_above_test(std::vector<VariateType>& ran, VariateType _min, VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        VariateType median = (VariateType) ((1L + ((long) _max - (long) _min)) / 2L);
        std::map<int, int> histo;
        int counted = 0;
        int runLength = 0;
        VariateType current = ran.at(0);
        // TODO: Schleife parallelisieren
        for (int i = 1; i < (int)ran.size(); ++i)
        {
            if (ran.at(i) >= median)
            {
                ++runLength;
            }
            else
            {
                ++histo[runLength];
                runLength = 0;
                ++counted;
            }
            current = ran.at(i);
        }
        int maxIdx = 0;
        for (std::map<int, int>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            if ((*i).first > maxIdx)
                maxIdx = (*i).first;
        std::vector<int> histo2(maxIdx+1);
        for (std::map<int, int>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            histo2[(*i).first] = (*i).second;
        std::vector<int> expected(histo2.size());
        for (int i = 0; i < (int)histo2.size(); ++i)
            expected[i] = (int) ((double) ran.size() / pow((double) 2, (int) (i+2)));
        double res = ChiSq<int>(histo2, expected);
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
        VariateType median = (VariateType) ((1L + ((long) _max - (long) _min)) / 2L);
        std::map<int, int> histo;
        int counted = 0;
        int runLength = 0;
        VariateType current = ran.at(0);
        for (int i = 1; i < (int) ran.size(); ++i)
        {
            if (ran.at(i) < median)
            {
                ++runLength;
            }
            else
            {
                ++histo[runLength];
                runLength = 0;
                ++counted;
            }
            current = ran.at(i);
        }
        int maxIdx = 0;
        for (std::map<int, int>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            if ((*i).first > maxIdx)
                maxIdx = (*i).first;
        std::vector<int> histo2(maxIdx+1);
        for (std::map<int, int>::const_iterator i = histo.begin(); i != histo.end(); ++i)
            histo2[(*i).first] = (*i).second;
        std::vector<int> expected(histo2.size());
        for (int i = 0; i < (int) histo2.size(); ++i)
            expected[i] = (size_t) ((double) ran.size() / pow((double) 2, (int) (i+2)));
        double res = ChiSq<int>(histo2, expected);
        double p = ChiSquareProbability(res, histo2.size()-1);
        return p;
    };


};

#endif // __RUNS_ABOVE_BELOW_H_
