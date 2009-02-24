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

std::vector<bool> is_prime(10000);
std::vector<unsigned int> primes;


/// Generate prime numbers with Atkin's sieve.
void generate_primes(size_t limit)
{
    if (is_prime.size() < limit)
        is_prime.resize(limit);
    for (size_t i = 0; i < is_prime.size(); ++i)
        is_prime[i] = false;
    size_t n;
    size_t square = (size_t) sqrt((float) limit);
    for (size_t x = 1; x < square; ++x)
    {   
        for (size_t y = 1; y < square; ++y)
        {
            n = 4 * x * x + y * y;
            if (n <= limit && (n % 12 == 1 || n % 12 == 5))
                is_prime[n] = !is_prime[n];

            n = 3 * x * x + y * y;
            if (n <= limit && (n % 12 == 7))
                is_prime[n] = !is_prime[n];

            n = 3 * x * x - y * y;
            if (x > y && n < limit && n % 12 == 11)
                is_prime[n] = !is_prime[n];
        } 
    } 
    is_prime[2] = true;
    is_prime[3] = true;
    for (n = 5; n < square; ++n)
    {
        if (is_prime[n])
        {
            for (size_t k = 1; k * n * n <= limit; ++k)
                is_prime[k * n * n] = false;
        }
    }

    // pack found primes into array
    for (size_t i = 2; i < is_prime.size(); ++i)
        if (is_prime[i])
            primes.push_back(i);
}


/// Find smallest prime divisor for a number.
/// @param x
size_t find_smallest_prime_divisor(size_t x)
{
    for (size_t i = 0; i < primes.size(); ++i)
    {
        if (x % primes[i] == 0)
            return primes[i];
    }
    return 1;
}


/// Find all prime factorials for a number.
/// @param x
/// @param factors
void find_all_prime_factorials(unsigned int x, std::vector<size_t>& factors)
{
    size_t d = x;
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
void make_all_divisors(size_t x, std::set<size_t>& divisors)
{
    std::vector<size_t> factors;
    find_all_prime_factorials(x, factors);
    for (size_t k = 1; k < factors.size(); ++k)
    {
        ctrandom::Combinations<size_t> c(factors, k);
        while (c.hasMore())
        {
            std::vector<size_t> combination = c.next();
            size_t d = 1;
            for (size_t i = 0; i < combination.size(); ++i)
                d *= combination.at(i);
            divisors.insert(d);
        }
    }
    divisors.insert(1);
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
    size_t maxBuckets = r_range;
    generate_primes(1 + (size_t) r_max);
    std::set<size_t> divisors;
    make_all_divisors(1 + (size_t) r_max, divisors);
    for (std::set<size_t>::const_reverse_iterator i = divisors.rbegin(); i != divisors.rend(); ++i)
    {
        size_t divisor = (*i);
        size_t num_buckets = r_range / divisor;
        if ((num_buckets >= 5) && (num_buckets <= maxBuckets))
        {
            double p = ctrandom::frequency_test<variate_t>(r, r_min, r_max, num_buckets);
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
