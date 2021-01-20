################### VLIBS

PROJ=volundr

CURRENT=$(realpath)
CC=$(shell which gcc)
AS=$(shell which as)
LD=$(shell which ld)
CFLAGS=-O0 -g -ggdb -Wall -Werror -fPIC -I. -Icommon -Iasm -Ielf

elf_SRCS=$(wildcard $(realpath elf)/*.c)
asm_SRCS=$(wildcard $(realpath asm)/*.S)

common_SRCS=$(wildcard $(realpath common)/*.c)
EX_ORIG_PATH=$(realpath ../examples/)

elf_OBJS=$(elf_SRCS:.c=.o)
common_OBJS=$(common_SRCS:.c=.o)
asm_OBJS=$(asm_SRCS:.S=.o)

trj1=et_exec_trojan
trj2=et_dyn_trojan
tst = example-long example-headers example-infect01 example-sctidx

LIB=lib$(PROJ).so
LIBDIR=$(realpath .)

all: libs example-lib
	$(AS) --64 $(EX_ORIG_PATH)/$(trj1).S -statistics -fatal-warnings \
		-size-check=error -o $(EX_ORIG_PATH)/$(trj1).o
	$(LD) -Ttext 200000 --oformat binary -o $(EX_ORIG_PATH)/$(trj1) $(EX_ORIG_PATH)/$(trj1).o
	$(AS) --64 $(EX_ORIG_PATH)/$(trj2).S -statistics -fatal-warnings \
		-size-check=error -o $(EX_ORIG_PATH)/$(trj2).o
	$(LD) $(EX_ORIG_PATH)/$(trj2).o -o $(EX_ORIG_PATH)/$(trj2)
	for i in $(tst) ; do \
		$(CC) $(EX_ORIG_PATH)/$$i.c -L$(realpath .) -lvolundr -I $(LIBDIR)/elf		\
		-I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ $(CFLAGS) -o $(EX_ORIG_PATH)/$$i ;	\
	done

example-lib:
	gcc $(EX_ORIG_PATH)/example-lib01-impl.c -o $(EX_ORIG_PATH)/lib01-impl.so -shared -fPIC
	gcc $(EX_ORIG_PATH)/example-lib01.c -L$(EX_ORIG_PATH) -l01-impl -o $(EX_ORIG_PATH)/example-lib

libs: $(elf_OBJS) $(common_OBJS)
	$(AS) asm/syscalls.S -statistics -fatal-warnings -size-check=error -o asm/syscalls.o
	$(CC) -shared -o $(LIB) $(elf_OBJS) $(common_OBJS) asm/syscalls.o $(CFLAGS)

docs:
	(cd $(CURRENT).. && doxygen doxygen.cfg)

clean:
	rm -f $(PROJ) $(elf_OBJS) $(common_OBJS) asm/syscalls.o $(LIB)
	rm -f $(EX_ORIG_PATH)/lib01-impl.so $(EX_ORIG_PATH)/example-lib
	rm -f $(EX_ORIG_PATH)/$(trj1) $(EX_ORIG_PATH)/$(trj2) $(EX_ORIG_PATH)/$(trj1).o
	for i in $(tst) ; do \
		rm -f $(EX_ORIG_PATH)/$$i ;	\
	done
