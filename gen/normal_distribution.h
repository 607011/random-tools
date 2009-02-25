// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __NORMALDISTRIBUTION_H_
#define __NORMALDISTRIBUTION_H_

namespace randomtools {

    template <class Generator, typename VariateType>
    // double shall be either double or float
    class NormalDistribution
    {
    public:
        NormalDistribution(Generator& rng, VariateType mean = 0.0, VariateType sigma = 1.0)
            : _RNG(rng), _Mean(mean), _Sigma(sigma), _Cached(false)
        {}

        VariateType operator()()
        {
            double r;
            if (_Cached)
            {
                _Cached = false;
                r = _R;
            }
            else
            {
                _Cached = true;
                VariateType s, v1, v2;
                do
                {
                    v1 = 2 * _Rand() - 1;
                    v2 = 2 * _Rand() - 1;
                    s = v1 * v1 + v2 * v2;
                }
                while (s >= 1);
                _R = v2 * sqrt(-2 * log(s) / s);
                r = v1 * sqrt(-2 * log(s) / s);
            }
            return _Mean + r * _Sigma;
        }

    private:
        Generator _RNG;
        VariateType _Mean;
        VariateType _Sigma;
        bool _Cached;
        VariateType _R;

        inline VariateType _Rand(void)
        {
            return (VariateType) _RNG() / (VariateType) (_RNG.max() - _RNG.min());
        }

    };

};

#endif // __NORMALDISTRIBUTION_H_
