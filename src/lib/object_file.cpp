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
    
    this->content = "blob";
    this->content += filePath;
    this->content += oss.str();
    
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