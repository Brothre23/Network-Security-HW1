#ifndef REQUEST_H
#define REQUEST_H

#include <unordered_map>
#include <string>
#include <cstring>

// Client request
class Request
{
public:
    Request(char *buf, size_t buf_len);

    std::string prot;
    std::string method;
    std::string uri;
    std::string qs;

    char *payload;
    size_t payload_size;
    std::unordered_map<std::string, std::string> headers;
};

#endif