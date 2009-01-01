// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include "StdAfx.h"

#ifdef BENCHMARK
#include <Windows.h>
LARGE_INTEGER freq;
LARGE_INTEGER t0;
#endif


void start(void)
{
#ifdef BENCHMARK
	QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);
#endif
}


void stop(void)
{
#ifdef BENCHMARK
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	long long duration = 1000LL * (t.QuadPart - t0.QuadPart) / freq.QuadPart;
    std::cout << duration << " ms " << std::endl;
#endif
}
