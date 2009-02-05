bin_PROGRAMS = rng-assessor

AM_CPPFLAGS = @BOOST_CPPFLAGS@
AM_LDFLAGS = -L../test -lrngtest -lbz2

rng_assessor_SOURCES  = rng-assessor.cpp 
rng_assessor_SOURCES += test-birthdayspacings.cpp
rng_assessor_SOURCES += test-frequency.cpp
rng_assessor_SOURCES += test-monte-carlo-pi.cpp
rng_assessor_SOURCES += test-runs.cpp
rng_assessor_SOURCES += test-collision.cpp
rng_assessor_SOURCES += test-gap.cpp
rng_assessor_SOURCES += test-permutation.cpp
rng_assessor_SOURCES += test-runs-fips.cpp
rng_assessor_SOURCES += test-couponcollector.cpp
rng_assessor_SOURCES += test-maxt.cpp
rng_assessor_SOURCES += test-poker.cpp
rng_assessor_SOURCES += test-serial.cpp
rng_assessor_SOURCES += test-entropy.cpp
rng_assessor_SOURCES += test-monobit.cpp
rng_assessor_SOURCES += test-poker-fips.cpp
rng_assessor_SOURCES += test-autocorrelation.cpp
rng_assessor_SOURCES += test-bzip2.cpp

DEFAULT_INCLUDES = -I. -I..


distclean-local:



