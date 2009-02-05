// $Id$
// Copyright (c) 2008-2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <getopt.h>

#include <unif01.h>
#include <ufile.h>
#include <gdef.h>
#include <swrite.h>
#include <bbattery.h>
#include <scatter.h>


enum _test_options {
    TEST_RABBIT = 0x10000000,
    TEST_CRUSH,
    TEST_FIPS140_2
};

enum _long_options {
    SELECT_HELP = 1,
    SELECT_VERBOSE,
    SELECT_QUIET,
    SELECT_SIZE,
    SELECT_NOPLOT,
    SELECT_3D
};

static struct option long_options[] = {
    { "quiet",             no_argument, 0, SELECT_QUIET },
    { "verbose",           no_argument, 0, SELECT_VERBOSE },
    { "size",        required_argument, 0, SELECT_SIZE },
    { "no-plot",           no_argument, 0, SELECT_NOPLOT },
    { "rabbit",            no_argument, 0, TEST_RABBIT },
    { "crush",             no_argument, 0, TEST_CRUSH },
    { "fips140-2",         no_argument, 0, TEST_FIPS140_2 },
    { NULL,                          0, 0, 0 }
};


static void usage(void)
{
    printf("Aufruf: rng-analyzer [Testoptionen] [Optionen] <Eingabedatei>\n\n"
           "Testoptionen:\n"
           "  --rabbit\n"
           "  --fips140-2\n"
           "  --crush\n"
           "Optionen:\n"
           "  --size n   |   -n n\n"
           "     n Bytes verwenden\n"
           "  --3d\n"
           "     3D-Plot anstelle eines 2D-Plots erzeugen\n"
           "  --no-plot\n"
           "     keine Gnuplot-Dateien erzeugen\n"
           "\n");
}


char* logFilename = NULL;
char* inputFilename = NULL;
double inputSize = -1;
int quiet = FALSE;
int verbose = 0;
int suite = TEST_RABBIT;
int plot = TRUE;


int main(int argc, char* argv[])
{
    for (;;) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "h?vqn:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case SELECT_NOPLOT:
            plot = FALSE;
            break;
        case SELECT_SIZE:
            // fall-through
        case 'n':
            if (optarg != NULL)
                inputSize = atof(optarg);
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
            quiet = TRUE;
            break;
        default:
            if (c < 0x10000000)
            {
                usage();
                exit(EXIT_FAILURE);
            }
            else 
            {
                suite = c;
            }
            break;
        }
    }

    if (optind < argc)
        inputFilename = argv[optind++];

    if (inputFilename == NULL)
    {
        usage();
        exit(EXIT_FAILURE);
    }

    if (inputSize < 0)
    {
        struct stat ifstat;
        stat(inputFilename, &ifstat);
        inputSize = ifstat.st_size;
    }
    
    swrite_Basic = (verbose > 0);

    switch (suite) {
    case TEST_RABBIT:
        printf("Rabbit-Tests laufen ..\n");
        bbattery_RabbitFile(inputFilename, inputSize*8);
        break;
    case TEST_FIPS140_2:
        printf("FIPS140-2-Tests laufen ..\n");
        {
            unif01_Gen* gen = ufile_CreateReadBin(inputFilename, (long)(inputSize/4));
            bbattery_FIPS_140_2(gen);    
            if (plot)
                scatter_PlotUnif(gen, "plot");
            ufile_DeleteReadBin(gen);
        }
        break;
    case TEST_CRUSH:
        printf("Crush-Tests laufen ..\n");
        {
            unif01_Gen* gen = ufile_CreateReadBin(inputFilename, (long)(inputSize/4));
            bbattery_Crush(gen);    
            if (plot)
                scatter_PlotUnif(gen, "plot");
            ufile_DeleteReadBin(gen);
        }
        break;
    default:
        break;
    }

    
    return EXIT_SUCCESS;
}
