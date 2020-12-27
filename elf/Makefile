#
# TODO: discover system bus size and endieness.
# 		__ELF{32,64}__ must be defined.
#

CC=gcc
# -D* shouldn't be declared here. XXX use config.h
CFLAGS=-O0 -g -ggdb -Wall -DDEBUG -fPIC
PROJ=elf
LDPATH=-L../common/
OBJS=print.o validate.o write.o types.o e_names.o file.o parse.o destroy.o
LIB=lib$(PROJ).so

$(LIB): ../common/libcommon.so $(OBJS)
	$(CC) -shared $(LDPATH) -o $(LIB) $(OBJS) -fPIC -lcommon

../common/libcommon.so: ../common/common.h
	cd ../common && make

print.o: print.c
	$(CC) -c -o print.o print.c $(CFLAGS)

write.o: write.c
	$(CC) -c -o write.o write.c $(CFLAGS)

parse.o: parse.c
	$(CC) -c -o parse.o parse.c $(CFLAGS)

validate.o: validate.c
	$(CC) -c -o validate.o validate.c $(CFLAGS)

types.o: types.c
	$(CC) -c -o types.o types.c $(CFLAGS)

e_names.o: e_names.c
	$(CC) -c -o e_names.o e_names.c $(CFLAGS)

file.o: file.c
	$(CC) -c -o file.o file.c $(CFLAGS)

destroy.o: destroy.c
	$(CC) -c -o destroy.o destroy.c $(CFLAGS)

clean:
	rm -rf $(PROJ) $(OBJS) $(LIB)
