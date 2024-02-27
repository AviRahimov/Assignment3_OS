#include <stdio.h> // for printf
#include <stdlib.h> // for atoi
#include <string.h> // for memset
#include <unistd.h> // for close and read
#include <sys/socket.h> // for socket, bind, listen, accept, recv
#include <netinet/in.h> // for sockaddr_in
#include <pthread.h> // for pthread

# define PORT 9034
# define MAX_BACKLOG 100
# define BUFFER_SIZE 1024

// this struct is used to store the information of the client
struct client
{
    char * name;
    int socket;
};

// this is s function that is used to handle with the client recieving messages
void handle_client(struct client* client);

// this is a function that is used to handle the listening of the server, with accepting the clients
void handle_listen(void *);
static struct client** clients;
static int client_count = 0;
static pthread_mutex_t mutex;

// this is the main that creates the server and listens to the clients using the handle_listen function
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
    // Create a thread to handle the listening
    pthread_t listen_thread;
    int * server_socket_ptr = (int *)malloc(sizeof(int));
    *server_socket_ptr = server_socket;
    pthread_create(&listen_thread, NULL, (void*(*)(void*))handle_listen, server_socket_ptr);
    pthread_join(listen_thread, NULL);
    close(server_socket);
    // free the memory
    free(server_socket_ptr);
    return 0;
}

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
    int server_socket = *((int*)server_socket_ptr);
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