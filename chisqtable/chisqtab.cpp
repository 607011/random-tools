// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <cstdlib>
#include <cstdio>

#include "test/chisq.h"

static size_t df[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 1000, 0 };

static double p[] = { .1, .05, .025, .01, .005, .001, 0 };

int main(int argc, char* argv[])
{
    printf("Chi-Quadrat-Schwellenwerte\n"
           "\tp\n"
           "df");
    for (double* j = p; *j > 0; ++j)
        printf("\t%7.3lf", (*j));
    printf("\n");
    for (size_t* i = df; *i > 0; ++i) {
        printf("%lu", *i);
        for (double* j = p; *j > 0; ++j) {
            printf("\t%7.2lf", randomtools::ChiSquareCritical(*j, *i));
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}
