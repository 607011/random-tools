// $Id: poker.h,v 845ec36a3b5b 2009/02/05 14:46:01 oliver $
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __POKER_H_
#define __POKER_H_

#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>

#include <boost/math/distributions.hpp>

#include "chisq.h"

namespace ctrandom {

    template <typename VariateType = unsigned int>
    class Hand {
    public:
        static const size_t size = 5;

        Hand(void) { }

        size_t count_pairs(void)
        {
            size_t pairCount = 0;
            for (typename std::map<VariateType, size_t>::const_iterator i = hand.begin(); i != hand.end(); ++i)
                if ((*i).second == 2)
                    ++pairCount;
            return pairCount;
        }

        inline bool all_different(void)
        {
            return hand.size() == size;
        }

        bool has_three(void)
        {
            for (typename std::map<VariateType, size_t>::const_iterator i = hand.begin(); i != hand.end(); ++i)
                if ((*i).second == 3)
                    return true;
            return false;
        }

        bool has_four(void)
        {
            for (typename std::map<VariateType, size_t>::const_iterator i = hand.begin(); i != hand.end(); ++i)
                if ((*i).second == 4)
                    return true;
            return false;
        }

        bool has_five()
        {
            for (typename std::map<VariateType, size_t>::const_iterator i = hand.begin(); i != hand.end(); ++i)
                if ((*i).second == 5)
                    return true;
            return false;
        }

        inline VariateType& operator[](size_t i)
        {
            return hand[i];
        }

    private:
        std::map<VariateType, size_t> hand;
    };


    template <typename VariateType>
    double poker_test(const std::vector<VariateType>& r)
    {
        enum _pokerprobs { AllDifferent = 0, OnePair, TwoPairs, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind, ProbCount };
        std::vector<size_t> histo(ProbCount);
        std::vector<size_t> expected(ProbCount);
        double chunkCount = (double) r.size() / (double) Hand<VariateType>::size;
        expected[AllDifferent] /* ABCDE */ = (size_t) (chunkCount * 0.961468583);
        expected[OnePair]      /* AABCD */ = (size_t) (chunkCount * 0.038153515);
        expected[TwoPairs]     /* AABBC */ = (size_t) (chunkCount * 1.508044079e-4);
        expected[ThreeOfAKind] /* AAABC */ = (size_t) (chunkCount * 1.525879e-5);
        expected[FullHouse]    /* AAABB */ = (size_t) (chunkCount * 5.937181413e-7);
        expected[FourOfAKind]  /* AAAAB */ = (size_t) (chunkCount * 5.960464e-8);
        expected[FiveOfAKind]  /* AAAAA */ = (size_t) (chunkCount * 2.328306e-10);
        for (size_t i = 0; i < r.size() - Hand<VariateType>::size; i += Hand<VariateType>::size)
        {
            Hand<VariateType> hand;
            for (size_t j = 0; j < Hand<VariateType>::size; ++j)
            {
                VariateType v = r.at(i+j);
                ++hand[v];
            }
            size_t pairCount = hand.count_pairs();
            if (hand.all_different()) {
                ++histo[AllDifferent];
            }
            else if (pairCount == 2) {
                ++histo[TwoPairs];
            }
            else if (pairCount == 1) {
                if (hand.has_three())
                    ++histo[FullHouse];
                else
                    ++histo[OnePair];
            }
            else if (hand.has_three()) {
                ++histo[ThreeOfAKind];
            }
            else if (hand.has_four()) {
                ++histo[FourOfAKind];
            }
            else if (hand.has_five()) {
                ++histo[FiveOfAKind];
            }
        }
        boost::math::chi_squared chi(ProbCount-1);
        double res = chisq<size_t>(histo, expected);
        double p = boost::math::cdf(chi, res);
        return p;
    }

};

#endif // __POKER_H_
