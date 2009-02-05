// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __FREQUENCY_H_
#define __FREQUENCY_H_

#include <cstdlib>
#include <cassert>
#include <vector>

#include "gen/equi_distribution.h"
#include "chisq.h"

namespace ctrandom {

    /// Frequency-Test à la Knuth.
    /// Zufallszahlen in Intervalle aufteilen und Häufigkeiten zählen.
    /// @param ran Array mit Zufallszahlen
    /// @param _min kleinstmöglicher Zufallswert
    /// @param _max größtmöglicher Zufallswert
    /// @param num_buckets Anzahl der Kategorien, in die die Zufallszahlen unterteilt werden sollen
    /// @return p-Wert des Chi-Quadrat-Tests der Häufigkeitsverteilung in den Kategorien
    template <typename VariateType>
    double frequency_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max, const size_t num_buckets)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        double interval = (double) (_max - _min) / (double) num_buckets;
        std::vector<size_t> histo(num_buckets);
        for (size_t i = 0; i < ran.size(); ++i)
        {
            const size_t idx = (size_t) ((double) (ran.at(i) - _min) / interval);
            ++histo[idx];
        }
        double expected = (double) ran.size() / (double) num_buckets;
        double res = ChiSq<size_t>(histo, expected);
        double p = ChiSquareProbability(res, num_buckets - 1);
        return p;
    }


    /// Frequency-Test à la Knuth.
    /// Zufallszahlen in Intervalle aufteilen und Häufigkeiten zählen.
    /// @param ran Array mit Zufallszahlen
    /// @param rng Generator, aus dem die Zufallszahlen hervorgegangen sind
    /// @param num_buckets Anzahl der Kategorien, in die die Zufallszahlen unterteilt werden sollen
    /// @return p-Wert des Chi-Quadrat-Tests der Häufigkeitsverteilung in den Kategorien
    template <class Generator, typename VariateType>
    inline double frequency_test(const std::vector<VariateType>& ran, const EquiDistribution<Generator, VariateType>& rng, const size_t num_buckets)
    {
        return frequency_test<VariateType>(ran, rng.min(), rng.max(), num_buckets);
    }


};

#endif // __FREQUENCY_H_
