// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _WIN32
#define _CRT_RAND_S
#include <cstdlib>
#endif

#include <ctime>
#include <cstdio>
#include <cassert>

#include "mcg.h"
#include "bbs.h"


namespace randomtools {

    namespace helper {

        static unsigned char _ToHex(BBSVariateType x)
        {
            unsigned char c = x & 0x0f;
            if (c < 10)
                return c + (unsigned char)'0';
            else
                return c + (unsigned char)'a' - 10;
        }


        static void _Gen(char* str, size_t n, RandomNumberGenerator<unsigned char>& rng)
        {
            assert(str != NULL);
            for (size_t i = 0; i < n; ++i)
                str[i] = _ToHex(rng());
            str[n] = '\0';
        }


        static void _MakePrime(mpz_t& x, const char* str)
        {
            assert(str != NULL);
            const int ProbabPrimeIterations = 13;
            mpz_init_set_str(x, str, 16);
            do 
            {
                mpz_nextprime(x, x);
            }
            while (!mpz_tstbit(x, 1) && !mpz_probab_prime_p(x, ProbabPrimeIterations));
        }

        static const BBSVariateType bitMul[32] =
        {
            0x00000001, 0x00000002, 0x00000004, 0x00000008,
            0x00000010, 0x00000020, 0x00000040, 0x00000080,
            0x00000100, 0x00000200, 0x00000400, 0x00000800,
            0x00001000, 0x00002000, 0x00004000, 0x00008000,
            0x00010000, 0x00020000, 0x00040000, 0x00080000,
            0x00100000, 0x00200000, 0x00400000, 0x00800000,
            0x01000000, 0x02000000, 0x04000000, 0x08000000,
            0x10000000, 0x20000000, 0x40000000, 0x80000000 
        };

    };

    BlumBlumShub::BlumBlumShub(size_t keyLength)
    {
        MCG rng;
        rng.seed(makeSeed());
        size_t nKeyHexDigits = keyLength / 4;
        char* str = new char[nKeyHexDigits + 1];

        mpz_t p;
        helper::_Gen(str, nKeyHexDigits, rng);
        helper::_MakePrime(p, str);

        mpz_t q;
        helper::_Gen(str, nKeyHexDigits, rng);
        helper::_MakePrime(q, str);

        mpz_init(_Blum);
        mpz_mul(_Blum, p, q);

        _KeyLen = mpz_sizeinbase(_Blum, 2);

        mpz_clear(p);
        mpz_clear(q);

        helper::_Gen(str, nKeyHexDigits, rng);
        mpz_init_set_str(_R, str, 16);

        delete [] str;

        mpz_t gcd;
        mpz_init(gcd);
        for (;;) {
            mpz_gcd(gcd, _Blum, _R);
            if (mpz_cmp_ui(gcd, 1) == 0)
                break;
            mpz_add_ui(_R, _R, 1);
        }
        mpz_clear(gcd);
        mpz_powm_ui(_R, _R, 2, _Blum);
    }


    BBSVariateType BlumBlumShub::operator()() 
    {
        BBSVariateType result = 0;
        const size_t bitsPerVariate = sizeof(BBSVariateType) * 8;
        for (size_t i = 0; i < bitsPerVariate; ++i)
        {
            mpz_powm_ui(_R, _R, 2, _Blum);
            if (mpz_tstbit(_R, 1))
                result |= helper::bitMul[i];
        }
        return result;
    }
};
