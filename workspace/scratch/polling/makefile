
EIBLIB=-L . -leib7

INC=-I./

DEBUG=-g

CFLAGS=$(INC) $(DEBUG) -DLinux

LIBS=-lpthread


ALL: pollposition

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

pollposition: pollposition.c
	gcc $(CFLAGS) -o pollposition pollposition.c $(EIBLIB) $(LIBS)
	    
