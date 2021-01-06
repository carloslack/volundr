#
# TODO: discover system bus size and endieness.
# 		__ELF{32,64}__ must be defined.
#

CC=gcc
CFLAGS=-O0 -g -ggdb -Wall -fPIC -I. -I../common
PROJ=elfo
LDPATH=-L../common/
OBJS=print.o validate.o write.o types.o file.o parse.o destroy.o
LIB=lib$(PROJ).so

$(LIB): $(OBJS)
	$(CC) -shared $(LDPATH) -o $(LIB) $(OBJS) -fPIC

%.o:	%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(PROJ) $(OBJS) $(LIB)
