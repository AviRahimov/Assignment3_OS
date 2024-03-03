# this make file will build the server using the proactor.so library

CC = gcc -g
CFLAGS = -Wall
LDFLAGS = -lpthread


all: proactor_server client

proactor_server: proactor_server.o proactor.so
	$(CC) $(CFLAGS) -o proactor_server proactor_server.o proactor.so $(LDFLAGS)

proactor_server.o: proactor_server.c
	$(CC) $(CFLAGS) -c proactor_server.c

proactor.so: proactor.o
	$(CC) -shared -o proactor.so proactor.o $(LDFLAGS)

proactor.o: proactor.c
	$(CC) $(CFLAGS) -c -fPIC proactor.c

client: client.o
	$(CC) $(CFLAGS) -o client client.o $(LDFLAGS)

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f proactor_server proactor_server.o proactor.so proactor.o client client.o