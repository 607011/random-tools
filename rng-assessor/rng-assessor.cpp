// $Id$
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _WIN32
#include "../config-win32.h"
#endif

#include <iostream>
#include <iomanip>
#include <fstream>

#include <getopt.h>

#include "tests.h"
#include "gen/randomfile.h"

#include "test/math_functions.h"
#include "test/chisq.h"

#include "rng-assessor.h"


typedef unsigned char variate_t;

enum _long_options {
    SELECT_HELP = 1,
    SELECT_VERBOSE,
    SELECT_NOFIPS,
    SELECT_QUIET
};

static struct option long_options[] = {
    { "quiet",           no_argument, 0, SELECT_QUIET },
    { "verbose",         no_argument, 0, SELECT_VERBOSE },
    { "no-fips",         no_argument, 0, SELECT_NOFIPS },
    { NULL,                        0, 0, 0 }
};

bool quiet = false;
bool runFIPS140_1 = true;
long blockSize = 0;
long offset = 0;
int verbose = 0;
char* inputFilename = NULL;

variate_t r_min;
variate_t r_max;
long r_range;
int r_bits;
RNGArray r;
bool timeIt = true;
double alpha = 0.01;


static void usage(void)
{
    std::cout << "Aufruf: rng-assessor [Optionen] Zufallszahlendatei" << std::endl
        << std::endl
        << "Options:" << std::endl
        << "  -n n" << std::endl
        << "     nur n Bytes des Zahlendatei auswerten" << std::endl
        << std::endl
        << "  -o off" << std::endl
        << "     Auswerten der Zahlendatei ab Offset off beginnen" << std::endl
        << std::endl
        << "  --quiet" << std::endl
        << "  -q" << std::endl
        << "     keine Verarbeitungsinformationen ausgeben" << std::endl
        << std::endl
        << "  --verbose" << std::endl
        << "  -v" << std::endl
        << "     mehr Verarbeitungsinformationen ausgeben" << std::endl
        << std::endl;
}


static void disclaimer(void)
{
    std::cout << "rng-assessor " << VERSION << " - Zufallszahlenanalyse 1." << std::endl
        << "Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>" << std::endl
        << "Alle Rechte vorbehalten." << std::endl
        << std::endl
        << "Diese Software wurde zu Lehr- und Demonstrationszwecken erstellt." << std::endl
        << "Alle Ausgaben ohne Gewaehr." << std::endl
        << std::endl;
}


int main(int argc, char* argv[]) 
{
    for (;;) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "h?vqn:o:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case 'n':
            if (optarg != NULL)
                blockSize = atol(optarg);
            break;
        case 'o':
            if (optarg != NULL)
                offset = atol(optarg);
            break;
        case 'h':
            // fall-through
        case '?':
            // fall-through
        case SELECT_HELP:
            usage();
            return 0;
            break;
        case SELECT_VERBOSE:
            // fall-through
        case 'v':
            ++verbose;
            break;
        case SELECT_QUIET:
            // fall-through
        case 'q':
            quiet = true;
            break;
        case SELECT_NOFIPS:
            runFIPS140_1 = false;
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
            break;
        }
    }

    if (!quiet)
        disclaimer();

    if (optind < argc)
        inputFilename = argv[optind++];

    if (inputFilename == NULL)
    {
        usage();
        exit(EXIT_FAILURE);
    }

    if (!quiet)
        std::cout << "Verarbeiten von '" << inputFilename << "' .." << std::endl << std::endl;
    randomtools::RandomFile<size_t> gen(inputFilename, true);
    if (!gen.stream().is_open())
    {
        std::cerr << "FEHLER: Oeffnen von '" << inputFilename << "' fehlgeschlagen." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (blockSize == 0L)
    {
        gen.stream().seekg(0, std::ios::end);
        blockSize = (long)gen.stream().tellg();
    }

    if (!quiet)
        std::cout << "Lesen von " << blockSize << " Bytes"
                  << " ab Offset " << offset << " .." << std::endl << std::endl;
    gen.seek(offset);
    r.resize(blockSize);
    for (long i = 0; i < blockSize; ++i)
        r[i] = gen();
    r_min = 0;
    r_max = std::numeric_limits<variate_t>::max();
    r_range = 1L + ((long) r_max - (long) r_min);
    r_bits = (int) (M_LOG2E * log(1 + (double) r_range));

    // "soft" tests
    test_entropy();
    test_pi();
    test_bzip2();

    // "hard" tests
    test_frequencies();
    test_monobit();
    test_autocorrelation();
    test_serial();
    test_gap();
    test_max_of_t();
    test_runs();
    test_runs_fips();
    test_couponcollector();
    test_poker_knuth();
    test_poker_fips();

    if (!quiet)
        std::cout << "Fertig." << std::endl << std::endl;

    return EXIT_SUCCESS;
}
