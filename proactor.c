// proactor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
        int server_sock = sockets[i];
        int client_sock;

        struct sockaddr_in client;
        int c = sizeof(struct sockaddr_in);

        //Listen
        listen(server_sock , 3);

        //Accept incoming connection
        printf("Waiting for incoming connections...\n");
        c = sizeof(struct sockaddr_in);

        while((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&c))) {
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

        if (client_sock < 0) {
            perror("accept failed");
            return 1;
        }
    }
}