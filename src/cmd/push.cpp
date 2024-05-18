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
    
    std::string port = repos.get_config("port");
    if (port == "")  {
        std::cerr << "port of the repos not available. Please add it with :\n    petitGit config port your_port" << std::endl;
        return;
    }

    int sockfd = connect_to_server(url, stoi(port));
    send_message(sockfd, "push");
    send_message(sockfd, repos_id);

    if (!send_repos(sockfd))    {
        std::cerr << "unable to send the repose" << std::endl;
        return;
    }

    std::string reponse = read_message(sockfd, 1000);
    std::cout << reponse << std::endl;

    close(sockfd);
}
