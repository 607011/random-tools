// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __EQUIDISTRIBUTION_H_
#define __EQUIDISTRIBUTION_H_

namespace ctrandom {

    template <class Generator, typename VariateType>
    class EquiDistribution
    {
    public:
        EquiDistribution(Generator& rng, VariateType _min = 0, VariateType _max = 1)
            : _RNG(rng), _Min(_min), _Max(_max)
        { }

        inline VariateType operator()() { return _Rand(); }
        inline VariateType min(void) { return _Min; }
        inline VariateType max(void) { return _Max; }

    private:
        Generator _RNG;
        VariateType _Min;
        VariateType _Max;

        VariateType _Rand(void)
        {
            VariateType diff = _Max - _Min;
            VariateType srcdiff = (VariateType) (_RNG.max() - _RNG.min());
            VariateType _r = (VariateType) _RNG();
            VariateType r = _Min + diff * _r / srcdiff;
            return r;
        }

    };

};

#endif // __EQUIDISTRIBUTION_H_
