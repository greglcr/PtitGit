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
    this->content += oss.str();
    
    this->filePath = filePath;

    std::string abc = this->content;
    this->content = "tree " + std::to_string(abc.size()) + '\n' + abc;

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