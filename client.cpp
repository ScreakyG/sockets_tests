#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <cstdio>

int main(int argc, char** argv)
{
    std::cout << "[CLIENT]" << std::endl;

    int status;
    int socket_fd;
    addrinfo    hint;
    addrinfo    *res;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(NULL, "4242", &hint, &res);
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
    status = connect(socket_fd, res->ai_addr, res->ai_addrlen);
    if (status != 0)
    {
        std::cout << "Error client connection : " << gai_strerror(status) << std::endl;
        return (1);
    }

    // Sending message to server. //
    char *msg = argv[1];
    int msg_len = strlen(msg);
    int bytes_send;

    bytes_send = send(socket_fd, msg, msg_len, 0);
    if (bytes_send == -1)
        std::cout << "Error when sending message to server : " << std::strerror(errno) << std::endl;
    else if (bytes_send == msg_len)
        std::cout << "Client sent full message to server" << std::endl;
    else
        std::cout << "Client sent partial message to server" << std::endl;


    // Receiving message from server. //
    int bytes_read = 1;
    char buffer[BUFSIZ];
    while (bytes_read >= 0)
    {
        bytes_read = recv(socket_fd, buffer, BUFSIZ, 0);
        if (bytes_read == 0)
        {
            std::cout << "Server connection closed" << std::endl;
            break ;
        }
        else if (bytes_read == -1)
        {
            std::cout << "Client recv error : " << std::strerror(errno) << std::endl;
            break ;
        }
        else
        {
            buffer[bytes_read] = '\0';
            std::cout << "Message from server : " << buffer << std::endl;
            break ;
        }
    }
    std::cout << "Closing socket fd" << std::endl;
    close(socket_fd);
    (void)argc;
    return (0);
}
