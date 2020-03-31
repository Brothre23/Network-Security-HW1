#include "server.h"

int main(int argc, char* argv[])
{
    Server server = Server(argv[1]);
    server.start_server();
    return 0;
}