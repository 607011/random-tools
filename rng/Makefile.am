bin_PROGRAMS = rng

AM_CPPFLAGS = @BOOST_CPPFLAGS@ @GMP_CPPFLAGS@
AM_LDFLAGS = @GMP_LDFLAGS@

rng_SOURCES = rng.cpp
rng_LDFLAGS = -L../gen -lrng

if USE_PADLOCK
rng_SOURCES += padlock_rng.cpp
endif

DEFAULT_INCLUDES = -I. -I..
