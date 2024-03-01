# Makefile for ProactorServer

CC = gcc -g
CFLAGS = -Wall
LDFLAGS = -lpthread

all: ProactorServer

ProactorServer: ProactorServer.o Proactor.o
	$(CC) $(CFLAGS) -o ProactorServer ProactorServer.o Proactor.o $(LDFLAGS)

ProactorServer.o: ProactorServer.c Proactor.h
	$(CC) $(CFLAGS) -c ProactorServer.c

Proactor.o: Proactor.c Proactor.h
	$(CC) $(CFLAGS) -c Proactor.c

clean:
	rm -f ProactorServer ProactorServer.o Proactor.o

.PHONY: all clean
