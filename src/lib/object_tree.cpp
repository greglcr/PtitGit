#include "hashing.h"
#include "object.h"
#include "object_file.h"
#include "object_tree.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

Tree::Tree(fs::path folderPath, bool create) {

    if (!fs::exists(folderPath)) {
        std::cerr << "Error in Tree construction : The given path doesn't exists (" << folderPath << ")" << std::endl;
    }
    if (!fs::is_directory(folderPath)) {
        std::cerr << "Error in Tree construction : The given path is not a directory (" << folderPath << ")" << std::endl;
    }

    //this->content += "tree\n";
    this->content += folderPath;
    this->content += '\n';
    
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            File curFile = File(folderPath / entry.path().filename(),create);
            this->filesInside.push_back(curFile);
            this->content += "file " + curFile.getHashedContent() + " " + entry.path().filename().string() + "\n";
        }
        else if (entry.is_directory() && entry.path().filename().string() != ".ptitgit") {
            Tree curFolder = Tree(folderPath / entry.path().filename().string(),create);
            this->treesInside.push_back(curFolder);
            this->content += "tree " + curFolder.getHashedContent() + " " + entry.path().filename().string() + "\n";
        }
    }

    std::string abc = this->content;
    this->content = "tree " + std::to_string(abc.size()) + '\n' + abc;

    this->hashedContent = hashString(this->content);

    if(create) this->writeObject();
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

Tree findTree(std::string hashedContent, bool create, bool objectOrNot){
    fs::path path;
    fs::path curPath = fs::current_path();
    while (curPath != curPath.root_directory() && !fs::exists(curPath / ".ptitgit")) {
        curPath = curPath.parent_path();
    }
    if(objectOrNot == true) path = curPath / ".ptitgit" / "objects" / get_path_to_object(hashedContent);
    else path = curPath / ".ptitgit" / "index" / get_path_to_object(hashedContent);

    std::ifstream fileToShow(path);

    if(!fileToShow.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier voulu dans get_object_content (" << path << ")" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();

    Tree T = Tree().createTreeFromContent(content,create);
    return T;
}

Tree Tree::createTreeFromContent(std::string content, bool create){
    File F;
    this->content = content;
    long long findSpace = content.find(' ');
    long long findEndl = content.find('\n');
    long long findNextSpace;
    if(content.substr(0,findSpace) != "tree") std::cerr<<"Not a tree here\n";
    if((long long) std::stoi(content.substr(findSpace+1,findEndl-findSpace-1)) != (long long) content.size()-findEndl-1) std::cerr<<"Bad size!\n";

    long long findNextEndl = content.find('\n',findEndl+1);
    this->folderPath = content.substr(findEndl+1, findNextEndl - findEndl -1);std::cout<<content.size()<<std::endl;

    while(findNextEndl < (long long) content.size()-1){
        findEndl = findNextEndl;
        findSpace = content.find(' ' , findNextEndl + 1);
        findNextSpace = content.find(' ' , findSpace + 1);
        findNextEndl = content.find('\n', findNextSpace + 1);
        std::cout<<findSpace <<" "<<findNextSpace<<" "<<findNextEndl<<std::endl;
        if(content.substr(findEndl + 1, findSpace - findEndl - 1) == "file")
            this->filesInside.push_back(findFile(content.substr(findSpace + 1, findNextSpace - findSpace - 1), create, false));
        else if(content.substr(findEndl + 1, findSpace - findEndl - 1) == "tree")
            this->treesInside.push_back(findTree(content.substr(findSpace + 1, findNextSpace - findSpace - 1), create, false));
        else std::cerr<<"What object is this? \n";
    }

    this->hashedContent = hashString(this->content);

    if(create) this->writeObject();
}