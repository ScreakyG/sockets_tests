#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


static int domainIpResolver(void)
{
    std::string domainName;
    std::cout << "Pleaser enter domain name : " << std::endl;
    std::cin >> domainName;
     if (domainName.empty() == true)
        return (std::cout << "Error \n", 1);

    addrinfo    hintt;
    addrinfo    *list;
    char        buffer[INET6_ADDRSTRLEN];

    // Sert de filtre pour getaddrinfo().
    memset(&hintt, 0, sizeof(hintt));
    hintt.ai_family = AF_UNSPEC; // IPV4 et IPV6.
    hintt.ai_socktype = SOCK_STREAM; // TCP (va persister pour le renvoie des donner si echec) , differement de UDP.
    //Permet de récuperer l'adresse IP (sous forme d'entier) à partir d'un nom de domaine et stocker le resultat dans une liste chainee.
    int result = 0;
    result = getaddrinfo(domainName.c_str(), "80", &hintt, &list);
    if (result != 0)
       std::cout << gai_strerror(result) << std::endl;

    addrinfo    *index = list;

    while (index)
    {
        if (index->ai_family == AF_INET)
        {
            std::cout << "IPV4 ADDRESS = ";

            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in*>(index->ai_addr);
            inet_ntop(index->ai_family, &ipv4->sin_addr, buffer, sizeof(buffer));
            std::cout << buffer << std::endl;
        }
        else if (index->ai_family == AF_INET6)
        {
            std::cout << "IPV6 ADDRESS = ";

            sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6*>(index->ai_addr);
            inet_ntop(index->ai_family, &ipv6->sin6_addr, buffer, sizeof(buffer));
            std::cout << buffer << std::endl;
        }
        index = index->ai_next;
    }
    freeaddrinfo(list);
    return (0);
}

int main(void)
{
    int menuInput;

    std::cout << "What you want to do ?" << std::endl;
    std::cout << "[1] Get ip address from domain name" << std::endl;
    std::cout << "[2] Try server/client connection" << std::endl;

    std::cin >> menuInput;
    switch(menuInput)
    {
        case 1:
        {
            domainIpResolver();
            break ;
        }
        case 2:
        {
            std::cout << "You selected option 2" << std::endl;
            break ;
        }
        default :
        {
            std::cout << "Wront input" << std::endl;
            break ;
        }
    }
}
