#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void)
{
    std::cout << "Hello im the client" << std::endl;

    int status;
    int socket_fd;
    addrinfo    hint;
    addrinfo    *res;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("www.google.com", "http", &hint, &res);
    if (status != 0)
    {
        std::cout << "getaddrinfo : "<< gai_strerror(status) << std::endl;
        return (1);
    }
    socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1)
    {
        std::cout << "Socket problem : " << std::strerror(errno) << std::endl;
        return (1);
    }
    connect(socket_fd, res->ai_addr, res->ai_addrlen);
    return (0);
}
