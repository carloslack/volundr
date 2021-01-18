################### VLIBS

PROJ=volundr

CURRENT=$(realpath)
CC=$(shell which gcc)
AS=$(shell which as)
LD=$(shell which ld)
CFLAGS=-O0 -g -ggdb -Wall -fPIC -I. -Icommon -Iasm -Ielf

elf_SRCS=$(wildcard $(realpath elf)/*.c)
asm_SRCS=$(wildcard $(realpath asm)/*.S)

common_SRCS=$(wildcard $(realpath common)/*.c)
EX_DST_PATH=$(realpath ../)
EX_ORIG_PATH=$(realpath examples/)

elf_OBJS=$(elf_SRCS:.c=.o)
common_OBJS=$(common_SRCS:.c=.o)
asm_OBJS=$(asm_SRCS:.S=.o)

ex1=example-long
ex2=example-headers
ex3=example-infect01
parasite=parasite

LIB=lib$(PROJ).so
LIBDIR=$(realpath .)

all: libs
	$(AS) --64 $(EX_ORIG_PATH)/$(parasite).S -statistics -fatal-warnings \
		-size-check=error -o $(EX_ORIG_PATH)/$(parasite).o
	$(LD) -Ttext 200000 --oformat binary -o $(EX_DST_PATH)/$(parasite) $(EX_ORIG_PATH)/$(parasite).o

	$(CC) $(EX_ORIG_PATH)/$(ex1).c -L$(realpath .) -lvolundr -I $(LIBDIR)/elf \
		-I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ $(CFLAGS) -o $(EX_DST_PATH)/$(ex1)
	$(CC) $(EX_ORIG_PATH)/$(ex2).c -L$(realpath .) -lvolundr -I $(LIBDIR)/elf \
		-I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ $(CFLAGS) -o $(EX_DST_PATH)/$(ex2)
	$(CC) $(EX_ORIG_PATH)/$(ex3).c -L$(realpath .) -lvolundr -I $(LIBDIR)/elf \
		-I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ $(CFLAGS) -o $(EX_DST_PATH)/$(ex3)


libs: $(elf_OBJS) $(common_OBJS)
	$(AS) asm/syscalls.S -statistics -fatal-warnings -size-check=error -o asm/syscalls.o
	$(CC) -shared -o $(LIB) $(elf_OBJS) $(common_OBJS) asm/syscalls.o $(CFLAGS)

docs:
	(cd $(CURRENT).. && doxygen doxygen.cfg)

clean:
	rm -f $(PROJ) $(elf_OBJS) $(common_OBJS) asm/syscalls.o asm/parasite.o $(LIB)
	rm -f $(EX_DST_PATH)/$(ex1)
	rm -f $(EX_DST_PATH)/$(ex2)
	rm -f $(EX_DST_PATH)/$(ex3)
	rm -f $(EX_DST_PATH)/$(parasite)
