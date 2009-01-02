// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "chisq.h"
#include "math_functions.h"

namespace ctrandom {

    // Portiert von http://statpages.org/pdfs.html

    static double _Norm(double z)
    {
        double q = z * z;
        if (fabs(z) > 7)
            return (1 - 1 / q + 3 / (q * q)) * exp(-q / 2) / (fabs(z) * sqrt(M_PI_2));
        else
            return ChiSquareProbability(q, 1);
    }


    double ChiSquareProbability(double x, size_t df)
    {
        if (x > 1000 || df > 1000)
        {
            double q = _Norm((pow(x / df, 1.0 / 3.0) + 2 / (9 * df) - 1) / sqrt((double) 2.0 / (9.0 * df))) / 2;
            if (x > df)
                return q;
            else
                return 1 - q;
        }
        double p = exp(-0.5 * x);
        if ( (df % 2) == 1)
            p *= sqrt(2 * x / M_PI);
        size_t k = df;
        while (k >= 2)
        {
            p = p * x / k;
            k -= 2;
        }
        double t = p;
        size_t a = df;
        while (t > 1e-15 * p)
        {
            a += 2;
            t = t * x / a;
            p += t;
        }
        return 1 - p;
    }


    double ChiSquareCritical(double p, size_t df)
    {
        const double nearnull = 1e-10;
        double v = 0.5;
        double dv = 0.5;
        double x = 0;
        while (dv > nearnull)
        {
            x = 1 / v - 1;
            dv /= 2;
            if (ChiSquareProbability(x, df) > p)
                v -= dv;
            else
                v += dv;
        }
        return x;
    }

};
