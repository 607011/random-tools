// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/array.hpp>
#include <boost/random.hpp>
#include <boost/math/distributions.hpp>
#include <boost/math/special_functions.hpp>

#include "gen/normal_distribution.h"
#include "gen/equi_distribution.h"
#include "gen/mersenne_twister.h"
#include "gen/randomfile.h"
#include "gen/lcg.h"
#include "test/chisq.h"
#include "test/frequency.h"

#define INTEGER

#ifdef INTEGER
typedef unsigned int VariateType;
#else
typedef double VariateType;
#endif

//typedef ctrandom::MT800 MTGenerator;
typedef ctrandom::MersenneTwister MTGenerator;
//typedef ctrandom::LCG_ANSIC MTGenerator;
//typedef ctrandom::BinaryFile<VariateType> MTGenerator;

int main(int argc, char* argv[])
{
    int count = (argc == 2)? atoi(argv[1]) : 100000;

    MTGenerator gen;
    //gen.open("../data/Hotbits.dat");
    //if (!gen.stream().is_open()) {
    //    std::cout << "geht nett" << std::endl;
    //    return 1;
    //}

#ifdef INTEGER
    // ctrandom::EquiDistribution<MTGenerator, VariateType> rng(gen, 0, 100);
    ctrandom::EquiDistribution<MTGenerator, VariateType> rng(gen, gen.min(), gen.max());
#else
    ctrandom::EquiDistribution<MTGenerator, VariateType> rng(gen);
#endif

    // Zufallszahlen erzeugen
    std::vector<VariateType> r(count);
    for (int i = 0; i < count; ++i)
        r[i] = rng();

    for (int i = 0; i < count; ++i)
        std::cout << r[i] << ' ';

    // boost::array<double, 13> p = { .995, .990, .975, .950, .900, .750, .500, .250, .100, .050, .025, .010, .005 };
    boost::array<double, 7> p = { { .99, .95, .75, .5, .25, .05, .01 } };
    for (unsigned int i = 0; i < p.size(); ++i)
        std::cout << '\t' << p[i];
    std::cout << std::endl;

    for (unsigned int v = 1; v < 10; ++v)
    {
        boost::math::chi_squared_distribution<double> chi(v);
        std::cout << v << '\t' << std::setprecision(4);
        for (unsigned int i = 0; i < p.size(); ++i)
            std::cout << boost::math::quantile(chi, p[i]) << '\t';
        std::cout << std::endl;
    }




    return 0;
}
