# include "Proactor.h"

// Proactor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
# include <stdbool.h> // for bool
# include <unistd.h> // for close


void *proactor_run_function(void * proactor)
{
    Proactor * proactor_ = (Proactor *)proactor;
    Proactor_node * curr = (Proactor_node *)proactor_->head;
    while (curr != NULL && proactor_->is_running)
    {
        if (curr->handler != 0)
        {
            curr->handler(curr->socket);
        }
        curr = curr->next;
    }
    proactor_->is_running = false;
    pthread_exit(proactor_);
    return NULL;
}

Proactor * create_proactor()
{
    Proactor * proactor = (Proactor *)malloc(sizeof(Proactor));
    proactor->thread = 0;
    proactor->head = NULL;
    proactor->is_running = false;
    proactor->size = 0;
    return proactor;
}

int runProactor(Proactor * proactor)
{
    Proactor * proactor_ = proactor;
    proactor_->is_running = true;
    pthread_create(&proactor->thread, NULL, proactor_run_function, (void *)proactor_);
    return 0;
}

int stopProactor(Proactor * proactor)
{
    Proactor * proactor_ = proactor;
    proactor_->is_running = false;
    pthread_cancel(proactor->thread);
    pthread_join(proactor->thread, NULL);
    return 0;
}

int addSocket(Proactor * proactor, int socket, handler_t handler)
{
    Proactor * proactor_ = proactor;
    Proactor_node * node = (Proactor_node *)malloc(sizeof(Proactor_node));
    node->socket = socket;
    node->handler = handler;
    node->next = NULL;
    if (proactor_->head == NULL)
    {
        proactor_->head = node;
    }
    else
    {
        Proactor_node * temp = proactor_->head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = node;
    }
    proactor_->size++;
    return 0;
}

int removeSocket(Proactor * proactor, int socket)
{
    Proactor * proactor_ = proactor;
    Proactor_node * curr = proactor_->head;
    if (curr->socket == socket)
    {
        proactor_->head = curr->next;
        free(curr);
        proactor_->size--;
        return 0;
    }
    while (curr->next != NULL)
    {
        if (curr->next->socket == socket)
        {
            Proactor_node * temp = curr->next;
            curr->next = temp->next;
            free(temp);
            proactor_->size--;
            return 0;
        }
        curr = curr->next;
    }
    return 1;
}

int destroyProactor(Proactor * proactor)
{
    Proactor * proactor_ = proactor;
    if (proactor_->is_running)
    {
        stopProactor(proactor_);
    }
    if (proactor_->head != NULL)
    {
        Proactor_node * curr = proactor_->head;
        while (curr != NULL)
        {
            Proactor_node * temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(proactor_);
    return 0;
}