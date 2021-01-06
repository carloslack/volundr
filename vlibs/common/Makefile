CC=gcc
# XXX -DDEBUG must go in config.h
CFLAGS=-O0 -g -ggdb -Wall -fPIC -I../asm -I../elf -I../common
PROJ=common
OBJS=log.o utils.o map.o
LIB=lib$(PROJ).so

$(LIB): $(OBJS)
	$(CC) -shared -o $(LIB) $(OBJS) -fPIC

%.o:	%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(LIB)
