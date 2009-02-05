// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __RANDOMFILE_H_
#define __RANDOMFILE_H_

#include "abstract_random_number_generator.h"

#include <cstdlib>
#include <string>
#include <fstream>

namespace ctrandom {

    template <typename VariateType>
    class RandomFile : public RandomNumberGenerator<VariateType>
    {
    public:
        RandomFile(void) : f(NULL), binary(true) { }

        RandomFile(const char* _Filename, bool _Binary = true) : f(NULL), binary(_Binary)
        {
            open(_Filename);
        }

        ~RandomFile()
        {
            if (f != NULL && f->is_open())
                f->close();
        }

        void open(const char* _Filename)
        {
            if (_Filename == NULL) 
                return;
            if (f == NULL) 
            {
                f = new std::ifstream;
                f->open(_Filename, std::ios::in | std::ios::binary);
            }
        }

        void seek(size_t offset)
        {
            f->seekg(offset);
        }

        VariateType operator()()
        {
            VariateType r = 0;
            if (f != NULL && f->is_open())
            {   
                if (binary)
                {
                    unsigned char c;
                    f->read((char*) &c, 1);
                    r = (VariateType) c;
                }
                else
                {
                    std::string line;
                    *f >> line;
                    r = (VariateType) atof(line.c_str());
                }
            }
            return r;
        }

        std::ifstream& stream(void)
        {
            return *f;
        }

    private:
        std::ifstream* f;
        bool binary;
    };

};

#endif // __RANDOMFILE_H_
