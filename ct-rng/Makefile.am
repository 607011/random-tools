bin_PROGRAMS = ct-rng

AM_CPPFLAGS=@BOOST_CPPFLAGS@

ct_rng_SOURCES = ct-rng.cpp

if HAVE_PADLOCK
ct_rng_SOURCES += padlock_rng.cpp
endif

ct_rng_LDFLAGS = -L../gen -lrng

DEFAULT_INCLUDES = -I. -I..
