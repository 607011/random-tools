// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <cstdlib>
#include <ctime>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <limits>

#include <fstream>
#include <vector>
#include <cmath>

#include <sys/types.h>

#ifdef _WIN32
#include <cv.h>
#include <highgui.h>
#define HAVE_OPENCV_GUI 1
#else
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif

#include <getopt.h>

#include "md5/md5.h"
#include "globals.h"
#include "Masher.h"


enum _long_options {
    SELECT_HELP = 1,
    SELECT_VERBOSE,
    SELECT_QUIET,
    SELECT_ADAPTIVE,
    SELECT_ALPHA,
    SELECT_CAMERA,
    SELECT_WIDTH,
    SELECT_HEIGHT,
    SELECT_FPS,
    SELECT_OUTPUTFILE,
    SELECT_APPEND,
    SELECT_VIDEOFILE,
    SELECT_RAW,
    SELECT_MAXMBYTECOUNT
};

static struct option long_options[] = {
    { "out",            required_argument, 0, SELECT_OUTPUTFILE },
    { "append",               no_argument, 0, SELECT_APPEND },
    { "cam",            required_argument, 0, SELECT_CAMERA },
    { "width",          required_argument, 0, SELECT_WIDTH },
    { "height",         required_argument, 0, SELECT_HEIGHT },
    { "fps",            required_argument, 0, SELECT_FPS },
    { "alpha",          required_argument, 0, SELECT_ALPHA },
    { "raw",                  no_argument, 0, SELECT_RAW },
    { "video",                no_argument, 0, SELECT_VIDEOFILE },
    { "max",            required_argument, 0, SELECT_MAXMBYTECOUNT },
    { "adaptive",             no_argument, 0, SELECT_ADAPTIVE },
    { "quiet",                no_argument, 0, SELECT_QUIET },
    { "verbose",              no_argument, 0, SELECT_VERBOSE },
    { NULL,                             0, 0, 0 },
};

const char* WindowName = "ct-randcam";
int verbose = 0;
int camNum = 0;
bool quiet = false;
char* outputFilename;
char outputVideoFilename[100];
char outputRawFilename[100];
bool grabVideo = false;
int hashesPerFrame;
int64 maxByteCount = LLONG_MAX;
int preferredWidth = DefaultWidth;
int preferredHeight = DefaultHeight;
int preferredFPS = DefaultFPS;
bool doAppend = false;
bool grabRawData = false;
bool adaptive = true;
double alpha = 1.0;
int gain = 70;

typedef enum _which_image_t {
    IMAGE_DIFFERENCE,
    IMAGE_RAW
} which_image_t;

which_image_t whichImage = IMAGE_DIFFERENCE;


