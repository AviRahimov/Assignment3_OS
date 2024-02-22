# include <stdio.h> // for printf
# include <stdlib.h> // for atoi
# include <string.h> // for memset
# include <unistd.h> // for close and read
# include <sys/socket.h> // for socket, bind, listen, accept, recv
# include <netinet/in.h> // for sockaddr_in
# include <pthread.h> // for pthread
# include <arpa/inet.h> // for inet_addr
# include <sys/types.h> // for close
# include <sys/socket.h> // for close

# define BUFFER_SIZE 1024
# define PORT 9034

// This function is used to handle the keyboard input from the user and send it to the server
void handle_keyboard_input(int);
void handle_listen(int server_socket);
static pthread_mutex_t mutex;

// Create a socket and connect to the server
int main(int argc, char const *argv[])
{
    int server_socket;
    struct sockaddr_in server_addr;
    int port = PORT;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        printf("Error creating socket\n");
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error connecting to server\n");
        return 1;
    }
    // Create two threads, one to handle the listening and one to handle the keyboard input
    pthread_t listen_thread, keyboard_input_thread;
    // mutex init
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&listen_thread, NULL, (void*)handle_listen, server_socket);
    pthread_create(&keyboard_input_thread, NULL, (void*)handle_keyboard_input, server_socket);
    pthread_join(listen_thread, NULL);
    pthread_join(keyboard_input_thread, NULL);
    close(server_socket);
    return 0;
}


void handle_listen(int server_socket)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;
    while(1)
    {   
        bytes_read = recv(server_socket, buffer, sizeof(buffer), 0);
        // if receiveing EOF character
        if(bytes_read == 0)
        {
            printf("Server has closed the connection\n");
            break;
        }
        buffer[bytes_read] = '\0';
        // lock the mutex
        pthread_mutex_lock(&mutex);
        printf("Server says: %s\n", buffer);
        // unlock the mutex
        pthread_mutex_unlock(&mutex);
    }
}

void handle_keyboard_input(int server_socket)
{
    char buffer[BUFFER_SIZE];
    while(1)
    {
        // if there is input from the user and not EOF
        if(fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            // lock the mutex
            pthread_mutex_lock(&mutex);
            buffer[strcspn(buffer, "\n")] = 0;
            send(server_socket, buffer, strlen(buffer), 0);
            // unlock the mutex
            pthread_mutex_unlock(&mutex);
        }
    }
}