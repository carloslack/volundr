CC=gcc
SHLIBS=-lelfo -lcommon -lsyscalls -fPIC
LDPATH=-L./vlibs/elf -L./vlibs/common -L./vlibs/asm
CFLAGS=-O0 -g -ggdb -Wall -fPIC -DDEBUG $(LDPATH) -Ivlibs/common -Ivlibs/asm -Ivlibs/elf
OUT=./example
LIB=$(OUT)
.PHONY: elf/libelfo.so volundr.o common/libcommon.so asm/syscalls.so

#  gcc volundr.c -Lvlibs -lvolundr -I vlibs/elf -I vlibs/common/ -I vlibs/asm/

all:
	make -C vlibs
	$(CC) example.c -Lvlibs -lvolundr -I vlibs/elf -I vlibs/common/ -I vlibs/asm/ -o $(OUT)

clean:
	make -C vlibs clean
	@rm -f example



