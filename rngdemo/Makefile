# $Id$
# Copyright (c) 2008 Oliver Lau <oliver@ersatzworld.net>
# Alle Rechte vorbehalten.

PROJ=rngdemo
INC=-I..
OPTIMIZE=-O2 -finline-functions
DEBUGFLAGS=-DDEBUG -D_DEBUG -ggdb -g3
CFLAGS=$(OPTIMIZE) $(INC) -Wall
CXXFLAGS=$(OPTIMIZE) $(INC) -Wall -fPIC
LDFLAGS=$(CXXFLAGS) -lstdc++ -fexceptions
CC=gcc
CXX=g++
LD=ld
AR=ar
X=
O=.o
MKDIR=mkdir -p
MV=mv
OUTDIR=.
STRIP=strip
LIPO=lipo

PROJ_O := $(foreach i,rngdemo,$i$(O))
PROJ_H := $(foreach i,,$i.h)
LIBS := $(foreach i,../test/$(OUTDIR)/librngtest ../gen/$(OUTDIR)/librng,$i.a)

all: $(PROJ)

macosx-release-universal:
	$(RM) -R x86
	$(RM) -R ppc
	$(MKDIR) x86
	$(MKDIR) ppc
	@echo "**************************************"
	@echo "************ X86 RELEASE *************"
	@echo "**************************************"
	$(MAKE) clean
	$(MAKE) macosx-release-x86
	@echo "**************************************"
	@echo "************ PPC RELEASE *************"
	@echo "**************************************"
	$(MAKE) clean
	$(MAKE) macosx-release-ppc
	@echo "**************************************"
	@echo "********** UNIVERSAL BINARY **********"
	@echo "**************************************"
	$(LIPO) -create ppc/$(PROJ) x86/$(PROJ) -output $(PROJ)
	$(STRIP) $(PROJ)

macosx-release-x86:
	$(MAKE) release \
	    O="$(O)" \
            CFLAGS="$(CFLAGS)" \
	    CXXFLAGS="$(CXXFLAGS) -arch i386" \
	    LDFLAGS="$(LDFLAGS) -arch i386" \
	    OUTDIR="x86"

macosx-release-ppc:
	$(MAKE) release \
	    O="$(O)" \
	    CFLAGS="$(CFLAGS)" \
	    CXXFLAGS="$(CXXFLAGS) -arch ppc" \
	    LDFLAGS="$(LDFLAGS) -arch ppc" \
	    OUTDIR="ppc"

release:
	$(MAKE) all \
	    O="$(O)" \
	    CFLAGS="$(CFLAGS)" \
            CXXFLAGS="$(CXXFLAGS)" \
            LDFLAGS="$(LDFLAGS)" \
	    OUTDIR="$(OUTDIR)" 

debug:
	$(MAKE) all \
	    O="$(O)" \
	    CFLAGS="$(CFLAGS) $(DEBUGFLAGS)" \
            CXXFLAGS="$(CXXFLAGS) $(DEBUGFLAGS)" \
            LDFLAGS="$(LDFLAGS) $(DEBUGFLAGS)"
	    OUTDIR="$(OUTDIR)" 

clean:
	$(RM) $(PROJ_O)
	$(RM) $(PROJ)

distclean: clean
	$(RM) *.dat
	$(RM) *.plt
	$(RM) *.js
	$(RM) *.kml

%.o: %.cpp Makefile $(PROJ_H)
	$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $<

../test/librngtest.a:
	$(MAKE) -C ../test

../gen/librng.a:
	$(MAKE) -C ../gen

$(PROJ): $(PROJ_O) $(LIBS) Makefile
	$(CXX) $(LDFLAGS) -o $(OUTDIR)/$(PROJ)$(X) $(PROJ_O) $(LIBS)
