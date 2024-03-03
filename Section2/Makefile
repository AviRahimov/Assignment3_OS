# this makefile builds dynamic library represinting the Proactor pattern called Proactor.so

CC = gcc -g
CFLAGS = -Wall
LDFLAGS = -lpthread

all: proactor.so

proactor.so: proactor.o
	$(CC) -shared -o proactor.so proactor.o $(LDFLAGS)

proactor.o: proactor.c
	$(CC) $(CFLAGS) -c -fPIC proactor.c

clean:
	rm -f proactor.so proactor.o