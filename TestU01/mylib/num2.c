/*************************************************************************\
 *
 * Package:        MyLib
 * File:           num2.c
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

#include "num2.h"
#include "util.h"
#include "num.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>

#define EPSILON  1.0E-15
#define MAXI 50


/*=========================================================================*/

double num2_Factorial (int n)
{
   double T;
   int j;
   T = 1.0;
   for (j = 1; j <= n; j++) {
      T = T * (double) j;
   }
   return T;
}

/*=========================================================================*/

#define nlim 14

double num2_LnFactorial (int n)
{
   double x, y, z;
   int i;
   util_Assert (n >= 0, "LnFactorielle: n negatif");
   if (n == 0 || n == 1)
      return 0.0;
   if (n <= nlim) {
      z = 1.0;
      x = 1.0;
      for (i = 2; i <= n; i++) {
         x = x + 1.0;
         z = z * x;
      }
      return log (z);
   } else {
      x = (double) (n + 1);
      y = 1.0 / (x * x);
      z = ((-(5.95238095238E-4 * y) + 7.936500793651E-4) * y -
         2.7777777777778E-3) * y + 8.3333333333333E-2;
      z = ((x - 0.5) * log (x) - x) + 9.1893853320467E-1 + z / x;
      return z;
   }
}

/*=========================================================================*/
#ifndef HAVE_LGAMMA

/* The new standard ISO_C99 includes the lgamma function in math.h;
   otherwise, we shall have to use our own. */

double num2_LnGamma (double x)
{
   const double xlimbig = 1.0 / DBL_EPSILON;
   const double xlim1 = 18.0;
   const double dk2 = 0.91893853320467274178; /* Ln (sqrt (2 Pi)) */
   const double dk1 = 0.9574186990510627;
   const int N = 15;              /* Degree of Chebyshev polynomial */
   double y = 0, z = 0;
   int i, k;

   /* Chebyshev coefficients for lnGamma (x + 3), 0 <= x <= 1 In Yudell
      Luke: The special functions and their approximations, Vol. II,
      Academic Press, p. 301, 1969. There is an error in the additive
      constant in the formula: (Ln (2)). */
   static const double A[] = {
      0.52854303698223459887,
      0.54987644612141411418,
      0.02073980061613665136,
      -0.00056916770421543842,
      0.00002324587210400169,
      -0.00000113060758570393,
      0.00000006065653098948,
      -0.00000000346284357770,
      0.00000000020624998806,
      -0.00000000001266351116,
      0.00000000000079531007,
      -0.00000000000005082077,
      0.00000000000000329187,
      -0.00000000000000021556,
      0.00000000000000001424,
      -0.00000000000000000095
   };

   util_Assert (x > 0.0, "num2_LnGamma:   accepts only x > 0");
   if (x > xlim1) {
      if (x > xlimbig)
         y = 0.0;
      else
         y = 1.0 / (x * x);
      z = ((-(5.95238095238E-4 * y) + 7.936500793651E-4) * y -
         2.7777777777778E-3) * y + 8.3333333333333E-2;
      z = ((x - 0.5) * log (x) - x) + dk2 + z / x;
      return z;

   } else if (x > 4.0) {
      k = x;
      z = x - k;
      y = 1.0;
      for (i = 3; i < k; i++)
         y *= z + i;
      y = log (y);

   } else if (x <= 0.0) {
      return DBL_MAX;

   } else if (x < 3.0) {
      k = x;
      z = x - k;
      y = 1.0;
      for (i = 2; i >= k; i--)
         y *= z + i;
      y = -log (y);

   } else {                       /* 3 <= x <= 4 */
      z = x - 3.0;
      y = 0.0;
   }

   z = num2_EvalCheby (A, N, 2.0 * z - 1.0);
   return z + dk1 + y;
}

#endif
/*=========================================================================*/

#define slim 15                   /* pour eviter les debordements */

double num2_Combination (int n, int s)
{
   double Res;
   int i;
   int Diff;
   if (s == 0 || s == n)
      return 1.0;
   if (s < 0) {
      util_Warning (1, "num2_Combination:   s < 0");
      return 0.0;
   }
   if (s > n) {
      util_Warning (1, "num2_Combination:   s > n");
      return 0.0;
   }
   if (s > (n / 2))
      s = n - s;
   if (s <= slim) {
      Res = 1.0;
      Diff = n - s;
      for (i = 1; i <= s; i++) {
         Res = (Res * (double) (Diff + i)) / (double) (i);
      }
      return Res;
   } else {
      Res = (num2_LnFactorial (n) - num2_LnFactorial (s))
         - num2_LnFactorial (n - s);
      return exp (Res);
   }
}


/*=========================================================================*/
#ifndef HAVE_LOG1P

double num2_log1p (double x) 
{
   /* returns a value equivalent to log (1 + x) accurate also for small x. */
   if (fabs(x) > 0.1) {
      return log (1.0 + x);
   } else {
      double term = x;
      double sum = x;
      int s = 2;
      while (fabs(term) > EPSILON*fabs(sum) && s < MAXI) {
         term *= -x;
         sum += term/s;
         s++;
      }
      return sum;
   }
}

#endif
/*=========================================================================*/

