
EXE	=	daq_main

CC	=	gcc

COPTS	=	-g -fPIC -Wall -DLINUX -D_LHEP_USE_SHM_ 

FLAGS	=	-Wall

LIBS	=	-lCAENVME -lpthread
EIBLIB=-L . -leib7

ROOTCFLAGS   = $(shell root-config --cflags)
ROOTLIBS     = $(shell root-config --libs)  -L. -lGUI_display -lThread -L/usr/lib64 
ROOTGLIBS    = $(shell root-config --glibs)

INCLUDEDIR =	-I./CAEN_include ./HAID_include

SRCS	=	daq_main.c daq_vme.c daq_writer.c daq_laser.c
OBJS	=	$(SRCS:.c=.o)

#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS)
		$(CC) $(FLAGS) -o $(EXE) $(OBJS) $(EIBLIB) $(LIBS)

%.o	:	%.c
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<

#dep depend:
#	$(CC) -E -M $(SRCS) > .depend

#ifeq (.depend,$(wildcard .depend))
#include .depend
#endif
