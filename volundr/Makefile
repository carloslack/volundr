################### VLIBS

PROJ=volundr

CC=$(shell which gcc)
AS=$(shell which as)
CFLAGS=-O0 -g -ggdb -Wall -fPIC -I. -Icommon -Iasm -Ielf
#LDPATH=-L./common/

elf_SRCS=$(wildcard elf/*.c)
elf_OBJS=$(elf_SRCS:.c=.o)

common_SRCS=$(wildcard common/*.c)
common_OBJS=$(common_SRCS:.c=.o)

asm_SRCS=$(wildcard asm/*.S)
asm_OBJS=$(asm_SRCS:.S=.o)

LIB=lib$(PROJ).so

all:
	$(AS) $(asm_SRCS) -statistics -fatal-warnings -size-check=error -o $(asm_OBJS)
	make libs


libs: $(elf_OBJS) $(common_OBJS)
	$(CC) -shared -o $(LIB) $(elf_OBJS) $(common_OBJS) $(asm_OBJS) $(CFLAGS)


clean:
	rm -f $(PROJ) $(elf_OBJS) $(common_OBJS) $(asm_OBJS) $(LIB)
