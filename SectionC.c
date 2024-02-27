// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "proactor.h"

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

    int sockets[MAX_CONNECTIONS];
    int num_sockets = 0;

    while((client_sock = accept(server_sock, (struct sockaddr *)&client, &client_len))) {
        sockets[num_sockets++] = client_sock;
    }

    proactor(sockets, num_sockets);

    return 0;
}