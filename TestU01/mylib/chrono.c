/*************************************************************************\
 *
 * Package:        MyLib
 * File:           chrono.c
 * Environment:    ANSI C
 *
 * Copyright (c) 2002 Pierre L'Ecuyer, DIRO, Université de Montréal.
 * e-mail: lecuyer@iro.umontreal.ca
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted without a fee for private, research,
 * academic, or other non-commercial purposes.
 * Any use of this software in a commercial environment requires a
 * written licence from the copyright owner.
 *
 * Any changes made to this package must be clearly identified as such.
 *
 * In scientific publications which used this software, a reference to it
 * would be appreciated.
 *
 * Redistributions of source code must retain this copyright notice
 * and the following disclaimer.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
\*************************************************************************/

#include "Windows.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "chrono.h"
#include "gdef.h"
#include "util.h"
#include "num.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_WINDOWS_H

#include <windows.h>

static HANDLE currentProcess = NULL;

/*
 * A helper function for converting FILETIME to a LONGLONG [safe from memory
 * alignment point of view].
 */
static ULONGLONG
fileTimeToInt64 (const FILETIME * time)
{
    ULARGE_INTEGER _time;
    _time.LowPart = time->dwLowDateTime;
    _time.HighPart = time->dwHighDateTime;
    return _time.QuadPart;
}

static void Heure (unsigned long *tsec, unsigned long *tusec) {
   if (currentProcess == NULL)
      currentProcess = GetCurrentProcess();
   FILETIME creationTime, exitTime, kernelTime, userTime;
   /* Strongly inspired from
    * http://www.javaworld.com/javaworld/javaqa/2002-11/01-qa-1108-cpu.html */
   GetProcessTimes (currentProcess, &creationTime, &exitTime,
		   &kernelTime, &userTime);
   ULONGLONG rawTime = fileTimeToInt64 (&kernelTime) +
     fileTimeToInt64 (&userTime);
   /* We have to divide by 10000 to get milliseconds out of
    * the computed time */
   *tsec = (unsigned long)(rawTime / 10000000);
   *tusec = (unsigned long)((rawTime % 10000000) / 10);
}


#elif defined(USE_ANSI_CLOCK)
/* ANSI C timer */

static void Heure (
   unsigned long *tsec,            /* Seconds */
   unsigned long *tmicrosec        /* Micro-seconds */
   )
/* 
 * Function returning the CPU time used by a program since it was
 * started. This function is ANSI C compliant.
 */
{
   clock_t t;
   double y;

   t = clock ();
   y = ((double) t) / CLOCKS_PER_SEC;
   *tsec = y;
   *tmicrosec = (y - *tsec) * 1000000;
}


#else
/* POSIX timer */

// #include <sys/time.h>

static void Heure (unsigned long *tsec, unsigned long *tusec)
/*
 * Function returning the CPU time used by a program since it was
 * started. This function is NOT ANSI C compliant.
 */
{
    FILETIME idle;
    FILETIME kernel;
    FILETIME user;
    ULARGE_INTEGER t;
    GetSystemTimes(&idle, &kernel, &user);
    t.LowPart = user.dwLowDateTime;
    t.HighPart = user.dwHighDateTime;
    *tsec = (unsigned long) (t.QuadPart / 10000000ULL);
    *tusec = (unsigned long) ((t.QuadPart - 10000000ULL * (t.QuadPart / 10000000ULL)) / 10ULL);
}

#endif


/*------------------------------------------------------------------------*/

void chrono_Init (chrono_Chrono *C)
{
   Heure (&C->second, &C->microsec);
}


chrono_Chrono * chrono_Create (void)
{
   chrono_Chrono *C;
   C = util_Malloc (sizeof (chrono_Chrono));
   Heure (&C->second, &C->microsec);
   return C;
}


void chrono_Delete (chrono_Chrono *C)
{
   util_Free (C);
}


double chrono_Val (chrono_Chrono *C, chrono_TimeFormat Unit)
{
   double temps;                     /* Time elapsed, in seconds */
   chrono_Chrono now;
   Heure (&now.second, &now.microsec);
   temps = (((double) now.microsec - (double) C->microsec) / 1.E+6 +
             (double) now.second) - (double) C->second;

   switch (Unit) {
   case chrono_sec:
      return temps;
   case chrono_min:
      return temps * 1.666666667E-2;
   case chrono_hours:
      return temps * 2.777777778E-4;
   case chrono_days:
      return temps * 1.157407407E-5;
   case chrono_hms:
      util_Error ("chrono_Val : hms is a wrong arg for chrono_TimeUnit");
   }
   return 0.0;
}

void chrono_Write (chrono_Chrono * C, chrono_TimeFormat Form)
{
   long centieme;
   long minute;
   long heure;
   long seconde;
   double temps;
   if (Form != chrono_hms)
      temps = chrono_Val (C, Form);
   else
      temps = 0.0;
   switch (Form) {
   case chrono_sec:
      num_WriteD (temps, 10, 2, 1);
      printf (" seconds");
      break;
   case chrono_min:
      num_WriteD (temps, 10, 2, 1);
      printf (" minutes");
      break;
   case chrono_hours:
      num_WriteD (temps, 10, 2, 1);
      printf (" hours");
      break;
   case chrono_days:
      num_WriteD (temps, 10, 2, 1);
      printf (" days");
      break;
   case chrono_hms:
      temps = chrono_Val (C, chrono_sec);
      heure = (long) (temps * 2.777777778E-4);
      if (heure > 0)
         temps -= (double) (heure) * 3600.0;
      minute = (long) (temps * 1.666666667E-2);
      if (minute > 0)
         temps -= (double) (minute) * 60.0;
      seconde = (long) (temps);
      centieme = (long) (100.0 * (temps - (double) (seconde)));
      printf ("%02ld:", heure);
      printf ("%02ld:", minute);
      printf ("%02ld.", seconde);
      printf ("%02ld", centieme);
      break;
   }
}
