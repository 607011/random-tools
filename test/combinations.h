// $Id: combinations.h,v 6453a4c3269e 2009/02/25 13:55:36 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __COMBINATIONS_H_
#define __COMBINATIONS_H_

#include <cstdlib>
#include <cassert>
#include <vector>

#include "math_functions.h"

namespace randomtools {

    /// Eine Klasse, um k Elemente aus einer Menge von n Elementen ohne Wiederholungen herauszupicken.
    template <typename T>
    class Combinations {
    public:
        /// @param s Die Menge, aus der die Elemente kombiniert werden sollen.
        /// @param k Die Anzahl der Elemente, die kombiniert werden sollen.
        Combinations(const std::vector<T>& s, size_t k) : _S(s), _K(k)
        {
            _N = s.size();
            assert(k <= _N);
            assert(k > 0);
            _Total = (size_t) choose<unsigned long long>(_N, _K);
            a.resize(k);
            reset();
        }

        /// Abfrage, ob noch mehr Kombinationen verfügbar sind.
        /// @return true, wenn noch mehr Kombinationen verfügbar sind.
        inline bool hasMore(void) 
        {
            return _Left > 0;
        }

        /// Nächste Kombination ermitteln.
        /// @return nächste Kombination
        std::vector<T> next(void)
        {
            std::vector<T> result;
            if (hasMore())
            {
                if (_Left-- == _Total)
                    return _S;
                size_t i = _K - 1;
                while (a.at(i) == _S.size() - _K + i)
                    --i;
                ++a[i];
                for (size_t j = i + 1; j < _K; ++j)
                    a[j] = a[i] + j - i;
                for (i = 0; i < _K; ++i)
                    result.push_back(_S.at(a[i]));
            }
            return result;
        }

    private:
        const std::vector<T>& _S;
        size_t _N;
        size_t _K;
        size_t _Left;
        size_t _Total;
        std::vector<size_t> a;

    private:
        void reset(void)
        {
            for (size_t i = 0; i < a.size(); ++i)
                a[i] = i;
            _Left = _Total;
        }
    };

};


#endif // __COMBINATIONS_H_
