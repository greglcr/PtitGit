#include "object.h"

#include <filesystem>
#include <iterator>
#include <string>

namespace fs = std::filesystem;

std::string Object::getContent() {

    return this->content;

}

std::string Object::getHashedContent() {

    return this->hashedContent;

}

fs::path Object::getPathToWrite() {

    std::string::iterator beginContent = this->hashedContent.begin();
    std::string::iterator endContent = this->hashedContent.end();
    
    fs::path folder = std::string(beginContent, beginContent + 2);
    fs::path fileName = std::string(beginContent + 2, endContent);

    return folder / fileName;
}