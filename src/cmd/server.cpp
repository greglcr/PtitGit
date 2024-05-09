// comes from  https://ncona.com/2019/04/building-a-simple-server-with-cpp/
// will be changed soon

#include "server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>

void server()   {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "unable to create a socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999);

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Failed to bind  : " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 10) < 0) {
        std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    
    auto addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    auto bytesRead = read(connection, buffer, 100);
    std::cout << "The message was: " << buffer;

    std::string response = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);

    close(connection);
    close(sockfd);



    while (true)    {
        std::cout << "server is running" << std::endl;
        usleep(1000000);
    }
}
