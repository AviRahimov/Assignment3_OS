# make file for multi-threaded client-server chat application

CC = gcc -g
CFLAGS = -Wall
LDFLAGS = -lpthread

all: server client

server: server.o
	$(CC) $(CFLAGS) -o server server.o $(LDFLAGS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client: client.o
	$(CC) $(CFLAGS) -o client client.o $(LDFLAGS)

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f server server.o client client.o