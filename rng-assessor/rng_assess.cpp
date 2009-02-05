// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <iostream>
#include <iomanip>
#include <fstream>

#include <getopt.h>

#include "tests.h"
#include "gen/randomfile.h"

#include "test/math_functions.h"
#include "test/chisq.h"

typedef unsigned char variate_t;

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
    { "no-fips",         no_argument, 0, SELECT_NOFIPS },
    { "log",       required_argument, 0, SELECT_LOG },
    { NULL,                        0, 0, 0 }
};

bool quiet = false;
bool runFIPS140_1 = true;
size_t blockSize = 0;
size_t offset = 0;
int verbose = 0;
char* logFilename = NULL;
char* inputFilename = NULL;

size_t r_min;
size_t r_max;
size_t r_range;
size_t r_bits;
std::vector<size_t> r;
double alpha = 0.01;

static void usage(void)
{
    std::cout << "Aufruf: ct-rng-assessor [Optionen] Zufallszahlendatei" << std::endl
        << std::endl
        << "Options:" << std::endl
        << "  --log Filename" << std::endl
        << "     log results to Filename" << std::endl
        << std::endl
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
    std::cout << "ct-rng-assessor - Zufallszahlenanalyse 1." << std::endl
        << "Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>" << std::endl
        << "Copyright (c) 2008  Heise Zeitschriften Verlag." << std::endl
        << "Alle Rechte vorbehalten." << std::endl
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
                blockSize = (size_t) atoi(optarg);
            break;
        case 'o':
            if (optarg != NULL)
                offset = (size_t) atoi(optarg);
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
        case SELECT_LOG:
            if (optarg != NULL)
                logFilename = optarg;
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

    std::cout << "Verarbeiten von '" << inputFilename << "' .." << std::endl << std::endl;
    ctrandom::RandomFile<size_t> gen(inputFilename, true);
    if (!gen.stream().is_open())
    {
        std::cerr << "FEHLER: ffnen von '" << inputFilename << "' fehlgeschlagen." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (blockSize == 0)
    {
        gen.stream().seekg(0, std::ios::end);
        blockSize = gen.stream().tellg();
    }

    std::cout << "Lesen von " << blockSize << " Bytes ab Offset " << offset << " .." << std::endl << std::endl;
    gen.seek(offset);
    r.resize(blockSize);
    for (size_t i = 0; i < blockSize; ++i)
        r[i] = gen();
    r_min = 0;
    r_max = 1 + std::numeric_limits<variate_t>::max();
    r_range = r_max - r_min;
    r_bits = (size_t) (M_LOG2E * log((double) r_range));

    test_entropy();
    test_frequencies();
    test_monobit();
    test_serial();
    test_gap();
    test_max_of_t();
    test_runs();
    test_runs_fips();
    test_couponcollector();
    test_poker_knuth();
    test_poker_fips();
    test_pi();

    std::cout << "Fertig." << std::endl << std::endl;
    return 0;
}
