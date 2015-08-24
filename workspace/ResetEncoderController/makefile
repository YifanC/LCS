EIBLIB=-L . -leib7

INC=-I ./

DEBUG=-g

CFLAGS=$(INC) $(DEBUG) -DLinux

LIBS=-lpthread

#LIBPATH= -L ./

all: reset

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

reset: reset.c
	gcc $(CFLAGS) -o reset.o reset.c $(EIBLIB) $(LIBS) $(LIBPATH)
	    
