#include "push.h"
#include "../lib/repos.h"
#include "../lib/tcp.h"
#include <iostream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void push() {

    PtitGitRepos repos = PtitGitRepos();

    std::string url = repos.get_config("url");
    if (url == "")  {
        std::cerr << "url of the server not available. Please add it with :\n    petitGit config url your_url" << std::endl;
        return;
    }

    std::string repos_id = repos.get_config("repos-id");
    if (repos_id == "")  {
        std::cerr << "id of the repos not available. Please add it with :\n    petitGit config repos-id your_id" << std::endl;
        return;
    }
    
    std::string port_str = repos.get_config("port");
    if (repos_id == "")  {
        std::cerr << "port of the repos not available. Please add it with :\n    petitGit config port your_port" << std::endl;
        return;
    }




    struct sockaddr_in serv_addr;
    struct hostent *server;

    int port = stoi(port_str);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "ERROR when opening the socket" << std::endl;
        exit(-1);
    }
    server = gethostbyname(url.c_str());
    if (server == NULL) {
        std::cerr << "ERROR : no such host" << std::endl;
        exit(-1);
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,  server->h_length);

    serv_addr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "ERROR connecting" << std::endl;
        exit(-1);
    }

    if (!send_repos(sockfd))    {
        std::cerr << "unable to send the repose" << std::endl;
        return;
    }

    char response [1000];
    bzero(response, 1000);
    read(sockfd, response, 1000-1);
    std::cout << response << std::endl;

    close(sockfd);
}
