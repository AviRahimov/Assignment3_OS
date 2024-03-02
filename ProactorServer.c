# include "Proactor.h"
# include <string.h>
# define BUFFER_SIZE 1024
# define PORT 9034

// this struct is used to store the information of the client
struct client
{
    char * name;
    int socket;
};

static struct client** clients;
static int client_count = 0;
static pthread_mutex_t mutex;
Proactor * proactor;


void handle_client(struct client* client)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;
    while(1)
    {
        bytes_read = recv(client->socket, buffer, sizeof(buffer), 0);
        if(bytes_read <= 0)
        {
            printf("Client disconnected\n");
            close(client->socket);
            return;
        }
        buffer[bytes_read] = '\0';
        printf("%s says: %s\n", client->name, buffer);
        pthread_mutex_lock(&mutex);
        for(int i = 0; i < client_count; i++)
        {
            if (clients[i]->socket != client->socket) {
                char msg [BUFFER_SIZE + 20];
                bytes_read = bytes_read + strlen(client->name) + 2;
                // snsprintf(buffer, sizeof(buffer), "%s: %s", client->name, buffer);
                snprintf(msg, sizeof(msg), "%s: %s", client->name, buffer);
                send(clients[i]->socket, msg, bytes_read, 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < client_count; i++)
    {
        if(clients[i]->socket == client->socket)
        {
            free(clients[i]->name);
            free(clients[i]);
            for(int j = i; j < client_count - 1; j++)
            {
                clients[j] = clients[j + 1];
            }
            client_count--;
            clients = (struct client**)realloc(clients, client_count * sizeof(struct client*));
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void handle_listen(void * server_socket_ptr)
{
    int server_socket = (int)server_socket_ptr;
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_attr_t thread_attr;
    pthread_mutex_init(&mutex, NULL); // initialize the mutex
    pthread_attr_init(&thread_attr); // initialize the thread attribute
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED); // set the thread to be detached - once
    int count = 0;
    // the thread is done, it will be cleaned up automatically
    while(1)
    {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_socket < 0)
        {
            printf("Error accepting connection\n");
            continue;
        }
        printf("Client connected\n");
        // mutex lock to protect the client_count
        pthread_mutex_lock(&mutex);
        // create a new client struct and add it to the clients array
        struct client* new_client = (struct client*)malloc(sizeof(struct client));
        new_client->socket = client_socket;
        // convert int to string
        new_client->name = (char*)malloc(10 * sizeof(char));
        sprintf(new_client->name, "Client %d", count);
        count++;
        clients = (struct client**)realloc(clients, (client_count + 1) * sizeof(struct client*));
        clients[client_count] = new_client;
        client_count++;
        // create a new thread to handle the client with the thread attribute defined above
        pthread_t client_thread;
        pthread_create(&client_thread, &thread_attr, (void*(*)(void*))handle_client, new_client);
        // mutex unlock
        pthread_mutex_unlock(&mutex);
    }
    // clean up the thread attribute
    pthread_attr_destroy(&thread_attr);
    // clean up the mutex
    pthread_mutex_destroy(&mutex);
}


int main(int argc, char *argv[])
{
    int server_socket;
    struct sockaddr_in server_addr;
    int opt = 1;
    int port = PORT;
    int backlog = 10000;
    proactor = create_proactor();
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        printf("Error creating socket\n");
        return 1;
    }
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        printf("Error setting socket options\n");
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error binding socket\n");
        return 1;
    }
    if(listen(server_socket, backlog) < 0)
    {
        printf("Error listening on socket\n");
        return 1;
    }
    runProactor(proactor);
    addSocket(proactor, server_socket, (void *)handle_listen);
    pthread_join(proactor->thread, NULL);
    destroyProactor(proactor);
    close(server_socket);
    return 0;
}