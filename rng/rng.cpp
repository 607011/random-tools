// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#define VERSION "1.0.3"

#ifdef _WIN32
#include "../config-win32.h"
#else
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif //HAVE_CONFIG_H
#endif

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <getopt.h>

#ifdef HAVE_BOOST
#include <boost/random.hpp>
#endif

#ifdef HAVE_PADLOCK
#include "padlock.h"
#endif

#include "gen/circ.h"
#include "gen/lcg.h"
#include "gen/mcg.h"
#include "gen/knuth.h"
#include "gen/marsaglia.h"
#include "gen/mersenne_twister.h"
#include "gen/bbs.h"

#ifdef HAVE_LIBSSL
#include <openssl/rand.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#if _MSC_VER >= 1500
#define HAVE_TPM
#endif
#endif

enum _gen_types {
    GEN_SYSTEM_RAND = 0x10000000,
    GEN_MT19937,
    GEN_MT800,
    GEN_LCG_ANSIC,
    GEN_LCG_MINSTD,
    GEN_LCG_HOAGLIN1,
    GEN_LCG_HOAGLIN2,
    GEN_LCG_HOAGLIN3,
    GEN_LCG_HOAGLIN4,
    GEN_LCG_HOAGLIN5,
    GEN_LCG_HOAGLIN6,
    GEN_LCG_URN12,
    GEN_LCG_SIMSCRIPT,
    GEN_LCG_RANDU,
    GEN_LCG_FISHMAN,
    GEN_LCG_APPLE,
    GEN_LCG_SUPERDUPER,
    GEN_LCG_BCPL,
    GEN_LCG_BCSLIB,
    GEN_MCG,
    GEN_MWC,
#ifdef HAVE_LIBGMP
    GEN_BBS,
#endif
#ifdef HAVE_BOOST
    GEN_BOOST_MT19937,
#endif
#ifdef HAVE_PADLOCK
    GEN_PADLOCK,
#endif
    GEN_KNUTH,
    GEN_CIRCULAR,
#ifdef _WIN32
    GEN_CRYPT_WIN32,
#endif
#ifdef HAVE_LIBSSL
    GEN_OPENSSL,
#endif
    GEN_FILE,
#ifdef HAVE_TPM
    GEN_TPM
#endif
};

enum _long_options {
    SELECT_HELP = 0x1,
    SELECT_VERBOSE,
    SELECT_QUIET,
    SELECT_OUTPUT,
    SELECT_APPEND,
    SELECT_TEXT
};

