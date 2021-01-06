CC=gcc
SHLIBS=-lelfo -lcommon -lsyscalls -fPIC
LIBDIR=./volundr
LDPATH=-L./$(LIBDIR)/elf -L./$(LIBDIR)/common -L./$(LIBDIR)/asm
CFLAGS=-O0 -g -ggdb -Wall -fPIC -DDEBUG $(LDPATH) -I$(LIBDIR)/common -I$(LIBDIR)/asm -I$(LIBDIR)/elf
OUT=./example
LIB=$(OUT)

all:
	make -C $(LIBDIR)
	$(CC) example.c -L$(LIBDIR) -lvolundr -I $(LIBDIR)/elf -I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ -o $(OUT)

clean:
	make -C $(LIBDIR) clean
	@rm -f example



