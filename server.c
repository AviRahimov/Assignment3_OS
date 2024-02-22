// Server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 100

int client_sockets[MAX_CLIENTS];
int client_count = 0;

void *client_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char message[2000];

    while(recv(sock, message, 2000, 0) > 0) {
        printf("Client: %s\n", message);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(socket_desc, (struct sockaddr*)&server, sizeof(server));
    listen(socket_desc, 3);

    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

    while((client_sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c))) {
        printf("Connection accepted\n");

        pthread_t thread_id;
        int *new_sock = malloc(1);
        *new_sock = client_sock;

        if(pthread_create(&thread_id, NULL, client_handler, (void*)new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    if(client_sock < 0) {
        perror("Accept failed");
        return 1;
    }

    return 0;
}