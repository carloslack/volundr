CC=gcc
SHLIBS=-lelfo -lcommon -lsyscalls -fPIC
LIBDIR=./volundr
LDPATH=-L./$(LIBDIR)/elf -L./$(LIBDIR)/common -L./$(LIBDIR)/asm
CFLAGS=-O0 -g -ggdb -Wall -fPIC -DDEBUG $(LDPATH) -I$(LIBDIR)/common -I$(LIBDIR)/asm -I$(LIBDIR)/elf
ex1=./example-long
ex2=./example-headers

all:
	make -C $(LIBDIR)
	$(CC) $(ex1).c -L$(LIBDIR) -lvolundr -I $(LIBDIR)/elf -I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ -o $(ex1)
	$(CC) $(ex2).c -L$(LIBDIR) -lvolundr -I $(LIBDIR)/elf -I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ -o $(ex2)

clean:
	make -C $(LIBDIR) clean
	@rm -f $(ex1) $(ex2)



