// $Id: timer.cpp,v 6453a4c3269e 2009/02/25 13:55:36 oliver $

#include "timer.h"

#ifdef _WIN32

/*****************/
/* Windows stuff */
/*****************/

#include <windows.h>

LONGLONG tickFreq = 0;

LONGLONG initTimer()
{
   LARGE_INTEGER freq;
   QueryPerformanceFrequency(&freq);
   return freq.QuadPart;
}

__inline
LONGLONG stopTimer(TIMER *t)
{
   TIMER start;
   start.QuadPart = t->QuadPart;
   QueryPerformanceCounter(t);
   t->QuadPart -= start.QuadPart;
   if (tickFreq == 0)
      tickFreq = initTimer();
   return T_RESULT_RESOLUTION * t->QuadPart / tickFreq;
}

#else

/**************/
/* Unix stuff */
/**************/

/* used by startTimer() macro */
struct timezone tz = {0, 0};


/* convert struct timeval to unsigned long long
   NOTE: LONGLONG is incompatible to ANSI-C
*/
LONGLONG
tv2l(TIMER *t)
{
  return 1000000LL * (LONGLONG) t->tv_sec + (LONGLONG) t->tv_usec;
}


/* start timer */
inline void
startTimer(TIMER *t)
{
  gettimeofday(t, &tz);
}


/* stop timer */
inline LONGLONG
stopTimer(TIMER *t)
{
  TIMER start;
  start.tv_sec = t->tv_sec;
  start.tv_usec = t->tv_usec;
  gettimeofday(t, &tz);
  /* return result in milliseconds */
  return (tv2l(t) - tv2l(&start)) / 1000LL;
}

#endif /* WIN32 */
