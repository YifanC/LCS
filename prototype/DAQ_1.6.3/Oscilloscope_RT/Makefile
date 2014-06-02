
CC=g++

LD=$(CC)

CPPFLAGS= -Wall -g -O2 

# Library definition

LIBS=

ROOTCFLAGS   = $(shell root-config --cflags)
ROOTLIBS     = $(shell root-config --libs)
ROOTGLIBS    = $(shell root-config --glibs)

LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared

CPPFLAGS     += $(ROOTCFLAGS)
LIBS         += $(ROOTLIBS) -L. -lGUI_Oscilloscope -lThread $(QTLIBS)
GLIBS         = $(ROOTGLIBS)

SRCS = Tmonitor.cc  
OBJS = $(SRCS:%.cc=%.o)

RLIB =  libGUI_Oscilloscope.so

all:	Oscilloscope

Oscilloscope:	$(OBJS) $(RLIB)
	$(LD) -o Oscilloscope $(OBJS) $(QTINCLUDE) $(LIBS) $(QTLIBS) 


%.o:    %.cc
	$(CC) -c $(CPPFLAGS) $<

libGUI_Oscilloscope.so:  GUItest.cc GUItest.h
	make -f MakeGUItest

dep depend:
	$(CC) -E $(CPPFLAGS)  $(QTINCLUDE) -M $(SRCS) $(QTLIBS) Tmonitor.cc > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
