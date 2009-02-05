// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __CT_RANDCAM_GLOBALS_H_
#define __CT_RANDCAM_GLOBALS_H_

#define VERSION "1.0.5"

#ifndef LLONG_MAX
#define LLONG_MAX (9223372036854775807LL)
#endif

const int DefaultWidth = 640;
const int DefaultHeight = 480;
const int DefaultFPS = 25;
const int DefaultHashesPerFrame = 17;

extern double entropy(const unsigned char* ran, const size_t sz);
extern void usage(void);
extern void disclaimer(void);

#endif // __CT_RANDCAM_GLOBALS_H_
