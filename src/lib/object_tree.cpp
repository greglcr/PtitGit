#include "hashing.h"
#include "object.h"
#include "object_file.h"
#include "object_tree.h"
#include "repos.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;

Tree::Tree(fs::path folderPath, bool create, bool empty) {

    if (!fs::exists(folderPath)) {
        std::cerr << "Error in Tree construction : The given path doesn't exists (" << folderPath << ")" << std::endl;
    }
    if (!fs::is_directory(folderPath)) {
        std::cerr << "Error in Tree construction : The given path is not a directory (" << folderPath << ")" << std::endl;
    }

    if (folderPath.is_relative()) {
        folderPath = fs::current_path() / folderPath;
    }
    
    this->content += relativeToRepo(folderPath);
    this->content += '\n';
    this->folderPath = folderPath;

    if (!empty) {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                File curFile = File(folderPath / entry.path().filename(),create);
                this->filesInside.push_back(curFile);
            }
            else if (entry.is_directory() && entry.path().filename().string() != ".ptitgit") {
                Tree curFolder = Tree(folderPath / entry.path().filename().string(),create);
                this->treesInside.push_back(curFolder);
            }
        }
    }

    sort(this->treesInside.begin(), this->treesInside.end(), compareTree);
    sort(this->filesInside.begin(), this->filesInside.end(), compareFile);
    long long kk;
    for(kk = 0;kk < (long long) treesInside.size();kk++) this->content += "tree " + treesInside[kk].getHashedContent() + " " + treesInside[kk].get_folder_path().filename().string() + "\n";
    for(kk = 0;kk < (long long) filesInside.size();kk++) this->content += "file " + filesInside[kk].getHashedContent() + " " + filesInside[kk].get_file_path().filename().string() + "\n";

    std::string abc = this->content;
    this->content = "tree " + std::to_string(abc.size()) + '\n' + abc;

    this->hashedContent = hashString(this->content);
    if(create) {
        PtitGitRepos curRepos = PtitGitRepos(folderPath);
        curRepos.writeObject(*this);
    }
}
Tree::Tree()    {

}

fs::path Tree::get_folder_path() {

    return this->folderPath;

}

std::vector<File> Tree::get_blobs_inside() {

    return this->filesInside;    

}

std::vector<Tree> Tree::get_trees_inside() {

    return this->treesInside;

}

Tree findTree(std::string hashedContent, bool create, PtitGitRepos repos){

    fs::path path = repos.getWorkingFolder() / ".ptitgit" / "objects" / get_path_to_object(hashedContent);
    if(!fs::exists(path)) path = repos.getWorkingFolder() / ".ptitgit" / "index" / get_path_to_object(hashedContent);

    std::ifstream fileToShow(path);

    if(!fileToShow.is_open()) {
        std::cerr << "Error : impossible to open the file in get_object_content (" << path << ")" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();
    Tree T = Tree().createTreeFromContent(content,create,repos);
    return T;
}

Tree Tree::createTreeFromContent(std::string content, bool create, PtitGitRepos repos){
    this->content = content;this->filesInside = {}; this->treesInside = {};
    long long findSpace = content.find(' ');
    long long findEndl = content.find('\n');
    long long findNextSpace;
    if(content.substr(0,findSpace) != "tree") std::cerr<<"Not a tree here\n";
    if((long long) std::stoi(content.substr(findSpace+1,findEndl-findSpace-1)) != (long long) content.size()-findEndl-1) std::cerr<<"Bad size!\n"<<content.substr(findSpace+1,findEndl-findSpace-1)<<" "<<content.size()-findEndl-1<<std::endl;

    long long findNextEndl = content.find('\n',findEndl+1);
    this->folderPath = content.substr(findEndl+1, findNextEndl - findEndl -1);

    while(findNextEndl < (long long) content.size()-1){
        findEndl = findNextEndl;
        findSpace = content.find(' ' , findNextEndl + 1);
        findNextSpace = content.find(' ' , findSpace + 1);
        findNextEndl = content.find('\n', findNextSpace + 1);
        if(content.substr(findEndl + 1, findSpace - findEndl - 1) == "file")
            this->filesInside.push_back(findFile(content.substr(findSpace + 1, findNextSpace - findSpace - 1), create, repos));
        else if(content.substr(findEndl + 1, findSpace - findEndl - 1) == "tree")
            this->treesInside.push_back(findTree(content.substr(findSpace + 1, findNextSpace - findSpace - 1), create, repos));
        else std::cerr<<"What object is this? \n";
    }
    this->hashedContent = hashString(this->content);
    if(create) this->writeObject(repos.getWorkingFolder());
    return *this;
}

std::string get_next_line(std::string &s) {

    size_t pos = s.find('\n');

    if(pos == std::string::npos) {
        throw std::runtime_error("No newline found is the string");
    }
    
    std::string nextLine = s.substr(0, pos);
    s = s.substr(pos + 1, s.size());
    return nextLine;

}

std::map<std::string, std::string> cut_line(std::string contentLine) {

    std::map<std::string, std::string> m;

    size_t pos1 = contentLine.find(' ');
    m["type"] = contentLine.substr(0, pos1);

    contentLine = contentLine.substr(pos1 + 1, contentLine.size());

    size_t pos2 = contentLine.find(' ');
    m["hash"] = contentLine.substr(0, pos2);

    contentLine = contentLine.substr(pos2 + 1, contentLine.size());

    size_t pos3 = contentLine.find(' ');
    m["path"] = contentLine.substr(0, pos3);

    return m;

}

std::string get_object_type(std::string contentLine) {

    return cut_line(contentLine)["type"];

}

std::string get_object_hash(std::string contentLine) {

    return cut_line(contentLine)["hash"];

}

std::string get_object_path(std::string contentLine) {

    return cut_line(contentLine)["path"];

}

std::string insert_new_object(std::string content, std::string typeToInsert, std::string hashToInsert, std::string pathToInsert) {

    std::string updatedContent;

    get_next_line(content);
    std::string filePath = get_next_line(content);

    bool added = false;

    while (content != "") {
        std::string curLine = get_next_line(content);
        std::string curPath = get_object_path(curLine);
        std::string curType = get_object_type(curLine);
        if (!added && ((curPath > pathToInsert  && curType == typeToInsert) || (typeToInsert == "tree" && curType == "file"))) {
            updatedContent += typeToInsert + " " + hashToInsert + " " + pathToInsert + '\n';
            added = true;
        }
        updatedContent += curLine + '\n';
    }
    if (!added) {
        updatedContent += typeToInsert + " " + hashToInsert + " " + pathToInsert + '\n';
    }

    updatedContent = "tree " + std::to_string(updatedContent.size() + std::string(filePath).size() + 1) + '\n' + filePath + '\n' + updatedContent;

    return updatedContent;

}

std::pair<std::string, std::string> delete_object(std::string content, std::string hashToDelete) {

    std::string updatedContent;
    std::string deletedLine;
    
    get_next_line(content);
    std::string filePath = get_next_line(content);

    while (content != "") {
        std::string curLine = get_next_line(content);
        std::string curHash = get_object_hash(curLine);
        if (curHash != hashToDelete) {
            updatedContent += curLine + '\n';
        }
        else {
            deletedLine = curLine;
        }
    }

    updatedContent = "tree " + std::to_string(updatedContent.size() + std::string(filePath).size() + 1) + '\n' + filePath + '\n' + updatedContent;

    return std::pair(updatedContent, deletedLine);

}

bool compareTree(Tree T1, Tree T2){
    if(T1.get_folder_path().filename().string() < T2.get_folder_path().filename().string()) return true;
    else return false;
}
