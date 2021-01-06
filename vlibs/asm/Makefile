#
# syscalls Makefile
#

AS=$(shell which as)
LD=$(shell which ld)
LIB=syscalls

all:
	$(AS) $(LIB).S -statistics -fatal-warnings -size-check=error -o $(LIB).o
	$(LD) $(LIB).o -r -o lib$(LIB).so
	@echo "Done."

clean:
	rm -f *.o *.so
	@echo "Clean."
