# $Id$

PROJ = timer
DEBUGFLAGS = -DDEBUG -ggdb
OPTIMIZE = -O3 -march=i686
CFLAGS = $(DEBUGFLAGS) $(OPTIMIZE)
CPPFLAGS = $(DEBUGFLAGS) $(OPTIMIZE)
INC =
OUT = -o
O = .o
CC = gcc
CPP = g++

all: $(PROJ)

clean:
	$(RM) *$(O)
	$(RM) *.a
	$(RM) *.so

.SUFFIXES: .c .h $(O)
.cpp$O: Makefile
	$(CPP) $(CPPFLAGS) $(INC) -c $*.c

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INC) -c $(OUT) $@ $<

$(PROJ): $(PROJ)$(O) $(PROJ).h Makefile
	$(LD) -shared -o lib$(PROJ).so $(PROJ)$(O)
	$(AR) rcs lib$(PROJ).a $(PROJ)$(O)
