// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __CHISQ_H_
#define __CHISQ_H_

#include <cstdlib>
#include <cmath>
#include <cassert>
#include <vector>

namespace randomtools {

    /// p-Wert für Chi-Quadrat-Anpassungstest berechnen.
    /// @param chi2 Chi-Quadrat
    /// @param degrees_of_freedom Anzahl der Freiheitsgrade
    /// @return p-Wert
    extern double ChiSquareProbability(double chi, int degrees_of_freedom);

    /// Schwellwert für Chi-Quadrat-Anpassungstest aus p-Wert berechnen.
    /// @param p p-Wert
    /// @param degrees_of_freedom Anzahl der Freiheitsgrade
    /// @return Chi-Quadrat-Schwellwert
    extern double ChiSquareCritical(double p, int degrees_of_freedom);

    /// Chi-Quadrat für Anpassungstest berechnen.
    /// @param Y Array mit Messwerten
    /// @param E Erwartungswert für jeden einzelnen Messwert
    /// @return Chi-Quadrat
    template <typename VariateType>
    double ChiSq(typename std::vector<VariateType>& Y, double E)
    {
        assert(Y.size()>0);
        double V = 0;
        if (E != 0)
        {
            for (size_t i = 0; i < Y.size(); ++i)
            {
                double diff = (double) Y[i] - E;
                double x = diff * diff / (double) E;
                V += x;
            }
        }
        return V;
    }

    /// Chi-Quadrat für Anpassungstest berechnen.
    /// @param Y Array mit Messwerten
    /// @param E Array mit Erwartungswerten
    /// @return Chi-Quadrat
    template <typename VariateType>
    double ChiSq(typename std::vector<VariateType>& Y, typename std::vector<VariateType>& E)
    {
        assert(Y.size()>0);
        double V = 0;
        assert (Y.size() == E.size());
        for (size_t i = 0; i < Y.size(); ++i)
        {
            double diff = (double) Y[i] - (double) E[i];
            if (E[i] != 0)
                V += diff * diff / (double) E[i];
        }
        return V;
    }


};

#endif // __CHISQ_H_
