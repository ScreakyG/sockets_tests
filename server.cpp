#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>

#define PORT "4242"
#define BACKLOG 10

int main(void)
{
    std::cout << "[SERVER]" << std::endl;

    int status;
    int socket_fd;

    addrinfo hint;
    addrinfo *res;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORT, &hint, &res);
    if (status != 0)
    {
        std::cout << "getaddrinfo : " << gai_strerror(status) << std::endl;
        return (1);
    }
    socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    std::cout << "Created server socket_fd = " << socket_fd << std::endl;

    status = bind(socket_fd, res->ai_addr, res->ai_addrlen);
    if (status != 0)
    {
        std::cout << "bind : " << std::strerror(errno) << std::endl;
        return (1);
    }
    std::cout << "Server socket_fd boud to localhost on port : " << PORT << std::endl;

    listen(socket_fd, BACKLOG);
    std::cout << "Listening on port " << PORT << " ..." << std::endl;

    int client_fd;
    sockaddr_storage    clientAddr;
    socklen_t           addrSize = sizeof(clientAddr);

    client_fd = accept(socket_fd, reinterpret_cast<sockaddr*>(&clientAddr), &addrSize);
    if (client_fd == -1)
    {
        std::cout << "accept : " << std::strerror(errno) << std::endl;
        return (1);
    }
    std::cout << "Accepted client, client_socket_fd =  " << client_fd << std::endl;

    // RECEVOIR UN MESSAGE DU CLIENT. //

    int     readedBytes = 1;
    char    buffer[BUFSIZ];
    while (readedBytes >= 0)
    {
        std::cout << "[" << client_fd << "]" << " : Reading ... " << std::endl;
        readedBytes = recv(client_fd, buffer, BUFSIZ, 0);
        if (readedBytes == -1)
        {
            std::cout << "recv error : " << std::strerror(errno) << std::endl;
            break ;
        }
        else if (readedBytes == 0)
        {
            std::cout << "[" << client_fd << "]" << " : Closed connection. " << std::endl;
            break ;
        }
        else
        {
            std::string msg = "Got your message.";
            int sentBytes;

            buffer[readedBytes] = '\0';
            std::cout << "[" << client_fd << "]" << " : Message = " << buffer << std::endl;

            sentBytes = send(client_fd, msg.c_str(), msg.size(), 0);
            if (sentBytes == -1)
                std::cout << "sending error : " << std::strerror(errno) << std::endl;
            else if (sentBytes == static_cast<int>(msg.size()))
                std::cout << "Full message was sent to client " << client_fd << " : " << msg << std::endl;
            else
                std::cout << "Partial message was sent to client." << std::endl;
        }
    }
    std::cout << "Closing client socket" << std::endl;
    close(client_fd);
    std::cout << "Closing server socket" << std::endl;
    close(socket_fd);
    return (0);
}
