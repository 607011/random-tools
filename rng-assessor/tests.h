// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RNGASSESSOR_TESTS_H_
#define __RNGASSESSOR_TESTS_H_

#include <cstdlib>
#include <vector>

#include "timer/timer.h"

typedef unsigned char variate_t;
typedef std::vector<variate_t> RNGArray;

extern variate_t r_min;
extern variate_t r_max;
extern long r_range;
extern int r_bits;
extern RNGArray r;
extern double alpha;

#ifdef TESTTIMING
extern bool timeIt;
#endif

extern void test_frequencies(void);
extern void test_runs(void);
extern void test_runs_fips(void);
extern void test_serial(void);
extern void test_couponcollector(void);
extern void test_autocorrelation(void);
extern void test_gap(void);
extern void test_poker_knuth(void);
extern void test_poker_fips(void);
extern void test_permutation(void);
extern void test_max_of_t(void);
extern void test_collision(void);
extern void test_birthday_spacings(void);
extern void test_monobit(void);
extern void test_pi(void);
extern void test_entropy(void);
extern void test_bzip2(void);

#endif // __RNGASSESSOR_TESTS_H_
