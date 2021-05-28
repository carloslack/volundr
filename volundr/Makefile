#
# MIT License
#
# Copyright (c) 2021 Carlos Carvalho
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
PROJ=volundr

CC=$(shell which gcc)
AS=$(shell which as)
LD=$(shell which ld)
OBJCOPY=$(shell which objcopy)
CFLAGS=-O0 -g -ggdb -Wall -Werror -fPIC -I. -Icommon -Iasm -Ielf

elf_SRCS=$(wildcard $(realpath elf)/*.c)
asm_SRCS=$(wildcard $(realpath asm)/*.S)

common_SRCS=$(wildcard $(realpath common)/*.c)
EX=$(realpath ../examples/)

elf_OBJS=$(elf_SRCS:.c=.o)
common_OBJS=$(common_SRCS:.c=.o)
asm_OBJS=$(asm_SRCS:.S=.o)

et_exec=$(EX)/et_exec_trojan
et_dyn=$(EX)/et_dyn_trojan

tst = $(EX)/example-long $(EX)/example-headers \
	$(EX)/example-infect-text $(EX)/example-infect-note $(EX)/example-sctidx
sht_note = $(EX)/sht_note

LIB=lib$(PROJ).so
LIBDIR=$(realpath .)

all: libs et_exec et_dyn sht_note
	for i in $(tst) ; do \
		$(CC) $$i.c -L$(realpath .) -lvolundr -I $(LIBDIR)/elf		\
		-I $(LIBDIR)/common/ -I $(LIBDIR)/asm/ $(CFLAGS) -o $$i ;	\
	done

libs: $(elf_OBJS) $(common_OBJS)
	$(AS) asm/syscalls.S -statistics -fatal-warnings -size-check=error -o asm/syscalls.o
	$(CC) -shared -o $(LIB) $(elf_OBJS) $(common_OBJS) asm/syscalls.o $(CFLAGS)

et_exec:
	$(AS) --64 $(et_exec).S -statistics -fatal-warnings \
		-size-check=error -o $(et_exec).o
	$(LD) -Ttext 200000 --oformat binary -o $(et_exec) $(et_exec).o

et_dyn:
	$(AS) --64 $(et_dyn).S -statistics -fatal-warnings \
		-size-check=error -o $(et_dyn).o
	$(LD) -Ttext 200000 --oformat binary -o $(et_dyn) $(et_dyn).o

sht_note:
	# Create a 128b sht_note section in a shared lsb
	$(shell perl -e 'print"A"'x256 >$(EX)/note.txt)
	$(CC) $(sht_note).c -pie -fPIC -c -o $(sht_note).o
	$(OBJCOPY) --add-section .note.volundr=$(EX)/note.txt --set-section-flags \
		.note.custom=noload $(sht_note).o $(sht_note).o
	$(CC) $(sht_note).o -pie -fPIC -o $(sht_note)


docs:
	(cd $(realpath).. && doxygen doxygen.cfg)

clean:
	rm -f $(PROJ) $(elf_OBJS) $(common_OBJS) asm/syscalls.o $(LIB)
	rm -f $(et_exec) $(et_dyn) $(et_exec).o $(et_dyn).o
	rm -f $(sht_note).o $(sht_note) $(EX)/note.txt
	for i in $(tst) ; do \
		rm -f $$i ;	\
	done
