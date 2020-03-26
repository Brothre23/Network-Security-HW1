#include "server.h"

Server::Server(char *port)
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &server_info) != 0)
    {
        fprintf(stderr, "failed to get address.");
        exit(1);
    }

    if (server_info == NULL)
    {
        fprintf(stderr, "failed to set up sockets.");
        exit(1);
    }

    for (struct addrinfo *p = server_info; p != NULL; p=p->ai_next)
    {
        socket_fd = socket(p->ai_family, p->ai_socktype, 0);
        // failed to set socket
        if (socket_fd == -1)
            continue;
        // port still available if something goes wrong
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int));
        // exit loop if binding succeeds
        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == 0)
            break;
    }

    freeaddrinfo(server_info);

    if (listen(socket_fd, CONNMAX) != 0)
    {
        fprintf(stderr, "failed to enter listening status.");
        exit(1);
    }
}