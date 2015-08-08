
EIBLIB=-L . -leib7

INC=-I./

DEBUG=-g

CFLAGS=$(INC) $(DEBUG) -DLinux

LIBS=-lpthread


ALL: eib7info

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

eib7info: eib7info.c
	gcc $(CFLAGS) -o eib7info.o eib7info.c $(EIBLIB) $(LIBS)
	    
