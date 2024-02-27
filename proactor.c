// proactor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char message[2000];

    while(recv(sock, message, 2000, 0) > 0) {
        printf("Connection: %s\n", message);
    }

    close(sock);
    free(socket_desc);

    return 0;
}

void proactor(int sockets[], int num_sockets) {
    for (int i = 0; i < num_sockets; i++) {
        int client_sock = sockets[i];
        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;

        pthread_create(&thread_id, NULL, connection_handler, (void*) new_sock);
    }
}