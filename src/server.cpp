#include "server.h"

Server::Server(char *port)
{
    this->port = port;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &server_info) != 0)
    {
        fprintf(stderr, "failed to get address.\n");
        exit(1);
    }

    if (server_info == NULL)
    {
        fprintf(stderr, "failed to set up sockets.\n");
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
}

void Server::start_server()
{
    client_index = 0;

    // -1 means no clients connected
    for (int i = 0; i < CONNMAX; i++)
        clients[i] = -1;

    if (listen(socket_fd, CONNMAX) != 0)
    {
        fprintf(stderr, "failed to enter listening status.\n");
        exit(1);
    }
    printf("listen http://127.0.0.1:%s...\n", port);

    while(1)
    {
        addr_len = sizeof(client_addr);
        clients[client_index] = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_len);

        if(clients[client_index] < 0)
            fprintf(stderr, "connection with clients failed.\n");
        else
        { 
            // child process
            if (fork() == 0)
            {
                receive(client_index);
                exit(0);
            }
        }

        // find available client numbers
        while (clients[client_index] != -1)
            client_index = (client_index + 1) % CONNMAX;
    }
}

void Server::receive(int client_index)
{
    buffer = (char *)malloc(65535);
    buffer_len = recv(clients[client_index], buffer, 65535, 0);

    if (buffer_len < 0)
        fprintf(stderr, "failed to receive data.\n");
    else if (buffer_len == 0)
        fprintf(stderr, "client disconnected upexpectedly.\n");
    else
    {
        buffer[buffer_len] = '\0';
        Request request = Request(buffer, buffer_len);
        print_request(&request, clients[client_index]);
    }
    shutdown(clients[client_index], SHUT_RDWR);
    close(clients[client_index]);
    clients[client_index] = -1;
}

void Server::print_request(Request *request, int client_fd)
{
    if (request == NULL)
        dprintf(client_fd, "HTTP/1.1 500 Error\r\n\r\n");
    else if (request->uri == "/" && request->method == "GET")
    {
        dprintf(client_fd, "HTTP/1.1 200 OK\r\n");
        dprintf(client_fd, "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n");
        dprintf(client_fd, "Server: Meow_Server/1.0 (Mac)\r\n");
        dprintf(client_fd, "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT Content-Length: 88\r\n");
        dprintf(client_fd, "Content-Type: text/html\r\n");
        dprintf(client_fd, "Connection: Closed\r\n");
        dprintf(client_fd, "\r\n");
        dprintf(client_fd, "<html><body> <h1>Hello, World!</h1> </body></html>\r\n");
    }
    else if (request->uri == "/" && request->method == "POST")
    {
        dprintf(client_fd, "HTTP/1.1 200 OK\r\n\r\n");
        dprintf(client_fd, "Wow, seems that you POSTed %lu bytes. \r\n", request->payload_size);
        dprintf(client_fd, "Fetch the data using `payload` variable.");
    }
    else
        dprintf(client_fd, "HTTP/1.1 404 Not Found\r\n\r\n");
}

// dprintf(client_fd, "Server: Apache/2.2.14 (Win32)\r\n");