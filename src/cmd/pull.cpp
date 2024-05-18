#include "pull.h"
#include "../lib/repos.h"
#include "../lib/tcp.h"
#include "../lib/compare_repos.h"
#include "../lib/reftag.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <filesystem>
#include <fstream>

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

    
    std::ifstream HEAD_file(PtitGitRepos().getWorkingFolder() / ".ptitgit" / "HEAD");
    std::stringstream buffer_HEAD;
    buffer_HEAD << HEAD_file.rdbuf();
    std::string HEAD = buffer_HEAD.str();

    if (HEAD.find("ref: ") == 0)   {
        std::cout << "You are working on the branch '" << HEAD.substr(HEAD.rfind("/")+1) << "'" << std::endl;
        std::string commit_local = ref_resolve(PtitGitRepos(), "HEAD");
        std::cout << commit_local << std::endl;
    
        std::cout << tmp_dir + "/" + HEAD.substr(HEAD.find(".ptitgit")) << std::endl;
        if ( access( (tmp_dir + "/" + HEAD.substr(HEAD.find(".ptitgit"))).c_str(), F_OK ) == -1 )   {
            std::cout << "Warning, your branch does not exist in the remote repository. Therefore 'pull' did nothing" << std::endl;
        } else  {
            std::string commit_remote = ref_resolve(PtitGitRepos(tmp_dir), "HEAD");
            std::cout << commit_remote << std::endl;
        }
    } else  {
        std::cout << "Warning, you are not working on a branch. Therefore the 'pull' did nothing" << std::endl;
    }
}
