// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.


#ifndef __ABSTRACTRANDOMNUMBERGENERATOR_H_
#define __ABSTRACTRANDOMNUMBERGENERATOR_H_

#ifdef _WIN32
#define _CRT_RAND_S
#include <cstdlib>
#endif

#include <limits>

namespace ctrandom {

    template <typename VariateType>
    class RandomNumberGenerator
    {
    public:
        RandomNumberGenerator(void) {}
        virtual ~RandomNumberGenerator() {}
        virtual VariateType operator()(void) = 0;
        virtual void seed(VariateType) { }
        virtual inline VariateType min(void) const { return std::numeric_limits<VariateType>::min(); }
        virtual inline VariateType max(void) const { return std::numeric_limits<VariateType>::max(); }

        static VariateType makeSeed(void);
    };


    template <typename VariateType>
    VariateType RandomNumberGenerator<VariateType>::makeSeed(void)
    {
        VariateType seed;
#ifdef _WIN32
        rand_s((unsigned int*)&seed);
#else 
        FILE *fp = fopen("/dev/urandom", "r");
        if (fp != NULL)
        {
            fread(&seed, sizeof(VariateType), 1, fp);
            fclose(fp);
        }
        else
        {
            seed = (VariateType) time((time_t)0));
        }
#endif
        return seed;
    }


    typedef RandomNumberGenerator<unsigned long long> Int64RandomNumberGenerator;
    typedef RandomNumberGenerator<unsigned int> IntRandomNumberGenerator;
};

#endif //  __ABSTRACTRANDOMNUMBERGENERATOR_H_
