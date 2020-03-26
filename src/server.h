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

#define CONNMAX 1000

class Server
{
public:
    Server(char *port);
    void start_server();

private:
    struct addrinfo hints;
    struct addrinfo *server_info;
    int socket_fd, clients[CONNMAX];
    void receive(int client_index);
};

#endif