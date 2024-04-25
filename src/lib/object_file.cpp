#include "hashing.h"
#include "object.h"
#include "object_file.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

File::File(fs::path filePath) {

    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file : " << filePath << std::endl;
    }

    std::ostringstream oss;
    oss << inputFile.rdbuf();
    
    this->content = oss.str();
    this->filePath = filePath;
    this->hashedContent = hashString(this->content);
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

void File::printContent() {

    std::cout << "########## Début du fichier " << this->filePath << "##########\n\n";
    std::cout << this->content;
    std::cout << "########## Fin du fichier ##########\n";

}

std::string File::getHash() {

    return this->hashedContent;

}