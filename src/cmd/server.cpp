// comes from  https://ncona.com/2019/04/building-a-simple-server-with-cpp/
// will be changed soon

#include "server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <vector>

void* handle_tcp_connection (void* arg);
void server(int port)    {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "unable to create a socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Failed to bind  : " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "server started on port " << port << std::endl;

    if (listen(sockfd, 10) < 0) {
        std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
   
    

    auto addrlen = sizeof(sockaddr);
    int accepted;
    std::vector<pthread_t> threads;
    while ((accepted = accept(sockfd,(struct sockaddr *)&sockaddr, (socklen_t*)&addrlen)) > 0) {
        threads.push_back(0);
        if( pthread_create(&(threads.back()), NULL, &handle_tcp_connection, (void*)&accepted) != 0){
            std::cerr << "Unable to create a new thread" << std::endl;
        }
        std::cout << threads.front() << " " << threads.back() << std::endl;
    }

}
void* handle_tcp_connection (void* arg)   {
    std::cout << "Hi" << std::endl;
    int* intPtr = (int*) arg;
    int connection = *intPtr;
    while (true)    {
        char buffer[100];
        int n = read(connection, buffer, 100);
        buffer[n] = '\0';
        std::cout << "The message was: " << buffer << std::endl;

        std::string response = "Good talking to you\n";
        send(connection, response.c_str(), response.size(), 0);
    }
}
