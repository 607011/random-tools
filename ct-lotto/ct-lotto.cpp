// $Id$
// Copyright (c) 2009 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifdef _WIN32
#include "../config-win32.h"
#define _CRT_RAND_S
#else
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H
#endif

#define VERSION "1.0.0"


#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>

#include <getopt.h>

#include "gen/circ.h"
#include "gen/lcg.h"
#include "gen/mcg.h"
#include "gen/knuth.h"
#include "gen/marsaglia.h"
#include "gen/mersenne_twister.h"
#include "gen/bbs.h"

enum _gen_types {
    GEN_SYSTEM_RAND = 0x10000000,
    GEN_MT19937,
    GEN_LCG,
    GEN_MCG,
    GEN_MWC,
    GEN_KNUTH,
#ifdef HAVE_LIBGMP
    GEN_BBS,
#endif
};

enum _long_options {
    SELECT_HELP = 0x1,
};


static struct option long_options[] = {
#ifdef HAVE_LIBGMP
    { "gen-bbs",        optional_argument, 0, GEN_BBS },
#endif
    { "gen-system-rand",      no_argument, 0, GEN_SYSTEM_RAND },
    { "gen-mt19937",          no_argument, 0, GEN_MT19937 },
    { "gen-lcg",              no_argument, 0, GEN_LCG },
    { "gen-mcg",              no_argument, 0, GEN_MCG },
    { "gen-mwc",              no_argument, 0, GEN_MWC },
    { "gen-knuth",            no_argument, 0, GEN_KNUTH },
    { "help",                 no_argument, 0, SELECT_HELP },
    { NULL,                             0, 0, 0 }
};


typedef int variate_t;
typedef variate_t (*GeneratorFunction)(void);


const size_t DefaultBBSKeyBits = 512;
const size_t DefaultFieldCount = 12;
const size_t DefaultSelectionCount = 6;

std::map<size_t, bool> lottoNumbers;
size_t selectedCount = 0;
size_t N = DefaultFieldCount;
size_t selectionCount = DefaultSelectionCount;
int generator = GEN_SYSTEM_RAND;
size_t bbsKeyBits = DefaultBBSKeyBits;
bool quiet = false;


static void usage(void)
{
    std::cout << "Aufruf: ct-lotto Generator [Optionen]" << std::endl
        << std::endl
        << "Optionen:" << std::endl
        << "  -n N" << std::endl
        << "     Lottozahlen (6 aus 49) für N Felder generieren (Vorgabe: " << DefaultFieldCount << " Felder)" << std::endl
        << std::endl
        << "  --help" << std::endl
        << "  -h" << std::endl
        << "  -?" << std::endl
        << "     Diese Hilfe anzeigen" << std::endl
        << std::endl
        << "  -q" << std::endl
        << "     Nur Zahlen ausgeben, sonst nichts" << std::endl
        << std::endl
        << std::endl
        << "Generatoren:" << std::endl
        << "  --gen-mt19937" << std::endl
        << "  --gen-lcg" << std::endl
        << "  --gen-mcg" << std::endl
        << "  --gen-mwc" << std::endl
        << "  --gen-knuth" << std::endl
#ifdef HAVE_LIBGMP
        << "  --gen-bbs [Schlüsselgröße in Bits]" << std::endl
#endif
        << std::endl
        << std::endl;
}


static void disclaimer(void)
{
    std::cout << "ct-lotto " << VERSION << " - Lottozahlen-Generator." << std::endl
        << "Copyright (c) 2009 Oliver Lau <ola@ctmagazin.de>" << std::endl
        << "Copyright (c) 2009 Heise Zeitschriften Verlag" << std::endl
        << "Alle Rechte vorbehalten." << std::endl
        << std::endl
        << "Diese Software wurde zu Lehr- und Demonstrationszwecken erstellt." << std::endl
        << "Alle Ausgaben ohne Gewähr." << std::endl
        << std::endl;
}


void printNumbers(void)
{
    for (std::map<size_t, bool>::const_iterator i = lottoNumbers.begin(); i != lottoNumbers.end(); ++i)
        std::cout << std::setw(2) << (*i).first << "  ";
    std::cout << std::endl;
}