void num2_CalcMatStirling (double ***M, int m, int n)
/* Calcul des elements de la matrice MatStirling [0..m, 0..n]. */
{
   int i, j, k;
   *M = util_Calloc ((size_t) (m + 1), sizeof (double *));
   for (i = 0; i <= m; i++)
      (*M)[i] = util_Calloc ((size_t) (n + 1), sizeof (double));

   for (i = 0; i <= m; i++) {
      for (j = 0; j <= n; j++) {
         (*M)[i][j] = 0.0;
      }
   }

   (*M)[0][0] = 1.0;
   for (j = 1; j <= n; j++) {
      (*M)[0][j] = 0.0;
      if (j <= m) {
         k = j - 1;
         (*M)[j][j] = 1.0;
      } else
         k = m;
      for (i = 1; i <= k; i++) {
         (*M)[i][j] = (double) (i) * (*M)[i][j - 1] + (*M)[i - 1][j - 1];
      }
   }
}


/*=========================================================================*/

void num2_FreeMatStirling (double ***M, int m)
{
   int i;
   for (i = 0; i <= m; i++)
      free ((*M)[i]);
   free (*M);
   *M = NULL;
}


/*=========================================================================*/
#if 0
double num2_VolumeSphere (int p, int k)
/* Calcul du volume de la sphere de rayon 1 en dimension k, norme p.     */
{
   int s;
   double kLR = (double) k;
   double pLR = (double) p;
   double Vol;

   util_Assert (p >= 0, "num2_VolumeSphere:   p < 0");
   switch (p) {
   case 0:
      Vol = num_TwoExp[k];
      break;
   case 1:
      Vol = num_TwoExp[k] / num2_Factorial (k);
      break;
   case 2:
      if ((k % 2) == 0) {
         Vol = pow (num_Pi, kLR / 2.0) / num2_Factorial (k / 2);
      } else {
         s = (k + 1) / 2;
         Vol = pow (num_Pi, (double) (s) - 1.0) * num2_Factorial (s) *
            num_TwoExp[2 * s] / num2_Factorial (2 * s);
      }
      break;
   default:
      Vol = pow (2.0 * exp (num2_LnGamma (1.0 + 1.0 / pLR)), kLR) /
         exp (num2_LnGamma (1.0 + kLR / pLR));
      break;
   }
   return Vol;
}
#endif

/*=========================================================================*/

double num2_VolumeSphere (double pLR, int k)
/* Returns volume of unit sphere in dimension k, norm p */
{
   const double eps = 2.0 * DBL_EPSILON;
   int p = pLR;
   double kLR = (double) k;
   double Vol;
   int s;

   util_Assert (pLR >= 0.0, "num2_VolumeSphere:   p < 0");
   if (fabs (pLR - p) <= eps) {
      switch (p) {
      case 0:
         return num_TwoExp[k];
         break;
      case 1:
         return num_TwoExp[k] / num2_Factorial (k);
         break;
      case 2:
         if ((k % 2) == 0) {
            return pow (num_Pi, kLR / 2.0) / num2_Factorial (k / 2);
         } else {
            s = (k + 1) / 2;
            return pow (num_Pi, (double) (s) - 1.0) * num2_Factorial (s) *
               num_TwoExp[2 * s] / num2_Factorial (2 * s);
         }
         break;
      default:
         break;
      }
   }
   Vol = kLR * (num_Ln2 + num2_LnGamma (1.0 + 1.0 / pLR)) -
      num2_LnGamma (1.0 + kLR / pLR);
   return exp (Vol);
}


/*=========================================================================*/

double num2_EvalCheby (const double A[], int N, double x)
{
   int j;
   double xx;
   double b0, b1, b2;
   util_Warning (fabs (x) > 1.0,
      "Chebychev polynomial evaluated at x outside [-1, 1]");
   xx = 2.0 * x;
   b0 = 0.0;
   b1 = 0.0;
   for (j = N; j >= 0; j--) {
      b2 = b1;
      b1 = b0;
      b0 = (xx * b1 - b2) + A[j];
   }
   return (b0 - b2) / 2.0;
}


/*=========================================================================*/

#define DegOfPoly 6

double num2_BesselK025 (double x)
{
   double rac;
   double xx;
   double temp;
   double Res;
   double C;
   double B;
   int j;
   static const double c[8] = {
      32177591145.0,
      2099336339520.0,
      16281990144000.0,
      34611957596160.0,
      26640289628160.0,
      7901666082816.0,
      755914244096.0
   };

   static const double b[8] = {
      75293843625.0,
      2891283595200.0,
      18691126272000.0,
      36807140966400.0,
      27348959232000.0,
      7972533043200.0,
      755914244096.0
   };

   if (x < 1.E-300)
      return DBL_MAX;

   /*------------------------------------------------------------------*/
   /* x > 0.6 => approximation asymptotique rationnelle dans Luke: */
   /* Yudell L.Luke "Mathematical functions and their approximations", */
   /* Academic Press Inc. New York, 1975, p.371 */
   /*------------------------------------------------------------------*/
   if (x >= 0.6) {
      B = b[DegOfPoly];
      C = c[DegOfPoly];
      for (j = DegOfPoly; j >= 1; j--) {
         B = B * x + b[j - 1];
         C = C * x + c[j - 1];
      }
      Res = sqrt (num_Pi / (2.0 * x)) * exp (-x) * (C / B);
      return Res;
   }

   /*------------------------------------------------------------------*/
   /* x < 0.6 => la serie de K_{1/4} = Pi/Sqrt(2) [I_{-1/4} - I_{1/4}] */
   /*------------------------------------------------------------------*/
   xx = x * x;
   rac = pow (x / 2.0, 0.25);
   Res = (((xx / 1386.0 + 1.0 / 42.0) * xx + 1.0 / 3.0) * xx + 1.0) /
      (1.225416702465177 * rac);
   temp = (((xx / 3510.0 + 1.0 / 90.0) * xx + 0.2) * xx + 1.0) * rac /
      0.906402477055477;
   Res = num_Pi * (Res - temp) / num_Rac2;
   return Res;
}

/*=========================================================================*/
