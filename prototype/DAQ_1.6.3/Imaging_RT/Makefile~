
CC=g++

LD=$(CC)

CPPFLAGS= -Wall -g -O2 

# Library definition

# LIBS=

ROOTCFLAGS   = $(shell root-config --cflags)
ROOTLIBS     = $(shell root-config --libs)
ROOTGLIBS    = $(shell root-config --glibs)
# ROOTINCLUDE  = -I. -I$(ROOTSYS)/include

LD            = g++
LDFLAGS       = -g -m64
SOFLAGS       = -shared

CPPFLAGS     += $(ROOTCFLAGS)
LIBS         += $(ROOTLIBS) -L. -lGUI_Imaging -lThread 
GLIBS         = $(ROOTGLIBS)

SRCS = Tmonitor.cc  
OBJS = $(SRCS:%.cc=%.o)

RLIB = libGUI_Imaging.so

all:	Tmonitor

clean: 
	/bin/rm -f *.o
	/bin/rm -f GUItestCint.*
	/bin/rm -f libGUI_Imaging.so

Tmonitor:	$(OBJS) $(RLIB)
	$(LD) -o Tmonitor $(OBJS) $(ROOTINCLUDE) $(LIBS)

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cc.o:   
	$(CC) $(CPPFLAGS) -c $<

libGUI_Imaging.so:  GUItest.cc GUItest.h
			make -f MakeGUItest

#dep depend:
#	$(CC) -E $(CPPFLAGS) $(ROOTINCLUDE) -M $(SRCS) Tmonitor.cc > .depend
#
#ifeq (.depend,$(wildcard .depend))
#include .depend
#endif
