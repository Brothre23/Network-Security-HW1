#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#include "request.h"

#define CONNMAX 1000

class Server
{
public:
    Server(char *port);
    void start_server();

private:
    struct addrinfo hints;
    struct addrinfo *server_info;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int socket_fd, clients[CONNMAX];
    int client_index;
    char *port;

    size_t buffer_len;
    char *buffer;

    void receive(int client_index);
    void print_request(Request *request, int client_fd);
};

#endif