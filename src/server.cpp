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
    printf("listening on http://127.0.0.1:%s...\n\n", port);

    while(1)
    {
        addr_len = sizeof(client_addr);
        clients[client_index] = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_len);

        if(clients[client_index] < 0)
            fprintf(stderr, "connection with clients failed.\n");
        else
        { 
/*             // child process
            if (fork() == 0)
            {
                Request *request = receive(client_index);
                exit(0);
            } */
            Request *request = receive(client_index);

            if (pipe(toCGI) < 0)
            {
                fprintf(stderr, "failed to establish pipe.");
                exit(EXIT_FAILURE);
            }
            if (pipe(fromCGI) < 0)
            {
                fprintf(stderr, "failed to establish pipe.");
                exit(EXIT_FAILURE);
            }
            if ((cpid = fork()) < 0)
            {
                fprintf(stderr, "failed to fork.");
                exit(EXIT_FAILURE);
            }

            // child process
            if (cpid == 0)
            {
                close(toCGI[1]);
                close(fromCGI[0]);

                // redirect the output from stdout to fromCGI
                dup2(fromCGI[1], STDOUT_FILENO);
                //redirect the input from stdin to toCGI
                dup2(toCGI[0], STDIN_FILENO);

                close(fromCGI[1]);
                close(toCGI[0]);

                execlp("./build/cgi", "cgi-meow", NULL);
                exit(0);
            }
            // parent process
            else
            {
                close(toCGI[0]);
                close(fromCGI[1]);

                // send the message to the CGI program
                write(toCGI[1], input_data, strlen(input_data));

                // waitpid(cpid, &process_state, 0);

                // receive the message from the  CGI program
                while(read(fromCGI[0], &c, 1) > 0)
                {
                    // write the message to client
                    write(clients[client_index], &c, 1);
                }

                write(clients[client_index], "\n", 1);

                close(fromCGI[0]);
                close(toCGI[1]);
                waitpid(cpid, &process_state, 0);
            }
        }

        shutdown(clients[client_index], SHUT_RDWR);
        close(clients[client_index]);
        clients[client_index] = -1;

        // find available client numbers
        while (clients[client_index] != -1)
            client_index = (client_index + 1) % CONNMAX;
    }
}

Request* Server::receive(int client_index)
{
    Request *request;

    buffer = (char *)malloc(65535);
    buffer_length = read(clients[client_index], buffer, 65535);

    if (buffer_length < 0)
        fprintf(stderr, "failed to receive data.\n");
    else if (buffer_length == 0)
        fprintf(stderr, "client disconnected upexpectedly.\n");
    else
    {
        buffer[buffer_length] = '\0';
        request = new Request(buffer, buffer_length);
        // print_response(request, clients[client_index]);
    }
    return request;
}

/* void Server::print_response(Request *request, int client_fd)
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
} */