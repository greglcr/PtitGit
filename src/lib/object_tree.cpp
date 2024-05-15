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
            File curFile = File(folderPath / entry.path().filename());
            this->filesInside.push_back(curFile);
            this->content += "file " + curFile.getHashedContent() + " " + entry.path().filename().string() + "\n";
        }
        else if (entry.is_directory() && entry.path().filename().string() != ".ptitgit") {
            Tree curFolder = Tree(folderPath / entry.path().filename().string());
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

Tree findTree(std::string hashedContent, bool create){
    fs::path curPath = fs::current_path();

    while (curPath != curPath.root_directory() && !fs::exists(curPath / ".ptitgit")) {
        curPath = curPath.parent_path();
    }
    fs::path path = curPath / ".ptitgit" / "objects" / get_path_to_object(hashedContent);

    std::ifstream fileToShow(path);

    if(!fileToShow.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier voulu dans get_object_content (" << path << ")" << std::endl;
        exit(0);
    }

    std::stringstream buffer;
    buffer << fileToShow.rdbuf();
    std::string content = buffer.str();
    fileToShow.close();

    long long abcd = content.find(' ');
    long long dcba = content.find('\n');

    if(content.substr(0,abcd) != "tree") std::cerr<<"Not a tree in fromstring Commit creation\n";
    if((long long) std::stoi(content.substr(abcd+1,dcba-abcd-1)) != (long long) content.size()-dcba-1) std::cerr<<"Bad size!\n";

    long long abba = content.find('\n',dcba+1);

    return Tree(content.substr(dcba+1,abba-dcba-1),create);
}