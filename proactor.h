// proactor.h
#ifndef PROACTOR_H
#define PROACTOR_H

#include <sys/socket.h>

void *connection_handler(void *socket_desc);
void proactor(int sockets[], int num_sockets);

#endif