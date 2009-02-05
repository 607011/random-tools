// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.
// Portiert von http://www.agner.org/random/

#ifndef __MULTIPLY_WITH_CARRY_H_
#define __MULTIPLY_WITH_CARRY_H_

#include "abstract_random_number_generator.h"

#undef _USE_INTRINSICS


#ifdef _USE_INTRINSICS
#include <intrin.h>
#endif

namespace ctrandom {

    class MultiplyWithCarry : public RandomNumberGenerator<unsigned int>
    {
    public:
        MultiplyWithCarry(unsigned int _Seed = 0x9908b0df)
        {
#ifdef _USE_INTRINSICS
            _R.R[3] = 2111111111;
            _R.R[2] = 1492ULL;
            _R.R[1] = 1776ULL;
            _R.R[0] = 5115ULL;
#endif
            seed(_Seed);
        }
#ifdef _USE_INTRINSICS
        unsigned int operator()()
        {
            // PMULUDQ mit 128-Bit-Operanden:
            //   DEST[63:0]   <- DEST[31:0] * SRC[31:0]
            //   DEST[127:64] <- DEST[95:64] * SRC[95:64]
            __m128i sum1 = _mm_mul_epu32(_C.c.coeff01, _R.r.r01);
            __m128i sum2 = _mm_mul_epu32(_C.c.coeff23, _R.r.r23);
            __m128i sum = _mm_
            // sum += _R.r.r4;
            _R.R[3] = _R.R[2];
            _R.R[2] = _R.R[1];
            _R.R[1] = _R.R[0];
            _R.R[4] = sum >> 32;
            _R.R[0] = sum & 0xffffffffLL;
            return (unsigned int) _R.R[0];
        }
#else
        unsigned int operator()()
        {
            // TODO: Implementierung in x86-Assembler: PMULUDQ bzw. 0x9908b0df

            unsigned long long sum =
                2111111111ULL * (unsigned long long) _R[3] +
                1492ULL       * (unsigned long long) _R[2] +
                1776ULL       * (unsigned long long) _R[1] +
                5115ULL       * (unsigned long long) _R[0] +
                                (unsigned long long) _R[4];
            _R[3] = _R[2];
            _R[2] = _R[1];
            _R[1] = _R[0];
            _R[4] = (unsigned int) (sum >> 32);
            _R[0] = (unsigned int) (sum & 0xffffffffLL);
            return _R[0];
        }
#endif
        inline void seed(unsigned int _Seed)
        { 
            warmup(_Seed);
        }

    private:
        inline void warmup(unsigned int s)
        {
            for (size_t i = 0; i < 5; ++i)
            {
                s = s * 29943829 - 1;
                _R[i] = s;
            }
            for (size_t i = 0; i < 19; ++i)
                (*this)();
        }
#ifdef _USE_INTRINSICS
        __declspec(align(16)) __m128i coeff01;
        __declspec(align(16)) __m128i coeff23;
        __declspec(align(16)) union _urt
        {
            __int64 R[5];
            struct _srt
            {
                __m128i r01;
                __m128i r23;
                __m64   r4;
            } r;
        } _R;
#else
        unsigned int _R[5];
#endif
    };

};


#endif // __MULTIPLY_WITH_CARRY_H_

