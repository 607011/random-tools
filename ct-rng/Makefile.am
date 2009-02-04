bin_PROGRAMS = ct-rng

AM_CPPFLAGS = @BOOST_CPPFLAGS@ @GMP_CPPFLAGS@
AM_LDFLAGS = @GMP_LDFLAGS@

ct_rng_SOURCES = ct-rng.cpp
ct_rng_LDFLAGS = -L../gen -lrng

if USE_PADLOCK
ct_rng_SOURCES += padlock_rng.cpp
endif

DEFAULT_INCLUDES = -I. -I..
