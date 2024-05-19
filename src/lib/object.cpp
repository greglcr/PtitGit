#include "object.h"
#include "repos.h"

#include <filesystem>
#include <iterator>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

std::string Object::getContent() {

    return this->content;

}

std::string Object::getHashedContent() {

    return this->hashedContent;

}

fs::path Object::get_folder_to_write() {

    return get_folder_to_object(this->hashedContent);

}

fs::path Object::getPathToWrite() {

    return get_path_to_object(this->hashedContent);

}

std::string get_folder_to_object(std::string hashedContent) {

    std::string::iterator beginContent = hashedContent.begin();
    std::string folder = std::string(beginContent, beginContent + 2);

    return folder;

}

std::string get_path_to_object(std::string hashedContent) {

    std::string::iterator beginContent = hashedContent.begin();
    std::string::iterator endContent = hashedContent.end();
    
    fs::path folder = std::string(beginContent, beginContent + 2);
    fs::path fileName = std::string(beginContent + 2, endContent);

    return folder / fileName;

}

void Object::writeObject(){
    fs::path curPath = fs::current_path();

    while (curPath != curPath.root_directory() && !fs::exists(curPath / ".ptitgit")) {
        curPath = curPath.parent_path();
    }

    fs::path path = curPath / ".ptitgit" / "objects" / get_path_to_object(this->getHashedContent());
    fs::create_directory(curPath / ".ptitgit" / "objects" / std::string(this->getPathToWrite()).substr(0,2));
    std::ofstream out(path);
    out << this->getContent();
    return;
}

fs::path relativeToRepo(fs::path path){
    fs::path repoPath = PtitGitRepos().getWorkingFolder();
    long long length = std::string(repoPath).size();
    //std::cerr<<path<<std::endl<<repoPath<<std::endl;
    if(path == repoPath) return ".ptitgit/..";
    return std::string(path).substr(length+1);
}