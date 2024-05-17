#include "server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../lib/tcp.h"
#include "../cmd/init.h"

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
    int* intPtr = (int*) arg;
    int connection = *intPtr;

    std::string cmd = read_message(connection, 100);

    if (cmd == "push")  {
        std::string directory = receive_repos(connection);
        send_message(connection,"Well received\nPlease note that this version of 'push' does not contain any guards.");
    } else if (cmd == "init-remote")    {
        long long repos_id = 1;
        struct stat info;
        while (stat(std::to_string(repos_id).c_str(), &info) == 0)
            repos_id++;
        
        std::string name_folder = std::to_string(repos_id);
        mkdir(name_folder.c_str(), 0700);
        std::cout << "Create repos with id : " << repos_id << std::endl;
        init(name_folder);
        send_message(connection,"repos created with id " + name_folder);
    } else  {
        send_message(connection,"INVALID ACTION");
    }
    pthread_exit(EXIT_SUCCESS);
}
