#include "repos.h"
#include "stagging_area.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

std::pair<std::string, std::string> get_next_line(std::string s) {

    size_t pos = s.find('\n');

    if(pos == std::string::npos) {
        throw std::runtime_error("No newline found is the string");
    }
    
    return std::pair(s.substr(0, pos), s.substr(pos, s.size()));

}

std::string get_file_hash(std::string contentLine) {

    size_t pos1 = contentLine.find(' ');
    contentLine = contentLine.substr(pos1, contentLine.size());

    size_t pos2 = contentLine.find(' ');
    return contentLine.substr(0, pos2);

}

void StaggingArea::construct_tree(std::string curFileHash) {

    std::string fileContent = this->repos.get_repos_content(fs::path("index/objects") / get_path_to_object(curFileHash));

    std::pair<std::string, std::string> separatedContent = get_next_line(fileContent);

    if (separatedContent.first == "tree") {
        try {
            fileContent = get_next_line(separatedContent.second).second; /*Erase file name*/
            while(fileContent != "") {
                separatedContent = get_next_line(fileContent);
                std::string nextFileHash = get_file_hash(separatedContent.first);
                this->treeStaggingArea[curFileHash].push_back(nextFileHash);
                fileContent = separatedContent.second;
            }
        }
        catch (const std::runtime_error &e) {
            
        }
    }
    else if (separatedContent.second == "blob") {
        /*En fait il n'y a rien à faire ?*/
    }
    else {
        std::cerr << "Error in StaggingArea::construct_tree : invalid file type (" << separatedContent.first << ")\n";
    }

}

StaggingArea::StaggingArea(PtitGitRepos repos) {

    this->repos = repos;
    this->rootTree = repos.get_repos_content("index/INDEX");

    this->construct_tree(this->rootTree);

}