
EIBLIB=-L . -leib7

INC=-I ./

DEBUG=-g

CFLAGS=$(INC) $(DEBUG) -DLinux

LIBS=-lpthread


all: 2encoder

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

2encoder: 2encoder.c
	gcc $(CFLAGS) -o 2encoder 2encoder.c $(EIBLIB) $(LIBS)
	    