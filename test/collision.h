// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __COLLISION_H_
#define __COLLISION_H_

#include <cstdlib>
#include <cassert>
#include <vector>
#include <cmath>

#include "chisq.h"
#include "math_functions.h"
#include "bitvector.h"

namespace ctrandom {

    /// Collision-Test  la Knuth.
    /// TODO: Collision-Test  la Knuth implementieren.
    template <typename VariateType>
    double collision_test(const std::vector<VariateType>& ran, const VariateType _min, const VariateType _max)
    {
        assert(_max > _min);
        assert(ran.size() > 100);
        size_t m = 1048576; // 2^20 urns
        size_t n = 16384;   // 2^14 balls
        size_t collisions = 0;
        const size_t VariateTypeSize = (size_t) round(M_LOG2E * ((double)(_max - _min)));
        BitVector Y(n);
        size_t k = 0;
        for (size_t i = 0; i < n / VariateTypeSize; ++i)
        {
            VariateType x = ran.at(k++);
            for (size_t j = 0; j < VariateTypeSize; ++j)
            {
                if ((x & 1) == 1)
                    Y.set(i * VariateTypeSize + j);
                x >>= 1;
            }
        }
        BitVector c(m);
        return 0.0;
    }

};

#endif // __COLLISION_H_