unsigned int getSeed(void)
{
    unsigned int seed;
#ifdef _WIN32
    rand_s(&seed);
#else
    seed = (unsigned int) time((time_t)0);
#endif
    return seed;
}


template <typename T>
void __generate(T gen)
{
    for (size_t i = 0; i < N; ++i)
    {
        lottoNumbers.clear();
        while (lottoNumbers.size() < selectionCount)
        {
            size_t r = 1 + gen() % 49;
            if (!lottoNumbers[r])
                lottoNumbers[r] = true;
        }
        printNumbers();
    }
}


void generate(GeneratorFunction gen)
{
    __generate<GeneratorFunction>(gen);
}


template <class T>
void generate(ctrandom::RandomNumberGenerator<T>& gen)
{
    __generate<ctrandom::RandomNumberGenerator<T>&>(gen);
}


int main(int argc, char* argv[])
{
    for (;;) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "h?vqn:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case 'q':
            quiet = true;
            break;
        case 'h':
            // fall-through
        case '?':
            // fall-through
        case SELECT_HELP:
            disclaimer();
            usage();
            return EXIT_SUCCESS;
            break;
        case 'n':
            if (optarg == NULL)
            {
                usage();
                exit(EXIT_FAILURE);
            }
            N = atoi(optarg);
            if (N <= 0)
                N = DefaultFieldCount;
            break;
#ifdef HAVE_LIBGMP
        case GEN_BBS:
            if (optarg != NULL)
                bbsKeyBits = atoi(optarg);
            generator = c;
            break;
#endif
        default:
            if (c < 0x10000000)
            {
                usage();
                exit(EXIT_FAILURE);
            }
            else
            {
                generator = c;
            }
            break;
        }
    }

    if (!quiet)
        disclaimer();

    switch (generator)
    {
    case GEN_SYSTEM_RAND:
        if (!quiet)
            std::cout << "system's rand() function .. " << std::endl << std::endl;
        srand(getSeed());
        generate(rand);
        break;
    case GEN_MT19937:
        if (!quiet)
            std::cout << "Mersenne-Twister 19937 .. " << std::endl << std::endl;
        {
            ctrandom::MersenneTwister mt19937;
            mt19937.seed(getSeed());
            // warm-up
            for (int i = 0; i < 10000; ++i)
                (void) mt19937();
            generate<unsigned int>(mt19937);
        }
        break;
    case GEN_LCG:
        if (!quiet)
            std::cout << "LCG (ANSI C) .. " << std::endl << std::endl;
        {
            ctrandom::LCG_ANSIC lcg;
            lcg.seed(getSeed());
            generate<unsigned int>(lcg);
        }
        break;
    case GEN_MCG:
        if (!quiet)
            std::cout << "MCG .. " << std::endl << std::endl;
        {
            ctrandom::MCG mcg;
            mcg.seed(getSeed());
            generate<unsigned char>(mcg);
        }
        break;
    case GEN_KNUTH:
        if (!quiet)
            std::cout << "Knuth .. " << std::endl << std::endl;
        {
            ctrandom::KnuthRand1 knuth;
            knuth.seed(getSeed());
            generate<unsigned int>(knuth);
        }
        break;
    case GEN_MWC:
        if (!quiet)
            std::cout << "Multiply-with-carry (Marsaglia) .. " << std::endl << std::endl;
        {
            ctrandom::MultiplyWithCarry mwc;
            mwc.seed(getSeed());
            generate<unsigned int>(mwc);
        }
        break;
#ifdef HAVE_LIBGMP
    case GEN_BBS:
        if (!quiet)
            std::cout << "Blum-Blum-Shub (" << bbsKeyBits << " Bits) .. " << std::endl << std::endl;
        {
            ctrandom::BlumBlumShub bbs(bbsKeyBits);
            generate<unsigned int>(bbs);
        }
        break;
#endif
    default:
        usage();
        exit(EXIT_FAILURE);
        break;
    }

    return EXIT_SUCCESS;
}
