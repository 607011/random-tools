// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <set>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "rng-assessor.h"
#include "test/frequency.h"
#include "test/combinations.h"

static std::vector<bool> is_prime(10000);
static std::vector<long> primes;


/// Generate prime numbers with Atkin's sieve.
void generate_primes(long limit)
{
    if ((long) is_prime.size() < limit)
        is_prime.resize(limit);
    for (long i = 0L; i < (long)is_prime.size(); ++i)
        is_prime[i] = false;
    long n;
    long square = (long) sqrt((float) limit);
    for (long x = 1L; x < square; ++x)
    {   
        for (long y = 1L; y < square; ++y)
        {
            n = 4 * x * x + y * y;
            if ((n <= limit) && (n % 12L == 1L || n % 12L == 5L))
                is_prime[n] = !is_prime[n];

            n = 3L * x * x + y * y;
            if (n <= limit && (n % 12 == 7))
                is_prime[n] = !is_prime[n];

            n = 3L * x * x - y * y;
            if ((x > y) && (n < limit) && (n % 12L == 11L))
                is_prime[n] = !is_prime[n];
        } 
    } 
    is_prime[2] = true;
    is_prime[3] = true;
    for (n = 5L; n < square; ++n)
        if (is_prime[n])
            for (long k = 1L; k * n * n <= limit; ++k)
                is_prime[k * n * n] = false;

    // pack found primes into array
    for (long i = 2L; i < (long)is_prime.size(); ++i)
        if (is_prime[i])
            primes.push_back(i);
}


/// Find smallest prime divisor for a number.
/// @param x
long find_smallest_prime_divisor(long x)
{
    for (long i = 0L; i < (long)primes.size(); ++i)
    {
        if (x % primes[i] == 0L)
            return primes[i];
    }
    return 1L;
}


/// Find all prime factorials for a number.
/// @param x
/// @param factors
void find_all_prime_factorials(long x, std::vector<long>& factors)
{
    long d = x;
    while (!is_prime[d])
    {
        size_t fac = find_smallest_prime_divisor(d);
        factors.push_back(fac);
        d /= fac;
    }
    if (d != x)
        factors.push_back(d);
}


/// Determine all divisors for a number.
/// @param x
/// @param divisors 
void make_all_divisors(long x, std::set<long>& divisors)
{
    std::vector<long> factors;
    find_all_prime_factorials(x, factors);
    for (int k = 1; k < (int)factors.size(); ++k)
    {
        randomtools::Combinations<long> c(factors, k);
        while (c.hasMore())
        {
            std::vector<long> combination = c.next();
            long d = 1L;
            for (long i = 0L; i < (long) combination.size(); ++i)
                d *= combination.at(i);
            divisors.insert(d);
        }
    }
    divisors.insert(1L);
}



////////////////////////////////////////////////////////////
/////
///// FREQUENCY TEST a.k.a. EQUIDISTRIBUTION TEST
/////
////////////////////////////////////////////////////////////
void test_frequencies(void)
{
    if (!quiet)
        std::cout << "FREQUENCY TEST" << std::endl;
    long maxBuckets = (long) r_range;
    generate_primes(1 + (long) r_max);
    std::set<long> divisors;
    make_all_divisors(1 + (long) r_max, divisors);
    for (std::set<long>::const_reverse_iterator i = divisors.rbegin(); i != divisors.rend(); ++i)
    {
        long divisor = (*i);
        long num_buckets = r_range / divisor;
        if ((num_buckets >= 5) && (num_buckets <= maxBuckets))
        {
            double p = randomtools::frequency_test<variate_t>(r, r_min, r_max, num_buckets);
            if (!quiet)
                std::cout << " ... " << std::flush
                          << std::setw(4) << std::right << num_buckets << " Klassen: " << std::flush
                          << "p = " << std::setprecision(5) << std::setw(9) << std::left << p << " "
                          << " " << ((alpha < p)? "OK" : "NICHT BESTANDEN") << '.' << std::endl;
        }
    }
    if (!quiet)
        std::cout << std::endl;
}
