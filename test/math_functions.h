// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __MATHFUNCTIONS_H_
#define __MATHFUNCTIONS_H_

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef M_PI_2
#define M_PI_2 (1.57079632679489661923)
#endif

#ifndef M_SQRT2
#define M_SQRT2 (1.41421356237309504880)
#endif

#ifndef M_LOG2E
#define M_LOG2E (1.4426950408889634074)
#endif


namespace ctrandom {

    /// Berechnen des Binomial-Koeffizienten (n über k).
    /// @param n
    /// @param k
    /// @return Binomial-Koeffizient (n über k)
    template <typename T>
    T choose(unsigned int n, unsigned int k)
    {
        if (k > n)
            return 0;
        if (k > n / 2)
            k = n - k;
        T accum = 1;
        for (T i = 1; i <= k; ++i)
            accum = accum * (T) (n - k + i) / (T) i;
        return accum;
    }


    /// Berechnen der Fakultät.
    /// @param n
    /// @return n!
    template <typename T>
    T factorial(T n)
    {
        if (n == 0)
            return 1;
        T f = n;
        while (--n > 0)
            f *= n;
        return f;
    }


    /// Berechnen der Stirling-Zahl S{n,k} zweiter Art.
    /// @param n
    /// @param k
    /// @return S{n,k}
    template <typename T>
    T stirling(size_t n, size_t k)
    {
        if (n == k)
            return 1;
        if (k == 1)
            return 1;
        return stirling<T>(n - 1, k - 1) + k * stirling<T>(n - 1, k);
    }

};

#endif // __MATHFUNCTIONS_H_
