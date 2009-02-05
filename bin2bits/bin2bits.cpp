// $Id$
// Copyright (c) 2008 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc < 3)
        return 1;
    const char* in_filename  = argv[1];
    const char* out_filename = argv[2];
    std::ifstream fi(in_filename, std::ios::binary);
    std::ofstream fo(out_filename);
    if (fi.is_open() && fo.is_open())
    {
        std::cout << "Eingabedatei (bin)  = " << in_filename << std::endl;
        std::cout << "Ausgabedatei (bits) = " << out_filename << std::endl;
        unsigned int count = 0;
        unsigned int word;
        const unsigned int BufSize = sizeof(word)*8;
        char buf[BufSize];
        while (!fi.eof())
        {
            fi.read((char*) &word, sizeof(word));
            for (int k = 0; k < BufSize; ++k)
            {
                buf[k] = ((word & 1) == 1)? '1' : '0';
                word >>= 1;
            }
            fo.write(buf, BufSize);
            ++count;
            if (count % 1024 == 0)
                std::cout << count/1024 << " KByte gelesen." << "\r";
        }
        fi.close();
        fo.close();
        std::cout << "\r                                   " << std::endl;
        std::cout << "Fertig." << std::endl;
    }

    return 0;
}
