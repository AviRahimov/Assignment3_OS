// Proactor.c
#include "Proactor.h"
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
static pthread_mutex_t mutex;

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char message[2000];

    while(recv(sock, message, 2000, 0) > 0) {
        printf("Connection: %s\n", message);
    }

    return 0;
}

void start_proactor(int socket_desc) {
    int connection_sock, c;
    struct sockaddr_in connection;
    c = sizeof(struct sockaddr_in);

    while((connection_sock = accept(socket_desc, (struct sockaddr*)&connection, (socklen_t*)&c))) {
        printf("Connection accepted\n");

        pthread_t thread_id;
        int *new_sock = malloc(1);
        *new_sock = connection_sock;

        if(pthread_create(&thread_id, NULL, connection_handler, (void*)new_sock) < 0) {
            perror("Could not create thread");
            return;
        }

        printf("Handler assigned\n");
    }

    if(connection_sock < 0) {
        perror("Accept failed");
        return;
    }
}

void stop_proactor() {
    // Lock the mutex to prevent other threads from modifying the connections while we're iterating over them
    pthread_mutex_lock(&mutex);

    // Iterate over all the connections
    for(int i = 0; i < connection_count; i++) {
        // Close the socket
        close(connection_sockets[i]);
    }

    // Reset the connection count
    connection_count = 0;

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    printf("Proactor stopped\n");
}