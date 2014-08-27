
EIBLIB=-L . -leib7
ZMQLIB= -lzmq

INC=-I ./

DEBUG=-g

CFLAGS=$(INC) $(DEBUG) -DLinux

LIBS=-lpthread


all: encoder

#
#
# cleanup targets
#
#

clean:
	rm -f *.o

distclean: clean
	rm -f *.o

#
#
# executables
#
#

encoder: encoder.c
	gcc $(CFLAGS) -o encoder encoder.c $(EIBLIB) $(ZMQLIB) $(LIBS)
	    