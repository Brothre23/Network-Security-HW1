#include "server.h"
#include "string"

int main(int argc, char* argv[])
{
    std::string port;
    if (argv[1])
        port = std::string(argv[1]);
    else
        port = std::string("80");
    Server server = Server(port.c_str());
    server.start_server();
    return 0;
}