// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "proactor.h"
#include <pthread.h>

#define MAX_CONNECTIONS 100
#define PORT 8080

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr *)&server, sizeof(server));
    listen(server_sock, MAX_CONNECTIONS);

    while((client_sock = accept(server_sock, (struct sockaddr *)&client, &client_len))) {
        printf("Connection accepted\n");

        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        if(pthread_create(&thread_id, NULL, connection_handler, (void*) new_sock) < 0) {
            perror("could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    return 0;
}