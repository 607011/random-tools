// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#ifndef __FIFOBUFFER_H_
#define __FIFOBUFFER_H_

#include <cstdlib>
#include <sys/types.h>

struct ServerResponse {
    uint32_t length;
    unsigned char r[BufSize];
    static const size_t BufSize = 1024;
};


class BufferFile {
 public:
    BufferFile(const char* filename);
    ~BufferFile();

 private:

};


#endif // __FIFOBUFFER_H_
