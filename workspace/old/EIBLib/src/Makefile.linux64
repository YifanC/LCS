
EIBLIB=../lib64/libeib7_64.so


INC=-I../include -I.

# 
# Debug information
#
# For Release version comment 'DEBUG'
#
#  DEBUG=-g
DEBUG=


CFLAGS=-c $(INC) $(DEBUG) -fpic -DLinux -Wall

LIBS=-lpthread

LFLAGS=$(DEBUG)
DLLFLAGS=-shared


ALL: $(EIBLIB)

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
# library make rules
#
#

OBJ=eib7.o eib7_cmd.o cmdtcp.o linux_commtcp.o linux_threading.o handles.o commands.o fifo.o cmdtcp_list.o


	
$(EIBLIB): $(OBJ) 
	gcc $(LFLAGS) $(DLLFLAGS) -o $(EIBLIB) $(OBJ) $(LIBS)

#
#
# source file dependencies
#
#

linux_threading.o: Makefile.linux64 ../include/eib7.h linux_threading.c threading.h definitions.h linux_threading.h

linux_commtcp.o: Makefile.linux64 ../include/eib7.h linux_commtcp.c commtcp.h definitions.h linux_commtcp.h

cmdtcp.o: Makefile.linux64  cmdtcp.c ../include/eib7.h definitions.h commtcp.h \
  linux_commtcp.h cmdtcp.h cmdtcp_list.h commands.h eib7_types.h threading.h \
  linux_threading.h fifo.h

cmdtcp_list.o: Makefile.linux64  cmdtcp.c cmdtcp_list.c ../include/eib7.h definitions.h \
  cmdtcp.h cmdtcp_list.h commands.h eib7_types.h threading.h \
  linux_threading.h fifo.h
  
eib7.o: Makefile.linux64 eib7.c ../include/eib7.h eib7_types.h definitions.h cmdtcp.h cmdtcp_list.h \
  commtcp.h linux_commtcp.h threading.h linux_threading.h fifo.h \
  eib7_cmd.h commands.h handles.h eib7_errors.h 

eib7_cmd.o: Makefile.linux64 eib7_cmd.c ../include/eib7.h eib7_cmd.h eib7_types.h \
  definitions.h cmdtcp.h cmdtcp_list.h commtcp.h linux_commtcp.h threading.h \
  linux_threading.h fifo.h commands.h handles.h

fifo.o: Makefile.linux64 ../include/eib7.h eib7_types.h fifo.h fifo.c

commands.o: ../include/eib7.h commands.c commands.h commtcp.h cmdtcp_list.h
