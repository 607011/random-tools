// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __RNGASSESS_TESTS_H_
#define __RNGASSESS_TESTS_H_

#include <cstdlib>
#include <vector>

extern size_t r_min;
extern size_t r_max;
extern size_t r_range;
extern size_t r_bits;
extern std::vector<size_t> r;
extern double alpha;

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
extern void test_compression(void);

#endif // __RNGASSESS_TESTS_H_
