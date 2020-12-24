#
# asm Makefile
#
#

AS= 		as
LD= 		ld
SFLAGS= 	-statistics -fatal-warnings -size-check=error
CFLAGS=		-Wall -Werror -I/usr/include/x86_64-linux-gnu/asm
LFLAGS=		-L. -lS -ltest
LIBSRC= 	./syscalls.S
LIBTESTSRC= 	./syscalls.S
LIBOBJS= 	./syscalls.o
LIBTESTOBJS= 	./syscalls.o
LIB=		./libsyscalls.so
LIBFLAGS=$(SFLAGS) 

lib:
	$(AS) $(LIBSRC) $(LIBFLAGS) -o $(LIBOBJS)
	$(LD) $(LIBOBJS) -r -o $(LIB) 

all:
	make clean
	make lib


clean:
	rm -rf *.o *.so
	@echo "Clean."
