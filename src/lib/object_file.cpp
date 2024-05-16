#include "hashing.h"
#include "object.h"
#include "object_file.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

File::File(fs::path filePath, bool create) {

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
    this->hashedContent = hashString(this->hashedContent);
}

fs::path File::get_file_path() {

    return this->filePath;

}
File findFile(std::string hashedContent, bool create){
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

    if(content.substr(0,abcd) != "file") std::cerr<<"Not a file in fromstring Commit creation\n";
    if((long long) std::stoi(content.substr(abcd+1,dcba-abcd-1)) != (long long) content.size()-dcba-1) std::cerr<<"Bad size!\n";

    long long abba = content.find('\n',dcba+1);

    return File(content.substr(dcba+1,abba-dcba-1),create);
}