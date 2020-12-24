#
# asm Makefile
#
#

AS= 		as
LD= 		ld
SFLAGS= 	-statistics -fatal-warnings -size-check=error
CFLAGS=		-Wall -Werror
LFLAGS=		-L.
LIBSRC= 	./syscalls.S
LIBOBJS= 	./syscalls.o
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
