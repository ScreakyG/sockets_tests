#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <stdlib.h>
#include <sstream>
#include "colors.hpp"

#define PORT "4242"
#define BACKLOG 10

static int  createServerSocket(void)
{
    int         status;
    int         server_socket;
    addrinfo    hint;
    addrinfo    *res;

    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM; // TCP OU UDP.
    hint.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORT, &hint, &res);
    if (status != 0)
    {
        std::cout << RED << "getaddrinfo : " << gai_strerror(status) << RESET << std::endl;
        return (1);
    }
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    std::cout << PURPLE << "Created server socket_fd = " << server_socket << RESET << std::endl;

    status = bind(server_socket, res->ai_addr, res->ai_addrlen);
    if (status != 0)
    {
        std::cout << "bind : " << std::strerror(errno) << std::endl;
        return (1);
    }
    std::cout << PURPLE << "Server socket_fd boud to localhost on port : " << PORT << RESET << std::endl;
    return (server_socket);
}

static void accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max)
{
    int client_fd;

    client_fd = accept(server_socket, NULL, NULL);
    if (client_fd == -1)
    {
        std::cout << RED << "accept : " << std::strerror(errno) << RESET << std::endl;
        return ;
    }
    FD_SET(client_fd, all_sockets); // Ajoute le nouveau socket client a la liste de tout les sockets.
    if (client_fd > *fd_max) // Si le nouveau client fd est plus grand le serveur alors on change le fd_max.
        *fd_max = client_fd;
    std::cout << "[Server] Accepted new connection on client socket " << client_fd << std::endl;

    // On envoie un message de bienvenue au client.
    int                 sentBytes;
    std::stringstream   ss;

    ss << "Welcome, you are the client " << client_fd << std::endl;
    std::string msg = ss.str();
    sentBytes = send(client_fd, msg.c_str(), msg.size(), 0);

    if (sentBytes == -1)
            std::cout << RED << "[Server] sending error : " << std::strerror(errno) << RESET << std::endl;
    else if (sentBytes != static_cast<int>(msg.size()))
            std::cout << YELLOW << "[Server] Partial message was sent to client." << RESET << std::endl;

}

static void handle_client(int socket, fd_set *all_sockets, int fd_max, int server_socket)
{
    int     status;
    int     readedBytes = 1;
    char    buffer[BUFSIZ];

    std::stringstream   ss;
    std::string         msg;

    memset(&buffer, '\0', sizeof (buffer));
    readedBytes = recv(socket, buffer, BUFSIZ, 0);
    if (readedBytes <= 0)
    {
        if (readedBytes == -1)
            std::cout << RED << "[Server] recv error : " << std::strerror(errno) << RESET << std::endl;
        else if (readedBytes == 0)
            std::cout << "[" << socket << "]" << " : Closed connection." << std::endl;
        close(socket);
        FD_CLR(socket, all_sockets);
    }
    else
    {
        ss << "[" << socket <<  "]" << " : " << buffer;
        msg = ss.str();
        for (int i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, all_sockets) && i != server_socket && i != socket)
            {
                status = send(i, msg.c_str(), msg.size(), 0);
                if (status == -1)
                    std::cout << RED << "[Server] sending error to client  " << i << " : " << std::strerror(errno) << RESET << std::endl;
            }
        }
    }
}

int main(void)
{
    std::cout << "[SERVER]" << std::endl;

    fd_set          all_sockets;
    fd_set          read_fds;
    int             fd_max;
    struct  timeval timer;

    int             status;

    int server_socket = createServerSocket();
    if (server_socket == 1)
        return (1);

    listen(server_socket, BACKLOG);
    std::cout << PURPLE << "Listening on port " << PORT << " ..." << RESET << std::endl;


    FD_ZERO(&all_sockets); // Initialise la liste des sockets a 0.
    FD_ZERO(&read_fds);    // Initialise la liste des reads sockets a 0.
    FD_SET(server_socket, &all_sockets); // Ajoute le socket du server dans la liste de tout les sockets.
    fd_max = server_socket; // Le descripteur de fichier le plus grand est le dernier creer , donc le serveur.

    while (1)
    {
        read_fds = all_sockets;
        timer.tv_sec = 2; // Timeout de 2 sec pour select().
        timer.tv_usec = 0;

        status = select(fd_max + 1, &read_fds, NULL, NULL, &timer);
        if (status == -1)
        {
            std::cout << RED << "[Server] Select error : " << std::strerror(errno) << RESET << std::endl;
            exit(1);
        }
        else if (status == 0) // Auncun descripteur de fichier socket n'est pret pour la lecture.
        {
            std::cout << "[Server] Waiting... " << std::endl;
            continue ;
        }

        // Sinon on boucle sur les sockets et on traite ceux qui sont prets.
        for (int i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, &read_fds) != 1)
                continue ; // Le fd i n'est pas une socket a surveiller alors on continue.
            std::cout << "[" << i << "]" << " Ready for I/O operation" << std::endl;
            if (i == server_socket)
                accept_new_connection(server_socket, &all_sockets, &fd_max); // Accepter une nouvelle socket client.
            else
                handle_client(i, &all_sockets, fd_max, server_socket);// Lire la socket client correspondant a i.
        }
    }
    return (0);
}
