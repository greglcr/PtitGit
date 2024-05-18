#include "pull.h"
#include "../lib/repos.h"
#include "../lib/tcp.h"
#include "../lib/compare_repos.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void pull() {

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
    send_message(sockfd, "pull");
    send_message(sockfd, repos_id);

    std::string tmp_dir = receive_repos(sockfd);
    if (tmp_dir == "")  {
        std::cout << "repository not received" << std::endl;
        close(sockfd);
        return;
    }
    std::cout << "tmp dir : " << tmp_dir << std::endl; 
    close(sockfd);


    if (!copy_all_objects(tmp_dir, ".")) {
        std::cerr << "An error as occured, try again." << std::endl;
        return;
    }
 
}