static struct option long_options[] = {
    { "gen-circular",         no_argument, 0, GEN_CIRCULAR },
    { "gen-system-rand",      no_argument, 0, GEN_SYSTEM_RAND },
    { "gen-mt19937",          no_argument, 0, GEN_MT19937 },
    { "gen-mt800",            no_argument, 0, GEN_MT800 },
    { "gen-lcg-ansic",        no_argument, 0, GEN_LCG_ANSIC },
    { "gen-lcg-minstd",       no_argument, 0, GEN_LCG_MINSTD },
    { "gen-lcg-hoaglin1",     no_argument, 0, GEN_LCG_HOAGLIN1 },
    { "gen-lcg-hoaglin2",     no_argument, 0, GEN_LCG_HOAGLIN2 },
    { "gen-lcg-hoaglin3",     no_argument, 0, GEN_LCG_HOAGLIN3 },
    { "gen-lcg-hoaglin4",     no_argument, 0, GEN_LCG_HOAGLIN4 },
    { "gen-lcg-hoaglin5",     no_argument, 0, GEN_LCG_HOAGLIN5 },
    { "gen-lcg-hoaglin6",     no_argument, 0, GEN_LCG_HOAGLIN6 },
    { "gen-lcg-urn12",        no_argument, 0, GEN_LCG_URN12 },
    { "gen-lcg-fishman",      no_argument, 0, GEN_LCG_FISHMAN },
    { "gen-lcg-simscript",    no_argument, 0, GEN_LCG_SIMSCRIPT },
    { "gen-lcg-randu",        no_argument, 0, GEN_LCG_RANDU },
    { "gen-lcg-apple",        no_argument, 0, GEN_LCG_APPLE },
    { "gen-lcg-superduper",   no_argument, 0, GEN_LCG_SUPERDUPER },
    { "gen-lcg-bcpl",         no_argument, 0, GEN_LCG_BCPL },
    { "gen-lcg-bcslib",       no_argument, 0, GEN_LCG_BCSLIB },
    { "gen-mcg",              no_argument, 0, GEN_MCG },
    { "gen-mwc",              no_argument, 0, GEN_MWC },
#ifdef HAVE_LIBGMP
    { "gen-bbs",        optional_argument, 0, GEN_BBS },
#endif
#ifdef HAVE_BOOST
    { "gen-boost-mt19937",    no_argument, 0, GEN_BOOST_MT19937 },
#endif
#ifdef _WIN32
    { "gen-crypt-win32",      no_argument, 0, GEN_CRYPT_WIN32 },
#endif
#ifdef HAVE_PADLOCK
    { "gen-padlock",          no_argument, 0, GEN_PADLOCK },
#endif
#ifdef HAVE_LIBSSL
    { "gen-openssl",          no_argument, 0, GEN_OPENSSL },
#endif
#ifdef HAVE_TPM
    { "gen-tpm",              no_argument, 0, GEN_TPM },
#endif
    { "gen-knuth",            no_argument, 0, GEN_KNUTH },
    { "gen-file",       required_argument, 0, GEN_FILE },
    { "output",         required_argument, 0, SELECT_OUTPUT },
    { "quiet",                no_argument, 0, SELECT_QUIET },
    { "verbose",              no_argument, 0, SELECT_VERBOSE },
    { "append",               no_argument, 0, SELECT_APPEND },
    { "help",                 no_argument, 0, SELECT_HELP },
    { "text",                 no_argument, 0, SELECT_TEXT },
    { NULL,                             0, 0, 0 }
};


typedef int variate_t;
typedef variate_t (*GeneratorFunction)(void);

const size_t DefaultChunkSize = 1024; /* KByte */
const size_t DefaultCount = 20 * 1024;
const size_t DefaultBBSKeyBits = 1024;
int generator = GEN_SYSTEM_RAND;
char* outputFilename = NULL;
char* inputFilename = NULL;
int verbose = 0;
bool quiet = false;
size_t N = DefaultCount;
size_t chunkSize = DefaultChunkSize;
unsigned char* rngBuf;
size_t bbsKeyBits = DefaultBBSKeyBits;
bool doAppend = false;
bool binary = true;

