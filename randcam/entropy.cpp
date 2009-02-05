// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <cmath>
#include <cstdlib>

double entropy(const unsigned char* ran, const size_t sz)
{
#ifndef M_LOG2E
    const double M_LOG2E = 1.44269504088896340736;
#endif
    const size_t hop = 1;
    const size_t RANGE = 256;
    size_t histo[RANGE];
    for (size_t i = 0; i < RANGE; ++i) 
        histo[i] = 0;
    for (size_t i = 0; i < sz; i += hop)
        ++histo[ran[i]];
    double p[RANGE];
    for (size_t i = 0; i < RANGE; ++i)
        p[i] = (double) histo[i] / sz;
    double ent = 0.0;
    for (size_t i = 0; i < RANGE; ++i)
        if (p[i] > 0.0)
            ent += p[i] * M_LOG2E * log(1.0 / p[i]);
    return .125 * ent;
}
