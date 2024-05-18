#include "init-remote.h"
#include "../lib/tcp.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void init_remote(std::string url, int port)  {
    int sockfd = connect_to_server(url, port);
    
    send_message(sockfd, "init-remote");
    std::string response = read_message(sockfd, 1000);

    std::cout << response << std::endl;
    close(sockfd);
}

