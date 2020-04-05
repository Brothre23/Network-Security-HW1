#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "request.h"

#include <string>
#include <fstream>

/*
Please make sure you understand host.c
*/

int main(void)
{
    int unread;
    char *buffer;

    char ch;

    // wait for stdin
    while (unread < 1)
    {
        if (ioctl(STDIN_FILENO, FIONREAD, &unread))
        {
            perror("ioctl");
            exit(EXIT_FAILURE);
        }
    }
    buffer = (char *)malloc(sizeof(char) * (unread + 1));

    // read from stdin fd
    read(STDIN_FILENO, buffer, unread);

    Request response(buffer, strlen(buffer));

    // output to stdout
    printf("HTTP/1.1 200 OK\r\n");
    printf("Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n");
    printf("Server: Meow_Server/1.0 (Mac)\r\n");
    printf("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT Content-Length: 88\r\n");
    printf("Content-Type: text/html\r\n");
    printf("Connection: Closed\r\n");
    printf("\r\n");
    
    if (response.method == "GET")
    {
        if (response.url == "form")
        {
            std::ifstream web;
            web.open("./form.html");
            while (!web.eof())
            {
                web.get(ch);
                printf("%c", ch);
            }
        }
        else 
            printf("<html><body> Hello World! <br> %s <br> %s </body></html>\r\n", response.url.c_str(), response.qs.c_str());
    }
    if (response.method == "POST")
        printf("%s", response.payload);
}