// server.c
#include "Proactor.h" // Include the proactor library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9034

struct client
{
    char * name;
    int socket;
};

void handle_client(struct client* client);
static struct client** clients;
static int client_count = 0;
static pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    int server_socket;
    struct sockaddr_in server_addr;
    int port = PORT;
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        printf("Error creating socket\n");
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error binding socket\n");
        return 1;
    }
    if(listen(server_socket, 5) < 0)
    {
        printf("Error listening on socket\n");
        return 1;
    }

    // Start the proactor
    start_proactor(server_socket);

    // The rest of your server code...
    // Wait for a shutdown signal
    while(1) {
        char command[100];
        fgets(command, sizeof(command), stdin);

        // If the command is "shutdown", break the loop
        if(strncmp(command, "shutdown", 8) == 0) {
            break;
        }
    }

    // Stop the proactor
    stop_proactor();

    // Clean up resources
    for(int i = 0; i < client_count; i++) {
        close(clients[i]->socket);
        free(clients[i]);
    }
    free(clients);
    close(server_socket);

    return 0;
}
