// $Id: timer.h,v 6453a4c3269e 2009/02/25 13:55:36 oliver $

#ifndef __TIMER_H_
#define __TIMER_H_

#define T_RESULT_RESOLUTION       (1000ULL)

#ifdef _WIN32
#include <windows.h>
typedef LARGE_INTEGER TIMER;
extern LONGLONG tickFreq;

#define startTimer(t) QueryPerformanceCounter(t)

#else

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#ifdef __GNUC__
#include <sys/time.h>
#endif

typedef struct timeval TIMER;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;

extern struct timezone tz;

extern LONGLONG tv2l(TIMER *);
extern void     startTimer(TIMER *t);

#endif /* WIN32 */

extern LONGLONG stopTimer(TIMER *);

#ifdef TESTTIMING
#define START()           startTimer(&stopwatch)
#define STOP(result)      result = stopTimer(&stopwatch)
#else
#define START()
#define STOP(result)
#endif // TESTTIMING

#endif // __TIMER_H_