static void usage(void)
{
    std::cout << "Aufruf: ct-rng Generator [Optionen]" << std::endl
        << std::endl
        << "Optionen:" << std::endl
        << "  -n N" << std::endl
        << "     N KByte Zufallsbytes generieren (Vorgabe: " << DefaultCount << " KByte)" << std::endl
        << std::endl
        << "  --output filename" << std::endl
        << "  -o filename" << std::endl
        << "     Zufallszahlen in Datei 'filename' schreiben" << std::endl
        << std::endl
        << "  --text" << std::endl
        << "     Generierte Zahlen in Textdatei schreiben anstelle in Binärdatei." << std::endl
        << "     Jede Zeile enthält eine Zahl zwischen 0 und 255 (jeweils inklusive)" << std::endl
        << std::endl
        << "  --append" << std::endl
        << "     Zufallszahlen an Datei anhängen" << std::endl
        << std::endl
        << "  --quiet" << std::endl
        << "  -q" << std::endl
        << "     Keine Informationen nicht ausgeben" << std::endl
        << std::endl
        << "  --verbose" << std::endl
        << "  -v" << std::endl
        << "     Mehr Informationen über Verarbeitungsschritte ausgeben" << std::endl
        << std::endl
        << "  --help" << std::endl
        << "  -h" << std::endl
        << "  -?" << std::endl
        << "     Diese Hilfe anzeigen" << std::endl
        << std::endl
        << "Generatoren:" << std::endl
        << "  --gen-file filename" << std::endl
        << "  --gen-system-rand" << std::endl
        << "  --gen-mt19937" << std::endl
        << "  --gen-mt800" << std::endl
        << "  --gen-lcg-ansic" << std::endl
        << "  --gen-lcg-minstd" << std::endl
        << "  --gen-lcg-hoaglin1" << std::endl
        << "  --gen-lcg-hoaglin3" << std::endl
        << "  --gen-lcg-hoaglin4" << std::endl
        << "  --gen-lcg-hoaglin5" << std::endl
        << "  --gen-lcg-hoaglin6" << std::endl
        << "  --gen-lcg-urn12" << std::endl
        << "  --gen-lcg-simscript" << std::endl
        << "  --gen-lcg-randu" << std::endl
        << "  --gen-mcg" << std::endl
        << "  --gen-mwc" << std::endl
        << "  --gen-knuth" << std::endl
#ifdef HAVE_LIBGMP
        << "  --gen-bbs [Schlüsselgröße in Bits]" << std::endl
#endif
#ifdef HAVE_BOOST
        << "  --gen-boost-mt19937" << std::endl
#endif
#ifdef HAVE_PADLOCK
        << "  --gen-padlock" << std::endl
#endif
        << "  --gen-circular" << std::endl
#ifdef HAVE_LIBSSL
        << "  --gen-openssl" << std::endl
#endif
#ifdef _WIN32
        << "  --gen-crypt-win32" << std::endl
#endif
#ifdef HAVE_TPM
        << "  --gen-tpm" << std::endl
#endif
        << std::endl;
}


static void disclaimer(void)
{
    std::cout << "ct-rng " << VERSION << " - Generator für Zufallsbytes." << std::endl
        << "Copyright (c) 2008-2009 Oliver Lau <ola@ctmagazin.de>" << std::endl
        << "Copyright (c) 2008-2009 Heise Zeitschriften Verlag" << std::endl
        << "Alle Rechte vorbehalten." << std::endl
        << std::endl
        << "Diese Software wurde zu Lehr- und Demonstrationszwecken erstellt." << std::endl
        << "Alle Ausgaben ohne Gewähr." << std::endl
        << std::endl;
}


static size_t count = 0;
static size_t pct = 0;
static size_t oldpct = 0;


static void updateProcess(void)
{
    pct = 100 * count / N;
    if (pct > oldpct)
    {
        std::cout << std::right << std::setw(4) << pct << "% "
            << std::flush << "\b\b\b\b\b\b";
        oldpct = pct;
    }
}


void generate(GeneratorFunction gen, std::ofstream& fs)
{
    for (count = 0; count < N; ++count)
    {
        updateProcess();
        if (binary)
        {
            for (size_t i = 0; i < chunkSize; ++i)
                rngBuf[i] = (unsigned char) (gen() & 0xff);
            if (fs.is_open())
                fs.write((char*) rngBuf, chunkSize);
        }
        else
        {
            if (fs.is_open())
            {
                for (size_t i = 0; i < chunkSize; ++i)
                    fs << (gen() & 0xff) << std::endl;
            }
        }
    }
}


template <class T>
void generate(ctrandom::RandomNumberGenerator<T>& gen, std::ofstream& fs)
{
    for (count = 0; count < N; ++count)
    {
        updateProcess();
        if (binary)
        {
            for (size_t i = 0; i < chunkSize; ++i)
                rngBuf[i] = (unsigned char) (gen() & 0xff);
            if (fs.is_open())
                fs.write((char*) rngBuf, chunkSize);
        }
        else
        {
            if (fs.is_open())
            {
                for (size_t i = 0; i < chunkSize; ++i)
                    fs << (gen() & 0xff) << std::endl;
            }
        }
    }
}


