# define PROACTOR_H
# ifdef PROACTOR_H

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
    // handler
    handler_t handler;
    // the next proactor
    Proactor_node *next;
} Proactor_node;

// this method is respinsible for creating a proactor, a list of sockets and their appropriate handlers
Proactor *create_proactor(int size);

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

# endif // PROACTOR_H