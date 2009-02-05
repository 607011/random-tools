// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <fstream>

#include <getopt.h>
#include "gen/randomfile.h"

#include "test/math_functions.h"


enum _long_options {
    SELECT_HELP = 1,
    SELECT_VERBOSE,
    SELECT_NOFIPS,
    SELECT_QUIET,
    SELECT_LOG
};

static struct option long_options[] = {
    { "quiet",           no_argument, 0, SELECT_QUIET },
    { "verbose",         no_argument, 0, SELECT_VERBOSE },
    { "daemon",          no_argument, 0, SELECT_DAEMON },
    { "log",       required_argument, 0, SELECT_LOG },
    { NULL,                        0, 0, 0 }
};

bool quiet = false;
int verbose = 0;
char* logFilename = NULL;
char* inputFilename = NULL;
