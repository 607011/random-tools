// $Id$
// Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>, Heise Zeitschriften Verlag.
// Alle Rechte vorbehalten.

#include <iostream>
#include "globals.h"


void usage(void)
{
    std::cout << "Aufruf: ct-randcam [Optionen]" << std::endl
        << std::endl
        << "Optionen:" << std::endl
        << "  --out Dateiname" << std::endl
        << "  -o Dateiname" << std::endl
        << "     Zufallszahlen nach 'Dateiname' schreiben (Vorgabe: YYYMMDD-hhmmss-randcam.dat)" << std::endl
        << "  --append" << std::endl
        << "     Zufallszahlen an existierende Datei anhängen (funktioniert nur mit der Option -o)" << std::endl
        << std::endl
        << "  --cam x" << std::endl
        << "     Kamera x wählen (x = 0..1)" << std::endl
        << std::endl
        << "  --raw" << std::endl
        << "     von der Kamera gelieferte Rohdaten in Datei schreiben (YYYMMDD-hhmmss-raw.dat)" << std::endl
        << std::endl
        << "  --video" << std::endl
        << "     Bilder als DivX-Video in Datei schreiben (YYYMMDD-hhmmss.avi)" << std::endl
        << std::endl
        << "  --width w" << std::endl
        << "  -x w" << std::endl
        << "     Breite des Webcam-Bildes in Pixel vorgeben (Vorgabe: " << DefaultWidth << ")" << std::endl
        << std::endl
        << "  --height h" << std::endl
        << "  -y h" << std::endl
        << "     Höhe des Webcam-Bildes in Pixel vorgeben (Vorgabe: " << DefaultHeight << ")" << std::endl
        << std::endl
        << "  --fps fps" << std::endl
        << "     gewünschte Anzahl der Bilder pro Sekunde (Vorgabe: " << DefaultFPS << ")" << std::endl
        << std::endl
        << "  --alpha x" << std::endl
        << "     Koeffizient für Bitrate (x = 0.01 .. 100)" << std::endl
        << std::endl
        << "  --adaptive" << std::endl
        << "     automatische Bitratenanpassung" << std::endl
        << std::endl
        << "  -n x" << std::endl
        << "     n Hashes pro Bild erzeugen (Vorgabe: " << DefaultHashesPerFrame << "); schaltet automatische Bitratenanpassung ab" << std::endl
        << std::endl
        << "  --quiet" << std::endl
        << "  -q" << std::endl
        << "     keine Informationen über Verarbeitungsschritte ausgeben" << std::endl
        << std::endl
        << "  --verbose" << std::endl
        << "  -v" << std::endl
        << "     mehr Informationen über Verarbeitungsschritte ausgeben" << std::endl
        << std::endl;
}



void disclaimer(void)
{
    std::cout << "ct-randcam - Zufallszahlen aus Webcam-Bildern generieren." << std::endl
              << "Copyright (c) 2008 Oliver Lau <ola@ctmagazin.de>" << std::endl
              << "Copyright (c) 2008 Heise Zeitschriften Verlag" << std::endl
              << "Alle Rechte vorbehalten." << std::endl
              << std::endl;
}
