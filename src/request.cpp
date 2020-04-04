#include "request.h"

Request::Request(char *buffer, size_t buffer_length)
{
    // printf("%s\n\n", buffer);

    // parse protocol
    method = strtok(buffer, " \t\r\n");
    url = strtok(NULL, " \t");
    protocol = strtok(NULL, " \t\r\n");

    url = url.substr(1);

    if (int pos = url.find("?"); pos != std::string::npos)
    {
        qs = url.substr(pos + 1);
        url = url.substr(0, pos);
    }

    /*     
    if (int pos = uri.find("?"); pos != std::string::npos)
    {
        qs = uri.substr(pos, uri.length());
        uri = uri.substr(0, pos);
    } 
    */
    fprintf(stderr, "[%s] %s %s\n", method.c_str(), protocol.c_str(), url.c_str());

    // parse header
    while (true)
    {
        header_name = strtok(NULL, "\r\n: \t");
        if (!header_name)
            break;
        header_value = strtok(NULL, "\r\n");
        // skip over spaces
        while (*header_value && *header_value == ' ')
            header_value++;
        this->headers[header_name] = header_value;
        fprintf(stderr, "[%s]: %s\n", header_name, header_value);

        remain_data = header_value + strlen(header_value) + 1 + 1; // + strlen + \0 + \n
        // blank line which contains "\r\n" is used to seperate header and body
        if (remain_data[0] == '\r' && remain_data[1] == '\n')
            break;
    }

    // parse payload
    payload = strtok(NULL, "\r\n"); // the beginning of user payload
    if (payload)
    {
        // related header if there is
        if (content_length_string = headers["Content-Length"]; !content_length_string.empty())
            payload_size = stoi(content_length_string);
        else
            payload_size = buffer_length - (payload - buffer);
        payload[payload_size + 1] = '\0';
        fprintf(stderr, "[PAYLOAD] %s\n", payload);
        /* fprintf(stderr, "[PAYLOAD]\n");
        for (int i = 0; i < payload_size; i++)
            fprintf(stderr, "%c", payload[i]); */
    }
    fprintf(stderr, "\n");
};