int main(int argc, char* argv[])
{
    time_t _t;
    time(&_t);
    struct tm* t = gmtime(&_t);

    for (;;) {
        int option_index = 0;
        int c = getopt_long(argc, argv, "h?vqn:o:x:y:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c)
        {
        case SELECT_VIDEOFILE:
            grabVideo = true;
            break;
        case SELECT_CAMERA:
            if (optarg != NULL)
                camNum = atoi(optarg);
            break;
        case SELECT_APPEND:
            doAppend = true;
            break;
        case SELECT_WIDTH:
            // fall-through
        case 'x':
            if (optarg != NULL)
                preferredWidth = atoi(optarg);
            break;
        case SELECT_HEIGHT:
            // fall-through
        case 'y':
            if (optarg != NULL)
                preferredHeight = atoi(optarg);
            break;
        case 'n':
            if (optarg != NULL)
                hashesPerFrame = atoi(optarg);
            if (hashesPerFrame < 1)
                hashesPerFrame = 1;
            adaptive = false;
            break;
        case SELECT_RAW:
            grabRawData = true;
            strftime(outputRawFilename, 99, "%Y%m%d-%H%M%S-raw.dat", t);
            break;
        case SELECT_FPS:
            if (optarg != NULL)
                preferredFPS = atoi(optarg);
            break;
        case SELECT_MAXMBYTECOUNT:
            if (optarg != NULL)
                maxByteCount = (int64) atoi(optarg) * 1048576LL;
            break;
        case SELECT_ALPHA:
            alpha = atof(optarg);
            break;
        case SELECT_ADAPTIVE:
            adaptive = true;
            break;
        case SELECT_OUTPUTFILE:
            // fall-through
        case 'o':
            outputFilename = optarg;
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
        default:
            break;
        }
    }

    if (!quiet)
        disclaimer();

    if (outputFilename == NULL)
    {
        outputFilename = new char[100];
        strftime(outputFilename, 99, "%Y%m%d-%H%M%S-randcam.dat", t);
    }

#ifdef HAVE_OPENCV_GUI
    cvInitSystem(argc, argv);
    cvNamedWindow(WindowName, CV_WINDOW_AUTOSIZE);
#endif

    if (!quiet)
        std::cout << "Initialisierung der Webcam " << camNum << " .. " << std::flush;

    CvCapture* capture = cvCaptureFromCAM(camNum);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, preferredWidth);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, preferredHeight);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, preferredFPS);
    if(!cvGrabFrame(capture))
    {
        std::cerr << "FEHLER: Einlesen von Bildern nicht möglich. Keine Kamera angeschlossen?" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ofstream fs(outputFilename, (doAppend
                                      ? (std::ios::binary | std::ios::out | std::ios::app | std::ios::ate)
                                      : (std::ios::binary | std::ios::out)));
    if (!fs.is_open() || fs.fail())
    {
        std::cerr << "FEHLER: Öffnen der Zufallszahlendatei '" << outputFilename << "' fehlgeschlagen." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ofstream raw;
    if (grabRawData)
    {
        raw.open(outputRawFilename, (doAppend
                                     ? (std::ios::binary | std::ios::out | std::ios::app | std::ios::ate)
                                     : (std::ios::binary | std::ios::out)));
        if (!raw.is_open() || raw.fail())
        {
            std::cerr << "FEHLER: Öffnen der Rohdatendatei '" << outputRawFilename << "' fehlgeschlagen." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    int64 frameCount = 0;
    int64 byteCount = fs.tellp();

    IplImage* img1 = cvQueryFrame(capture);
    int width = img1->width;
    int height = img1->height;
    int step = img1->widthStep;
    int depth = img1->depth;
    int channels = img1->nChannels;
    size_t imgSize = step * height;
    IplImage* img2 = cvCloneImage(img1);
    if (!quiet)
    {
        std::cout << std::endl
                  << "Breite         : " << width << std::endl
                  << "Höhe           : " << height << std::endl
                  << "Bits pro Pixel : " << depth << std::endl
                  << "Kanäle         : " << channels << std::endl 
                  << std::endl;
        std::cout << (doAppend? "Anhängen" : "Schreiben") << " der Zufallsbytes " << (doAppend? "an" : "in") << " Datei '" << outputFilename << "'."
                  << std::endl;
    }

    CvVideoWriter* vw = NULL;
    if (grabVideo)
    {
        strftime(outputVideoFilename, 99, "%Y%m%d-%H%M%S-randcam.avi", t);
        vw = cvCreateVideoWriter(outputVideoFilename, CV_FOURCC('D', 'I', 'V', 'X'), 16.67, cvSize(width, height), 1);
        if (vw != NULL)
            std::cout << "Schreiben des Videos in '" << outputVideoFilename << "'." << std::endl;
        else
            std::cout << "ACHTUNG: Erzeugen des Video-Writers fehlgeschlagen." << std::endl;
    }

    if (!quiet)
        std::cout << std::endl;

    IplImage* img = cvCreateImage(cvSize(width, height), depth, channels);
    Masher masher((const unsigned char*) img->imageData, imgSize);

    int64 ticks0 = cvGetTickCount();
    int64 ticks = ticks0;
    for (int key = -1; (key != 'q') && (byteCount < maxByteCount); key = cvWaitKey(20))
    {
        switch (key)
        {
        case 'd':
            whichImage = IMAGE_DIFFERENCE;
            break;
        case 'r':
            whichImage = IMAGE_RAW;
            break;
        case '+':
            gain += 10;
            break;
        case '-':
            gain -= 10;
            break;
        default:
            break;
        }
        img1 = cvQueryFrame(capture);
        ++frameCount;
        ticks = cvGetTickCount() - ticks;

        cvSub(img2, img1, img);

        if (raw.is_open())
            raw.write(img->imageData, imgSize);
        if (vw != NULL)
            cvWriteFrame(vw, img);

        double ent = entropy((const unsigned char*) img->imageData, imgSize);
        if (adaptive)
            hashesPerFrame = (int) sqrt(alpha * ent * imgSize / Masher::HashSize);
        if (hashesPerFrame > 0) 
        {
            size_t rem = hashesPerFrame % 6;
            if (rem != 1 && rem != 5)
                hashesPerFrame -= rem-1;
            masher.mash(hashesPerFrame);
            fs.write((const char*) masher.rng(), masher.size());
            byteCount += masher.size();
        }

        switch (whichImage)
        {
        case IMAGE_DIFFERENCE:
            cvAddS(img, cvScalar(gain, gain, gain), img); // Helligkeit anheben
            break;
        default:
            cvCopy(img1, img);
            break;
        }

#ifdef HAVE_OPENCV_GUI
        cvShowImage(WindowName, img);
#endif

        if (!quiet) 
        {
            int64 ticks0stop = cvGetTickCount() - ticks0;
            std::cout
                << "Total: "
                << std::dec << std::setprecision(3) << std::setfill(' ') << std::right
                << std::setw(5) << (frameCount * 1e6 * cvGetTickFrequency() / ticks0stop) << " fps "
                << std::setw(11) << byteCount << " Bytes ("
                << std::setw(3) << (100 * byteCount / maxByteCount) << "%) "
                << std::setw(10) << std::setprecision(7) << ent << " "
                << std::setw(6) << hashesPerFrame << "    "
                << std::flush
                << "\r";
        }
        cvCopy(img1, img2);
        ticks = cvGetTickCount();
    }
    fs.close();

    std::cout << std::endl << std::endl
        << "Laufzeit: " << std::setprecision(4) << (1e-6 * (double) (ticks - ticks0) / cvGetTickFrequency()) << " Sekunden"
        << std::endl
        << "Datenrate: " << std::setprecision(4) << (1e6 * (double) byteCount / ((double) (ticks - ticks0) / cvGetTickFrequency()) / 1024) << " KByte/s"
        << std::endl;

    if (!quiet)
        std::cout << std::endl;
    if (raw.is_open())
        raw.close();
    if (vw != NULL)
        cvReleaseVideoWriter(&vw);
    if (capture != NULL) 
        cvReleaseCapture(&capture);
    return EXIT_SUCCESS;
}
