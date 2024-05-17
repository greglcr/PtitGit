#include "hashing.h"
#include "object.h"
#include "object_file.h"
#include "object_tree.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
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
    
    //TO DO : Ensure that files and folders are given in the alphabetic order so the tree keeps a consistent structure. For the moment, we assume that this is done like
    //that

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
        if (curPath > pathToInsert && !added) {
            updatedContent += typeToInsert + " " + hashToInsert + " " + pathToInsert + '\n';
            added = true;
        }
        updatedContent += curLine + '\n';
    }
    if (!added) {
        updatedContent += typeToInsert + " " + hashToInsert + " " + pathToInsert + '\n';
    }

    updatedContent = "tree " + std::to_string(updatedContent.size()) + '\n' + filePath + '\n' + updatedContent;

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

    updatedContent = "tree " + std::to_string(updatedContent.size()) + '\n' + filePath + '\n' + updatedContent;

    return std::pair(updatedContent, deletedLine);

}