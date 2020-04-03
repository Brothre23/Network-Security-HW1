#ifndef REQUEST_H
#define REQUEST_H

#include <unordered_map>
#include <string>
#include <cstring>

// Client request
class Request
{
public:
    Request(char *buffer, size_t buffer_length);

    std::string protocol;
    std::string method;
    std::string uri;
    std::string qs;

    char *header_name, *header_value;

    char *remain_data;
    char *payload;
    size_t payload_size;
    std::string content_length_string;

    std::unordered_map<std::string, std::string> headers;
};

#endif