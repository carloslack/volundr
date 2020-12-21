CC=gcc
# -D* shouldn't be declared here. XXX use config.h
CFLAGS=-O0 -g -ggdb -Wall -D__ELF32__ -DDEBUG -fPIC
PROJ=common
OBJS=log.o utils.o map.o
LIB=lib$(PROJ).so

$(LIB): $(OBJS)
	$(CC) -shared -o $(LIB) $(OBJS) -fPIC

log.o: log.c
	$(CC) -c -o log.o log.c $(CFLAGS)

utils.o: utils.c
	$(CC) -c -o utils.o utils.c $(CFLAGS)

map.o: map.c
	$(CC) -c -o map.o map.c $(CFLAGS) 

clean:
	rm -rf $(PROJ) $(OBJS) $(LIB)
