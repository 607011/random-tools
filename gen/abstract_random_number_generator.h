// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.


#ifndef __ABSTRACTRANDOMNUMBERGENERATOR_H_
#define __ABSTRACTRANDOMNUMBERGENERATOR_H_

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
    };


    typedef RandomNumberGenerator<unsigned long long> Int64RandomNumberGenerator;
    typedef RandomNumberGenerator<unsigned int> IntRandomNumberGenerator;
};

#endif //  __ABSTRACTRANDOMNUMBERGENERATOR_H_
