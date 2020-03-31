#include "request.h"

Request::Request(char *buf, size_t buf_len)
{
    // parse protocol
    method = strtok(buf, " \t\r\n");
    uri = strtok(NULL, " \t");
    prot = strtok(NULL, " \t\r\n");

    if (auto pos = uri.find("?"); pos != std::string::npos)
    {
        qs = uri.substr(pos, uri.length());
        uri = uri.substr(0, pos);
    }
    fprintf(stderr, "[%s] %s %s\n", method.c_str(), prot.c_str(), uri.c_str());

    // parse header
    while (true)
    {
        char *header_name, *header_value;
        header_name = strtok(NULL, "\r\n: \t");
        if (!header_name)
            break;
        header_value = strtok(NULL, "\r\n");
        while (*header_value && *header_value == ' ')
            header_value++;
        this->headers[header_name] = header_value;
        fprintf(stderr, "[HEADER] %s: %s\n", header_name, header_value);

        char *remain = header_value + strlen(header_value) + 1 + 1; // + strlen + \0 + \n
        // blank line which contains "\r\n" is used to seperate header and body
        if (remain[0] == '\r' && remain[1] == '\n')
            break;
    }

    // parse payload
    char *remain_data = strtok(NULL, "\r\n"); // the beginning of user payload
    if (remain_data != NULL)
    {
        // related header if there is
        if (auto contentLenStr = headers["Content-Length"]; !contentLenStr.empty())
            payload_size = stoi(contentLenStr);
        else
            payload_size = buf_len - (remain_data - buf);
        payload = remain_data;
        payload[payload_size + 1] = '\0';
        fprintf(stderr, "[PAYLOAD] %s\n", payload);
    }
    fprintf(stderr, "\n");
};