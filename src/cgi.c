#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

/*
Please make sure you understand host.c
*/

int main(void)
{
    int unread;
    char *buf;

    // fprintf(stderr, "\nMEOW\n");
    // wait for stdin
    while (unread < 1)
    {
        if (ioctl(STDIN_FILENO, FIONREAD, &unread))
        {
            perror("ioctl");
            exit(EXIT_FAILURE);
        }
    }
    buf = (char *)malloc(sizeof(char) * (unread + 1));

    // read from stdin fd
    read(STDIN_FILENO, buf, unread);

    // output to stdout
    printf("HTTP/1.1 200 OK\r\n");
    printf("Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n");
    printf("Server: Meow_Server/1.0 (Mac)\r\n");
    printf("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT Content-Length: 88\r\n");
    printf("Content-Type: text/html\r\n");
    printf("Connection: Closed\r\n");
    printf("\r\n");
    printf("<html><body> <h1>Hello, World!</h1> </body></html>\r\n");
}