# include <pthread.h>
# include <stdbool.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/epoll.h>

# ifndef PROACTOR_H
# define PROACTOR_H


typedef struct Proactor Proactor;
typedef struct Proactor_node Proactor_node;
typedef Proactor * (*handler_t)(int);

typedef struct Proactor
{
    // the curr proactor
    Proactor_node *head;
    // indicate if the proactor is running
    bool is_running;
    // thread
    pthread_t thread;
    // size of the proactor
    int size;
} Proactor;

typedef struct Proactor_node
{
    // the socket
    int socket;
    // the handler
    handler_t handler;
    // the next node
    Proactor_node *next;
} Proactor_node;

// this method is respinsible for creating a proactor, a list of sockets and their appropriate handlers
Proactor *create_proactor();

// this function start the proactor work
int runProactor(Proactor * proactor);

// this function stop the proactor work
int stopProactor(Proactor * proactor);

// this function add a new socket fd and its handler to the proactor
int addSocket(Proactor * proactor, int socket, handler_t handler);

// this function remove a socket from the proactor
int removeSocket(Proactor * proactor, int socket);

// destroy the proactor
int destroyProactor(Proactor * proactor);

void *proactor_run_function(void * proactor);

# endif