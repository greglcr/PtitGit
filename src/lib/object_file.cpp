#include "hashing.h"
#include "object.h"
#include "object_file.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

File::File(fs::path filePath, bool create) {

    if (filePath.is_relative()) {
        filePath = fs::current_path() / filePath;
    }

    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file : " << filePath << std::endl;
    }

    std::ostringstream oss;
    oss << inputFile.rdbuf();
    
    this->content += filePath;
    this->content += '\n';
    this->content += oss.str();
    
    this->filePath = filePath;

    std::string abc = this->content;
    this->content = "file " + std::to_string(abc.size()) + '\n' + abc;

    this->hashedContent = hashString(this->content);

    if(create) this->writeObject();
}

void File::updateContent() {
    

    std::ifstream inputFile(this->filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file : " << this->filePath << std::endl;
    }

    std::ostringstream oss;
    oss << inputFile.rdbuf();

    this->content = oss.str();
    this->hashedContent = hashString(this->content);
}

fs::path File::get_file_path() {

    return this->filePath;

}
File findFile(std::string hashedContent, bool create, bool objectOrNot){
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
    return File(".").createFileFromContent(content,create);
}

File File::createFileFromContent(std::string content, bool create){
    this->content = content;
    long long findSpace = content.find(' ');
    long long findEndl = content.find('\n');
    
    if(content.substr(0,findSpace) != "file") std::cerr<<"Not a file here\n";
    if((long long) std::stoi(content.substr(findSpace+1,findEndl-findSpace-1)) != (long long) content.size()-findEndl-1) std::cerr<<"Bad size!\n";

    long long findNextEndl = content.find('\n',findEndl+1);
    this->filePath = content.substr(findEndl + 1, findNextEndl - findEndl - 1);

    this->hashedContent = hashString(this->content);

    if(create) this->writeObject();
}