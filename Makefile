CC=gcc
SHLIBS=-lelf -lcommon -lasm -fPIC
LDPATH=-L./elf -L./common -L./asm
CFLAGS=-O0 -g -ggdb -Wall -D__ELF64__ -fPIC -DDEBUG $(LDPATH)
OUT=./volundr
OBJS=volundr.o
LIB=$(OUT)

.PHONY: volundr.o common/libcommon.so elf/libelf.so asm/libasm.so

$(OUT): $(OBJS) common/libcommon.so elf/libelf.so asm/libasm.so 
	$(CC) -o $(OUT) $(OBJS) $(CFLAGS) $(SHLIBS)
	@echo Done.

volundr.o: volundr.c
	make files
	$(CC) -c -o volundr.o volundr.c $(CFLAGS)
	chmod +x run

common/libcommon.so: common/common.h
	cd common && make

elf/libelf.so: elf/elf.h
	cd elf && make

asm/libasm.so: asm/asm.h
	cd asm && make

files:
	rm -rf TAGS tags
	find . -iname "*.c" | ctags -a * 
	find . -iname "*.h" | ctags -a *
	ctags -R *
	tree -nif > FILES

clean:
	rm -rf $(OUT) $(OBJS) *.so;
	rm -rf core* *~ *-ELF.txt elf/*~ common/*~ asm/*~ # XXX debug only
	rm -rf FILES TAGS tags
	cd common && make clean;
	cd elf && make clean;
	cd asm && make clean;
