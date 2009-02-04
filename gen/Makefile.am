lib_LIBRARIES=librng.a
librng_a_SOURCES=bbs.cpp mersenne_twister.cpp
library_includedir=$(includedir)/gen
library_include_HEADERS=abstract_random_number_generator.h knuth.h mersenne_twister.h bbs.h lcg.h normal_distribution.h circ.h marsaglia.h randomfile.h equi_distribution.h mcg.h

AM_CPPFLAGS = @GMP_CPPFLAGS@
AM_LDFLAGS = @GMP_LDFLAGS@
