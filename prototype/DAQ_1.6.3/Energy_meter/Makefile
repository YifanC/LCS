
CC=g++

LD=$(CC)

CPPFLAGS= -Wall -g -O2

# Library definition
LIBS=

ROOTCFLAGS   = $(shell root-config --cflags)
ROOTLIBS     = $(shell root-config --libs)
ROOTGLIBS    = $(shell root-config --glibs)

SOFLAGS       = -shared

CPPFLAGS     += $(ROOTCFLAGS)
LIBS         += $(ROOTLIBS) -L. -lThread -L/usr/lib
GLIBS         = $(ROOTGLIBS)
# -lfftw3
SRCS = read_file.C 
OBJS = $(SRCS:%.C=%.o)

all:	calib

clean:	
	/bin/rm -f $(OBJS) $(EXE)
	/bin/rm -f *.o

.C.o:   $(OBJS) $(RLIB)
	$(CC) -c $(CPPFLAGS) $<

calib: $(OBJS) $(RLIB)
	 $(CC) -o calib $(OBJS) $(QTINCLUDE) $(LIBS) $(QTLIBS) 

dep depend:
	$(CC) -E $(CPPFLAGS)  $(QTINCLUDE) -M $(SRCS) $(QTLIBS) calib.C > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
