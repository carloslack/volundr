CC=gcc
SHLIBS=-lelfo -lcommon -lsyscalls -fPIC
LDPATH=-L./vlibs/elf -L./vlibs/common -L./vlibs/asm
CFLAGS=-O0 -g -ggdb -Wall -fPIC -DDEBUG $(LDPATH) -Ivlibs/common -Ivlibs/asm -Ivlibs/elf
OUT=./volundr
OBJS=volundr.o
LIB=$(OUT)
.PHONY: elf/libelfo.so volundr.o common/libcommon.so asm/syscalls.so

$(OUT): $(OBJS) vlibs/common/libcommon.so vlibs/elf/libelfo.so vlibs/asm/syscalls.so
	$(CC) -o $(OUT) $(OBJS) $(CFLAGS) $(SHLIBS)
	@echo Done.

volundr.o: volundr.c
	make devtags
	$(CC) -c -o volundr.o volundr.c $(CFLAGS)
	chmod +x run

vlibs/common/libcommon.so:
	make -C vlibs/common

vlibs/elf/libelfo.so:
	make -C vlibs/elf

vlibs/asm/syscalls.so:
	make -C vlibs/asm

devtags:
	rm -rf TAGS tags
	find . -iname "*.c" | ctags -a *
	find . -iname "*.h" | ctags -a *
	ctags -R .
	tree -nif > FILES

clean:
	make -C vlibs/common clean
	make -C vlibs/elf clean
	make -C vlibs/asm clean
	rm -f $(OUT) $(OBJS) *.so;
	rm -f core* *~ *-ELF.txt vlibs/elf/*~ vlibs/common/*~ vlibs/asm/*~
	rm -f FILES TAGS tags
