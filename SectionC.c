// Server with Proactor
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CONNECTIONS 100

int connection_sockets[MAX_CONNECTIONS];
int connection_count = 0;

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char message[2000];

    while(recv(sock, message, 2000, 0) > 0) {
        printf("Connection: %s\n", message);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int socket_desc, connection_sock, c;
    struct sockaddr_in server, connection;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(socket_desc, (struct sockaddr*)&server, sizeof(server));
    listen(socket_desc, 3);

    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

    while((connection_sock = accept(socket_desc, (struct sockaddr*)&connection, (socklen_t*)&c))) {
        printf("Connection accepted\n");

        pthread_t thread_id;
        int *new_sock = malloc(1);
        *new_sock = connection_sock;

        if(pthread_create(&thread_id, NULL, connection_handler, (void*)new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    if(connection_sock < 0) {
        perror("Accept failed");
        return 1;
    }

    return 0;
}