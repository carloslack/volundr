CC=gcc
CFLAGS=-O0 -g -ggdb -Wall -D__ELF32__ -DDEBUG# <- shouldn't be declared here. XXX
PROJ=asm
OBJS=asm.o
LIB=lib$(PROJ).so

$(LIB): $(OBJS)
	$(CC) -shared -o $(LIB) $(OBJS)

asm.o: asm.c
	$(CC) -c -o asm.o asm.c $(CFLAGS)

clean:
	rm -rf $(PROJ) $(OBJS) $(LIB)
