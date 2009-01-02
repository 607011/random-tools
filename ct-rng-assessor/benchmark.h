// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __BENCHMARK_H_
#define __BENCHMARK_H_

#undef BENCHMARK

#ifdef BENCHMARK
#include <iostream>
#include <string>
#endif

extern void start(void);
extern void stop(void);

#endif // __BENCHMARK_H_
