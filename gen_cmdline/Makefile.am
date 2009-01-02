bin_PROGRAMS = gen_cmdline


gen_cmdline_SOURCES = gen_cmdline_main.cpp

if HAVE_PADLOCK
gen_cmdline_SOURCES += padlock_rng.cpp
endif


gen_cmdline_LDFLAGS = -L../gen -lrng

DEFAULT_INCLUDES = -I. -I.. -I/opt/boost/include/boost-1_37
