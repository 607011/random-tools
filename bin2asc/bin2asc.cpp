// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
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
        std::cout << "Eingabedatei (bin) = " << in_filename << std::endl;
        std::cout << "Ausgabedatei (asc) = " << out_filename << std::endl;
        unsigned int count = 0;
        unsigned char byte;
        while (!fi.eof())
        {
            fi.read((char*) &byte, 1);
            fo << (unsigned int) byte << std::endl;
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