#ifdef HAVE_BOOST
template <class T>
void generate(T& gen, std::ofstream& fs)
{
    boost::uniform_int<> distribution(0, 255);
    boost::variate_generator<T, boost::uniform_int<> > rnd(gen, distribution);
    for (count = 0; count < N; ++count)
    {
        updateProcess();
        if (binary)
        {
            for (size_t i = 0; i < chunkSize; ++i)
                rngBuf[i] = (unsigned char) (rnd() & 0xff);
            if (fs.is_open())
                fs.write((char*) rngBuf, chunkSize);
        }
        else
        {
            if (fs.is_open())
            {
                for (size_t i = 0; i < chunkSize; ++i)
                    fs << (rnd() & 0xff) << std::endl;
            }
        }
    }
}
#endif


int main(int argc, char* argv[])
{
    for (;;) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "h?vqo:n:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case SELECT_OUTPUT:
            // fall-through
        case 'o':
            if (optarg == NULL)
            {
                usage();
                exit(EXIT_FAILURE);
            }
            outputFilename = optarg;
            break;
        case SELECT_APPEND:
            doAppend = true;
            break;
        case SELECT_TEXT:
            binary = false;
            break;
        case 'h':
            // fall-through
        case '?':
            // fall-through
        case SELECT_HELP:
            disclaimer();
            usage();
            return 0;
            break;
        case 'n':
            if (optarg == NULL)
            {
                usage();
                exit(EXIT_FAILURE);
            }
            N = atoi(optarg);
            if (N <= 0)
                N = DefaultCount;
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
#ifdef HAVE_LIBGMP
        case GEN_BBS:
            if (optarg != NULL)
                bbsKeyBits = atoi(optarg);
            generator = c;
            break;
#endif
        case GEN_FILE:
            if (optarg != NULL)
                inputFilename = optarg;
            generator = c;
            break;
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

    std::ofstream fs;
    if (outputFilename != NULL)
    {
        fs.open(outputFilename, (doAppend
            ? (std::ios::binary | std::ios::out | std::ios::app | std::ios::ate)
            : (std::ios::binary | std::ios::out)));
        if (!fs.is_open() || fs.fail())
        {
            std::cerr << "FEHLER: Öffnen von " << outputFilename << " fehlgeschlagen." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    rngBuf = new unsigned char[chunkSize];
    if (rngBuf == NULL)
    {
        std::cerr << "FEHLER: Speicherallokation fehlgeschlagen." << std::endl;
        exit(EXIT_FAILURE);
    }



#ifdef _WIN32
    LARGE_INTEGER freq, t0;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);
#endif
    switch (generator)
    {
    case GEN_SYSTEM_RAND:
        std::cout << "system's rand() function .. " << std::flush;
        srand((unsigned int) time(0));
        generate(rand, fs);
        break;
    case GEN_MT19937:
        std::cout << "Mersenne-Twister 19937 .. " << std::flush;
        {
            ctrandom::MersenneTwister mt19937;
            mt19937.seed((unsigned int) time(0));
            generate<unsigned int>(mt19937, fs);
        }
        break;
    case GEN_MT800:
        std::cout << "Mersenne-Twister 800 .. " << std::flush;
        {
            ctrandom::MersenneTwister mt800;
            mt800.seed((unsigned int) time(0));
            generate<unsigned int>(mt800, fs);
        }
        break;
    case GEN_LCG_ANSIC:
        std::cout << "LCG (ANSI C) .. " << std::flush;
        {
            ctrandom::LCG_ANSIC lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_MINSTD:
        std::cout << "LCG (MINSTD) .. " << std::flush;
        {
            ctrandom::LCG_MINSTD lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_FISHMAN:
        std::cout << "LCG (FISHMAN) .. " << std::flush;
        {
            ctrandom::LCG_FISHMAN lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_HOAGLIN1:
        std::cout << "LCG (Hoaglin1) .. " << std::flush;
        {
            ctrandom::LCG_HOAGLIN1 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_HOAGLIN2:
        std::cout << "LCG (Hoaglin2) .. " << std::flush;
        {
            ctrandom::LCG_HOAGLIN2 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_HOAGLIN3:
        std::cout << "LCG (Hoaglin3) .. " << std::flush;
        {
            ctrandom::LCG_HOAGLIN3 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_HOAGLIN4:
        std::cout << "LCG (Hoaglin4) .. " << std::flush;
        {
            ctrandom::LCG_HOAGLIN4 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_HOAGLIN5:
        std::cout << "LCG (Hoaglin5) .. " << std::flush;
        {
            ctrandom::LCG_HOAGLIN5 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_HOAGLIN6:
        std::cout << "LCG (Hoaglin6) .. " << std::flush;
        {
            ctrandom::LCG_HOAGLIN6 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_URN12:
        std::cout << "LCG (URN12) .. " << std::flush;
        {
            ctrandom::LCG_URN12 lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_SIMSCRIPT:
        std::cout << "LCG (SIMSCRIPT) .. " << std::flush;
        {
            ctrandom::LCG_SIMSCRIPT lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_RANDU:
        std::cout << "LCG (RANDU) .. " << std::flush;
        {
            ctrandom::LCG_RANDU lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_APPLE:
        std::cout << "LCG (APPLE) .. " << std::flush;
        {
            ctrandom::LCG_APPLE lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_SUPERDUPER:
        std::cout << "LCG (SUPERDUPER) .. " << std::flush;
        {
            ctrandom::LCG_SUPERDUPER lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_BCPL:
        std::cout << "LCG (BCPL) .. " << std::flush;
        {
            ctrandom::LCG_BCPL lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_LCG_BCSLIB:
        std::cout << "LCG (BCSLIB) .. " << std::flush;
        {
            ctrandom::LCG_BCSLIB lcg;
            lcg.seed((unsigned int) time(0));
            generate<unsigned int>(lcg, fs);
        }
        break;
    case GEN_MCG:
        std::cout << "MCG .. " << std::flush;
        {
            ctrandom::MCG mcg;
            mcg.seed((unsigned int) time(0));
            generate<unsigned char>(mcg, fs);
        }
        break;
    case GEN_KNUTH:
        std::cout << "Knuth .. " << std::flush;
        {
            ctrandom::KnuthRand1 knuth;
            knuth.seed((unsigned int) time(0));
            generate<unsigned int>(knuth, fs);
        }
        break;
    case GEN_MWC:
        std::cout << "Multiply-with-carry (Marsaglia) .. " << std::flush;
        {
            ctrandom::MultiplyWithCarry mwc;
            mwc.seed((unsigned int) time(0));
            generate<unsigned int>(mwc, fs);
        }
        break;
#ifdef HAVE_LIBGMP
    case GEN_BBS:
        std::cout << "Blum-Blum-Shub (" << bbsKeyBits << " Bits) .. " << std::flush;
        {
            ctrandom::BlumBlumShub bbs(bbsKeyBits);
            generate<unsigned int>(bbs, fs);
        }
        break;
#endif
    case GEN_CIRCULAR:
        std::cout << "Circular .. " << std::flush;
        {
            ctrandom::CircularBytes circ;
            generate<unsigned int>(circ, fs);
        }
        break;
#ifdef HAVE_BOOST
    case GEN_BOOST_MT19937:
        std::cout << "Mersenne Twister (boost::mt19937) .." << std::flush;
        {
            boost::mt19937 generator;
            generator.seed((unsigned int) time(0));
            generate<boost::mt19937>(generator, fs);
        }
        break;
#endif
#ifdef HAVE_PADLOCK
    case GEN_PADLOCK:
        std::cout << "VIA PadLock .. " << std::flush;
        {
            if (padlock_rng_available() == 0)
            {
                std::cerr << " ERROR: VIA PadLock RNG ist auf dieser Plattform nicht verfügbar." << std::endl;
                exit(EXIT_FAILURE);
            }
            for (count = 0; count < N; ++count)
            {
                padlock_rng_rand(rngBuf, chunkSize);
                fs.write((char*) rngBuf, chunkSize);
                updateProcess();
            }
        }
        break;
#endif
#ifdef _WIN32
    case GEN_CRYPT_WIN32:
        std::cout << "CryptGenRandom (Win32) .. " << std::flush;
        {
            HCRYPTPROV hCryptProv;
            bool CryptGenWin32 = false;
            if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0) != 0)
            {
                DWORD dwErr = GetLastError();
                if (dwErr == NTE_BAD_KEYSET)
                {
                    if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET) == 0)
                    {
                        std::cerr << "CryptAcquireContext() fehlgeschlagen: Erzeugen des Schlüsselcontainers misslungen." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else if (dwErr != 0)
                {
                    std::cerr << "CryptAcquireContext() fehlgeschlagen: Kryptographie-Handle konnte nicht bezogen werden. (0x" << std::setfill('0') << std::setw(8) << std::hex << dwErr<< ")" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                DWORD err = GetLastError();
                std::cerr << "CryptAcquireContext() fehlgeschlagen (0x" << std::setfill('0') << std::setw(8) << std::hex << err << ")" << std::endl;
                exit(EXIT_FAILURE);
            }
            for (count = 0; count < N; ++count)
            {
                updateProcess();
                CryptGenRandom(hCryptProv, chunkSize, rngBuf);
                if (fs.is_open())
                    fs.write((char*) rngBuf, chunkSize);
            }
            if (!CryptReleaseContext(hCryptProv, 0)) {
                std::cerr << "CryptReleaseContext() fehlgeschlagen: Kryptographie-Handle konnte nicht freigegeben werden." << std::endl;
            }
        }
        break;
#endif
#ifdef HAVE_TPM
    case GEN_TPM:
        std::cout << "Trusted Platform Module .. " << std::flush;
        {
            for (count = 0; count < N; ++count)
            {
                updateProcess();
                RAND_bytes(rngBuf, chunkSize);
                if (fs.is_open())
                    fs.write((char*) rngBuf, chunkSize);
            }
        }
        break;
#endif
#ifdef HAVE_LIBSSL
    case GEN_OPENSSL:
        std::cout << "OpenSSL .. " << std::flush;
        {
            for (count = 0; count < N; ++count)
            {
                updateProcess();
                RAND_bytes(rngBuf, chunkSize);
                if (fs.is_open())
                    fs.write((char*) rngBuf, chunkSize);
            }
        }
        break;
#endif
    case GEN_FILE:
        std::cout << inputFilename << " .. " << std::flush;
        {
            std::ifstream ifs(inputFilename, std::ios::binary | std::ios::in);
            if (ifs.is_open()) 
            {
                for (count = 0; count < N; ++count)
                {
                    updateProcess();
                    ifs.read((char*) rngBuf, chunkSize);
                    if (fs.is_open())
                        fs.write((char*) rngBuf, chunkSize);
                }
            }
        }
        break;
    default:
        usage();
        exit(EXIT_FAILURE);
        break;
    }

#ifdef _WIN32
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    t.QuadPart = t.QuadPart - t0.QuadPart;
    std::cout << std::endl << std::endl
        << (N / ((double) t.QuadPart / (double) freq.QuadPart)) << " KByte/s" << std::endl;
#endif
    if (!quiet)
        std::cout << std::endl;
    fs.close();

    delete [] rngBuf;

    if (!quiet)
        std::cout << count << " KByte geschrieben." << std::endl << std::endl;
    return EXIT_SUCCESS;
}
