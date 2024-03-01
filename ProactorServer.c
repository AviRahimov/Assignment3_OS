# include "Proactor.h"
# include <string.h>
# define BUFFER_SIZE 1024
# define PORT 9034

static struct client** clients;
static int client_count = 0;
static pthread_mutex_t mutex;
Proactor * proactor;


// this struct is used to store the information of the client
struct client
{
    char * name;
    int socket;
};


Proactor * handle_client(int client_socket)
{
    char * buffer = (char *)malloc(BUFFER_SIZE);
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_read < 0)
    {
        printf("Client %d disconnected\n", client_socket);
        removeSocket(proactor, client_socket);
        close(client_socket);
        return NULL;
    }
    if (bytes_read < BUFFER_SIZE)
    {
        buffer[bytes_read] = '\0';
    }
    else
    {
        buffer[BUFFER_SIZE - 1] = '\0';
    }
    printf("Client %d says: %s\n", client_socket, buffer);
    // send the message to all other clients
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i]->socket != client_socket)
        {
            char msg[BUFFER_SIZE + 20];
            // socket_ +client, not socket  
            bytes_read = bytes_read + sizeof(int) + 2;
            snprintf(msg, sizeof(msg), "Client %d: %s", client_socket, buffer);
            send(clients[i]->socket, msg, bytes_read, 0);
        }
    }
    free (buffer);
    runProactor(proactor);
    pthread_join(proactor->thread, NULL);
    return proactor;
}

Proactor * handle_listen(int server_socket)
{
    while(1)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if(client_socket < 0)
        {
            perror("accept failed");
            continue;
        }
        struct client * client = (struct client *)malloc(sizeof(struct client));
        client->socket = client_socket;
        client->name = (char *)malloc(20);
        int bytes_read = recv(client->socket, client->name, 20, 0);
        if(bytes_read <= 0)
        {
            printf("Client disconnected\n");
            close(client->socket);
            free(client->name);
            free(client);
            continue;
        }
        client->name[bytes_read] = '\0';
        pthread_mutex_lock(&mutex);
        client_count++;
        clients = (struct client**)realloc(clients, client_count * sizeof(struct client*));
        clients[client_count - 1] = client;
        pthread_mutex_unlock(&mutex);
        addSocket(proactor, client->socket, handle_client);
    }
}
int main(int argc, char *argv[])
{
    int port = PORT;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        perror("socket failed");
        return 1;
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        return 1;
    }
    if(listen(server_socket, 5) < 0)
    {
        perror("listen failed");
        return 1;
    }
    clients = (struct client**)malloc(0);
    pthread_mutex_init(&mutex, NULL);
    proactor = create_proactor();
    addSocket(proactor, server_socket, handle_listen);
    runProactor(proactor);
    pthread_join(proactor->thread, NULL);
    close(server_socket);
    free(proactor);
    return 0;
}